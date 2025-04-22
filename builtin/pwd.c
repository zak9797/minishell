#include "../minishell.h"

void execute_pwd(t_env *env)
{
    char *pwd = get_env_val(env, "PWD");  // Use your internal env list

    if (pwd != NULL)
    {
        printf("%s\n", pwd);
    }
    else
    {
        perror("PWD not found in internal environment");
    }
}