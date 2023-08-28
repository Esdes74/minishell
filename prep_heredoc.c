#include "minishell.h"

char    **prep_hd(t_cmd *pip, t_list *spt)
{
    int     compt;
    int     i;
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
        pip->hd_history = (char **) malloc(sizeof(char *) * (compt + 1));
    if (pip->hd_history == NULL)
        return (NULL);

    // Remplie `ret` avec les hd
    i = 0;
    tmp = spt->head;
    while (i < compt)
    {
        // Recherche des heredoc a faire
        while (tmp != NULL && ((char *) (tmp->data_cell->data))[0] != '<' \
        && ((char *) (tmp->data_cell->data))[1] != '<')
            tmp = tmp->next;

        if (((char *) (tmp->data_cell->data))[2] == '\0')
            stop = (char *) tmp->next->data_cell->data;
        else
            stop = &(((char *) tmp->data_cell->data)[2]);
        tmp = tmp->next;

        buf = NULL;
        buff = NULL;
        rd_line = NULL;
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
        buf = ft_strjoin(buff, "\n");
        if (buf == NULL)
            return (error(JOIN, "0"), free(buff), free(rd_line), NULL);
        free(buff);
        buff = ft_strjoin(rd_line, buf);
        if (buff == NULL)
            return (error(JOIN, "0"), free(rd_line), free(buf), NULL);
        free(rd_line);
        free(buf);
        pip->hd_history[i] = buff;
        i++;
    }
    pip->hd_history[i] = NULL;
    pip->ind_hd = -1;
    return (pip->hd_history);
}