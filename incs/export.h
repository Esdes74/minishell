/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:06:55 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/26 13:19:44 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT
# define EXPORT

typedef struct t_exp{
    char    **new_env;
    char    *na_val;
    int     flag;
    char    *buf;
} t_exp;


#endif