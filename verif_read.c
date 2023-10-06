/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/08/29 11:08:54 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_arg(t_list *spt);

char *verif_read(char *b, char *rd_line, t_cmd *pip)
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
        return(free(b), annihilation(spt, free, DEBUG), buff);
    if (check_variables(spt, pip) == 1)
        return (NULL);
    flag = execution_center(spt, pip);
    if (flag == 1)
        return (free(b), NULL);
    else if (flag == -1)
        return (NULL);
    else if (flag == -2)
        return (free(b), annihilation(spt, free, DEBUG), exitt(pip), NULL);
    annihilation(spt, free, DEBUG);
    return (buff);
}

static int check_arg(t_list *spt)
{
    t_cell *tmp;

    tmp = spt->head;
    while (tmp != NULL)
    {
        if (tmp->next != NULL && ((char *)tmp->data_cell->data)[0] ==  '|' && ((char *)tmp->next->data_cell->data)[0] == '|')
            return (error(SYNTAX, "'|'"), 1);
        tmp = tmp->next;
    }
    return (0);
}
