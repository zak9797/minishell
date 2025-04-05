#include"minishell.h"

void    Getcwd(char *buf, size_t size)
{
    if(getcwd(buf, size) == NULL)
    perror("getcwd failed");
}