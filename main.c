/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 14:04:59 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/03 14:17:00 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*list;

int	main(int argc, char **argv, char **env)
{
	char	*buff;
	char	*buf;
	char	*test = NULL;
	int		id;
	char	*cmd;

	list = (t_list *) malloc(sizeof(t_list));
	if (list == NULL)
		return (error(MALLOC, NULL), 1);
	init_list(list);
	if (add_list(getpid(), list) == 1)
		return (error(ADD_LIST, NULL), 1);
	signals();
	using_history();
	rl_bind_key('\t', rl_complete);
	while (1)
	{
		buf = getcwd(NULL, 0);
		buff = ft_strjoin(buf, "$ ");
		free(buf);
		test = readline(buff);
		free(buff);
		if (test == NULL)
			quit();
		// id = fork();
		// if (id == -1)
		// 	return (1);
	}
	return (0);
}
