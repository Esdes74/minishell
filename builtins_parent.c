/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/18 18:56:17 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  unset_env(t_cmd *pip, char *name_value, int len);
static int  unset_exp_env(t_cmd *pip, char *name_value, int len);

unsigned char    intermediate_exit(t_list *tmp)
{
    char    *new_tmp;
    int     ret_value;
    int     i;

    if (tmp->len > 2)
        return(error(TOO_MANY_ARG, "exit"), exitt(1));
    if (tmp->len == 1)
        return (exitt(0));
    else if (tmp->len == 2)
    {
        new_tmp = check_quote((char *) tmp->head->next->data_cell->data);
        if (new_tmp != tmp->head->next->data_cell->data)
            free(tmp->head->next->data_cell->data);
        tmp->head->next->data_cell->data = new_tmp;
        i = 0;
        while (new_tmp[i])
        {
            if (!ft_isdigit(new_tmp[i]))
                if (i > 0 || (i == 0 && new_tmp[i] != '-' && new_tmp[i] != '+'))
                    return (error(NUM_ARG, "0"), exitt(2));
            i++;
        }
        ret_value = ft_atoi(new_tmp);
    }
    return (exitt((unsigned char) ret_value));
}

unsigned char    exitt(unsigned char ret_value)
{
    // ft_printf_fd(1, "exit\n");
    // free_all(pip);
    status = (int)ret_value;
    // silent_quit();
    return(ret_value);
}

void    cd(char *path, t_cmd *pip, t_list *spt)
{
    if (spt->len <= 2 && path != NULL && chdir(path) != 0)
    {
        ft_printf_fd(2, "-bash: cd: %s: No such file or directory\n");
        return ;
    }
    else if (spt->len > 2)
    {
        error(TOO_MANY_ARG, "cd");
        return ;
    }
    pip->builtin = TRUE;
}

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
        return (1);
    if (na_val != name_value)
        free(name_value);
    if (!ft_isalpha(na_val[0]))
        return (ft_printf_fd(2, "Error : not a valid identifier\n"), free(na_val), status = 1, 0);
    if (na_val[0] == '=')
        return (ft_printf_fd(2, "Error : not a valid identifier\n"), free(na_val), status = 1, 0);
    while (na_val[i] && na_val[i] != '=')
    {
        if ((na_val[i] == '-' || (i > 0 && na_val[i] == '+' && na_val[i - 1] != '+' && (na_val[i + 1] != '=' || na_val[i + 1] == '\0'))))
            return(ft_printf_fd(2, "Error : not a valid identifier\n"), status = 1, free(na_val), 0);
        else if (na_val[i + 1] && na_val[i] == '+' && na_val[i + 1] == '=')
            flag = 1;
        i++;
    }
    if (na_val[i] == '\0' && na_val[i - 1] != '=' && flag == 0) // a utiliser pour export sans rien
    {
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
    while (pip->env[i]) // cherche dans env pour remplacer
    {
        if (ft_strncmp_wo_plus(pip->env[i], var_name, ft_strlen(var_name) - 1) == 0)
        {
            if (flag == 1)
            {
                na_val = rm_char(na_val, '+');
                while (na_val[z] != '=')
                    z++;
                j = z;
                na_val = rm_char(na_val, '=');
                z = ft_strlen(na_val + j) + ft_strlen(pip->env[i]);
                buf = malloc(sizeof(char) * (z + 1));
                if (!buf)
                    return (1);
                ft_strlcpy(buf, pip->env[i], ft_strlen(pip->env[i]) + 1);
                ft_strlcat(buf, na_val + j, z + 1);
                free(pip->env[i]);
                pip->env[i] = buf;
                break ;
            }
            free(pip->env[i]);
            pip->env[i] = na_val;
            break;
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
        new_env[i] = na_val;
        new_env[i + 1] = NULL;
        free(pip->env);
        pip->env = new_env;
    }
    i = 0;
    if (pip->exp_env != NULL)
    {
        while (pip->exp_env[i]) // cherche dans exp_env pour remplacer
        {
            if (ft_strncmp(pip->exp_env[i] + 11, var_name, (ft_strlen(var_name))) == 0)
            {
                buff = ft_strdup(na_val);
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
            if (add_exp_env(pip, na_val) == 1)
                return (free(var_name), 2);
    }
    pip->builtin = TRUE;
    free(var_name);
    return (0);
}

int unset(t_cmd *pip, char *name_value)
{
    int     i;
    int     trigger;

    i = 0;
    trigger = 0;
    while (pip->env[i])
        if (ft_strncmp(pip->env[i++], name_value, (ft_strlen(name_value))) == 0)
            trigger++;
    i = 0;
    while (pip->exp_env[i])
        if (ft_strncmp(pip->exp_env[i++] + 11, name_value, \
        (ft_strlen(name_value))) == 0)
            trigger++;
    if (trigger == 0)
    {
        pip->builtin = TRUE;
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
        if (strncmp(pip->env[j], name_value, (ft_strlen(name_value))) == 0 && ft_in('=', name_value) == 1)
            free(pip->env[j++]);
        else if (strncmp(pip->env[j], name_value, (ft_strlen(name_value) + 1)) == 0 && ft_in('=', name_value) == 0)
            free(pip->env[j++]);
        else
            new_env[i++] = pip->env[j++];
    }
    new_env[i] = NULL;
    free(pip->env);
    pip->env = new_env;
    pip->builtin = TRUE;
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
    return (0);
}
