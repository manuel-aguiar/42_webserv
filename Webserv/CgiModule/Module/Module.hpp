

#ifndef MODULE_HPP

# define MODULE_HPP

# include "../CgiModule/CgiModule.hpp"

// forward declarations
class Globals;
namespace Cgi {class Request;}
namespace Events {class Manager;}

namespace Cgi
{
    class Module : private ImplModule
    {
        public:
            Module(size_t workers, size_t backlog, size_t maxTimeout, Events::Manager& eventManager, Globals& globals);
            ~Module();

            Cgi::Request*	acquireRequest();
            void			enqueueRequest(Cgi::Request& data, bool isCalledFromEventLoop);
            void			modifyRequest(Cgi::Request& data, bool isCalledFromEventLoop, Options::Mask newOptions);
            void			finishRequest(Cgi::Request& data, bool isCalledFromEventLoop);
            
            int				processRequests();
            void			stopAndReset();

            void			addInterpreter(const InterpExtension& extension, const InterpPath& path);
            void			removeInterpreter(const InterpExtension& extension);

            size_t          getBusyWorkerCount() const;
            size_t			getQueueSize() const;
            const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
                            getBaseEnvKeys() const;
            const std::map<Cgi::InterpExtension, Cgi::InterpPath>&	
                            getInterpreters() const;

    };

};


#endif