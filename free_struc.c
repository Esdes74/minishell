/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 14:39:50 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/26 14:14:20 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

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
			return (error(CLOSE, "0"), anihilation((char **) pi->pipe), 1);
		if (close(pi->pipe[j][1]) == -1)
			return (error(CLOSE, "0"), anihilation((char **) pi->pipe), 1);
		free(pi->pipe[j]);
		j++;
	}
	free(pi->pipe);
	return (0);
}

void	free_all(t_cmd *pip)
{
	anihilation(pip->env);
	anihilation(pip->exp_env);
	anihilation(pip->save_path);
	pip->env = NULL;
	pip->exp_env = NULL;
	pip->save_path = NULL;
}
