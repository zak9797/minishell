#include "minishell.h"

int g_last_exit_code = 0;  // Global variable for the exit status

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig)
{
    (void)sig;
    g_shell.signal_flag = 1;
    write(1, "^C\n", 3);  // Print ^C when Ctrl+C is pressed
    rl_replace_line("", 0);  // Clear the current input
    rl_on_new_line();        // Move to a new line
    rl_redisplay();          // Redraw the prompt to show a clean state
    g_shell.last_exit_status = 130;  // Set exit status to 130 for Ctrl+C
    g_shell.signal_flag = 0;         // Reset the signal flag
}

// Function to initialize signal handling in non-interactive mode
void init_noninteractive_signals(void)
{
    struct sigaction sa;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;  // Ignore SIGINT and SIGQUIT in non-interactive mode
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

// Function to initialize signal handling for interactive mode
void init_signals(void)
{
    struct sigaction sa;

    rl_catch_signals = 0;  // Disable readline's default signal handling

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Set SIGINT handler (Ctrl+C)
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT (Ctrl+\)
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);

    // Initialize shell state
    g_shell.signal_flag = 0;
    g_shell.last_exit_status = 0;
}

// Add a token to the linked list
void add_token(t_token **head, t_token *new)
{
    if (!*head)
        *head = new;
    else
    {
        t_token *temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new;
    }
}