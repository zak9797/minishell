#include "minishell.h"

int open_output_redirect(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open output"); exit(EXIT_FAILURE); }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return fd;
}

int open_append_output(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) { perror("open append"); exit(EXIT_FAILURE); }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return fd;
}

int open_input_redirect(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0) { perror("open input"); exit(EXIT_FAILURE); }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return fd;
}
int open_input_redirect_h(const char *file)
{
    (void) file;
    int fd = open(".heredoc_tmp", O_RDONLY);
    if (fd < 0) { perror("open input"); exit(EXIT_FAILURE); }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return fd;
}


int apply_and_clean_redirection(t_token **curr_ptr, t_token **prev_ptr, t_token **tokens)
{
    int fd = -1;
    t_token *curr = *curr_ptr;
    char *file = curr->next->value;

    if (curr->type == T_OUTPUT_REDIRECT)
        fd = open_output_redirect(file);
    else if (curr->type == T_APPEND_OUTPUT)
        fd = open_append_output(file);
    else if (curr->type == T_INPUT_REDIRECT)
        fd = open_input_redirect(file);
    else if (curr->type == T_HEREDOC)
    fd = open_input_redirect_h(file);

    t_token *to_free = curr;
    t_token *file_token = curr->next;
    *curr_ptr = file_token->next;

    if (*prev_ptr)
        (*prev_ptr)->next = *curr_ptr;
    else
        *tokens = *curr_ptr;
    free(file_token);
    free(to_free);

    return fd;
}

int handle_redirections(t_token *tokens, t_env *env)
{
    int prev_fd = -1;
    (void)env;
    t_token *curr = tokens;
    t_token *prev = NULL;

    while (curr)
    {
        if ((curr->type == T_OUTPUT_REDIRECT || curr->type == T_APPEND_OUTPUT ||
             curr->type == T_INPUT_REDIRECT || curr->type == T_HEREDOC) && curr->next)
        {
            prev_fd = apply_and_clean_redirection(&curr, &prev, &tokens);
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
    return prev_fd;
}