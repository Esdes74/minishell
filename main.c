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

int	main(void)
{
	char	*buff;
	char	*buf;
	char	*test = NULL;
	int		id;

	list = (t_list *) malloc(sizeof(t_list));
	if (list == NULL)
		return (error(MALLOC), 1);
	init_list(list);
	if (add_list(getpid(), list) == 1)
		return (error(ADD_LIST), 1);
	signals();
	while (1)
	{
		buf = getcwd(NULL, 0);
		buff = ft_strjoin(buf, "$ ");
		free(buf);
		test = readline(buff);
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
