/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 14:39:50 by dbaule            #+#    #+#             */
/*   Updated: 2023/07/19 16:19:30 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    anihilation(char **str)
{
    size_t  y;

    y = 0;
    while (str[y])
    {
        free(str[y]);
        y++;
    }
    free(str);
}

int	close_all_pipes(t_cmd *pi)
{
	int	j;

	j = 0;
	while (j < pi->nb_pipe)
	{
		if (close(pi->pipe[j][0]) == -1)
			return (errors(CLOSE_P0, "0"), anihilation((char **) pi->pipe), 1);
		if (close(pi->pipe[j][1]) == -1)
			return (errors(CLOSE_P1, "0"), anihilation((char **) pi->pipe), 1);
		j++;
	}
	anihilation((char **) pi->pipe);
	return (0);
}
