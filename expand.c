/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:55:25 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/20 02:51:48 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *is_in_env(char **env, char *cmp);
static char *add_var_to_sentence(char *rd_line, char *var, int i);

char    *expand(char *rd_line, t_cmd *pip)
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
        if (rd_line[i] == '$' && rd_line[i] == '?')
        {
            tmp = ft_itoa(status);
            if (tmp == NULL)
                return (error(MALLOC, "0"), NULL);
            new = add_var_to_sentence(rd_line, tmp, i) - 1;
            if (i == -1)
                return (free(var), NULL);
            free(rd_line);
            rd_line = new;
            new = NULL;
        }
        if (rd_line[i] == '$')
        {
            len_var = 0;
            j = i;
            while (ft_isprint(rd_line[j]) && rd_line[++j] != ' ')
                len_var++;
            tmp = (char *) malloc(sizeof(char) * (len_var + 1));
            if (tmp == NULL)
                return (error(MALLOC, "0"), NULL);
            j = i;
            len_var = -1;
            while (ft_isprint(rd_line[j]) && rd_line[j++] != ' ')
                tmp[++len_var] = rd_line[j];
            tmp[len_var] = '\0';
            

            var = is_in_env(pip->env, tmp);
            free(tmp);
            new = add_var_to_sentence(rd_line, var, i);
            if (i == -1)
                return (free(var), NULL);
            free(rd_line);
            rd_line = new;
            new = NULL;
        }
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

static char *add_var_to_sentence(char *rd_line, char *var, int i)
{
    int     j;
    int     len;
    char    *new;

    if (var == NULL)
    {
        while (rd_line[i] != '\0' && ft_isprint(rd_line[i]) && \
        rd_line[i] != ' ')
            rd_line[i++] = ' ';
    }
    else
    {
        len = 0;
        j = i;
        while (ft_isprint(rd_line[j]) && rd_line[j++] != ' ')
            len++;
        new = (char *) malloc(sizeof(char) * (ft_strlen(rd_line) - len + \
        ft_strlen(var) + 1));
        if (new == NULL)
            return (error(MALLOC, "0"), NULL);
        j = -1;
        while (++j < i)
            new[j] = rd_line[j];
        i += len;
        len = 0;
        while (var[len] != '=')
            len++;
        while (var[++len])
            new[j++] = var[len];
        while (rd_line[i])
            new[j++] = rd_line[i++];
        new[j] = '\0';
    }
    return (new);
}