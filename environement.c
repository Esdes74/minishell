/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 14:26:08 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/26 11:54:29 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int check_if_double(char **env, char *cmp);
static char **ft_cpy_env(char **env);

int cpy_env(char **env, t_cmd *pip)
{
    int i;
    int z;
    int for_sh;
    char    *sh_lvl;
    char    *buf;
    
    i = 0;
    while (env[i])
        i++;
    pip->env = malloc(sizeof(char*) * (i + 1));
    if (pip->env == NULL)
        return (error(MALLOC, 0), 1);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "PWD=", 4) == 0)
        {
            pip->save_path = ft_split(&(env[i])[4], '/');
            if (!pip->save_path)
                return (error(MALLOC, 0), pip->env = '\0', anihilation(pip->env), 1);
        }
        pip->env[i] = ft_strdup(env[i]);
        if (pip->env[i] == NULL)
            return (error(MALLOC, 0), pip->env = '\0', anihilation(pip->env), anihilation(pip->save_path), 1);
        i++;
    }
    pip->env[i] = NULL;
    i = 0;
    // met le SHLVL à 2 fait bugger le tester


    while (pip->env[i]) 
    {
        if (strncmp(pip->env[i], "SHLVL=", 6) == 0)
        {
            z = 0;
            while ((pip->env[i][z] >= 'A' && pip->env[i][z] <= 'Z') || pip->env[i][z] == '=')
                z++;
            for_sh = ft_atoi(&pip->env[i][z]);
            for_sh++;
            if (for_sh > 1000)
            {
                for_sh = 1;
                ft_printf_fd(2, "warning : shell level (1000) too high, resetting to 1\n");
            }
            sh_lvl = ft_itoa(for_sh);
            if (!sh_lvl)
                return (anihilation(pip->env), error(MALLOC, 0), 1);
            buf = malloc(sizeof(char) * (ft_strlen(sh_lvl) + 7));
            if (!buf)
                return (anihilation(pip->env), 1);
            ft_strlcpy(buf, "SHLVL=", 7);
            ft_strlcat(buf, sh_lvl, ft_strlen(sh_lvl) + 7);
            free(sh_lvl);
            if (export(pip, buf) == 1)
                return (anihilation(pip->env), anihilation(pip->save_path), free(buf), 1);
            free(buf);
        }
        i++;
    }
    return (0);
}

int initialize_exp_env(t_cmd *pip, char **env)
{
	int i;
	int z;
	char *buf;
	char **buf_env;
	
	i = 0;
	buf_env = ft_cpy_env(env);
	if (!buf_env)
		return (1);
	while (buf_env[i])
	{
		z = i + 1;
		while (buf_env[z])
		{
			if (ft_strncmp(buf_env[i], buf_env[z], ft_strlen(buf_env[i])) > 1)
			{
				if (check_if_double(buf_env, buf_env[z]) == 0)
				{
					buf = buf_env[i];
					buf_env[i] = buf_env[z];
					buf_env[z] = buf;
				}
			}
			z++;
		}
		i++;
	}
	buf_env[i] = NULL;
	pip->exp_env = ft_calloc(sizeof (char*), (i + 1));
	if (!pip->exp_env)
		return (1);
	i = 0;
	if (strncmp(buf_env[i], "declare -x ", 11) != 0)
	{
		while (buf_env[i])
		{
			z = 11 + ft_strlen(buf_env[i]);
			pip->exp_env[i] = ft_calloc(sizeof(char), (z + 1));
			ft_strlcpy(pip->exp_env[i], "declare -x ", z);
			ft_strlcpy(&(pip->exp_env[i][11]), buf_env[i], ft_strlen(buf_env[i]) + 1);
			i++;
		}
	}
	else
	{
		while (buf_env[i])
		{
			z = ft_strlen(buf_env[i]);
			pip->exp_env[i] = ft_calloc(sizeof(char), (z + 1));
			ft_strlcpy(pip->exp_env[i], buf_env[i], ft_strlen(buf_env[i]) + 1);
			i++;
		} 
	}
	pip->exp_env[i] = NULL;
	anihilation(buf_env);
	return (0);
}

static int check_if_double(char **env, char *cmp)
{
	int i;
	int trigger;

	i = 0;
	trigger = 0;
	while (env[i])
	{
		if (strncmp(env[i], cmp, ft_strlen(cmp)) == 0)
			trigger++;
		i++;
	}
	if (trigger > 1)
		return (1);
	return (0);
}

static char **ft_cpy_env(char **env)
{
	int     i;
	char    **buf_env;

	i = 0;
	while (env[i])
		i++;
	buf_env = (char **) malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		buf_env[i] = ft_strdup(env[i]);
		i++;
	}
	buf_env[i] = NULL;
	return (buf_env);
}

