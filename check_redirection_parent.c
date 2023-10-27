/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection_parent.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:50:36 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/27 14:56:29 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

static int  check_heredoc(char **arg, t_cmd *struc);
static int  write_hd_to_pip(t_cmd *struc);

char    **check_redirection_parent(char **arg, t_cmd *struc)
{
    char    **tmp;
    int     file;
    int     compt;
    int     i;
    int     j;
    char    **buf;

    i = 0;
    struc->in = FALSE;
    struc->out = FALSE;
    if (check_heredoc(arg, struc) == 1)
        return (NULL);
    while (arg[i])
        i++;
    buf = malloc(sizeof(char *) * (i + 1));
    if (!buf)
        return (NULL);
    i = 0;
    while (arg[i])
    {
        if (arg[i][0] == '"' || arg[i][0] == '\'')
        {
            buf[i] = ft_strdup(arg[i]);
            if (!buf[i])
                return (NULL);
        }
        else
        {
            buf[i] = trash_quote(arg[i]);
            if (!buf[i])
                return (NULL);
        }
        i++;
    }
    buf[i] = NULL;
    anihilation(arg);
    i = 0;
    while (buf[i]) // récupération des différentes redirection
    {
        if (buf[i][0] == '<')
        {
            struc->in = TRUE;
            if (buf[i][1] == '\0')
            {
                if (buf[i + 1] == NULL)
                    return (anihilation(buf), error(TOKEN, "0"), NULL);
                file = open(buf[i + 1], O_RDONLY);
                if (file == -1)
                    return (anihilation(buf), error(OPEN, "0"), g_status = 1, NULL);
                close(file);
                i++;
            }
            else if (buf[i][1] == '<' && buf[i][2] == '\0')
                i++;
            else if (buf[i][1] != '<' || buf[i][2] == '\0')
            {
                file = open(&buf[i][1], O_RDONLY);
                if (file == -1) // 
                {
                    return (anihilation(buf), error(OPEN, "0"), g_status = 1, NULL); // le g_status dans certains cas doit valoir 2 ici
                    // g_status = 1;
                    // break;
                }
                close(file);
                // ft_printf_fd(2, "TEST\n");
            }
        }
        else if (buf[i][0] == '>')
        {
            struc->out = TRUE;
            if (buf[i][1] == '\0')
            {
                if (buf[i + 1] == NULL)
                    return (anihilation(buf), error(TOKEN, "0"), NULL);
                file = open(buf[i + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (anihilation(buf), error(OPEN, "0"), g_status = 1, NULL);
                close(file);
                i++;
            }
            else if (buf[i][1] == '>')
            {
                if (buf[i][2] == '\0')
                {
                    if (buf[i + 1] == NULL)
                        return (anihilation(buf), error(TOKEN, "0"), NULL);
                    file = open(buf[i + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (anihilation(buf), error(OPEN, "0"), g_status = 1, NULL);
                    close(file);
                    i++;
                }
                else
                {
                    file = open(&buf[i][2], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (anihilation(buf), error(OPEN, "0"), g_status = 1, NULL);
                    close(file);
                }
            }
            else
            {
                file = open(&buf[i][1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (anihilation(buf), error(OPEN, "0"), g_status = 1, NULL);
                close(file);
            }
        }
        i++;
    }
    // anihilation(buf);
    // if (struc->in == FALSE && struc->out == FALSE) // S'il n'y a pas de redirections pas besoin de retravailler le tableau de chaine de caractères
    //     return (buf);
    if (struc->heredoc == 1)
        if (write_hd_to_pip(struc) == 1)
            return (anihilation(buf), NULL);
    i = 0;
    compt = 0;
    while (buf[i]) // compte le nombre de redirection pour refaire un tableau de chaines de caractères
    {
        if (buf[i][0] == '<')
        {
            compt--;
            if (buf[i][1] == '\0' || (buf[i][1] == '<' && buf[i][2] == '\0'))
                i += 1;
        }
        else if (buf[i][0] == '>')
        {
            compt--;
            if (buf[i][1] == '\0' || (buf[i][1] == '>' && buf[i][2] == '\0'))
                i += 1;
        }
        i++;
        compt++;
    }
    tmp = (char **) malloc(sizeof(char *) * (compt + 1));
    i = 0;
    j = 0;
    while (buf[i]) // ici on ajoute les chaines de caractère correctes a tmp, on free les redirections car elles ne servent plus et on remet tmp dans buf a la fin
    {
        if (buf[i][0] == '<')
        {
            if (buf[i][1] == '\0' || (buf[i][1] == '<' && buf[i][2] == '\0'))
            {
                // free(buf[i]);
                i++;
            }
            // free(buf[i]);
        }
        else if (buf[i][0] == '>')
        {
            if (buf[i][1] == '\0' || (buf[i][1] == '>' && buf[i][2] == '\0'))
            {
                // free(buf[i]);
                i++;
            }
            // free(buf[i]);
        }
        else
        {
            tmp[j] = ft_strdup(buf[i]);
            if (tmp[j] == NULL)
                return (NULL);
            j++;
        }
        i++;
    }
    anihilation(buf);
    // buf = tmp;
    tmp[j] = NULL;
    return (tmp);
}

// Cette fonction doit mettre en place le pipe du heredoc s'il y en a besoin,
// pour le savoir elle check si la dernière redirection est un heredoc ou non
static int  check_heredoc(char **arg, t_cmd *struc)
{
    int i;

    i = 0;
    struc->heredoc = 0;
    while (arg[i]) // vérification que la dernière redirection est bien un heredoc car si c'est le cas il va falloire faire le pipe
    {
        if (arg[i][0] == '<')
        {
            if (arg[i][1] == '<')
            {
                struc->ind_hd++;
                struc->heredoc = 1;
            }
            else
                struc->heredoc = 0;
        }
        i++;
    }
    if (struc->heredoc == 0)
        return (0);
    struc->here_pipe = (int *)ft_calloc(2, sizeof(int));
    if (struc->here_pipe != NULL && pipe(struc->here_pipe) == -1) // création du pipe pour le heredoc
        return (error(PIPE, "0"), 1);
    else if (struc->here_pipe == NULL)
        return (error(CALLOC, "0"), 1);
    return (0);
}

static int  write_hd_to_pip(t_cmd *struc)
{
    int     i;
    int     compt;
    char    *tmp;

    i = -1;
    compt = 0;
    while (struc->hd_history[struc->ind_hd][++i])
    {
        if (struc->hd_history[struc->ind_hd][i] == '\n' && \
        struc->hd_history[struc->ind_hd][i + 1] != '\0')
            compt = i;
    }
    tmp = (char *) malloc(sizeof(char) * (compt + 2));
    if (tmp == NULL)
        return (error(MALLOC, "0"), 1);
    i = -1;
    while (++i <= compt)
        tmp[i] = struc->hd_history[struc->ind_hd][i];
    tmp[i] = '\0';
    if (write(struc->here_pipe[1], tmp, ft_strlen(tmp)) == -1)
        return (free(tmp), error(WRITE, "0"), 1);
    if (close(struc->here_pipe[1]) == -1)
        return (free(tmp), error(CLOSE, "0"), 1);
    if (close(struc->here_pipe[0]) == -1)
        return (free(tmp), error(CLOSE, "0"), 1);
    free(tmp);
    return (0);
}
