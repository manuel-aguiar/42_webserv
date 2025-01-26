/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:52:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/26 22:44:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_H

# define CGIMODULE_H

# include "m_CgiNamespace.h"
# include "CgiModule/CgiModule.hpp"
# include "CgiRequest/CgiRequest.hpp"

// some typedefs that may be useful for the user
typedef Cgi::ImplModule								CgiModule;
typedef Cgi::ImplModule::Request					Request;
typedef Cgi::ImplModule::Request					CgiRequest;
typedef Cgi::ImplModule::Options					CgiOptions;
typedef Cgi::ImplModule::User						CgiUser;
typedef Cgi::ImplModule::IO							CgiIO;
typedef Cgi::ImplModule::Notify						CgiNotify;
typedef Cgi::ImplModule::EnvKey						CgiEnvKey;
typedef Cgi::ImplModule::EnvValue					CgiEnvValue;

typedef Cgi::ImplModule::ScriptPath					CgiScriptPath;
typedef Cgi::ImplModule::InterpExtension			CgiInterpExtension;
typedef Cgi::ImplModule::InterpPath					CgiInterpPath;
typedef Cgi::ImplModule::EnvVariables				CgiEnvVariables;

typedef Cgi::Env::Enum							    CgiEnvEnum;

#endif