#include "../minishell.h"

void	execute_pwd(t_env *env)
{
	char	*pwd;

	if (!env)
	{
		ft_putstr_fd("minishell: env is NULL\n", 2);
		return ;
	}
	pwd = get_env_val(env, "PWD");
	if (pwd)
		printf("%s\n", pwd);
	else
		ft_putstr_fd("minishell: PWD not found in environment\n", 2);
}
