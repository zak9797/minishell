#include "minishell.h"

void	exit_program_leak(char **ptr1, int fd1, int fd2)
{
	if (ptr1 != NULL)
		ft_free(ptr1);
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
	write(STDERR_FILENO, "Error: program couldn't proceed\n", \
	ft_strlen("Error: program couldn't proceed\n"));
	exit(EXIT_FAILURE);
}
size_t	ft_strlen_d(char **s)
{
	size_t	n;

	n = 0;
	if (s == NULL || *s == NULL)
		return (0);
	while (s[n] != NULL)
		n++;
	return (n);
}

char	*ft_strtrim_start(char const *s1, char const *set)
{
	int		t;
	char	*ptr;
	int		r;

	r = 0;
	t = ft_strlen(s1);
	while (s1[r] && ft_strchr(set, s1[r]))
		r++;
	ptr = malloc(t - r + 1);
	if (!ptr)
		return (NULL);
	ft_memcpy((void *)ptr, &s1[r], t - r);
	if (!ptr)
		return (NULL);
	ptr[t - r] = '\0';
	return (ptr);
}

void	ft_free(char **matrix)
{
	size_t	row;

	row = 0;
	if (!matrix)
		return ;
	if (!(*matrix))
		return (free(matrix));
	while (row < ft_strlen_d(matrix))
	{
		free (matrix[row]);
		row++;
	}
	free (matrix);
}

char **command_matrix(t_token *tokens)
{
    int count = 0;
    t_token *current = tokens;

    // Count how many valid tokens we have
    while (current != NULL) {
        if (current->value && strlen(current->value) > 0)
            count++;
        current = current->next;
    }

    // Allocate memory for the command array (with NULL terminator)
    char **cmd = malloc((count + 1) * sizeof(char *));
    if (cmd == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Fill the command array with valid tokens
    current = tokens;
    int i = 0;
    while (current != NULL) {
        if (current->value && ft_strlen(current->value) > 0) {
            cmd[i] = strdup(current->value);  // Copy the token's value
            if (cmd[i] == NULL) {
                perror("strdup failed");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        current = current->next;
    }

    cmd[i] = NULL;  // NULL-terminate the array
    return cmd;
}

void	error_norm(char *text)
{
	perror(text);
	exit(EXIT_FAILURE);
}

char	**check_cmd_path(char **cmd, t_env *env)
{
	char	*path_env;
	char	*temp;

	path_env = NULL;
	if (cmd == NULL || *cmd == NULL)
		return (cmd);
	if (ft_strncmp(cmd[0], ".", 1) == 0 || ft_strncmp(cmd[0], "/", 1) == 0)
	{
		if (access(cmd[0], F_OK | X_OK) != 0)
			printf("Error: %s: %s\n", cmd[0], strerror(errno));
	}
	else
	{
		path_env = get_env_path(env); // Modified for t_env
		temp = check_cmd_exist(cmd, path_env);
		if (temp == NULL)
			return (NULL);
		free(cmd[0]);
		cmd[0] = temp;
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

char	*check_cmd_exist(char **cmd, char *path_env)
{
	char		**dir;
	char		*path;
	char		*temp;
	size_t		i;

	dir = get_directories(cmd, path_env);
	if (dir == NULL)
		return (NULL);
	i = 0;
	while (dir[i])
	{
		temp = ft_strjoin(dir[i++], "/");
		if (temp == NULL)
			return(free_norm (dir, NULL));
		path = ft_strjoin(temp, cmd[0]);
		free(temp);
		if (path == NULL)
			return(free_norm (dir, NULL));
		if (access(path, F_OK | X_OK) == 0)
			return(free_norm (dir, path));
		free(path);
	}
	write(2, cmd[0], ft_strlen(cmd[0]));
	write(2, ": command not found\n", ft_strlen(": command not found\n"));
	return (NULL);
}

char	*free_norm(char **ptr, char *ret)
{
	if (ptr != NULL)
		ft_free(ptr);
	return (ret);
}
void	exit_program(char **ptr, int fd1, int fd2)
{
	if (ptr != NULL)
		ft_free(ptr);
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
	perror("Error");
	exit(EXIT_FAILURE);
}

void	dupfd(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		exit_program(NULL, -1, oldfd);
	close(oldfd);
}


void execute_simple(char **arg, t_env *env)
{
	char **arr = env_list_to_array(env);
	 //t_env *envp = init_env(env);
	t_token *tokens = tokenizee_input(arg[0]);
     pid_t id;
    char **args;
    char **cmd;
	int redir_fd = redirect_for_builtin(tokens);
	 t_token *cleaned = clean_command_tokens(tokens);
	// print_tokens(cleaned);
if (check_cmd(tokens))
{

	execute_builtin(cleaned, env);
	restore_stdio(redir_fd);
}
else {
    id = fork();

    if (id == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
	
    else if (id == 0)
	{
		printf("here\n");
		 cmd = command_matrix(cleaned);	  
        args = check_cmd_path(cmd, env);
		// if (!args || !args[0]) {
		// 	fprintf(stderr, "command not found\n");
		// 	exit(127); // Same exit code bash uses for "command not found"
		// }
		if (access(args[0], X_OK) == 0)
			execve(args[0], cmd, arr);
        perror("execve failed");
        exit(EXIT_FAILURE);
	} 
	else
	{
		restore_stdio(redir_fd);
        int status;
        waitpid(id, &status, 0); 
	}
}
}

char **env_list_to_array(t_env *env)
{
	int		i = 0;
	int		count = 0;
	t_env	*tmp = env;
	char	**envp;
	char	*tmp_str;

	// Count number of variables
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}

	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);

	tmp = env;
	while (tmp)
	{
		tmp_str = ft_strjoin(tmp->key, "=");
		envp[i] = ft_strjoin(tmp_str, tmp->value);
		free(tmp_str);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void execute_complex(char **args, t_env *env)
{
    int i = 0;
    int prev_fd = -1;
    pid_t pids[256]; // store pids to wait later
    pid_t pid;
    int num_cmds = 0;
	process_heredocs(args);
    while (args[i])
    {
        t_token *token = tokenizee_input(args[i]);
        int pipefd[2];
        if (args[i + 1] != NULL && pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == 0)
        {
			if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (args[i + 1] != NULL)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }
			int heredoc_fd = open(".heredoc_tmp", O_RDONLY);
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
            handle_redirections(token, env);
            if (check_cmd(token) && !args[1])
            {
                execute_builtin(token, env);
                exit(0);
            }
            else
            {
                char **cmd = command_matrix(clean_command_tokens(token));
                char **exec_path = check_cmd_path(cmd, env);
                char **arr = env_list_to_array(env);
                execve(exec_path[0], cmd, arr);
                // perror("execve");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            pids[num_cmds++] = pid;
             if (prev_fd != -1) close(prev_fd);

            if (args[i + 1] != NULL)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0]; // for next child
            }
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        i++;
    }

	for (int j = 0; j < num_cmds; j++)
	waitpid(pids[j], NULL, 0); 
	unlink(".heredoc_tmp");
}