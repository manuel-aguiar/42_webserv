/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:47:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 12:29:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../CgiModule/CgiModule.hpp"

// test helpers
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../../Toolkit/_Tests/test.h"

//C++ headers
# include <iostream>

// C headers
#include <unistd.h>

extern void prepareExpectedOutput(bool isExpectedValid, A_ProtoRequest& proto);

extern std::vector<std::string> g_mockGlobals_ErrorMsgs;

int TestPart2(int testNumber)
{

	return (testNumber);
}