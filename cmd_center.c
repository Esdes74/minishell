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
static int search_buildins(char *str, char **envi);
static int	execute_child(char **environ, char *str);

int cmd_center(char *str, t_cmd *fd, char **env) //j'ai enlever la condtion si env != NULL 
{
	int		i;
    fd->infile = NULL;
    fd->outfile = NULL;

    i = 0;
    if (search_buildins(str, env) == 1)
        return (0);
    fd->infile = NULL;
    fd->outfile = NULL;
    // if (fd->infile == NULL && fd->outfile == NULL)
    //     i = exec(fd, str, env);
    // else if (fd->infile == NULL)
    //     i = exec(fd, str, env);
    // else if (fd->outfile == NULL)
    //     i = exec(fd, str , env);
    if (execute_child(env, str) == 1)
        return (1);
    if (i == 2 || i == 1)
        return (1);
    return (0);
}

static int search_buildins(char *str, char **envi)
{
    if (strncmp(str, "pwd", 3) == 0)
        return (pwd(), 1);
    else if (strncmp(str, "exit", 4) == 0)
        return (exitt(), 1);
    else if (strncmp(str, "env", 3) == 0) // segfault
        return (env(envi), 1);
    else if (strncmp(str, "echo", 4) == 0) // prend pas en compte le \n sois la string de base, soit option ?
        return (echo(str, 0), 1);
    else if (strncmp(str, "cd", 2) == 0) // marche pas vraiment ?
        return (cd(pwd()), 1);
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
