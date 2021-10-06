#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void hello(char * input, char *output) {
	output = input;
}


int main(int argc, char const *argv[]) {
	void (*funcs[1])(char *, char *);
	funcs[1] = hello;
	char * message = "{\"function\": \"hello\", \"parameter\": \"world\"}";
	message_send("127.0.0.1", message);


}
