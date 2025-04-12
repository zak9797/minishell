/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <zsalah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:18:19 by zsalah            #+#    #+#             */
/*   Updated: 2025/04/12 17:37:15 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

t_cmd *create_cmd_node(t_token *tokens)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;

    cmd->tokens = tokens;  // Store the tokens for reference
    cmd->next = NULL;

    // Convert tokens into args (array of strings)
    cmd->args = token_list_to_args(tokens);

    // Count the arguments
    int count = 0;
    while (cmd->args && cmd->args[count])
        count++;
    cmd->arg_count = count;

    return cmd;
}

t_cmd *token_list_to_cmds(t_token *tokens)
{
    t_cmd *cmd_head = NULL;
    t_cmd *cmd_tail = NULL;

    t_token *current = tokens;
    while (current)
    {
        // Create a command node for each set of tokens between pipes
        t_cmd *new_cmd = create_cmd_node(current);
        if (!new_cmd)
            return NULL; // Add error handling as necessary

        // Append the new command to the list
        if (!cmd_head)
            cmd_head = new_cmd;
        else
            cmd_tail->next = new_cmd;
        cmd_tail = new_cmd;

        // Move to the next command (skip over the pipe)
        while (current && current->type != T_PIPE)
            current = current->next;
        
        // Skip the pipe token if present
        if (current && current->type == T_PIPE)
            current = current->next;  // Skip the pipe itself
    }

    return cmd_head;
}

char **token_list_to_args(t_token *tokens)
{
    int count = 0;
    t_token *temp = tokens;

    // First, count how many tokens we have (ignoring pipes)
    while (temp)
    {
        if (temp->type != T_PIPE)  // Only count non-pipe tokens
            count++;
        temp = temp->next;
    }

    // Now, allocate memory for the arguments array
    char **args = malloc(sizeof(char *) * (count + 1));  // +1 for NULL terminator
    if (!args)
        return NULL;

    // Fill the args array with the token values
    temp = tokens;
    int i = 0;
    while (temp)
    {
        if (temp->type != T_PIPE)  // Skip pipes
        {
            args[i] = strdup(temp->value);
            i++;
        }
        temp = temp->next;
    }
    args[i] = NULL;  // NULL-terminate the array

    return args;
}
