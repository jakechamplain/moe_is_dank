#include <cstdlib>
#include <stdio.h>

int main (int argc, char* argv[]) {
	int support = atoi(argv[1]);
	int confidence = atoi(argv[2]);
	printf("%d",support);
	printf("%d", confidence);
	return 0;
}