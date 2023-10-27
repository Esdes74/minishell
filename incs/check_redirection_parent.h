/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection_parent.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:22:39 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/27 15:36:23 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_REDIRECTION_H
# define CHECK_REDIRECTION_H

typedef struct t_red_par
{
    char    **tmp;
    int     file;
    int     compt;
    char    **buf;
}	t_red_par;

#endif
