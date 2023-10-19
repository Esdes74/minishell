/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/19 22:00:05 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_arg(t_list *spt);

char *verif_read(char *rd_line, t_cmd *pip)
{
    int     flag;
    char    *buff;
    char    *buf;
    t_list  *spt;

    flag = 0;
    spt = (t_list *) malloc(sizeof(t_list));
	if (spt == NULL)
		return (error(MALLOC, NULL), NULL);
    init_list(spt);
    if (rd_line[0] == '|')
        return (error(TOKEN, "0"), NULL);
    if (expand(rd_line, pip) == 1)
        return (annihilation(spt, free, DEBUG), NULL);
    parsing(rd_line, &flag, spt);
    if (spt->len == 0)
        return (annihilation(spt, free, DEBUG), rd_line);
    buff = rd_line;
    while (flag == 1 || flag == 2)
    {
        rd_line = readline("> ");
        parsing(rd_line, &flag, spt);
        buf = ft_strjoin(buff, "\n");
        free(buff);
        buff = ft_strjoin(buf, rd_line);
        free(buf);
    }
    if (check_arg(spt) == 1)
        return(annihilation(spt, free, DEBUG), buff);
    if (check_variables(spt, pip) == 1)
        return (NULL);
    flag = execution_center(spt, pip);
    if (flag == 1)
        return (NULL);
    else if (flag == -1)
        return (free(buff), NULL);
    // else if (flag == 0)
    //     return (status = 0, NULL); // annihilation(spt, free, DEBUG) exitt(pip, TRUE),
    annihilation(spt, free, DEBUG);
    return (buff);
}

static int check_arg(t_list *spt)
{
    char    *str;
    t_cell  *tmp;

    tmp = spt->head;
    while (tmp != NULL)
    {
        str = (char *)tmp->data_cell->data;
        if (tmp->next != NULL && str[0] ==  '|' && ((char *)tmp->next->data_cell->data)[0] == '|')
            return (error(SYNTAX, "'|'"), 1);
        if (str[0] == '<')
        {
            if (str[1] == '\0' && tmp->next == NULL)
                return (error(SYNTAX, "'newline'"), 1);
            else if (str[1] == '<' && str[2] == '\0' && tmp->next == NULL)
                return (error(SYNTAX, "'newline'"), 1);
        }
        else if (str[0] == '>')
        {
            if (str[1] == '\0' && tmp->next == NULL)
                return (error(SYNTAX, "'newline'"), 1);
            else if (str[1] == '>' && str[2] == '\0' && tmp->next == NULL)
                return (error(SYNTAX, "'newline'"), 1);
        }
        tmp = tmp->next;
    }
    return (0);
}
