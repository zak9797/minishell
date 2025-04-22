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
	T_COMMAND,
	T_BUILTIN_COMMAND,
	T_ARG,
	T_WORD,
	T_PIPE,
	T_OUTPUT_REDIRECT,
	T_APPEND_OUTPUT,
	T_INPUT_REDIRECT,
	T_HEREDOC,
}	t_token_type;


//tokinzer
typedef enum e_quote_type {
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_quote_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
	t_quote_type quote;
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
void execute_export(t_token *token, t_env *env);
void execute_pwd(t_env *env);
void execute_env(t_env *env);
void execute_cd(t_token *arg, t_env *env);
void execute_echo(t_token *arg, t_env *env);
char *get_env_val(t_env *env, const char *key);
void set_env_val(t_env *env, const char *key, const char *val);
char **env_cpy(t_env *env);
t_env *init_env(char **envp);
void free_env_copy(char **env);
void free_env(t_env *env);

//expander
char *expand_variables(const char *input, t_env *env);
char *expand_all(char *input, t_env *env);
char *handle_double_quotes(const char *input, int *i, t_env *env);
char *handle_single_quotes(const char *input, int *i);
char *expand_variables(const char *input, t_env *env);


//toknizer
t_token *tokenizee_input(char *input);
 void	add_token(t_token **head, t_token *new);
 t_token *create_token(t_token_type type, char *value);
 t_token_type get_token_type(char *input, int i);



//external
void execute_simple(char **arg, t_env *env);
void execute_complex(char **args, t_env *env);
char **env_list_to_array(t_env *env);
// redirection
t_token *handle_redirections(t_token *tokens , t_env *env);
int	redirect_for_builtin(t_token *tokens);
void	restore_stdio(int packed_fd);
t_token *clean_command_tokens(t_token *tokens);
// char **handle_redirectionss(char **args, t_env *env);




void    Getcwd(char *buf, size_t size);
char	**check_cmd_path(char **cmd, t_env *env);
char	*get_env_path(t_env *env);
char	**get_directories(char **cmd, char *path_env);
char	*check_cmd_exist(char **cmd, char *path_env);
char	*free_norm(char **ptr, char *ret);
void	ft_free(char **matrix);
char	*free_norm(char **ptr, char *ret);



