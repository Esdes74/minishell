/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 14:04:59 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/04 16:41:58 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*list;

int	main(int argc, char **argv, char **env)
{
	char	*buff;
	char	*buf;
	char	*bufff;
	char	*test = NULL;
	int		id;
	char	*cmd;
	(void)	argc;
	(void)	argv;

	list = (t_list *) malloc(sizeof(t_list));
	if (list == NULL)
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
		test = readline(buff);
		// récupère la string complete
		cmd = cmd_build(test, env);
		historic_fct(bufff, test);
		bufff = test;
		free(buff);
		if (test == NULL)
			quit();
		id = fork();
		if (id == -1) // TODO : tester correctement le ctrl+C en mettant les id des enfants dans la liste
			return (1);
		else if (id == 0)
		{
			if (add_list(getpid(), list) == 1)
				return (error(ADD_LIST), 1);
		}
		else
			if (add_list(id, list) == 1)
				return (error(ADD_LIST), 1);
	}
	return (0);
}
