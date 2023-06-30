/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 14:04:59 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/27 17:02:37 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*buff;
	// int		id;

	init_list(&list);
	if (add_list(getpid(), &list) == 1)
		return (error(ADD_LIST), 1);
	signals();
	while (1)
	{
		buff = getcwd(NULL, 0);
		readline(ft_strjoin(buff, "$ "));
		// id = fork();
		// if (id == -1)
		// 	return (1);
	}
	return (0);
}
