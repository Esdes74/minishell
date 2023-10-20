/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:47:27 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/20 02:58:02 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void sort_export(t_cmd *pip);
static int check_sort(t_cmd *pip);
static int pars_exp(int *flag, char	*na_val);

int export(t_cmd *pip, char *name_value)
{
    int     i;
    int     j;
    int     z;
    char    *var_name;
    char    *buff;
    char    **new_env;
    char    *na_val;
    int     flag;
    char    *buf;

    i = 0;
    flag = 0;
    na_val = check_quote(name_value);
    if (!na_val)
        return (error(MALLOC, "0"), 1);
    if (na_val != name_value)
	{
        free(name_value);
	}
	if (!ft_isalpha(na_val[0]))
        return (ft_printf_fd(2, "Error : not a valid identifier\n"), free(na_val), status = 1, -1);
	i = pars_exp(&flag, na_val);
	if (i == -1)
		return (0);
    if (na_val[i] == '\0' && na_val[i - 1] != '=' && flag == 0) // a utiliser pour export sans rien
    {
		// if (check_if_double(pip->exp_env, na_val) == 1)
			// return (0);
        if (add_exp_env(pip, na_val) == 1)
            return (free(na_val), 1);
        free(na_val);
        pip->builtin = TRUE;
        return (0);
    }
    var_name = malloc(sizeof(char) * (i + 2)); // + 2 pour gerer le '='
    if (!var_name)
        return (error(MALLOC, "0"), 1);
    z = 0;
    while (z <= i)
    {
        var_name[z] = na_val[z];
        z++;
    }
    var_name[z] = '\0';
    i = 0;
    z = 0;
	j = pars_exp(&flag, na_val);
    while (pip->env[i]) // cherche dans env pour remplacer
    {
		z = pars_exp(&flag, pip->env[i]);
            if (flag == 1)
            {
				if (ft_strncmp_wo_plus(pip->env[i], na_val, z + 1) == 0)
        		{
					na_val = rm_char(na_val, '+');
					j = pars_exp(&flag, na_val);
					j++;
					z = ft_strlen(na_val + j) + ft_strlen(pip->env[i]);
					buf = malloc(sizeof(char) * (z + 1));
					if (!buf)
						return (free(na_val), 1);
					ft_strlcpy(buf, pip->env[i], ft_strlen(pip->env[i]) + 1);
					ft_strlcat(buf, na_val + j, z + 1);
					free(pip->env[i]);
					pip->env[i] = ft_strdup(buf);
					free (buf);
					break ;
            	}
			}
			else 
			{
				if (ft_strncmp(pip->env[i], na_val, z + 1) == 0)
				{
					free(pip->env[i]);
					pip->env[i] = ft_strdup(na_val);
					break;
				}
			}
        i++;
    }
    if (pip->env[i] == NULL)
    {
        new_env = malloc(sizeof(char *) * (i + 2));
        if (!new_env)
            return (error(MALLOC, "0"), free(var_name), 1);
        i = 0;
        while (pip->env[i])
        {
            new_env[i] = pip->env[i];
            i++;
        }
        na_val = rm_char(na_val, '+');
        new_env[i] = ft_strdup(na_val);
		if (!new_env[i])
			return (free(var_name), free(na_val), 2);
        new_env[i + 1] = NULL;
        free(pip->env);
        pip->env = new_env;
    }
    i = 0;
	j = pars_exp(&flag, na_val);
	z = 0;
    if (pip->exp_env != NULL)
    {
        while (pip->exp_env[i]) // cherche dans exp_env pour remplacer
        {
			z = pars_exp(&flag, &(pip->exp_env[i])[11]);
			if (flag == 1)
			{
				if (ft_strncmp_wo_plus(pip->exp_env[i] + 11, na_val, z + 1) == 0) // -1 est bon mais il faut une autre comapraison
				{
					j++;
					buff = ft_strjoin(pip->exp_env[i], na_val + j);
					free(pip->exp_env[i]);
					pip->exp_env[i] = ft_strdup(buff);
					free(buff);
					break;
				}
			}
			else
			{
				if (ft_strncmp(pip->exp_env[i] + 11, na_val, z + 1) == 0)
				{
					buff = ft_strdup(na_val); // peut etre douteux maintenant que je supprime le '=' a voir comment je peux remplacer ca 
					if (!buff)
						return (free(var_name), free(na_val), 2);
					free(pip->exp_env[i]);
					pip->exp_env[i] = ft_strjoin("declare -x ", buff);
					if (!pip->exp_env[i])
						return (free(buff), free(var_name), free(na_val), 2);
					free(buff);
					break;
				}
			}
            i++;
        }
        if (pip->exp_env[i] == NULL) // Ajoute dans exp_env si rien trouvé
            if (add_exp_env(pip, na_val) == 1)
                return (free(var_name), 2);
    }
    pip->builtin = TRUE;
    free(var_name);
    free(na_val);
    return (0);
}

static int	check_double_exp_env(char **exp_env, char *str)
{
	size_t	i;
	int		z;

	i = 0;
	while(exp_env[i])
	{
		z = pars_exp(&0, &(exp_env[i])[11]);
			if (ft_strncmp(&(exp_env[i])[11], str, z))
	}	
}
