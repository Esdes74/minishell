/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/08/29 11:08:54 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

    // Compte du nombre de potentiels hd pour savoir de combien doit etre fait
    // le malloc
    compt = 0;
    tmp = spt->head;
    while (tmp != NULL)
    {
        if (((char *) (tmp->data_cell->data))[0] == '<')
            if (((char *) (tmp->data_cell->data))[1] == '<')
                compt++;
        tmp = tmp->next;
    }

    // Fait le malloc afin de créer la structure de hd
    if (compt != 0)
        pip->hd_history = (char **) ft_calloc(compt + 1, sizeof(char *));
    if (pip->hd_history == NULL)
        return (NULL);

    // Regarde a la fin de la ligne de commande s'il y a un historique
    // si c'est le cas alors elle appel la fonction qui gère ca
    i = 0;
    while (((char *) spt->tail->data_cell->data)[i] != '\n' \
    && ((char *) spt->tail->data_cell->data)[i] != '\0')
        i++;
    if (((char *) spt->tail->data_cell->data)[i] == '\n')
        i = handle_hist(compt, spt, pip);
    else
        i = 0;
    // i est initialisé par la fonction handle_hist pour savoir par la suite 
    // s'il y a besoin de compléter ce qu'il y a déjà ou si tout est remplis 
    // correctement

    // Remplie `ret` avec les hd
    tmp = spt->head;
    while (i < compt)
    {
        // Recherche des heredoc a faire
        while (tmp != NULL && ((char *) (tmp->data_cell->data))[0] != '<' \
        && ((char *) (tmp->data_cell->data))[1] != '<')
            tmp = tmp->next;

        // Récupération du mot qui stop le heredoc en cours
        if (((char *) (tmp->data_cell->data))[2] == '\0')
            stop = (char *) tmp->next->data_cell->data;
        else
            stop = &(((char *) tmp->data_cell->data)[2]);
        tmp = tmp->next;

        // Si ce mot est le dernier heredoc et qu'il contient l'historique
        // des heredoc alors je met \0 pour pouvoir les supprimer et n'avoir 
        // que le mot voulus
        j = 0;
        while (stop[j] != '\n' && stop[j] != '\0')
            j++;
        stop[j] = '\0';

        buf = NULL;
        buff = NULL;
        rd_line = pip->hd_history[i];
        // Ici je demande a l'utilisateur de rentrer ses phrases jusqu'a ce que je retrouve le bon mot
        buff = readline("> ");
        while (ft_strncmp(stop, buff, ft_strlen(buff) + 1) != 0)
        {
            if (rd_line == NULL) // Si c'est la première phrase alors je join un \n
            {
                rd_line = ft_strjoin(buff, "\n");
                if (rd_line == NULL)
                    return (error(JOIN, "0"), free(buff), free(buf), NULL);
                free(buff);
            }
            else // Sinon je join un \n et je join le nouveau a l'ancien
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
        // Je récupère le dernier mot afin de pouvoir l'ajouter a l'historique
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
        // J'ajoute ce mot a l'historique
        // free(pip->hd_history[i]);
        pip->hd_history[i] = buff;
        i++;
    }
    // Je ferme la fin de l'historique et met a -1 l'ind pour l'utilisation 
    // ultérieure
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

    i = 0;
    tmp = spt->head;
    // Je split la fin afin d'avoir tous l'hitorique mot par mot pour 
    // pouvoir réutiliser l'algo d'au dessus sans me casser la tete
    rdline = ft_split((char *) spt->tail->data_cell->data, '\n');
    if (rdline == NULL)
        return (error(SPLIT, "0"), -1);
    i = 0;
    j = 1;
    test_buff = 0;
    while (i < compt)
    {
        // Recherche des heredoc a faire
        while (tmp != NULL && ((char *) (tmp->data_cell->data))[0] != '<' \
        && ((char *) (tmp->data_cell->data))[1] != '<')
            tmp = tmp->next;

        // Meme chose qu'au dessus je récupère le mot stop
        if (((char *) (tmp->data_cell->data))[2] == '\0')
            stop = (char *) tmp->next->data_cell->data;
        else
            stop = &(((char *) tmp->data_cell->data)[2]);
        tmp = tmp->next;

        // Meme chose qu'au dessus je met un \0 pour supprimer les autres 
        // mots de l'hitorique
        k = 0;
        while (stop[k] != '\n' && stop[k] != '\0')
            k++;
        stop[k] = '\0';

        buf = NULL;
        buff = NULL;
        rd_line = NULL;
        // Ici je récupère les phrases jusqu'a ce que je retrouve le bon mot
        buff = rdline[j++];
        if (buff == NULL)
            return (i);
        while (buff != NULL && ft_strncmp(stop, buff, ft_strlen(buff) + 1) != 0)
        {
            if (rd_line == NULL) // Si c'est la première phrase alors je join un \n
            {
                rd_line = ft_strjoin(buff, "\n");
                if (rd_line == NULL)
                    return (error(JOIN, "0"), free(buff), free(buf), -1);
                free(buff);
            }
            else // Sinon je join un \n et je join le nouveau a l'ancien
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
        // Comme au dessus je join le dernier mot pour l'historique
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
        // Je fais un dup afin de pouvoir ajouter ce que j'ai trouvé a l'historique
        if (buff == NULL)
        {
            test_buff++;
            buff = rd_line;
        }
        pip->hd_history[i] = ft_strdup(buff);
        if (pip->hd_history[i] == NULL)
            return (anihilation(pip->hd_history), error(STRDUP, "0"), -1);
        i++;
    }
    return (i - test_buff);
}