/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 21:49:39 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/27 11:02:01 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONEMENT_H
# define ENVIRONEMENT_H

typedef struct t_env
{
    int for_sh;
    char    *sh_lvl;
    char    *buf;
} t_env;

#endif