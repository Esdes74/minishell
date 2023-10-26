/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:02:22 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/26 13:06:26 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct t_pars{
	int     i;
	int     j;
	int     compt;
	int     tmp_flag;
	int     new_flag;
	int     save_flag;
	int     *tab;
	char    **spt;
}	t_pars;

#endif