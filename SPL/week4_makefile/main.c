#include <stdio.h>

int plus(const int a, const int b);
int minus(const int a, const int b);

int main()
{
	int a, b;
	scanf("%d %d", &a, &b);

	printf("%d %d\n", plus(a,b), minus(a,b ));
	
	return 0;
}
