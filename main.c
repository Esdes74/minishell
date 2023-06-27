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
	int		id;

	signals();
	while (1)
	{
		buff = getcwd(NULL, 0);
		readline(ft_strjoin(buff, "$ "));
		id = fork();
		if (id == -1)
			return (1);
		else if (id == 0)
			sleep(2);
	}
	return (0);
}
