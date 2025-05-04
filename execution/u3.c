#include "minishell.h"

char *search_executable_in_dirs(char **cmd, char **dir)
{
	size_t	i;
	char	*temp;
	char	*path;

	i = 0;
	while (dir[i])
	{
		temp = ft_strjoin(dir[i++], "/");
		if (temp == NULL)
			return (free_norm(dir, NULL));
		path = ft_strjoin(temp, cmd[0]);
		free(temp);
		if (path == NULL)
			return (free_norm(dir, NULL));
		if (access(path, F_OK | X_OK) == 0)
			return (free_norm(dir, path));
		free(path);
	}
	free_split(dir);
	return (NULL);
}

char *check_cmd_exist(char **cmd, char *path_env)
{
	char	**dir;
	char	*path;

	dir = get_directories(cmd, path_env);
	if (dir == NULL)
		return (NULL);

	path = search_executable_in_dirs(cmd, dir);
	if (path)
		return (path);

	// g_shell.last_exit_status = 127;
	char *tmp = ft_strjoin(cmd[0], ": command not found\n");
	write(2, tmp, ft_strlen(tmp));
	free(tmp);
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
