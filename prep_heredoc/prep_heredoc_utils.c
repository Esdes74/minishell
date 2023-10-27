/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 13:03:35 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/27 15:14:41 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include "../incs/prep_heredoc.h"

void	search_hd(t_prep *p)
{
	g_status = 0;
	p->compt = 0;
	while (p->tmp != NULL)
	{
		if (((char *)(p->tmp->data_cell->data))[0] == '<')
			if (((char *)(p->tmp->data_cell->data))[1] == '<')
				p->compt++;
		p->tmp = p->tmp->next;
	}
}

int	verif_g_status_hd(t_prep *p, t_cmd *pip)
{
	if (p->buff == NULL || g_status == 130)
	{
		if (p->buff == NULL)
		{
			g_status = 0;
			ft_printf("\n");
		}
		free(p->buff);
		free(p->buf);
		free(p->rd_line);
		pip->status_hd = 1;
		free(pip->hd_history);
		return (1);
	}
	return (0);
}
