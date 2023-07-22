/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_center.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:17:58 by marvin            #+#    #+#             */
/*   Updated: 2023/07/04 15:17:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
-   il faut prendre en compte entré et sortie en amont
-   uniquement la string que on lui envoie doit s'executer
-   le nombre de pipe et le nombre de processeur sera établis en amont
-   les dup et la fermeture des pipes en amont
-      en amont pour le heredoc
*/

#include "minishell.h"
static int  search_builtins(char *str, char **envi);
static int	execute_child(char **environ, char *str);
static char *adapt_string(char *str, int *option);
static char *determine_echo_or_cd(char *str, int *i, int *option);

int cmd_center(char *str, t_cmd *fd, char **env) //j'ai enlever la condtion si env != NULL 
{
	int		i;
    int     check;
    fd->infile = NULL;
    fd->outfile = NULL;

    i = 0;
    check = search_builtins(str, env);
    if (check == 1)
        return (0);
    if (check == 2)
        return (1);
    fd->infile = NULL;
    fd->outfile = NULL;
    if (execute_child(env, str) == 1)
        return (1);
    if (i == 2 || i == 1)
        return (1);
    return (0);
}

static int search_builtins(char *str, char **envi)
{
    char    *buf;
    int     option;
    int     i;

    option = 0;
    i = 0;
    while (str[i] == ' ') // actuellement  le probleme c'est si on met pas le echo en premier par exemple, il ne fonctionnera pas, mais avec un split avec les pipes ça dne devrais pas poser de soucis
        i++;
    if (ft_strncmp(str + i, "pwd", 3) == 0)
        return (pwd(), 1);
    else if (ft_strncmp(str + i, "exit", 4) == 0)
        return (exitt(), 1);
    else if (ft_strncmp(str + i, "env", 3) == 0)
        return (env(envi), 1);
    else if (ft_strncmp(str + i, "echo", 4) == 0 || ft_strncmp(str + i, "cd", 2) == 0)
    {
        buf = adapt_string(str, &option);
        if (buf == NULL)
            return (error(MALLOC, NULL), 2);
        if (option == 0)
            return(echo(buf, option), 1);
        else if (option == 1)
            return (echo(buf, option), 1);
        else if (option == 2)
            return (cd(buf), 1);
    }
    return (0);
}

static int	execute_child(char **environ, char *str)
{
	char	**splitted;
	char	*cmd;

	splitted = ft_split(str, ' '); //au parsing les commande genre ls -a devront être envoyé comme ça "ls -a"
	if (splitted == NULL)
		return (error(SPLIT, "0"), 1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (anihilation(splitted), 1);
	// if (close_all_pipes(pip) == 1)
	// 	return (free(cmd), anihilation(splitted), 2);
	execve(cmd, splitted, environ);
	error(EXEC, "0");
	return (free(cmd), anihilation(splitted), 2);
}

static char *adapt_string(char *str, int *option)
{
    char *buf;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    while (str[i] == ' ')
        i++;
    buf = determine_echo_or_cd(str, &i, option);
    if (buf != NULL)
        return (buf);
    k = ft_strlen(str) - i;
    buf = ft_calloc(sizeof(char), k + 1);
    if (buf == NULL)
        return (NULL);
    while (str[i] == ' ')
        i++;
    while (j < k)
    {
        if (str[i] == ' ' && str[i + 1] == ' ')
        {
            while (str[i] == ' ')
                i++;
            if (str[i] == '\0')
                return(buf);
            i--;
        }
        if (str[i] != '"')
        {
            buf[j] = str[i];
            j++;
        }
        i++;
    }
    return (buf);
}

static char     *determine_echo_or_cd(char *str, int *i, int *option)
{
    char *buf;

    if (ft_strncmp(str + *i, "echo", 4) == 0)
    {
        *i = *i + 4;
        if (str[*i] == '\0')
        {
            buf = ft_calloc(1, 1);
            return (buf);
        }
        if (str[*i] != ' ')
            return (*option = 3, NULL);
        while (str[*i] == ' ' || str[*i] == '"') // je sais pas pourquoi mais je compilait pas avec i++; ?
            *i += 1;
        if (str[*i] == '\0')
        {
            buf = ft_calloc(1, 1);
            return (buf);
        }
        if (ft_strncmp(str + *i, "-n", 2) == 0)
        {
            *option = 1;
            *i = *i + 2;
            if (str[*i] == '\0')
            {
                buf = ft_calloc(1, 1);
                return (buf);
            }
            while (str[*i] == '"' || str[*i] == ' ' || str[*i] == '\n')
                *i += 1;
            if (str[*i] == '\0')
            {
                buf = ft_calloc(1, 1);
                return (buf);
            }
        }
    }
    else if (ft_strncmp(str + *i, "cd", 2) == 0)
    {
        *i = *i + 2;
        if (str[*i] == 0)
        {
            buf = ft_calloc(1, 1);
            return (buf);
        }
        if (str[*i] != ' ')
            return(*option = 3, NULL);
        *option = 2;
    }
    return (NULL);
}
