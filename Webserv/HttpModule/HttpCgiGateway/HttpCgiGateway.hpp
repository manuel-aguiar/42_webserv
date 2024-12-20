/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCgiGateway.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 08:44:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 12:11:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCGIGATEWAY_CPP

# define HTTPCGIGATEWAY_CPP

// Project headers
# include "../../CgiModule/Cgi_Definitions.h"

//forward declarations
class Callback;
class HttpRequest;

class HttpCgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onRead(Callback& Callback);
		static void onWrite(Callback& Callback);
		static void onError(Callback& Callback);
		static void onClose(Callback& Callback);
		static void onTimeout(Callback& Callback);

		static void (*Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback);

		// Implementation of Callbacks
		static void CgiOnRead(HttpRequest& request);
		static void CgiOnWrite(HttpRequest& request);
		static void CgiOnError(HttpRequest& request);
		static void CgiOnClose(HttpRequest& request);
		static void CgiOnTimeout(HttpRequest& request);
};



#endif