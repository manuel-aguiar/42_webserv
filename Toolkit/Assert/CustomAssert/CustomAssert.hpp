/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomAssert.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:48:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 11:22:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUSTOMASSERT_HPP

# define CUSTOMASSERT_HPP

	#ifndef NDEBUG

		# include <cassert>
		
	    #define CUSTOM_ASSERT(expr, msg) \
		if (!(expr)) { \
		    std::cerr << "Assertion failed: " << #expr << "\n" \
			    << "Message: " << msg << "\n" \
			    << "File: " << __FILE__ << "\n" \
			    << "Line: " << __LINE__ << std::endl; \
		    assert(expr); \
		}

	#endif

#endif