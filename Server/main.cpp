#include <cstdio>
#include <unistd.h>

int main()
{
	printf("hello from Server!\n");
	for (;;) sleep(1.0f);
	return 0;
}