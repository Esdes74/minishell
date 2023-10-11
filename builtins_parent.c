/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/09/05 02:48:24 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  unset_env(t_cmd *pip, char *name_value, int len);
static int  unset_exp_env(t_cmd *pip, char *name_value, int len);

int    exitt(t_cmd *pip, t_list *tmp, t_bool ret_value)
{
    if (tmp->len > 1)
        return(error(TOO_MANY_ARG, "exit"), 1);
    free_all(pip);
    silent_quit();
    return((int) ret_value);
}

void    cd(char *path, t_cmd *pip, t_list *spt)
{
    if (spt->len <= 2 && path != NULL && chdir(path) != 0)
    {
        pip->status = 1;
        ft_printf_fd(2, "-bash: cd: %s: No such file or directory\n");
        return ;
    }
    else if (spt->len > 2)
    {
        error(TOO_MANY_ARG, "cd");
        pip->status = 1;
        return ;
    }
    pip->status = 0;
    pip->builtin = TRUE;
}

int export(t_cmd *pip, char *name_value)
{
    int     i;
    int     z;
    char    *var_name;
    char    *buff;
    char    **new_env;

    i = 0;
    pip->status = 1;
    if (name_value[i] == '=') // ca doit faire une erreur
        return (ft_printf_fd(2, "not a valid identifier\n"), 2);
    while (name_value[i] && name_value[i] != '=')
    {
        if (name_value[i] == '-' || name_value[i] == '+')
            return(ft_printf_fd(2, " not a valid identifier\n"), 2);
        i++;
    }
    if (name_value[i] == '\0') // a utiliser pour export sans rien
    {
        if (add_exp_env(pip, name_value) == 1)
            return (1);
        free(name_value);
        pip->builtin = TRUE;
        pip->status = 0;
        return (0);
    }
    var_name = malloc(sizeof(char) * (i + 1));
    if (!var_name)
        return (error(MALLOC, "0"), 1);
    z = i;
    i = 0;
    while (i < z)
    {
        var_name[i] = name_value[i];
        i++;
    }
    var_name[i] = '\0';
    i = 0;
    while (pip->env[i]) // cherche dans env pour remplacer
    {
        if (strncmp(pip->env[i], var_name, (ft_strlen(var_name))) == 0)
        {
            free(pip->env[i]);
            pip->env[i] = name_value;
            break;
        }
        i++;
    }
    if (pip->env[i] == NULL) // ajoute dans env si rien trouvé
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
        new_env[i] = name_value;
        new_env[i + 1] = NULL;
        free(pip->env);
        pip->env = new_env;
    }
    i = 0;
    while (pip->exp_env[i]) // cherche dans exp_env pour remplacer
    {
        if (strncmp(pip->exp_env[i] + 11, var_name, \
        (ft_strlen(var_name))) == 0)
        {            
            buff = ft_strdup(name_value);
            if (!buff)
                return (free(var_name), 2);
            free(pip->exp_env[i]);
            pip->exp_env[i] = ft_strjoin("declare -x ", buff);
            if (!pip->exp_env[i])
                return (free(buff), free(var_name), 2);
            free(buff);
            break;
        }
        i++;
    }
    if (pip->exp_env[i] == NULL) // Ajoute dans exp_env si rien trouvé
        if (add_exp_env(pip, name_value) == 1)
            return (free(var_name), 2);
    pip->builtin = TRUE;
    pip->status = 0;
    free(var_name);
    return (0);
}

int unset(t_cmd *pip, char *name_value)
{
    int     i;
    int     trigger;

    i = 0;
    trigger = 0;
    pip->status = 1;
    while (pip->env[i])
        if (strncmp(pip->env[i++], name_value, (ft_strlen(name_value))) == 0)
            trigger++;
    i = 0;
    while (pip->exp_env[i])
        if (strncmp(pip->exp_env[i++] + 11, name_value, \
        (ft_strlen(name_value))) == 0)
            trigger++;
    if (trigger == 0)
    {
        pip->builtin = TRUE;
        pip->status = 0;
        return (0);
    }
    else
        if (unset_env(pip, name_value, i) == 1)
            return (1);
    if (unset_exp_env(pip, name_value, i) == 1)
        return (1);
    return (0);
}

static int  unset_env(t_cmd *pip, char *name_value, int len)
{
    int     i;
    int     j;
    char    **new_env;

    i = 0;
    j = 0;
    new_env = (char **) malloc(sizeof(char *) * (len + 1));
    if (new_env == NULL)
        return (error(MALLOC, "0"), 1);
    while (pip->env[j])
    {
        if (strncmp(pip->env[j], name_value, (ft_strlen(name_value))) == 0)
            free(pip->env[j++]);
        else
            new_env[i++] = pip->env[j++];
    }
    new_env[i] = NULL;
    free(pip->env);
    pip->env = new_env;
    pip->builtin = TRUE;
    pip->status = 0;
    return (0);
}

static int  unset_exp_env(t_cmd *pip, char *name_value, int len)
{
    int     i;
    int     j;
    char    **new_env;

    i = 0;
    j = 0;
    new_env = (char **) malloc(sizeof(char *) * (len + 1));
    if (new_env == NULL)
        return (error(MALLOC, "0"), 1);
    while (pip->exp_env[j])
    {
        if (strncmp(pip->exp_env[j] + 11, name_value, \
            (ft_strlen(name_value))) == 0)
            free(pip->exp_env[j++]);
        else
            new_env[i++] = pip->exp_env[j++];
    }
    new_env[i] = NULL;
    free(pip->exp_env);
    pip->exp_env = new_env;
    pip->builtin = TRUE;
    pip->status = 0;
    return (0);
}

void    print_export(char **exp_env)
{
    int i;
    int j;
    int flag;

    i = 0;
    while (exp_env[i])
    {
        j = 0;
        flag = 0;
        while (exp_env[i][j])
        {
            ft_printf("%c", exp_env[i][j++]);
            if (exp_env[i][j - 1] == '=' && flag++ == 0)
                ft_printf("\"");
        }
        if (flag != 0)
            ft_printf("\"");
        ft_printf("\n");
        i++;
    }
}