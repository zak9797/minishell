#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>  
#include <stdbool.h> 

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_OUTPUT_REDIRECT,
	T_APPEND_OUTPUT,
	T_INPUT_REDIRECT,
	T_HEREDOC,
	T_BUILTIN_COMMAND  // New type for built-in commands
}	t_token_type;


//tokinzer
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;


// Function declarations
struct s_token	*tokenize_input(char *input);
void			print_tokens(struct s_token *head);
void			free_tokens(struct s_token *head);

//builtins
int check_cmd(t_token *cmd_token);
void execute_builtin(t_token *tokens, t_env *env);
void execute_pwd(t_env *env);
void execute_env(t_env *env);
void execute_cd(t_token *arg, t_env *env);
void execute_echo(t_token *arg, t_env *env);
char *get_env_val(t_env *env, const char *key);
char **env_cpy(t_env *env);
t_env *init_env(char **envp);
void free_env_copy(char **env);
void free_env(t_env *env);

//parsing
//t_cmd *token_list_to_cmds(t_token *tokens);
//t_cmd *create_cmd_node(t_token *tokens);
//char **token_list_to_args(t_token *tokens);




void    Getcwd(char *buf, size_t size);
char	**check_cmd_path(char **cmd, char **envp);
char	*get_env_path(char **envp);
char	**get_directories(char **cmd, char *path_env);
char	*check_cmd_exist(char **cmd, char *path_env);
char	*free_norm(char **ptr, char *ret);
void	ft_free(char **matrix);
char	*free_norm(char **ptr, char *ret);



