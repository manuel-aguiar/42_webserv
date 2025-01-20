

// Project headers
# include "../../CgiModule.h"

# include "CgiStressTest.hpp"

// test helpers
# include "../TestProtoRequest/TestProtoRequest.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../../Toolkit/_Tests/test.h"

//C++ headers
# include <iostream>

// C headers
#include <unistd.h>

static char* findExtraEnvVariable(const char* key, const Cgi::Module::Request& request)
{
    const Cgi::Module::EnvVariables& envVars = request.getEnvVars();

    for (DynArray<std::pair<Cgi::Module::EnvKey, Cgi::Module::EnvValue> >::const_iterator it = envVars.envExtra.begin(); it != envVars.envExtra.end(); ++it)
    {
        if (it->first == key)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getExtraEntry(const Cgi::Module::Request& request, const char* textKey)
{
    char* const     target = findExtraEnvVariable(textKey, request);
    std::string     result;

    result += textKey;
    result += ": ";
    if (target)
        result += target;
    else
        result += "<not set>";
    result += "\n";
    return (result);
}

static char* findBaseEnvVariable(const Cgi::Env::Enum::Type target, const Cgi::Request& request)
{
    const Cgi::Module::EnvVariables& envVars = request.getEnvVars();

    for (DynArray<std::pair<Cgi::Env::Enum::Type, Cgi::Module::EnvValue> >::const_iterator it = envVars.envBase.begin(); it != envVars.envBase.end(); ++it)
    {
        if (it->first == target)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getBaseEntry(const Cgi::Request& request, std::string textKey, const Cgi::Env::Enum::Type enumKey)
{
    char* const     target = findBaseEnvVariable(enumKey, request);
    std::string     result;

    result += textKey;
    if (target)
        result += target;
    else
        result += "<not set>";
    result += "\n";
    return (result);
}



void CgiStressTest::prepareExpectedOutput(bool isExpectedValid, TestProtoRequest& proto)
{
    std::string     temp;

    Cgi::Request* requestData = proto.m_CgiRequestData;

    // didn't get a request or the test is made to error on purpose, empty expected output
    if (!isExpectedValid || !requestData)
        return ;

    proto.m_msgBody = "i am proto " + StringUtils::to_string(proto.m_id) + " and i am alive "
    "i have a lot of BS to say blabla i just want to fill the buffer a little bit and "
    " that's it\n";
    
    // CGI BASE ENTRIES
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::AUTH_TYPE) + ": ",           Cgi::Env::Enum::AUTH_TYPE);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::CONTENT_LENGTH) + ": ",      Cgi::Env::Enum::CONTENT_LENGTH);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::CONTENT_TYPE) + ": ",        Cgi::Env::Enum::CONTENT_TYPE);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::GATEWAY_INTERFACE) + ": ",   Cgi::Env::Enum::GATEWAY_INTERFACE);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::PATH_INFO) + ": ",           Cgi::Env::Enum::PATH_INFO);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::PATH_TRANSLATED) + ": ",     Cgi::Env::Enum::PATH_TRANSLATED);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::QUERY_STRING) + ": ",        Cgi::Env::Enum::QUERY_STRING);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_ADDR) + ": ",         Cgi::Env::Enum::REMOTE_ADDR);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_HOST) + ": ",         Cgi::Env::Enum::REMOTE_HOST);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_IDENT) + ": ",        Cgi::Env::Enum::REMOTE_IDENT);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_USER) + ": ",         Cgi::Env::Enum::REMOTE_USER);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REQUEST_METHOD) + ": ",      Cgi::Env::Enum::REQUEST_METHOD);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SCRIPT_NAME) + ": ",         Cgi::Env::Enum::SCRIPT_NAME);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_NAME) + ": ",         Cgi::Env::Enum::SERVER_NAME);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_PORT) + ": ",         Cgi::Env::Enum::SERVER_PORT);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_PROTOCOL) + ": ",     Cgi::Env::Enum::SERVER_PROTOCOL);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_SOFTWARE) + ": ",     Cgi::Env::Enum::SERVER_SOFTWARE);

    // EXTRA ENTRIES
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY1");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY2");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY3");
    
    temp += proto.m_msgBody;

    StringUtils::move(proto.m_ExpectedOutput, temp);
}
