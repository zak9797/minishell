#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>  
typedef struct s_token
{
    char *value;
    char *type;
    struct s_token *next;
}   t_token;

void    Getcwd(char *buf, size_t size);

