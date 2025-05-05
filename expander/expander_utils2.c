
#include "../minishell.h"

char	*parse_dquote_end(char *res, const char *str, int *i)
{
	char	*tmp;
	char	*quote;

	if (str[*i] == '"')
		(*i)++;
	quote = ft_strdup("\"");
	if (!quote)
		return (free(res), NULL);
	tmp = str_append(res, quote);
	if (!tmp)
	{
		free(quote);
		return (free(res), NULL);
	}
	free(quote);
	return (tmp);
}

char	*parse_double_quote(const char *str, int *i, t_env *env, char *argv, int *last_exit_status)
{
	char	*res;
	char	*frag;
	char	*tmp;

	if (!(res = ft_strdup("\"")))
		return (NULL);
	(*i)++;
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
			frag = expand_env(str, i, env, argv, last_exit_status);
		else if (!(frag = ft_calloc(2, sizeof(char))))
			return (free(res), NULL);
		else
			frag[0] = str[(*i)++];
		tmp = str_append(res, frag);
		free(frag);
		if (!tmp)
			return (free(res), NULL);
		res = tmp;
	}
	return (parse_dquote_end(res, str, i));
}

char	*expand_all_parts(const char *str, t_env *env, char *argv, int *last_exit_status)
{
	int		i;
	char	*res;
	char	*part;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'')
			part = parse_single_quote(str, &i);
		else if (str[i] == '"')
			part = parse_double_quote(str, &i, env, argv, last_exit_status);
		else
			part = parse_unquoted(str, &i, env, argv, last_exit_status);
		if (!part)
			return (free(res), NULL);
		res = str_append(res, part);
		free(part);
		if (!res)
			return (NULL);
	}
	return (res);
}

char	*expand_all(char *input, t_env *env, char *argv, int *last_exit_status)
{
	if (!input)
		return (NULL);
	if (has_unclosed_quote(input))
	{
		ft_putendl_fd("syntax error: unclosed quote", 2);
		return (NULL);
	}
	return (expand_all_parts(input, env, argv, last_exit_status));
}