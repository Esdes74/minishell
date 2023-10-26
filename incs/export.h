/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:06:55 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/26 18:53:31 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT
# define EXPORT

typedef struct t_exp{
    char    **new_env;
    char    *na_val;
    int     flag;
    char    *buf;
	int		i;
} t_exp;

int strcmp_plus(char *first, char *sec, size_t n);

int pars_exp(int *flag, char *na_val);

int	check_if_replace_env(t_exp *exp, t_cmd *pip, int i, int j);

int	check_if_replace_exp_env(t_cmd *pip, t_exp *exp, int z, int j);

void sort_export(t_cmd *pip);

#endif