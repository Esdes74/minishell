/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:47:27 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/19 19:04:38 by dbaule           ###   ########.fr       */
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

int add_exp_env(t_cmd *pip, char *str)
{
    int     i;
    char    *buf;
    char    **new_one;
    int     flag;

    buf = malloc(sizeof(char) * (ft_strlen(str) + 12));
    if (!buf)
        return (1);
    ft_strlcpy(buf, "declare -x ", 12);
    ft_strlcpy(&(buf[11]), str, ft_strlen(str) + 1);
    i = 0;
    flag = 0;
    while (pip->exp_env[i])
    {
        if (strncmp(buf, pip->exp_env[i], ft_strlen(buf) + 1) == 0 && ft_in('=', buf) == 1)
            return (free(buf), 0);
        else if (strncmp(buf, pip->exp_env[i], ft_strlen(buf)) == 0 && ft_in('=', buf) == 0 && pip->exp_env[i][ft_strlen(buf)] == '=')
            return (free(buf), 0);
        if (ft_in('=', buf) == 1 && strncmp(buf, pip->exp_env[i], ft_strlen(pip->exp_env[i])) == 0)
            flag = 1;
        i++;
    }
    while (pip->exp_env[i])
        i++;
    if (flag == 0)
        new_one = malloc(sizeof(char*) * (i + 2));
    else
        new_one = malloc(sizeof(char*) * (i + 1));
    if (new_one == NULL)
        return (free(buf), error(MALLOC, 0), 1);
    i = 0;
    while (pip->exp_env[i])
    {
        if (ft_in('=', buf) == 1 && strncmp(buf, pip->exp_env[i], ft_strlen(pip->exp_env[i])) == 0)
        {
            new_one[i] = ft_strdup(buf);
            if (!new_one[i])
                return (free(new_one), free(buf), 1);
            flag = 1;
        }
        else
        {
            new_one[i] = ft_strdup(pip->exp_env[i]);
            if (!new_one[i])
                return (free(new_one), free(buf), 1);
        }
        i++;
    }
    if (flag == 0)
    {
        new_one[i] = buf;
        new_one[i + 1] = NULL;
    }
    else
    {
        new_one[i] = NULL;
        free(buf);
    }
    anihilation(pip->exp_env);
    pip->exp_env = new_one;
    sort_export(pip);
    return (0);
}

static void sort_export(t_cmd *pip)
{
    size_t  i;
    char    *buf;

    i = 0;
    while (pip->exp_env[i])
    {
        if (pip->exp_env[i + 1] && strncmp(pip->exp_env[i], pip->exp_env[i + 1],ft_strlen(pip->exp_env[i])) > 0)
        {
            buf = pip->exp_env[i];
            pip->exp_env[i] = pip->exp_env[i + 1];
            pip->exp_env[i + 1] = buf;
        }
        i++;
    }
    if (check_sort(pip) == 1)
        sort_export(pip);
    else
        return ;
}

static int check_sort(t_cmd *pip)
{
    size_t i;

    i = 0;
    while (pip->exp_env[i + 1])
    {
        if (ft_strncmp(pip->exp_env[i], pip->exp_env[i + 1], ft_strlen(pip->exp_env[i])) > 0)
            return (1);
        i++;
    }
    return (0);
}

static int pars_exp(int *flag, char	*na_val)
{
	int	i;

	i = 0;
    if (na_val[0] == '=')
        return (ft_printf_fd(2, "Error : not a valid identifier\n"), free(na_val), status = 1, -1);
    while (na_val[i] && na_val[i] != '=')
    {
        if ((na_val[i] == '-' || (i > 0 && na_val[i] == '+' && na_val[i - 1] != '+' && (na_val[i + 1] != '=' || na_val[i + 1] == '\0'))))
            return(ft_printf_fd(2, "Error : not a valid identifier\n"), status = 1, free(na_val), -1);
        else if (na_val[i + 1] && na_val[i] == '+' && na_val[i + 1] == '=')
            *flag = 1;
        i++;
    }
	return (i);
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
