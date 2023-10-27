/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection_parent.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:50:36 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/27 16:23:14 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include "../incs/check_redirection_parent.h"


static int  check_heredoc(char **arg, t_cmd *struc);
static int  write_hd_to_pip(t_cmd *struc);
static int	initialise_redir_parent(t_red_par *red, char **arg, t_cmd *struc);
static int	search_open_file(t_red_par *red, t_cmd *struc);
static int	allocate_tmp_char(t_red_par *red);
static int	fill_tmp(int *j, t_red_par *red);

char    **check_redirection_parent(char **arg, t_cmd *struc)
{
	t_red_par	red;
	int			j;

	if (initialise_redir_parent(&red, arg, struc) == 1)
		return (NULL);
	if (search_open_file(&red, struc) == 1)
		return (NULL);
    if (struc->heredoc == 1)
        if (write_hd_to_pip(struc) == 1)
            return (anihilation(red.buf), NULL);
    red.compt = 0;
	if (allocate_tmp_char(&red) == 1)
		return (anihilation(red.buf), NULL);
    j = 0;
	if (fill_tmp(&j, &red) == 1)
		return (NULL);
    anihilation(red.buf);
    red.tmp[j] = NULL;
	if (j == 0)
		return (anihilation(red.tmp), NULL);
    return (red.tmp);
}

static int	fill_tmp(int *j, t_red_par *red)
{
	int i;

	i = 0;
	while (red->buf[i])
    {
        if (red->buf[i][0] == '<')
        {
            if (red->buf[i][1] == '\0' || (red->buf[i][1] == '<' && red->buf[i][2] == '\0'))
                i++;
        }
        else if (red->buf[i][0] == '>')
        {
            if (red->buf[i][1] == '\0' || (red->buf[i][1] == '>' && red->buf[i][2] == '\0'))
                i++;
        }
        else
        {
            // red->tmp[*j] = ft_strdup(red->buf[i]);
			red->tmp[*j] = NULL;
            if (red->tmp[*j] == NULL)
                return (1);
            *j += 1;
        }
        i++;
    }
	return (0);
}

static int	allocate_tmp_char(t_red_par *red)
{
	int i;

	i = 0;
	while (red->buf[i])
    {
        if (red->buf[i][0] == '<')
        {
            red->compt--;
            if (red->buf[i][1] == '\0' || (red->buf[i][1] == '<' && red->buf[i][2] == '\0'))
                i += 1;
        }
        else if (red->buf[i][0] == '>')
        {
            red->compt--;
            if (red->buf[i][1] == '\0' || (red->buf[i][1] == '>' && red->buf[i][2] == '\0'))
                i += 1;
        }
        i++;
        red->compt++;
    }
    red->tmp = (char **) malloc(sizeof(char *) * (red->compt + 1));
	if (!red->tmp)
		return (1);
	return (0);
}

static int	search_open_file(t_red_par *red, t_cmd *struc)
{
	int i;
	
	i = 0;
	while (red->buf[i])
    {
        if (red->buf[i][0] == '<')
        {
            struc->in = TRUE;
            if (red->buf[i][1] == '\0')
            {
                if (red->buf[i + 1] == NULL)
                    return (anihilation(red->buf), error(TOKEN, "0"), 1);
                red->file = open(red->buf[i + 1], O_RDONLY);
                if (red->file == -1)
                    return (anihilation(red->buf), error(OPEN, "0"), g_status = 1, 1);
                close(red->file);
                i++;
            }
            else if (red->buf[i][1] == '<' && red->buf[i][2] == '\0')
                i++;
            else if (red->buf[i][1] != '<' || red->buf[i][2] == '\0')
            {
                red->file = open(&red->buf[i][1], O_RDONLY);
                if (red->file == -1)
                {
                    return (anihilation(red->buf), error(OPEN, "0"), g_status = 1, 1);
                }
                close(red->file);
            }
        }
        else if (red->buf[i][0] == '>')
        {
            struc->out = TRUE;
            if (red->buf[i][1] == '\0')
            {
                if (red->buf[i + 1] == NULL)
                    return (anihilation(red->buf), error(TOKEN, "0"), 1);
                red->file = open(red->buf[i + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (red->file == -1)
                    return (anihilation(red->buf), error(OPEN, "0"), g_status = 1, 1);
                close(red->file);
                i++;
            }
            else if (red->buf[i][1] == '>')
            {
                if (red->buf[i][2] == '\0')
                {
                    if (red->buf[i + 1] == NULL)
                        return (anihilation(red->buf), error(TOKEN, "0"), 1);
                    red->file = open(red->buf[i + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (red->file == -1)
                        return (anihilation(red->buf), error(OPEN, "0"), g_status = 1, 1);
                    close(red->file);
                    i++;
                }
                else
                {
                    red->file = open(&red->buf[i][2], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (red->file == -1)
                        return (anihilation(red->buf), error(OPEN, "0"), g_status = 1, 1);
                    close(red->file);
                }
            }
            else
            {
                red->file = open(&red->buf[i][1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (red->file == -1)
                    return (anihilation(red->buf), error(OPEN, "0"), g_status = 1, 1);
                close(red->file);
            }
        }
        i++;
    }
	return (0);
}

static int	initialise_redir_parent(t_red_par *red, char **arg, t_cmd *struc)
{
	int i;
	
	i = 0;
    struc->in = FALSE;
    struc->out = FALSE;
    if (check_heredoc(arg, struc) == 1)
        return (1);
    while (arg[i])
        i++;
    red->buf = malloc(sizeof(char *) * (i + 1));
    if (!red->buf)
        return (1);
    i = 0;
    while (arg[i])
    {
        if (arg[i][0] == '"' || arg[i][0] == '\'')
        {
            red->buf[i] = ft_strdup(arg[i]);
            if (!red->buf[i])
                return (1);
        }
        else
        {
            red->buf[i] = trash_quote(arg[i]);
            if (!red->buf[i])
                return (1);
        }
        i++;
    }
    red->buf[i] = NULL;
    anihilation(arg);
	return (0);
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
