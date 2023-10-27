/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/27 10:56:07 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

static int  handle_hist(int compt, t_list *spt, t_cmd *pip);

char    **prep_hd(t_cmd *pip, t_list *spt)
{
    int     compt;
    int     i;
    int     j;
    t_cell  *tmp;

    char    *buf;
    char    *buff;
    char    *stop;
    char    *rd_line;

    status = 0;
    compt = 0;
    tmp = spt->head;
    while (tmp != NULL)
    {
        if (((char *) (tmp->data_cell->data))[0] == '<')
            if (((char *) (tmp->data_cell->data))[1] == '<')
                compt++;
        tmp = tmp->next;
    }
    if (compt != 0)
        pip->hd_history = (char **) ft_calloc(compt + 1, sizeof(char *));
    if (pip->hd_history == NULL)
        return (NULL);
    i = 0;
    while (((char *) spt->tail->data_cell->data)[i] != '\n' \
    && ((char *) spt->tail->data_cell->data)[i] != '\0')
        i++;
    if (((char *) spt->tail->data_cell->data)[i] == '\n')
        i = handle_hist(compt, spt, pip);
    else
        i = 0;
    tmp = spt->head;
    while (i < compt)
    {
        while (tmp != NULL && ((char *) (tmp->data_cell->data))[0] != '<' \
        && ((char *) (tmp->data_cell->data))[1] != '<')
            tmp = tmp->next;
        if (((char *) (tmp->data_cell->data))[2] == '\0' && tmp->next == NULL)
            return (error(SYNTAX, "newline"), free(pip->hd_history), status = 2, NULL);
        if (((char *) (tmp->data_cell->data))[2] == '\0')
            stop = (char *) tmp->next->data_cell->data;
        else
            stop = &(((char *) tmp->data_cell->data)[2]);
        tmp = tmp->next;
        j = 0;
        while (stop[j] != '\n' && stop[j] != '\0')
            j++;
        stop[j] = '\0';
        buf = NULL;
        buff = NULL;
        rd_line = pip->hd_history[i];
        buff = readline("> ");
        while (buff != NULL && status != 130)
        {
            if (ft_strncmp(stop, buff, ft_strlen(buff) + 1) == 0)
                break;
            if (rd_line == NULL)
            {
                rd_line = ft_strjoin(buff, "\n");
                if (rd_line == NULL)
                    return (error(JOIN, "0"), free(buff), free(buf), NULL);
                free(buff);
            }
            else
            {
                buf = ft_strjoin(buff, "\n");
                if (buf == NULL)
                    return (error(JOIN, "0"), free(buff), free(rd_line), NULL);
                free(buff);
                buff = ft_strjoin(rd_line, buf);
                if (buff == NULL)
                    return (error(JOIN, "0"), free(rd_line), free(buf), NULL);
                free(rd_line);
                free(buf);
                rd_line = buff;
            }
            buff = readline("> ");
        }
        if (buff == NULL || status == 130)
        {
            if (buff == NULL)
            {
                status = 0;
                ft_printf("\n");
            }
            free(buff);
            free(buf);
            free(rd_line);
            pip->status_hd = 1;
            free(pip->hd_history);
            return (NULL);
        }
        if (rd_line != NULL)
        {
            buf = ft_strjoin(buff, "\n");
            if (buf == NULL)
                return (error(JOIN, "0"), free(buff), free(rd_line), NULL);
            free(buff);
            buff = ft_strjoin(rd_line, buf);
            if (buff == NULL)
                return (error(JOIN, "0"), free(rd_line), free(buf), NULL);
            free(rd_line);
            free(buf);
        }
        pip->hd_history[i] = ft_strdup(buff);
        free(buff);
        i++;
    }
    pip->hd_history[i] = NULL;
    pip->ind_hd = -1;
    return (pip->hd_history);
}

static int  handle_hist(int compt, t_list *spt, t_cmd *pip)
{
    int     i;
    int     j;
    int     k;
    int     test_buff;
    char    *buf;
    char    *buff;
    char    *stop;
    char    *rd_line;
    char    **rdline;
    t_cell  *tmp;
    t_cell  *unt_lst;


    i = 0;
    tmp = spt->head;
    unt_lst = untail_list(spt, DEBUG);
    rdline = ft_split((char *) unt_lst->data_cell->data, '\n');
    free(unt_lst->data_cell->data);
    free(unt_lst->data_cell);
    free(unt_lst);
    if (rdline == NULL)
        return (error(SPLIT, "0"), -1);
    j = 0;
    test_buff = 0;
    while (i < compt)
    {
        while (tmp != NULL && ((char *) (tmp->data_cell->data))[0] != '<' \
        && ((char *) (tmp->data_cell->data))[1] != '<')
            tmp = tmp->next;
        if (((char *) (tmp->data_cell->data))[2] == '\0')
            stop = (char *) tmp->next->data_cell->data;
        else
            stop = &(((char *) tmp->data_cell->data)[2]);
        tmp = tmp->next;
        k = 0;
        while (stop[k] != '\n' && stop[k] != '\0')
            k++;
        stop[k] = '\0';
        buf = NULL;
        buff = NULL;
        rd_line = NULL;
        buff = rdline[j++];
        if (buff == NULL)
            return (i);
        while (buff && ft_strncmp(stop, buff, ft_strlen(buff) + 1) != 0)
        {
            if (rd_line == NULL)
            {
                rd_line = ft_strjoin(buff, "\n");
                if (rd_line == NULL)
                    return (error(JOIN, "0"), free(buff), free(buf), -1);
                free(buff);
            }
            else
            {
                buf = ft_strjoin(buff, "\n");
                if (buf == NULL)
                    return (error(JOIN, "0"), free(buff), free(rd_line), -1);
                free(buff);
                buff = ft_strjoin(rd_line, buf);
                if (buff == NULL)
                    return (error(JOIN, "0"), free(rd_line), free(buf), -1);
                free(rd_line);
                free(buf);
                rd_line = buff;
            }
            buff = rdline[j++];
        }
        if (rd_line != NULL && buff != NULL)
        {
            buf = ft_strjoin(buff, "\n");
            if (buf == NULL)
                return (error(JOIN, "0"), free(buff), free(rd_line), -1);
            free(buff);
            buff = ft_strjoin(rd_line, buf);
            if (buff == NULL)
                return (error(JOIN, "0"), free(rd_line), free(buf), -1);
            free(rd_line);
            free(buf);
        }
        if (buff == NULL)
        {
            test_buff++;
            buff = rd_line;
        }
        pip->hd_history[i] = ft_strdup(buff);
        free(buff);
        if (pip->hd_history[i] == NULL)
            return (anihilation(pip->hd_history), error(STRDUP, "0"), -1);
        i++;
    }
    free(rdline);
    return (i - test_buff);
}
