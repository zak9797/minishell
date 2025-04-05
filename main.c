#include"minishell.h"

#include <readline/readline.h>
#include <readline/history.h>  

void    create_token(t_token **token, char *value, char *type)
{
    t_token *new;
    t_token *temp;

    new = malloc(sizeof(t_token));
    if (!new)
        return;
    new->value = value;
    new->type = type;
    new->next = NULL;

    if (!*token)
        *token = new;
    else
    {
        temp = *token;
        while (temp->next)
            temp = temp->next;
        temp->next = new;
    }
}

int     count_words(char *line)
{
    int     i;
    int     words;

    i = 0;
    while(isspace(line[i]))
        i++;
    if(!line[i])
        return (0); //handle empty line right here !
    words = 1;
    while(line[i])
    {
        if(isspace(line[i]))
        {
            words++;
            while(isspace(line[i]))
                i++;
        }
        if(line[i] == '\'' || line[i] == '\"')
        {
            while(line[i] != '\'' && line[i] != '\"')
                i++;
        }
        i++;
    }
    return(words);
}

void    handle_quotes(char *line)
{
    int     i;
    char    flag;

    i = 0;
    flag = 1;
    while (line[i])
    {
        if(line[i] == '\'' || line[i] == '\"')
        {
            flag = line[i];
            while(line[i])
            {
                i++;
                if(line[i] == flag)
                {
                    flag = 1;
                    break;
                }
            }
        }
        i++;
    }
    if (flag != 1)
        printf("Command not found"); //free line
}
int     count_currect_word(char *line, int  i)
{
    int     count;

    count = 0;
    while(line[i] && !isspace(line[i]))
    {
        i++;
        count++;
    }
    return(count);
}

char **split_line(char *line, char **array)
{
    int i = 0, j = 0, n = 0;

    while (line[i]) {
        n = 0;
        if (!isspace(line[i])) {
            // Count the length of the current word
            n = count_currect_word(line, i);

            // Allocate space for the word
            array[j] = malloc(n + 1);
            if (!array[j]) {
                // Handle memory allocation failure (free the array)
                while (j > 0)
                    free(array[--j]);
                free(array);
                return NULL;
            }

            // Copy the word into the allocated space
            for (int k = 0; k < n; k++)
                array[j][k] = line[i + k];

            // Null-terminate the word
            array[j][n] = '\0';
            j++;
            i += n;  // Move the index to the next word
        }
        else {
            i++;  // Skip spaces
        }
    }
    array[j] = NULL;  // Null-terminate the array of tokens
    return array;
}

int     check_command(char *word)
{
    if(ft_strcmp(word, "echo") == 0 || ft_strcmp(word, "cd") == 0 ||
        ft_strcmp(word, "pwd") == 0 || ft_strcmp(word, "export") == 0 ||
        ft_strcmp(word, "unset") == 0 || ft_strcmp(word, "env") == 0 ||
        ft_strcmp(word, "exit") == 0)
        return(1);
    return(0);
}

void    tokenize(char **array, t_token **token)
{
    int     i;

    i = 0;
    while (array[i])
    {
        if (check_command(array[i]))
        create_token(token, array[i], "command");
        else if (ft_strcmp(array[i], "|") == 0)
            create_token(token, "|", "pipe");
        else if (ft_strcmp(array[i], "<") == 0)
            create_token(token, "<", "redirect input");
        else if (ft_strcmp(array[i], ">") == 0)
            create_token(token, ">", "redirect output");
        else if (ft_strcmp(array[i], ">>") == 0)
            create_token(token, ">>", "append output");
        else if (ft_strcmp(array[i], "<<") == 0)
            create_token(token, "<<", "here-document");
        else
            create_token(token, array[i], "word");
        i++;
    }
}

char **tokenizer(char *line, t_token **token)
{
    char **array;
    int words;
    int i;

    words = 0;
    i = 0;
    handle_quotes(line);
    words = count_words(line);
    array = malloc(sizeof(char *) * (words + 1));
    if (!array)
        return NULL; // malloc failed

    array = split_line(line, array);  // split the line into words
    tokenize(array, token);  // Create t_token list

    // You may want to return the array of tokens here
    return array;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    char *read;
    t_token *token = NULL;

    while (1)
    {
        read = readline("minishell > ");
        add_history(read);

        // Get the tokens as a char ** array
        char **tokens = tokenizer(read, &token);

        // Print out the tokens (for debugging)
        for (int i = 0; tokens[i] != NULL; i++)
        {
            printf("Token[%d]: '%s'\n", i, tokens[i]);
        }

        // Don't forget to free memory
        int i = 0;
        while (tokens[i] != NULL)
        {
            free(tokens[i]);
            i++;
        }
        free(tokens);
    }

    return 0;
}