/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_center_simple.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:17:58 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/30 15:06:53 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int	execute_child(char **environ, char **str, t_cmd *pip);
static int	in_case_execve_didnt_work(t_cmd *pip, DIR *dir, char *cmd);
// static void	avoid_quote(int *j, char *str, char **new_str);

int	cmd_center_simple(char **str, t_cmd *pip)
{
	int	i;
	char **buf;

	i = 0;
	while (str[i])
	{
		buf[i] = trash_quote(str[i]);
		if (!buf[i])
			return (close_all_pipes(pip), 1);
		i++;
	}
	if (search_builtins(str, pip, buf) == 1)
		return (anihilation(buf), 1);
	if (execute_child(pip->env, buf, pip) == 1)
		return (anihilation(buf), 1);
	return (-1);
}

static int	execute_child(char **environ, char **str, t_cmd *pip)
{
	// int		i;
	char	*cmd;
	// char	*new_str;
	DIR		*dir;

	// i = -1;
	dir = NULL;
	// while (str[++i])
	// {
	// 	new_str = check_quote(str[i]);
	// 	if (str[i] != new_str)
	// 		free(str[i]);
	// 	str[i] = new_str;
	// }
	if (str[0][0] == '\0')
		return (error(CMD, str[0]), 1);
	cmd = cmd_build(str[0], environ);
	if (cmd == NULL)
		return (1);
	exec_signals();
	execve(cmd, str, pip->env);
	if (in_case_execve_didnt_work(pip, dir, cmd) == 1)
		return (1);
	return (1);
}

static int	in_case_execve_didnt_work(t_cmd *pip, DIR *dir, char *cmd)
{
	pip->ani_flag = 1;
	dir = opendir(cmd);
	if (dir == NULL && access(cmd, F_OK | X_OK) != -1)
	{
		ft_printf_fd(2, "Error : invalid permission\n");
		free(cmd);
		return (1);
	}
	else if (dir != NULL)
		error(DIREC, cmd);
	else
		error(EXEC, "0");
	closedir(dir);
	free(cmd);
	return (0);
}

// char	*check_quote(char *str)
// {
// 	int		i;
// 	int		j;
// 	char	*new_str;

// 	j = 0;
// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '"' || str[i] == '\'')
// 			j++;
// 		i++;
// 	}
// 	if (j == 0)
// 		return (str);
// 	new_str = malloc(sizeof(char) * ((i - j) + 1));
// 	if (!new_str)
// 		return (error(MALLOC, "0"), NULL);
// 	j = 0;
// 	avoid_quote(&j, str, &new_str);
// 	new_str[j] = '\0';
// 	return (new_str);
// }

// static void	avoid_quote(int *j, char *str, char **new_str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] != '"' && str[i] != '\'')
// 		{
// 			new_str[0][*j] = str[i];
// 			*j += 1;
// 		}
// 		i++;
// 	}
// }
