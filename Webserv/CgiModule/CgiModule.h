/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:52:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/20 17:08:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_H

# define CGIMODULE_H

# include "CgiNamespace.h"
# include "CgiModule/CgiModule.hpp"
# include "CgiRequest/CgiRequest.hpp"

// some typedefs that may be useful for the user
typedef Cgi::Module								CgiModule;
typedef Cgi::Module::Request					Request;
typedef Cgi::Module::Request					CgiRequest;
typedef Cgi::Module::Options					CgiOptions;
typedef Cgi::Module::User						CgiUser;
typedef Cgi::Module::IO_Callback				CgiIO_Callback;
typedef Cgi::Module::Runtime_Callback			CgiRuntime_Callback;
typedef Cgi::Module::EnvKey						CgiEnvKey;
typedef Cgi::Module::EnvValue					CgiEnvValue;

typedef Cgi::Module::InterpExtension			CgiExtension;
typedef Cgi::Module::InterpPath					CgiPath;
typedef Cgi::Module::EnvVariables				CgiEnvVariables;

typedef Cgi::Env::Enum							CgiEnvEnum;

#endif