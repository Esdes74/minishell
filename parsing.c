/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:22 by dbaule            #+#    #+#             */
/*   Updated: 2023/07/21 09:54:05 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  - 1 : prob au niveau des espaces, le bash de base semble fonctionner lorsque l'on sépare pas les commandes c'est à dire que ls|grep minishell, fonctionne!
// par contre si je fais "ls|grep minishell", il ,ne fonctionne pas, les guillemets veut dire : "l'intérieur est une commande"

//  - 2 : notre minishell actuel ne fonctionne pas lorsque l'on met une commande entre "", tout simplement parce qu'il prend en compte comme char les guillemets
// Attention ! le vrai bash ne fontionne pas non plus si l'on met les commandes entre guillemets si l'on ajoute une option comme "ls -a" 
// donc guillemets veut dire "commande", si l'on rajouter quoi que ce soit qui diffère d'une commande comme un espace entre les guillemts, celle-ci ne fonctionnera pas

//  - 3 : ca me parait bizarre, mais apperement si je tape ça dans le bash  : ls -a|grep minishell|                 wc -l
// et bien il effectue bien la commande, même si les pipes sont collé (à verifier)

//  -4 : si dans le cas ou les pipes peuvent etre collé

/*
This function checks whether the parameter sent by the user is actually the one he should send in terms of syntax.
*/

int parsing_check(char *rd_line) // on peux compter le nombrede pipe dans cette fonction
{
    int i;
    int j;
    char **buf;

    i = 0;
    j = 0;
    buf = ft_split(rd_line, ' ');
    if (buf == NULL)
        return (error(SPLIT, 0), 1);
    while (buf[i])
    {
        if (buf[i][j] == '|' && buf[i + 1] && buf[i + 1][j] == '|')
            return(ft_printf_fd(2, "syntax error near unexpected token %c\n", buf[i][j]), 1); // faut pas free rd_line, enleve de l'historique
        while (buf[i][j])
        {
            //if (buf[i][j] == '"') il ne faut pas regarder dans les "" !
            j++;
        }
        j = 0;
        i++;
    }
    return (0);
}
