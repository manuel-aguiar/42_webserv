
# include <cstddef>



size_t mystrlen(const char* str)
{
	size_t len = 0;
	while (*str++)
		len++;
	return len;
}