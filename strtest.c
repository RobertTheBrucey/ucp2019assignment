#include <stdio.h>
int main()
{
	char x[100], y[100];
	int read;
	while (1)
	{
		read = scanf(" %[^,],%[^\n]", x, y);
		if (read != 2)
		{
			while (getchar() != '\n');
		}
		printf("read = %d, x = %s, y = %s\n", read, x, y);
	}

	return 0;
}
