#include"minishell.h"

char *expand_variables(const char *input, t_env *env)
{
    char *result = strdup(""); // Start with empty string
    int i = 0;

    while (input[i])
    {
        if (input[i] == '$')
        {
            i++; // skip $
            int var_start = i;
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                i++;

            char *var_name = ft_strndup(&input[var_start], i - var_start);
            char *var_value = get_env_val(env, var_name);
            char *tmp = result;
            result = ft_strjoin(tmp, var_value); // append expanded value
            free(tmp);
            free(var_name);
        }
        else
        {
            // Copy one literal character at a time
            char str[2] = { input[i], 0 };
            char *tmp = result;
            result = ft_strjoin(tmp, str);
            free(tmp);
            i++;
        }
    }
    return result;
}

char *handle_single_quotes(const char *input, int *i)
{
    
    int start = *i;
    (*i)++;
    // if(input[*i]  == ' ')
    // {
    //     s
    // }
    while (input[*i] && input[*i] != '\'')
        (*i)++;
    char *literal = ft_strndup(&input[start], *i - start);
    (*i)++; // skip closing '
    return literal;
}
int has_special_inside(const char *str)
{
    while (*str)
    {
        if (*str == '*' || *str == '~' || *str == '{' || *str == ';' || *str == '&')
            return 1;
        str++;
    }
    return 0;
}

char *handle_double_quotes(const char *input, int *i, t_env *env)
{
    (*i)++; // skip opening "
    int start = *i;

    while (input[*i] && input[*i] != '"')
        (*i)++;

    char *inside = ft_strndup(&input[start], *i - start);

    char *expanded;
     if (has_special_inside(inside))
     {
         // ❌ Do NOT expand if there’s a single quote inside
         expanded = strdup(inside);
     }
     else
     {
      expanded = expand_variables(inside, env);   
         // ✅ Safe to expand    
     }

    free(inside);

    if (input[*i] == '"')
        (*i)++; // skip closing quote

    return expanded;
}
int has_unclosed_quotes(const char *input)
{
    int single = 0, double_q = 0;
    while (*input)
    {
        if (*input == '\'' && double_q % 2 == 0)
            single++;
        else if (*input == '"' && single % 2 == 0)
            double_q++;
        input++;
    }
    return (single % 2 != 0 || double_q % 2 != 0);
}
char *expand_all(char *input, t_env *env)
{
if (has_unclosed_quotes(input))
{
    printf("Unclosed quote detected\n");
    return NULL;
}
    int i = 0;
    char *result = strdup("");

    while (input[i])
    {
         char *fragment = NULL;
        //   if (input[i] == '\'')
        //       fragment = handle_single_quotes(input, &i);
        // else if (input[i] == '"')
        //      fragment = handle_double_quotes(input, &i, env);
         if (input[i] == '$')
        {
            int start = i++;
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                i++;
            char *var_name = ft_strndup(&input[start + 1], i - start - 1);
            char *var_value = get_env_val(env, var_name);
            fragment = var_value ? strdup(var_value) : strdup(""); // Handle empty case properly
            free(var_name);
        }
        else
        {
            // literal char
            char tmp[2] = { input[i++], '\0' };
            fragment = strdup(tmp);
        }

        // Ensure result keeps appending
        char *tmp_result = result;
        result = ft_strjoin(tmp_result, fragment);
        free(tmp_result);
        free(fragment);
    }
        //printf("result: %s\n", result); 

    return result;
}