/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 20:48:44 by dbaule            #+#    #+#             */
/*   Updated: 2023/07/19 16:42:39 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_child(t_cmd *pip, char *str, char **environ);
static int	execute_child(t_cmd *pip, char **environ, char *str);

/*
Forking -> creating a child process that will execute command
*/

int	exec(t_cmd *struc, char *str, char **environ)
{
	int	id;
	int	err;

	id = fork();
	if (id == -1)
		return (error(FORK, "0"), 1);
	if (id == 0)
	{
		err = exec_child(struc, str, environ);
		if (err == 1)
			return (1);
		if (err == 2)
			return (2);
	}
	return (0);
}

/*
The hub to execute all the child that we need to
*/

static int	exec_child(t_cmd *pip, char *str, char **environ)
{
	int	err;

    // dup2(fd->infile, STDIN_FILENO);
    // dup2(fd->outfile, STDOUT_FILENO);
	err = execute_child(pip, environ, str);
	if (err == 1)
		return (1);
	else if (err == 2)
		return (2);
	return (0);
}

/* Get the command from the arguments that the user set it up for the computer
and execute it */

static int	execute_child(t_cmd *pip, char **environ, char *str)
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
