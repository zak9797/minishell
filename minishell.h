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
#include <signal.h>
# include <sys/signal.h>
# include <bits/sigaction.h>

extern volatile sig_atomic_t	g_sig_int;

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
	int heredoc_fd;
	int exit_status;
}	t_token;




typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

// void init_signals(void);         // Declare these if you define them elsewhere
// int  check_signal(t_token *token);

// Function declarations
extern volatile sig_atomic_t	g_sig_int;
void				handle_sigint(int sig);
void				init_signals(void);
int					check_signal();
int	handle_signals_and_input(char ***args, t_env *env, int *last_exit_status, char *argv);

struct s_token	*tokenize_input(char *input);
void			print_tokens(struct s_token *head);
void			free_tokens(struct s_token *head);

//builtins
char	*get_env_val(t_env *env, char *key);
void update_existing_key(t_env *env, char *key, char *val);
void append_new_key(t_env *head, char *key, char *val);
void	set_env_val(t_env *env, char *key, char *val);
void	update_pwd_env(t_env *env, char *oldpwd);
char *expand_home_path(char *path, t_env *env);
char *resolve_cd_path(t_token *arg, t_env *env);
void execute_cd(t_token *arg, t_env *env);
/*
******************************builtin_cnd****************************************
*/
void execute_exit(t_token *token);
int	is_builtin(const char *cmd);
int	check_cmd(t_token *cmd_token);
int dispatch_builtin(t_token *cmd_token, t_env *env, int last_exit_status);
int execute_builtin(t_token *tokens, t_env *env, int last_exit_status);
/*
**************************************echo*******************************
 */
int	is_n_flag(const char *str);
int 	print_arg_value(t_token *arg, int last_exit_status);
int execute_echo(t_token *arg, t_env *env, int last_exit_status);

/*
**************************************env*******************************
 */
void	free_env(t_env *env);
t_env	*init_env(char **envp);
t_env	*new_env_node(char *key, char *value);
void	free_env_copy(char **env);
int	env_count(t_env *env);
char	*build_env_line(t_env *e);
char	**env_cpy(t_env *env);
void	execute_env(t_env *env);
/*
**************************************export*******************************
 */
char	*strip_quotes(const char *val);
int	is_valid_export_arg(char *arg);
void	print_export_env(t_env *env);
void	handle_assign(char *arg, t_env *env);
void	execute_export(t_token *token, t_env *env);

/*
**************************************pwd*******************************
 */
void	execute_pwd(t_env *env);
/*
**************************************unset*******************************
 */
void	unset_env_var(t_env *env, t_token *token);

//expander
char *str_append(char *dst, const char *src);
char *expand_env(const char *str, int *i, t_env *env, char *argv);
char *parse_single_quote(const char *str, int *i);  // OK — no argv needed
char *parse_unquoted(const char *str, int *i, t_env *env, char *argv); // <-- FIXED
int has_unclosed_quote(const char *str);  // OK
char *parse_dquote_end(char *res, const char *str, int *i); // <-- FIXED
char *parse_double_quote(const char *str, int *i, t_env *env); // <-- FIXED
char *expand_all_parts(const char *str, t_env *env, char *argv); // <-- FIXED
char *expand_all(char *input, t_env *env, char *argv);



//toknizer
t_token *process_input_part1(char *input, char **current_word, int *i, t_token **tokens);
t_token *process_input_loop(char *input, char **current_word);
t_token *tokenize_input(char *input);
void handle_double_redirection(char **current_word, t_token **tokens, int *i, char *input);
void handle_single_operator(char **current_word, t_token **tokens, int *i, char *input);
void free_split(char **arr);
t_token *create_token(t_token_type type, char *value, t_quote_type quote_type);
t_token_type get_token_type(char *input, int i);
char *str_append1(char *s1, char *s2);
void handle_whitespace(char **current_word, t_token **tokens, int *i, char *input);
void handle_quotes(char **current_word, int *i, char *input);
void init_noninteractive_signals(void);
void add_token(t_token **head, t_token *new);
void freee_tokens(t_token *head);
void free_env_list(t_env *env);

//external
int execute_simple(char **arg, t_env *env,int last_exit_status);
int execute_complex(char **args, t_env *env,int last_exit_status);
char **env_list_to_array(t_env *env);
int wait_for_children(pid_t *pids, int num_cmds);
void handle_parent(pid_t pid, pid_t *pids, int *num_cmds, int *prev_fd, int *pipefd, char **args, int i);
int handle_child(t_token *token, t_env *env, int prev_fd, int *pipefd, char **args, int last_exit_status);
void setup_child_io(int prev_fd, int *pipefd, char **args);
void	exit_program_leak(char **ptr1, int fd1, int fd2);
size_t	ft_strlen_d(char **s);
char	*ft_strtrim_start(char const *s1, char const *set);
void	ft_free(char **matrix);
int count_tokens(t_token *tokens);
char **command_matrix(t_token *tokens);
void	error_norm(char *text);
char	**check_cmd_path(char **cmd, t_env *env,int last_exit_status);
char	*get_env_path(t_env *env);
char	**get_directories(char **cmd, char *path_env);
char *search_executable_in_dirs(char **cmd, char **dir);
char *check_cmd_exist(char **cmd, char *path_env);
char	*free_norm(char **ptr, char *ret);
void	exit_program(char **ptr, int fd1, int fd2);
void	dupfd(int oldfd, int newfd);
int fork_and_execute(t_token *cleaned, t_env *env, char **arr, int redir_fd, int last_exit_status);
// void execute_simple(char **arg, t_env *env);
char **env_list_to_array(t_env *env);
t_token *setup_token_and_pipe(char **args, int i, int *pipefd);

// redirection
void setup_signal_handlers();
t_token	*clean_command_tokens(t_token *tokens);
void	skip_redirection_pair(t_token **curr, t_token **prev, t_token **tokens);
void restore_stdio(int packed_fd);
int	redirect_for_builtin(t_token *tokens, t_env *env);
int	process_redirect_token(t_token *curr, t_env *env);
int	handle_heredoc(t_token *curr, t_env *env);
void	read_heredoc_input(int write_fd, const char *delim, t_env *env);
int	handle_input_redirect(char *file);
int	handle_append_redirect(char *file);
int	handle_output_redirect(char *file);
void signal_handler(int sig);
void process_heredocs(char **args,t_env *env);
void handlee_heredoc(char *delim, t_env *env);
void handle_signal(int sig);
void restore_std_fds(int stdin_backup, int stdout_backup);
void free_tokens(t_token *tokens);
int handle_redirections(t_token *tokens, t_env *env);
int apply_and_clean_redirection(t_token **curr_ptr, t_token **prev_ptr, t_token **tokens);
int open_input_redirect(const char *file);
int open_append_output(const char *file);
int open_output_redirect(const char *file);




void    Getcwd(char *buf, size_t size);
char	*get_env_path(t_env *env);
char	**get_directories(char **cmd, char *path_env);
char	*check_cmd_exist(char **cmd, char *path_env);
char	*free_norm(char **ptr, char *ret);
void	ft_free(char **matrix);
char	*free_norm(char **ptr, char *ret);



















