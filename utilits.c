#include"minishell.h"

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

char	**command_matrix(char *argv)
{
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if (cmd == NULL)
	{
		perror("cmd allocate");
		exit(EXIT_FAILURE);
	}
	return (cmd);
}

void	error_norm(char *text)
{
	perror(text);
	exit(EXIT_FAILURE);
}

char	**check_cmd_path(char **cmd, char **envp)
{
	char		*path_env;
	char		*temp;

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
		path_env = get_env_path(envp);
		temp = check_cmd_exist(cmd, path_env);
		if (temp == NULL)
			return (NULL);
		free(cmd[0]);
		cmd[0] = temp;
	}
	return (cmd);
}

char	*get_env_path(char **envp)
{
	int	i;
	int	list;

	i = 0;
	list = ft_strlen_d(envp);
	while (i < list)
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			return (envp[i]);
		i++;
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