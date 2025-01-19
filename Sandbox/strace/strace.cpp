

#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char str[] = "malloc starts";

	write(1, str, sizeof(str));

	int *cenas = (int *)malloc(5);

	write(1, str, sizeof(str));
	free(cenas);
	(void)cenas;
	return (0);
}