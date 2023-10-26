/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:55:25 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/26 14:14:14 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

static char *is_in_env(char **env, char *cmp);
static char *add_var_to_sentence(char *rd_line, char *var, int i, int *flag);

char    *expand(char *rd_line, t_cmd *pip, int *flag)
{
    char    *new;
    char    *tmp;
    char    *var;
    int     i;
    int     j;
    int     len_var;

    i = 0;
    while (rd_line[i])
    {
        if (*flag == 1 && rd_line[i] == '\'')
            *flag = 0;
        else if (*flag == 0 && rd_line[i] == '\'')
            *flag = 1;
        else if (*flag == 2 && rd_line[i] == '"')
            *flag = 0;
        else if (*flag == 0 && rd_line[i] == '"')
            *flag = 2;
        if (rd_line[i] == '$' && rd_line[i + 1] == '?' && *flag != 1)
        {
            tmp = ft_itoa(status);
            if (tmp == NULL)
                return (error(MALLOC, "0"), NULL);
            new = add_var_to_sentence(rd_line, tmp, i, flag);
            free(tmp);
            if (new != NULL)
            {
                free(rd_line);
                rd_line = new;
            }
            new = NULL;
        }
        if (rd_line[i] == '$' && ft_isalpha(rd_line[i + 1]) && *flag != 1)//&& rd_line[i + 1] != '\0' && rd_line[i + 1] != ')' && *flag != 1)
        {
            len_var = 0;
            j = i + 1;
            while (ft_isprint(rd_line[j]) && rd_line[j] != ' ' && rd_line[j++] != '\'')
                len_var++;
            if (rd_line[i + len_var] == '"')
                len_var--;
            tmp = (char *) malloc(sizeof(char) * (len_var + 1));
            if (tmp == NULL)
                return (error(MALLOC, "0"), NULL);
            j = i + 1;
            len_var = 0;
            while (ft_isprint(rd_line[j]) && rd_line[j] != ' ' && \
            rd_line[j] != '\'' && rd_line[j] != '"')
                tmp[len_var++] = rd_line[j++];
            tmp[len_var] = '\0';

            var = is_in_env(pip->env, tmp);
            free(tmp);
            new = NULL;
            new = add_var_to_sentence(rd_line, var, i, flag);
            if (new != NULL)
            {
                free(rd_line);
                rd_line = new;
            }
            new = NULL;
        }
        if (rd_line[i] != '\0')
            i++;
    }
    return (rd_line);
}

static char *is_in_env(char **env, char *cmp)
{
    int i;

    i = 0;
    while (env[i])
    {
        if (ft_strlen(cmp) <= ft_strlen(env[i]) &&\
        env[i][ft_strlen(cmp)] == '=' && \
        strncmp(env[i], cmp, ft_strlen(cmp)) == 0)
            return (env[i]);
        i++;
    }
    return (NULL);
}

static char *add_var_to_sentence(char *rd_line, char *var, int i, int *flag)
{
    int     j;
    int     len;
    int     quote;
    char    *new;

    len = 0;
    j = i;
    quote = 0;
    if (*flag == 2)
        quote = 1;
    if (rd_line[i + 1] == '?')
        len = 2;
    else
        while (ft_isprint(rd_line[j]) && rd_line[j] != ' ' && \
            rd_line[j] != '\'' && rd_line[j] != '"' && rd_line[j++] != '|')
            len++;
    if (var == NULL)
        new = (char *) malloc(sizeof(char) * (ft_strlen(rd_line) - len + \
    1 + quote)); // provoque Conditional jump or move depends on uninitialised value si +1 a la place de +2 : invalid read
    else
        new = (char *) malloc(sizeof(char) * (ft_strlen(rd_line) - len + \
    ft_strlen(var) + 1 + quote));
    if (new == NULL)
        return (error(MALLOC, "0"), NULL);
    j = -1;
    while (++j < i)
        new[j] = rd_line[j];
    i += len;
    len = 0;
    if (var != NULL)
    {
        if (var[0] > '9' || var[0] < '0')
        {
            while (var[len] != '=')
                len++;
            len++;
        }
        while (var[len])
            new[j++] = var[len++];
    }
    while (rd_line[i])
        new[j++] = rd_line[i++];
    new[j] = '\0';
    return (new);
}
