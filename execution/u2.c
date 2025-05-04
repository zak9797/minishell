#include "minishell.h"


char **command_matrix(t_token *tokens)
{
    int count = 0;
    t_token *current = tokens;

    count = count_tokens(current);  // Count the tokens
    char **cmd = malloc((count + 1) * sizeof(char *));  // Allocate space for the command matrix
    if (cmd == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    current = tokens;
    int i = 0;
    while (current != NULL) {
        if (current->value && ft_strlen(current->value) > 0) {  // Check if the token has a value
            cmd[i] = strdup(current->value);  // Duplicate the value to the command matrix
            if (cmd[i] == NULL) {
                // Free previously allocated memory before exiting
                while (i > 0) {
                    free(cmd[--i]);
                }
                free(cmd);
                perror("strdup failed");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        current = current->next;  // Move to the next token
    }

    cmd[i] = NULL;  // Null-terminate the command matrix
    return cmd;
}

void	error_norm(char *text)
{
	perror(text);
	exit(EXIT_FAILURE);
}

char	**check_cmd_path(char **cmd, t_env *env, int last_exit_status)
{
	char	*path_env;
	char	*temp;

	path_env = NULL;
	if (cmd == NULL || *cmd == NULL)
		return (cmd);

	// Check for absolute or relative path (e.g., "./", "/bin/ls")
	if (ft_strncmp(cmd[0], ".", 1) == 0 || ft_strncmp(cmd[0], "/", 1) == 0)
	{
		if (access(cmd[0], F_OK | X_OK) != 0)
		{
			last_exit_status = 127;
			printf("Error: %s: %s\n", cmd[0], strerror(errno));
		}
	}
	else
	{
		path_env = get_env_path(env);
		if (path_env != NULL)
		{
			temp = check_cmd_exist(cmd, path_env);
			if (temp == NULL)
			{
				// Command not found in PATH
				last_exit_status = 127;
				return (NULL);
			}
			free(cmd[0]);
			cmd[0] = temp;
		}
		else
		{
			// PATH environment variable is not set
			last_exit_status = 127;
			return (NULL);
		}
	}
	return (cmd);
}

char	*get_env_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	// g_shell.last_exit_status = 127;
	return (NULL);
}

char	**get_directories(char **cmd, char *path_env)
{
	char		**dir;
	char		*path_var;

	path_var = ft_strtrim_start(path_env, "PATH=");
	if (path_var == NULL)
		exit_program_leak(cmd, -1, -1);
	dir = ft_split(path_var, ':');
	if (path_var)
		free(path_var);
	if (dir == NULL)
		exit_program_leak(cmd, -1, -1);
	return (dir);
}
