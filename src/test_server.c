#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void hello(char * input, char *output) {
	printf("input: %s\n", input);
	strcpy(output, input);
	printf("function returning with output: %s\n", output);
}


int main(int argc, char const *argv[]) {
	void (*funcs[2])(char *, char *);
	funcs[0] = hello;
	char * message = "{\"function\": \"hello\", \"parameter\": \"world\"}";
	char *funcs_table[] = {"hello"};
	message_receive(funcs_table, funcs, 1);
	return 0;
}
