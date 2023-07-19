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
-   il faut prendre en compte entré et sortie
-   uniquement la string que on lui envoie doit s'executer
-   le nombre de pipe et le nombre de processeur sera établis en amont
-   les dup et la fermeture des pipes en amont
-      en amont pour le heredoc
*/

#include "minishell.h"
static int	parent_process_wait(t_cmd *struc);
static int	end(t_cmd *struc);

int cmd_center(char *str, t_cmd *fd, char **env) //j'ai enlever la condtion si env != NULL
{
	int		i;
    char    **buf;


    i = exec(fd, str, env);
    if (i == 2 || i == 1)
        return (1);
}

