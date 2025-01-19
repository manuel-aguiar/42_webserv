/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiStressTest_ExpectedOutput.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:57:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/19 14:29:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../../CgiModule/CgiModule.hpp"

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

static char* findExtraEnvVariable(const char* key, const CgiModule::Request& request)
{
    const CgiModule::EnvVariables& envVars = request.getEnvVars();

    for (DynArray<std::pair<CgiModule::EnvKey, CgiModule::EnvValue> >::const_iterator it = envVars.envExtra.begin(); it != envVars.envExtra.end(); ++it)
    {
        if (it->first == key)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getExtraEntry(const CgiModule::Request& request, const char* textKey)
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

static char* findBaseEnvVariable(const Cgi::EnvType& target, const CgiModule::Request& request)
{
    const CgiModule::EnvVariables& envVars = request.getEnvVars();

    for (DynArray<std::pair<Cgi::EnvType, CgiModule::EnvValue> >::const_iterator it = envVars.envBase.begin(); it != envVars.envBase.end(); ++it)
    {
        if (it->first == target)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getBaseEntry(const CgiModule::Request& request, const char* textKey, const Cgi::EnvType& enumKey)
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

    CgiModule::Request* requestData = proto.m_CgiRequestData;

    // didn't get a request or the test is made to error on purpose, empty expected output
    if (!isExpectedValid || !requestData)
        return ;

    proto.m_msgBody = "i am proto " + StringUtils::to_string(proto.m_id) + " and i am alive "
    "i have a lot of BS to say blabla i just want to fill the buffer a little bit and "
    " that's it\n";
    
    // CGI BASE ENTRIES
    temp += getBaseEntry(*requestData, "AUTH_TYPE: ",           Cgi::ENV_AUTH_TYPE);
    temp += getBaseEntry(*requestData, "CONTENT_LENGTH: ",      Cgi::ENV_CONTENT_LENGTH);
    temp += getBaseEntry(*requestData, "CONTENT_TYPE: ",        Cgi::ENV_CONTENT_TYPE);
    temp += getBaseEntry(*requestData, "GATEWAY_INTERFACE: ",   Cgi::ENV_GATEWAY_INTERFACE);
    temp += getBaseEntry(*requestData, "PATH_INFO: ",           Cgi::ENV_PATH_INFO);
    temp += getBaseEntry(*requestData, "PATH_TRANSLATED: ",     Cgi::ENV_PATH_TRANSLATED);
    temp += getBaseEntry(*requestData, "QUERY_STRING: ",        Cgi::ENV_QUERY_STRING);
    temp += getBaseEntry(*requestData, "REMOTE_ADDR: ",         Cgi::ENV_REMOTE_ADDR);
    temp += getBaseEntry(*requestData, "REMOTE_HOST: ",         Cgi::ENV_REMOTE_HOST);
    temp += getBaseEntry(*requestData, "REMOTE_IDENT: ",        Cgi::ENV_REMOTE_IDENT);
    temp += getBaseEntry(*requestData, "REMOTE_USER: ",         Cgi::ENV_REMOTE_USER);
    temp += getBaseEntry(*requestData, "REQUEST_METHOD: ",      Cgi::ENV_REQUEST_METHOD);
    temp += getBaseEntry(*requestData, "SCRIPT_NAME: ",         Cgi::ENV_SCRIPT_NAME);
    temp += getBaseEntry(*requestData, "SERVER_NAME: ",         Cgi::ENV_SERVER_NAME);
    temp += getBaseEntry(*requestData, "SERVER_PORT: ",         Cgi::ENV_SERVER_PORT);
    temp += getBaseEntry(*requestData, "SERVER_PROTOCOL: ",     Cgi::ENV_SERVER_PROTOCOL);
    temp += getBaseEntry(*requestData, "SERVER_SOFTWARE: ",     Cgi::ENV_SERVER_SOFTWARE);

    // EXTRA ENTRIES
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY1");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY2");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY3");
    
    temp += proto.m_msgBody;

    StringUtils::move(proto.m_ExpectedOutput, temp);
}
