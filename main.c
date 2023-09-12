/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 14:04:59 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/05 14:03:50 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*list;

int	main(int argc, char **argv, char **env)
{
	char	*buff;
	char	*buf;
	char	*bufff;
	char	*rd_line = NULL;
	/* int		id; */
	char	*cmd;
	(void)	argc;
	(void)	argv;
	t_cmd   pip;

	cmd = NULL;
	if (getenv("PATH") == NULL)
		return (0);
	if (cpy_env(env, &pip) == 1)
		return (error(MALLOC, '\0'), 1);
	if (initialize_exp_env(&pip, pip.env) == 1)
		return (error(MALLOC, 0), 1);
	list = (t_list *) malloc(sizeof(t_list));
	if (list == NULL && cmd == NULL)
		return (error(MALLOC, NULL), 1);
	init_list(list);
	if (add_list(getpid(), list) == 1)
		return (error(ADD_LIST, NULL), 1);
	signals();
	using_history();
	rl_bind_key('\t', rl_complete);
	bufff = NULL;
	pip.hd_history = NULL;
	pip.status = 0;
	while (1)
	{
		if (pip.hd_history != NULL)
		{
			anihilation(pip.hd_history);
			pip.hd_history = NULL;
		}
		pip.nb_proc = 0;
		buf = getcwd(NULL, 0);
		buff = ft_strjoin(buf, "$ ");
		free(buf);
		rd_line = readline(buff);
		free(buff);
		if (rd_line == NULL)
		{
			free(bufff);
			free(rd_line);
			free_all(&pip);
			return (quit(), 1);
		}
		rd_line = verif_read(rd_line, &pip);
		if (rd_line == NULL)
			return (1);
		free(cmd);
		historic_fct(bufff, rd_line, &pip);
		free(bufff);
		bufff = ft_strdup(rd_line);
	}
	return (0);
}
