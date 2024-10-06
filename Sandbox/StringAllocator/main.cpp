#include <iostream>
#include <string>
#include <limits>  // For std::numeric_limits<size_t>
#include <cstddef> // For std::ptrdiff_t

#include "../../Webserv_sketch/MemoryPool/MemoryPool.h"

// In C++98, explicit template specialization requires a typedef
typedef std::basic_string<char, std::char_traits<char>, StringAllocator<char> > StringWithPoolAllocator;

#include <map>

int main() {
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);  // Create a memory pool

    // Create a string using the custom allocator
    StringAllocator<char> alloc(*pool);
    StringWithPoolAllocator str("Hello, Memory Pool!", alloc);
    StringWithPoolAllocator str2("HelloA", alloc);
    std::cout << str << "   " << str2 << std::endl;

    StringWithPoolAllocator str3(str);
    str3 += "yoyoyoyoyoyoyoyoy";

    std::map<int, StringWithPoolAllocator > myMap;
    
    std::pair<std::map<int, StringWithPoolAllocator >::iterator, bool> result = myMap.insert(
    std::make_pair(3, StringWithPoolAllocator("potato", alloc)));

    std::cout << "getting c_str" << std::endl;
    const char* ptr = str3.c_str();

    std::cout << "String: " << ptr << std::endl;

    pool->destroy();

    return 0;
}