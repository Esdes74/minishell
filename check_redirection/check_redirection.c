/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/27 14:56:29 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include "../incs/check_redirection.h"

static int	init_redirection(t_red *r, char **arg, t_cmd *struc);
static void	creat_tab(t_red *r, char **arg);
static void	fill_tab(t_red *r, char **arg);
static int	check_heredoc(char **arg, t_cmd *struc);

char	**check_redirection(char **arg, t_cmd *struc)
{
	t_red	r;

	if (init_redirection(&r, arg, struc) == -1)
		return (NULL);
	if (duplication(&r, arg, struc) == -1)
		return (NULL);
	anihilation(r.buf);
	if (struc->in == FALSE && struc->out == FALSE)
		return (arg);
	if (struc->heredoc == 1)
		if (write_hd_to_pip(struc) == 1)
			return (anihilation(arg), NULL);
	creat_tab(&r, arg);
	r.tmp = (char **) malloc(sizeof(char *) * (r.compt + 1));
	if (r.tmp == NULL)
		return (error(MALLOC, "0"), anihilation(arg), g_status = 1, NULL);
	r.i = 0;
	r.j = 0;
	fill_tab(&r, arg);
	free(arg);
	arg = r.tmp;
	arg[r.j] = NULL;
	return (arg);
}

static int	init_redirection(t_red *r, char **arg, t_cmd *struc)
{
	r->i = 0;
	struc->in = FALSE;
	struc->out = FALSE;
	if (check_heredoc(arg, struc) == 1)
		return (-1);
	while (arg[r->i])
		r->i++;
	r->buf = malloc(sizeof(char *) * (r->i + 1));
	r->i = 0;
	while (arg[r->i])
	{
		if (arg[r->i][0] == '"' || arg[r->i][0] == '\'')
			r->buf[r->i] = ft_strdup(arg[r->i]);
		else
			r->buf[r->i] = trash_quote(arg[r->i]);
		r->i++;
	}
	r->buf[r->i] = NULL;
	r->i = 0;
	return (0);
}

static void	creat_tab(t_red *r, char **arg)
{
	r->i = 0;
	r->compt = 0;
	while (arg[r->i])
	{
		if (arg[r->i][0] == '<')
		{
			r->compt--;
			if (arg[r->i][1] == '\0' || (arg[r->i][1] == '<' \
			&& arg[r->i][2] == '\0'))
				r->i += 1;
		}
		else if (arg[r->i][0] == '>')
		{
			r->compt--;
			if (arg[r->i][1] == '\0' || (arg[r->i][1] == '>' \
			&& arg[r->i][2] == '\0'))
				r->i += 1;
		}
		r->i++;
		r->compt++;
	}
}

static void	fill_tab(t_red *r, char **arg)
{
	while (arg[r->i])
	{
		if (arg[r->i][0] == '<')
		{
			if (arg[r->i][1] == '\0' || (arg[r->i][1] == '<' \
			&& arg[r->i][2] == '\0'))
				free(arg[r->i++]);
			free(arg[r->i]);
		}
		else if (arg[r->i][0] == '>')
		{
			if (arg[r->i][1] == '\0' || (arg[r->i][1] == '>' \
			&& arg[r->i][2] == '\0'))
				free(arg[r->i++]);
			free(arg[r->i]);
		}
		else
			r->tmp[r->j++] = arg[r->i];
		r->i++;
	}
}

// Cette fonction doit mettre en place le pipe du heredoc s'il y en a besoin,
// pour le savoir elle check si la dernière redirection est un heredoc ou non
static int	check_heredoc(char **arg, t_cmd *struc)
{
	int	i;

	i = -1;
	struc->heredoc = 0;
	while (arg[++i])
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
	}
	if (struc->heredoc == 0)
		return (0);
	struc->here_pipe = (int *)ft_calloc(2, sizeof(int));
	if (struc->here_pipe != NULL && pipe(struc->here_pipe) == -1)
		return (error(PIPE, "0"), 1);
	else if (struc->here_pipe == NULL)
		return (error(CALLOC, "0"), 1);
	return (0);
}