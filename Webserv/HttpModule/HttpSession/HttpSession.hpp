

#ifndef HTTPSESSION_HPP

# define HTTPSESSION_HPP

//C++ headers
# include <string>
# include <cstddef>

class HttpSession
{
	public:
		HttpSession();
		~HttpSession();

	private:

		size_t	  			m_timeout;
		std::string         m_session_id;
		size_t      		m_logged_times;
};

#endif
