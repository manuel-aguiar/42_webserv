

// Project headers
# include "../../CgiModule.h"

# include "CgiStressTest.hpp"

// test helpers
# include "../TestProtoRequest/TestProtoRequest.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../../Toolkit/_Tests/TestHelpers.h"

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

static char* findBaseEnvVariable(const CgiEnvEnum::Type target, const Cgi::Request& request)
{
    const Cgi::Module::EnvVariables& envVars = request.getEnvVars();

    for (DynArray<std::pair<CgiEnvEnum::Type, Cgi::Module::EnvValue> >::const_iterator it = envVars.envBase.begin(); it != envVars.envBase.end(); ++it)
    {
        if (it->first == target)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getBaseEntry(const Cgi::Request& request, std::string textKey, const CgiEnvEnum::Type enumKey)
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
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::AUTH_TYPE) + ": ",           CgiEnvEnum::AUTH_TYPE);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::CONTENT_LENGTH) + ": ",      CgiEnvEnum::CONTENT_LENGTH);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::CONTENT_TYPE) + ": ",        CgiEnvEnum::CONTENT_TYPE);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::GATEWAY_INTERFACE) + ": ",   CgiEnvEnum::GATEWAY_INTERFACE);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::PATH_INFO) + ": ",           CgiEnvEnum::PATH_INFO);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::PATH_TRANSLATED) + ": ",     CgiEnvEnum::PATH_TRANSLATED);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::QUERY_STRING) + ": ",        CgiEnvEnum::QUERY_STRING);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_ADDR) + ": ",         CgiEnvEnum::REMOTE_ADDR);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_HOST) + ": ",         CgiEnvEnum::REMOTE_HOST);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_IDENT) + ": ",        CgiEnvEnum::REMOTE_IDENT);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REMOTE_USER) + ": ",         CgiEnvEnum::REMOTE_USER);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::REQUEST_METHOD) + ": ",      CgiEnvEnum::REQUEST_METHOD);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SCRIPT_NAME) + ": ",         CgiEnvEnum::SCRIPT_NAME);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_NAME) + ": ",         CgiEnvEnum::SERVER_NAME);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_PORT) + ": ",         CgiEnvEnum::SERVER_PORT);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_PROTOCOL) + ": ",     CgiEnvEnum::SERVER_PROTOCOL);
    temp += getBaseEntry(*requestData, std::string(Cgi::Env::Str::SERVER_SOFTWARE) + ": ",     CgiEnvEnum::SERVER_SOFTWARE);

    // EXTRA ENTRIES
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY1");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY2");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY3");
    
    temp += proto.m_msgBody;

    StringUtils::move(proto.m_ExpectedOutput, temp);
}
