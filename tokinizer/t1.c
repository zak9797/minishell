#include "minishell.h"

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