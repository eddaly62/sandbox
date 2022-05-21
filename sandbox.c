#include <stdio.h>


#define ASCII_ESC (27)


int send(void)
{
	fprintf(stdout, "Hello World\n");
//	printf("%c%s", (char)ASCII_ESC, "[H"); // move cursor to upper left corner
//	printf("%c%s", (char)ASCII_ESC, "#8"); // Screen alighnment display
//	printf("%c%s", (char)ASCII_ESC, "[?1;4c"); // request DA, graphics option
//	printf("%c%s", (char)ASCII_ESC, "[?5n"); // request DSR
}

int listen(void)
{
	char sreturn[100];
//	fscanf(stdin, "%s", sreturn);
//	printf("%s\n", sreturn);
}


int main(int argc, char *argv[])
{
	send();

}



