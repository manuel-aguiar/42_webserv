/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_prepareExpectedOutput.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:57:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 18:29:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../CgiModule/CgiModule.hpp"

# include "CgiStressTest.hpp"

// test helpers
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../Toolkit/_Tests/test.h"

//C++ headers
# include <iostream>

// C headers
#include <unistd.h>

static char* findExtraEnvVariable(const char* key, const CgiRequestData& request)
{
    const t_CgiRequestEnv& envVars = request.getEnvVars();

    for (DynArray<std::pair<t_CgiEnvKey, t_CgiEnvValue> >::const_iterator it = envVars.envExtra.begin(); it != envVars.envExtra.end(); ++it)
    {
        if (it->first == key)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getExtraEntry(const CgiRequestData& request, const char* textKey)
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

static char* findBaseEnvVariable(const e_CgiEnv& target, const CgiRequestData& request)
{
    const t_CgiRequestEnv& envVars = request.getEnvVars();

    for (DynArray<std::pair<e_CgiEnv, t_CgiEnvValue> >::const_iterator it = envVars.envBase.begin(); it != envVars.envBase.end(); ++it)
    {
        if (it->first == target)
            return (const_cast<char *>(it->second.c_str()));
    }
    return (NULL);
}

static std::string getBaseEntry(const CgiRequestData& request, const char* textKey, const e_CgiEnv& enumKey)
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



void CgiStressTest::prepareExpectedOutput(bool isExpectedValid, A_ProtoRequest& proto)
{
    std::string     temp;

    CgiRequestData* requestData = proto.m_CgiRequestData;

    // didn't get a request or the test is made to error on purpose, empty expected output
    if (!isExpectedValid || !requestData)
        return ;

    // CGI BASE ENTRIES
    temp += getBaseEntry(*requestData, "AUTH_TYPE: ", E_CGI_AUTH_TYPE);
    temp += getBaseEntry(*requestData, "CONTENT_LENGTH: ", E_CGI_CONTENT_LENGTH);
    temp += getBaseEntry(*requestData, "CONTENT_TYPE: ", E_CGI_CONTENT_TYPE);
    temp += getBaseEntry(*requestData, "GATEWAY_INTERFACE: ", E_CGI_GATEWAY_INTERFACE);
    temp += getBaseEntry(*requestData, "PATH_INFO: ", E_CGI_PATH_INFO);
    temp += getBaseEntry(*requestData, "PATH_TRANSLATED: ", E_CGI_PATH_TRANSLATED);
    temp += getBaseEntry(*requestData, "QUERY_STRING: ", E_CGI_QUERY_STRING);
    temp += getBaseEntry(*requestData, "REMOTE_ADDR: ", E_CGI_REMOTE_ADDR);
    temp += getBaseEntry(*requestData, "REMOTE_HOST: ", E_CGI_REMOTE_HOST);
    temp += getBaseEntry(*requestData, "REMOTE_IDENT: ", E_CGI_REMOTE_IDENT);
    temp += getBaseEntry(*requestData, "REMOTE_USER: ", E_CGI_REMOTE_USER);
    temp += getBaseEntry(*requestData, "REQUEST_METHOD: ", E_CGI_REQUEST_METHOD);
    temp += getBaseEntry(*requestData, "SCRIPT_NAME: ", E_CGI_SCRIPT_NAME);
    temp += getBaseEntry(*requestData, "SERVER_NAME: ", E_CGI_SERVER_NAME);
    temp += getBaseEntry(*requestData, "SERVER_PORT: ", E_CGI_SERVER_PORT);
    temp += getBaseEntry(*requestData, "SERVER_PROTOCOL: ", E_CGI_SERVER_PROTOCOL);
    temp += getBaseEntry(*requestData, "SERVER_SOFTWARE: ", E_CGI_SERVER_SOFTWARE);

    // EXTRA ENTRIES
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY1");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY2");
    temp += getExtraEntry(*requestData, "CUSTOM_ENTRY3");
    
    StringUtils::move(proto.m_ExpectedOutput, temp);
}
