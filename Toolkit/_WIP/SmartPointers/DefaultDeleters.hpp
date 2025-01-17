

#ifndef DEFAULTDELETERS_HPP

# define DEFAULTDELETERS_HPP

template<typename T>
struct DefaultDeleter
{
    void operator()(T* ptr) const {
        delete ptr;
    }
};

template<typename T>
struct DefaultDeleterArray
{
    void operator()(T* ptr) const {
        delete[] ptr;
    }
};

#endif