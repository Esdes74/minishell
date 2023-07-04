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
Il faut :
-   initialiser des pipe à chaque liste chainé ou alors, faire un split de ce qu'il y a entre caractere que l'on arrive a distinguer comme commande ?
-   récupéré les info dans le pipe[0] et envoyer les info en list->next.pipe[1]
-   à la fin, qu'il faudra déterminer, (fin de la chaine de caractere de readline?), récupéré la dernière pipe et la push soit dans le terminal directement, 
soit dans un fichier externe si demandé
-   il faudra différencier < et << et faire différents executable en fonction, (car pas la même ouverture de fichier)
-   on distinguera fichier et commande en fonction du parsing, s'il y a des éléments comme < ou >> ce genre de choses, 
pour ce faire, on enverra uniquement les éléments de commandes dans cette fonction !
*/

#include "minishell.h"

int cmd_center(char *str, t_list list, char **env)
{
    char    *cmd;

    cmd = cmd_build(str, env);
    if (pipe(list.fd) == -1);
        return (error(PIPE, NULL), 1);
}