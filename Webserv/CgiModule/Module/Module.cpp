


# include "Module.hpp"

namespace Cgi
{
    Module::Module(size_t workers, 
                    size_t backlog, 
                    size_t maxTimeout, 
                    Events::Manager& eventManager, 
                    Globals& globals) :
        ImplModule(workers, backlog, maxTimeout, eventManager, globals) {}
    
    Module::~Module() {}

    Cgi::Request*	
    Module::acquireRequest()
    {
        return (ImplModule::acquireRequest());
    }

    void			
    Module::enqueueRequest(Cgi::Request& data, bool isCalledFromEventLoop)
    {
        ImplModule::enqueueRequest(data, isCalledFromEventLoop);
    }

    void			
    Module::modifyRequest(Cgi::Request& data, bool isCalledFromEventLoop, Options::Mask newOptions)
    {
        ImplModule::modifyRequest(data, isCalledFromEventLoop, newOptions);
    }

    void			
    Module::finishRequest(Cgi::Request& data, bool isCalledFromEventLoop)
    {
        ImplModule::finishRequest(data, isCalledFromEventLoop);
    }

    int				
    Module::processRequests()
    {
        return (ImplModule::processRequests());
    }

    void			
    Module::stopAndReset()
    {
        ImplModule::stopAndReset();
    }

    void			
    Module::addInterpreter(const InterpExtension& extension, const InterpPath& path)
    {
        ImplModule::addInterpreter(extension, path);
    }

    void			
    Module::removeInterpreter(const InterpExtension& extension)
    {
        ImplModule::removeInterpreter(extension);
    }

    size_t          
    Module::getBusyWorkerCount() const
    {
        return (ImplModule::getBusyWorkerCount());
    }

    size_t			
    Module::getQueueSize() const
    {
        return (ImplModule::getQueueSize());
    }

    const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
    Module::getBaseEnvKeys() const
    {
        return (ImplModule::getBaseEnvKeys());
    }

    const std::map<Cgi::InterpExtension, Cgi::InterpPath>&	
    Module::getInterpreters() const
    {
        return (ImplModule::getInterpreters());
    }
}
