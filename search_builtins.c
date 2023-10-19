/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/19 23:53:58 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void determine_echo_or_cd(char **str, int *option);

int search_parent_builtins(t_cmd *pip, t_list *spt)
{
    int     ret;
    t_cell *tmp;
    char    *str;
    char    *buf;

    pip->builtin = FALSE;
    tmp = spt->head;
    if (tmp == NULL)
        return (1);
    str = ((char *)(tmp->data_cell->data));
    if (ft_strlen(str) == 4 && ft_strncmp(str, "exit", 4) == 0)
            return (intermediate_exit(spt), -1);
    else if (ft_strlen(str) == 2 && ft_strncmp(str, "cd", 2) == 0)
    {
        pip->parent_builtin = TRUE;
        if (tmp->next == NULL)
            return (1);
        tmp = tmp->next;
        return (cd(((char *)(tmp->data_cell->data)), pip, spt), 1);
    }
    else if (ft_strlen(str) == 6 && ft_strncmp(str, "export", 6) == 0)
    {
        if (tmp->next == NULL)
            return (2);
        pip->parent_builtin = TRUE;
        tmp = tmp->next;
        while (tmp != NULL)
        {
            buf = ft_strdup(((char *)(tmp->data_cell->data)));
            ret = export(pip, buf);
            if (ret == 1)
                return (free(buf), -1); 
            // else if (ret == 2)
            //     return (free(buf), 1); //status = 2,
            tmp = tmp->next;
        }
        return (1);
    }
    else if (ft_strlen(str) == 5 && ft_strncmp(str, "unset", 5) == 0)
    {
        pip->parent_builtin = TRUE;
        tmp = tmp->next;
        while (tmp != NULL)
        {
            buf = ft_strdup(((char *)(tmp->data_cell->data)));
            if (unset(pip, buf) == 1)
                return (free(buf), -1);
            free(buf);
            tmp = tmp->next;
        }
        return (1);
    }
    return (0);
}

int search_builtins(char **spt, t_cmd *pip)
{
    int     option;

    option = 0;
    if (spt == NULL || !spt[0])
        return (1);
    if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "pwd", 3) == 0)
        return (free(pwd()), 1);
    else if (ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "exit", 4) == 0)
        return (intermediate_exit(NULL), 1);
    else if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "env", 3) == 0)
        return (env(pip->env), 1);
    else if ((ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "echo", 4) == 0) \
    || (ft_strlen(spt[0]) == 2 && ft_strncmp(spt[0], "cd", 2) == 0))
    {
        determine_echo_or_cd(spt, &option);
        if (option == 0 || option == 1)
            return (echo(spt, option), status = 0, 1);
        else if (option == 2)
            return (cd(spt[1], pip, NULL), -1);
    }
    else if ((ft_strlen(spt[0]) == 6 && ft_strncmp(spt[0], "export", 6) == 0) && spt[1] == NULL)
        return (print_export(pip->exp_env), 1);
    else if ((ft_strlen(spt[0]) == 6 && ft_strncmp(spt[0], "export", 6) == 0) \
    || (ft_strlen(spt[0]) == 5 && ft_strncmp(spt[0], "unset", 5) == 0))
        return (1);
    return (0);
}

static void determine_echo_or_cd(char **str, int *option)
{
    if (ft_strlen(str[0]) == 4 && ft_strncmp(str[0], "echo", 4) == 0)
    {
        if (str[1] == NULL)
            return ;
        else if (ft_strncmp(str[1], "-n", 2) == 0)
        {
            *option = 1;
            if (str[2] == NULL)
                return ;
        }
    }
    else if (ft_strlen(str[0]) == 2 && ft_strncmp(str[0], "cd", 2) == 0)
    {
        *option = 1;
        if (str[1] == NULL)
            return ;
        *option = 2;
        if (str[2] != NULL) // TODO: Bulle a résoudre
        {
            error(TOO_MANY_ARG, str[0]);
            return ;
        }
    }
    else
        *option = 3;
    return ;
}
