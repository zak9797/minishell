#include "../minishell.h"

void handle_exit(t_token *token) {
	int status = 0;

	printf("exit\n");

	t_token *arg_token = token->next;

	if (arg_token) {
		char *arg = arg_token->value;

		// Validate numeric argument
		int i = 0;
		if (arg[i] == '+' || arg[i] == '-') i++; // optional sign
		while (arg[i]) {
			if (!isdigit(arg[i])) {
				fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
				exit(255);
			}
			i++;
		}

		if (arg_token->next) {
			// Too many arguments
			fprintf(stderr, "minishell: exit: too many arguments\n");
			// Normally you'd set a global error var here like g_exit_status = 1;
			return;
		}

		status = atoi(arg) % 256;
	}

	exit(status);
}

void execute_exit(t_token *token) {
	if (!token || !token->value)
		return;

	if (strcmp(token->value, "exit") == 0) {
		handle_exit(token);
		return;
	}

	// Handle other commands...
}