

# include "test.h"

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string FileLineFunction(const char* file, const int line, const char* function)
{
    return std::string("\tFile: ") + file + "\n\tLine: " + to_string(line) + "\n\tFunction: " + function + '\n';
}