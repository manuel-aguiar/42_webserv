/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticAssert.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:13:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 15:27:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef STATIC_ASSERT_HPP

# define STATIC_ASSERT_HPP

    #ifndef NDEBUG

        template<bool>
        struct StaticAssert;

        template<>
        struct StaticAssert<true> {};

        // Use a dummy declaration that the compiler cannot optimize out.
        #define STATIC_ASSERT(condition, message) \
            enum { STATIC_ASSERT_##message = sizeof(StaticAssert<(condition)>) }

    #endif

# endif

/*
    Thanks chatgpt. If the condition is false, the template fails to expand and results in a compile error,
    and you will see the "message" (sort of, enough to make it clear)
    THIS ONLY WORKS FOR CONSTANT EXPRESSIONS, everything else at runtime must be found with assert()
*/