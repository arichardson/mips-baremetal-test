#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
	printf("Main called with argc=%d, argv=%p\n", argc, argv);
	for (int i = 0; i < argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	printf("Main called with envp=%p\n", envp);
	for (int i = 0; envp[i]; i++) {
		printf("env[%d] = %s\n", i, envp[i]);
	}
	return argc;
}
