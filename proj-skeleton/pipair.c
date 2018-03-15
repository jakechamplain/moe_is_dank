#include <stdio.h>

int main (int argc, char* argv[]) {
	int support = (int)argv[1] - 48;
	int confidence = (int)argv[2] - 48;
	printf("%d",support);
	printf("%d", confidence);
	return 0;
}