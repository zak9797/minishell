
#include "../minishell.h"

char	*str_append(char *dst, const char *src)
{
	char	*new;

	if (!dst || !src)
		return (NULL);
	new = malloc(ft_strlen(dst) + ft_strlen(src) + 1);
	if (!new)
		return (free(dst), NULL);
	strcpy(new, dst);
	strcat(new, src);
	free(dst);
	return (new);
}
char *expand_numeric_var(const char *str, int *i, char *argv)
{
	char *result = NULL;

	// Read first digit after $
	char digit = str[*i];
	(*i)++;

	if (digit == '0')
		result = ft_strdup(argv);  // or "" if you prefer
	else
		result = ft_strdup("");  // $1–$9 return empty unless you implement args

	return result;
}

char	*expand_env(const char *str, int *i, t_env *env, char *argv)
{
	int		start;
	char	*key;
	char	*val;

	(*i)++;

	// 👇 Handle digits after $
	if (ft_isdigit(str[*i]))
		return expand_numeric_var(str, i, argv);

	start = *i;

	// Normal env var parsing
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (start == *i)
		return (ft_strdup("$"));

	key = ft_strndup(str + start, *i - start);
	if (!key)
		return (NULL);

	val = get_env_val(env, key);
	free(key);

	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

char	*parse_single_quote(const char *str, int *i)
{
	int		start;
	char	*content;
	char	*res;

	start = ++(*i);
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	content = ft_strndup(str + start, *i - start);
	if (!content)
		return (NULL);
	if (str[*i] == '\'')
		(*i)++;
	res = str_append(ft_strdup("'"), content);
	if (!res)
		return (free(content), NULL);
	res = str_append(res, ft_strdup("'"));
	free(content);
	return (res);
}

char	*parse_unquoted(const char *str, int *i, t_env *env, char *argv)
{
	char	*res;

	if (str[*i] == '$')
		return (expand_env(str, i, env, argv));
	res = ft_calloc(2, sizeof(char));
	if (!res)
		return (NULL);
	res[0] = str[(*i)++];
	return (res);
}

int	has_unclosed_quote(const char *str)
{
	int	i;
	int	single;
	int	dquote;

	i = 0;
	single = 0;
	dquote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && dquote % 2 == 0)
			single++;
		else if (str[i] == '"' && single % 2 == 0)
			dquote++;
		i++;
	}
	return (single % 2 || dquote % 2);
}