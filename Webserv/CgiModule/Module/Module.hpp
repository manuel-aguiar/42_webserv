

#ifndef MODULE_HPP

# define MODULE_HPP

# include "../ImplModule/ImplModule.hpp"

namespace Cgi
{
    class Module : private ImplModule
    {
        public:
            Module( const size_t workers, 
                    const size_t backlog, 
                    const size_t maxTimeout, 
                    const size_t workerBufferSize,
                    Events::Manager& eventManager, 
                    Globals& globals);
            ~Module();

            // client facing functions to acquire and enqueue a request
            Cgi::Request*	acquireRequest();
            void			enqueueRequest(Cgi::Request& data, bool isCalledFromEventLoop);
            void			modifyRequest(Cgi::Request& data, bool isCalledFromEventLoop, Options::Mask newOptions);
            void			finishRequest(Cgi::Request& data, bool isCalledFromEventLoop);
            
            // Controller functions to add to their event loop
            int				processRequests();
            void			stopAndReset();

            // configuration of the Module
            void			addInterpreter(const InterpExtension& extension, const InterpPath& path);
            void			removeInterpreter(const InterpExtension& extension);
            
            // informative stuff
            size_t          getBusyWorkerCount() const;
            size_t			getQueueSize() const;
            const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
                            getBaseEnvKeys() const;
            const std::map<Cgi::InterpExtension, Cgi::InterpPath>&	
                            getInterpreters() const;

    };

}

#endif
