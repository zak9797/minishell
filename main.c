#include"minishell.h"

#include <readline/readline.h>
#include <readline/history.h>  


// Function to create a new token
t_token *create_token(char *value, char *type) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        perror("malloc failed");
        return NULL;
    }
    new_token->value = strdup(value); // Copy the string into the token's value
    new_token->type = strdup(type);   // Copy the string into the token's type
    new_token->next = NULL;
    return new_token;
}

// Function to append a token to the linked list
void append_token(t_token **head, char *value, char *type) {
    t_token *new_token = create_token(value, type);
    if (!new_token) return;  // If creation fails, return

    if (!*head) {
        *head = new_token;  // If list is empty, set head to the new token
    } else {
        t_token *current = *head;
        while (current->next) {
            current = current->next;  // Traverse to the last node
        }
        current->next = new_token;  // Append the new token
    }
}

// Function to free the token list
void free_token_list(t_token *head) {
    t_token *current = head;
    t_token *next;
    
    while (current) {
        next = current->next;
        free(current->value);
        free(current->type);
        free(current);
        current = next;
    }
}

// Function to count the number of words in a line
int count_words(char *line) {
    int count = 0;
    int i = 0;

    while (line[i]) {
        while (isspace(line[i])) i++;  // Skip leading spaces
        if (!line[i]) break;          // End of string
        count++;
        while (!isspace(line[i]) && line[i] != '\0') i++;  // Skip current word
    }

    return count;
}
bool is_builtin_command(char *command) 
{
    const char *builtin_commands[] = {"cd", "exit", "echo", "pwd", "env", "set", NULL};
    for (int i = 0; builtin_commands[i] != NULL; i++) {
        if (strcmp(command, builtin_commands[i]) == 0) {
            return true;
        }
    }
    return false;
}
// Tokenizer function that processes the input line and fills the token list
void tokenize(char *line, t_token **token_list) {
    int i = 0;
    int n = 0;
    char *word = NULL;

    while (line[i]) {
        // Skip spaces
        while (isspace(line[i])) i++;

        if (line[i] == '\0') break;

        n = 0;
        while (!isspace(line[i]) && line[i] != '\0') {
            n++;
            i++;
        }

        // Allocate space for the word and copy it
        word = malloc(n + 1);
        if (word) {
            strncpy(word, &line[i - n], n);
            word[n] = '\0';  // Null-terminate the word
        }

        // Check if the word is a built-in command
        char *type = is_builtin_command(word) ? "builtin" : "word";

        append_token(token_list, word, type);  // Store token with appropriate type
        free(word);
    }
}

// Function to print tokens (for debugging)
void print_tokens(t_token *head) {
    t_token *current = head;
    int index = 0;

    while (current) {
        printf("Token[%d]: '%s' (Type: '%s')\n", index++, current->value, current->type);
        current = current->next;
    }
}

int main() {
    char *read;
    t_token *tokens = NULL;

    while (1) {
        read = readline("minishell > ");
        if (read == NULL) {
            printf("\nExiting minishell...\n");
            break;
        }

        // Tokenize the input and store in the linked list
        tokenize(read, &tokens);

        // Print the tokens (for debugging)
        print_tokens(tokens);

        // Free the memory for the tokens
        free_token_list(tokens);
        tokens = NULL;  // Reset the token list for the next loop

        free(read);  // Free the memory allocated by readline
    }

    return 0;
}