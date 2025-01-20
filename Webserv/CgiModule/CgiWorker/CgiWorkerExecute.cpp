

// Project Headers
# include "CgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

/*
	Prepare pipes, prepare fds, prepare env vars and execve arguments.

	Inform the user that execution will begin, user can prepare its setup based on the pipes provided
	(subscribind read and write ends to its epoll, for example)

	Fork, child does child, parent does parent
*/

namespace Cgi
{

	void   Cgi::Module::Worker::execute(bool markFdsAsStale)
	{
		Options::Flags options;

		options = m_curRequestData->getOptions();

		if (!mf_prepareExecve())
			return (m_CgiModule.mf_recycleStartupFailure(*this, markFdsAsStale));

		if (::pipe(m_ParentToChild) == -1 ||
			::pipe(m_ChildToParent) == -1 ||
			::pipe(m_EmergencyPhone) == -1)
		{
			m_CgiModule.mf_accessGlobals().logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
			return (m_CgiModule.mf_recycleStartupFailure(*this, markFdsAsStale));
		}

		//std::cout << "\nopening fd: " << (m_EmergencyPhone[0]) << ", phone read " << '\n';
		//std::cout << "opening fd: " << (m_EmergencyPhone[1]) << ", phone write " << '\n';
		//std::cout << "opening fd: " << (m_ParentToChild[0]) << ", child read " << '\n';
		//std::cout << "opening fd: " << (m_ParentToChild[1]) << ", parent write " <<  '\n';
		//std::cout << "opening fd: " << (m_ChildToParent[0]) << ", parent read " <<  '\n';
		//std::cout << "opening fd: " << (m_ChildToParent[1]) << ", child write " <<  '\n';

		if (!(options & Options::CANCEL_WRITE))
		{
			m_writeEvent.setFd(m_ParentToChild[1]);
			m_CgiModule.mf_accessEventManager().addEvent(m_writeEvent, markFdsAsStale);
		}

		if (!(options & Options::CANCEL_READ))
		{
			m_readEvent.setFd(m_ChildToParent[0]);
			m_CgiModule.mf_accessEventManager().addEvent(m_readEvent, markFdsAsStale);
		}

		m_EmergencyEvent.setFd(m_EmergencyPhone[0]);
		m_CgiModule.mf_accessEventManager().addEvent(m_EmergencyEvent, markFdsAsStale);
		

		//std::cout << "\nopening fd: " << (m_EmergencyPhone[0]) << ", phone read " << '\n';
		//std::cout << "opening fd: " << (m_EmergencyPhone[1]) << ", phone write " << '\n';
		//std::cout << "opening fd: " << (m_ParentToChild[0]) << ", child read " << '\n';
		//std::cout << "opening fd: " << (m_ParentToChild[1]) << ", parent write " <<  '\n';
		//std::cout << "opening fd: " << (m_ChildToParent[0]) << ", parent read " <<  '\n';
		//std::cout << "opening fd: " << (m_ChildToParent[1]) << ", child write " <<  '\n';

		m_pid = ::fork();
		if (m_pid == -1)
		{
			m_CgiModule.mf_accessGlobals().logError("InternalCgiWorker::execute(), fork(): " + std::string(strerror(errno)));
			return (m_CgiModule.mf_recycleStartupFailure(*this, markFdsAsStale));
		}
		

		if (m_pid == 0)
			mf_executeChild();
		else
			mf_executeParent(markFdsAsStale);
		
	}


	/*
		Parent just closes the pipe ends that doesn't use and simply waits for the EventManager
		to inform it that the child process has exited -> Events
	*/
	void	Cgi::Module::Worker::mf_executeParent(bool markFdsAsStale)
	{
		Module::Options::Flags options;

		options = m_curRequestData->getOptions();

		if ((!(options & Options::CANCEL_WRITE) && !FileDescriptor::setNonBlocking(m_ParentToChild[1])) ||
			(!(options & Options::CANCEL_READ) && !FileDescriptor::setNonBlocking(m_ChildToParent[0]))  ||
			!FileDescriptor::setNonBlocking(m_EmergencyPhone[0]))
		{
			m_CgiModule.mf_accessGlobals().logError("InternalCgiWorker::execute(), fcntl(): " + std::string(strerror(errno)));
			return (m_CgiModule.mf_recycleStartupFailure(*this, markFdsAsStale));
		}

		if (options & Options::CANCEL_WRITE)
			mf_closeFd(m_ParentToChild[1]);
		if (options & Options::CANCEL_READ)
			mf_closeFd(m_ChildToParent[0]);
		if (options & Options::HOLD_READ)
			disableReadHandler();
		if (options & Options::HOLD_WRITE)
			disableWriteHandler();

		mf_closeFd(m_EmergencyPhone[1]);
		mf_closeFd(m_ParentToChild[0]);
		mf_closeFd(m_ChildToParent[1]);

		m_EmergencyPhone[0] = -1;
		m_ParentToChild[1] = -1;
		m_ChildToParent[0] = -1;
	}

	/*
		The EmergencyCode message that the Child process may send to the parent is made of 2 bytes.

		The first byte is just the enum value of the syscall that failed. In our case, there are
		two major possibilities: dup2 (failed to redirect), execve(failed to execute the script)

		The second byte is the errno value recorded when the syscall failed, such that the parent worker
		can log the info correctly and then tell the User what happened.

		The parent will subscribe the emergencyphone event in the epoll, so epoll will inform it when the
		child process exits, whether with a message or a straight EOF.

		If everything goes according to plan, the process by which child is replaced in execve will close
		the pipe as it exits, the parent will receive an EOF directly and no other info, letting the parent
		know that everything went smoothly, and to peacefully waitpid the child.
	*/

	static void childCloseFd(Ws::fd fd)
	{
		if (fd != -1)
			::close(fd);
		//std::cerr << " child closed " << fd << std::endl;
	}




	void	Cgi::Module::Worker::mf_executeChild()
	{
		char EmergencyCode[2];

		childCloseFd(m_EmergencyPhone[0]);
		childCloseFd(m_ParentToChild[1]);
		childCloseFd(m_ChildToParent[0]);

		if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1 ||
			::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
		{
			EmergencyCode[0] = E_EMER_DUP2;
			EmergencyCode[1] = errno;
			::write(m_EmergencyPhone[1], EmergencyCode, 2);

			childCloseFd(m_EmergencyPhone[1]);
			childCloseFd(m_ParentToChild[0]);
			childCloseFd(m_ChildToParent[1]);
					
			
			// wait to be killed lol
			while (1)
				::usleep(1000);
		}
			
		::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());

		EmergencyCode[0] = E_EMER_EXECVE;
		EmergencyCode[1] = errno;
		::write(m_EmergencyPhone[1], EmergencyCode, 2);
		
		childCloseFd(m_EmergencyPhone[1]);
		childCloseFd(m_ParentToChild[0]);
		childCloseFd(m_ChildToParent[1]);
		
		//wait to be killed lol
		while (1)
			::usleep(1000);
	}


	/*
		Putting env variables in place for execution.
		Try-catch because the DynArray may throw if it fails to allocate memory
	*/
	bool	Cgi::Module::Worker::mf_prepareExecve()
	{
		typedef DynArray<std::pair<EnvKey, EnvValue> >::const_iterator	t_EnvExtraIter;

		const EnvVariables& 			envRequest = m_curRequestData->getEnvVars();
		const StackArray<EnvKey, Cgi::Env::Enum::COUNT>&
										envBase = m_CgiModule.getBaseEnvKeys();
		size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
		std::string						temp;
		

		try
		{
			std::map<InterpExtension, InterpPath>::const_iterator interpExtension 
			= m_CgiModule.getInterpreters().find(m_curRequestData->getExtension());

			if (interpExtension == m_CgiModule.getInterpreters().end())
				throw std::runtime_error("interpreter not found");

			assert (m_envStr.size() == 0 && m_envPtr.size() == 0 && m_argPtr.size() == 0);

			m_envStr.reserve(entryCount);
			m_envPtr.reserve(entryCount + 1);
			m_argPtr.reserve(3);

			for (size_t i = 0; i < envRequest.envBase.size(); i++)
			{
				temp = (envBase[envRequest.envBase[i].first] + "=" + envRequest.envBase[i].second);
				m_envStr.emplace_back();
				StringUtils::move(m_envStr.back(), temp);
			}
			
			for (t_EnvExtraIter it = envRequest.envExtra.begin(); it != envRequest.envExtra.end(); it++)
			{
				temp = (it->first + "=" + it->second);
				m_envStr.emplace_back();
				StringUtils::move(m_envStr.back(), temp);
			}

			for (size_t i = 0; i < entryCount; i++)
				m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));

			m_envPtr.push_back(NULL);

			m_argPtr.push_back(const_cast<char*>(interpExtension->second.c_str()));
			m_argPtr.push_back(const_cast<char*>(m_curRequestData->getScriptPath().c_str()));
			m_argPtr.push_back(NULL);
			
			return (true);
		}
		catch(const std::exception& e)
		{
			m_CgiModule.mf_accessGlobals().logError("InternalCgiWorker::mf_prepareExecve(): " + std::string(e.what()));
		}
		return (false);
	}

}; // namespace Cgi

