/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 14:26:08 by dbaule            #+#    #+#             */
/*   Updated: 2023/09/01 16:07:27 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int cpy_env(char **env, t_cmd *pip)
{
    int i;
    
    i = 0;
    while (env[i])
        i++;
    pip->env = malloc(sizeof(char*) * (i + 1));
    if (pip->env == NULL)
        return (error(MALLOC, 0), 1);
    i = 0;
    while (env[i])
    {
        pip->env[i] = ft_strdup(env[i]);
        if (pip->env[i] == NULL)
            return (error(MALLOC, 0), free(pip->env), 1);
        i++;
    }
    pip->env[i] = NULL;
    return (0);
}