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
	int	 	vr;
	char	*rd_line = NULL;
	/* int		id; */
	char	*cmd;
	(void)	argc;
	(void)	argv;
	(void)	env;

	cmd = NULL;
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
	while (1)
	{
		buf = getcwd(NULL, 0);
		buff = ft_strjoin(buf, "$ ");
		free(buf);
		rd_line = readline(buff);
		if (rd_line == NULL)
			return (free(bufff), free(buff), quit(), 1);
		vr = verif_read(rd_line);
		if (vr == 1)
			return (1);
		// if (bu == 0)
		// {
		// 	if (cmd_center_simple(rd_line, env) == 1)
		// 		return (free(buff), free(cmd), exit(1), 1);
		// 	free(buff);
		// 	free(cmd);
		// }
		// historic_fct(bufff, rd_line);
		// free(bufff);
		// bufff = rd_line;
		/* id = fork(); */
		/* if (id == -1) // TODO : tester correctement le ctrl+C en mettant les id des enfants dans la liste */
		/* 	return (1); */
		/* else if (id == 0) */
		/* { */
		/* 	if (add_list(getpid(), list) == 1) */
		/* 		return (error(ADD_LIST, NULL), 1); */
		/* } */
		/* else */
		/* 	if (add_list(id, list) == 1) */
		/* 		return (error(ADD_LIST, NULL), 1); */
	}
	return (0);
}
