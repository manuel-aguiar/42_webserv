/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 08:48:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 09:01:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../../Toolkit/Arrays/DynArray/DynArray.hpp"

class StringUtils {
public:
    static void move(std::string& dest, std::string& src) {
        dest.swap(src);
        src.clear();
    }
};

std::string func()
{
    return ("This is a long string that will be moved");
}

int main(void)
{
    DynArray<std::string> arr;
    arr.reserve(2);

    // Use a temporary to hold the result of func()
    std::string temp = func();

    // Move the string into the DynArray
    arr.emplace_back();                // Add an empty string to arr[0]
    StringUtils::move(arr[0], temp);   // Move the content into arr[0]

    arr.emplace_back();                // Add another empty string to arr[1]
    StringUtils::move(arr[1], arr[0]); // Move the content of arr[0] into arr[1]

    return 0;
}