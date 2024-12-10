/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservDefaultConfig.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:19:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/19 13:06:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVDEFAULTCONFIG_H

# define WEBSERVDEFAULTCONFIG_H

# define DEFAULTCONF_HTTP_MAX_CLIENT_BODY_SIZE "1000000"
# define DEFAULTCONF_HTTP_MAX_CLIENT_HEADER_SIZE "1000000"
# define DEFAULTCONF_BLOCK_IP_LISTEN "0.0.0.0"
# define DEFAULTCONF_BLOCK_MAX_CONNECTIONS "10"
# define DEFAULTCONF_BLOCK_MAX_CGI "10"

// add the remaining error pages
# define DEFAULTCONF_BLOCK_ERROR_400_PAGE "../StaticAssets/ErrorPages/error_400.html"
# define DEFAULTCONF_BLOCK_ERROR_404_PAGE "../StaticAssets/ErrorPages/error_404.html"

# define DEFAULTCONF_LOCATION_AUTOINDEX "0";
# define DEFAULTCONF_LOCATION_METHODS "get post delete"; // Methods separated by spaces, lowercase 
# define DEFAULTCONF_LOCATION_TYPE "regular";

#endif
