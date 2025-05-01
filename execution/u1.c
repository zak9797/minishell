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
int count_tokens(t_token *tokens) {
    int count = 0;
    while (tokens != NULL) {
        if (tokens->value && strlen(tokens->value) > 0)
            count++;
        tokens = tokens->next;
    }
    return count;
}