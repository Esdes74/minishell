/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslambert <eslamber@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:22 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/21 09:54:05 by eslamber         ###   ########.fr       */
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

//  - 4 : si dans le cas ou les pipes peuvent etre collé

/*
This function checks whether the parameter sent by the user is actually the one he should send in terms of syntax.
*/

void    parsing(const char *rd_line, int *flag, t_list *ret)
{
    int     i;
    int     j;
    int     compt;
    int     tmp_flag;
    int     new_flag;
    int     save_flag;
    int     *tab;
    char    **spt;

    i = 0;
    compt = 1;
    save_flag = *flag;
    spt = 0;
    new_flag = 0;
    while (rd_line[i]) // Compte le nombre d'aguments qu'il y a (le | sont contés comme des arguments a part entière)
    {
        if (rd_line[i] == '\n' && *flag == 0)
            break;
        if (rd_line[i] != ' ' && *flag == 3)
            *flag = 0;
        if (*flag == 4 && rd_line[i] == '\n')
        {
            compt++;
            *flag = 0;
        }
        if (*flag == 4 && rd_line[i] == ' ')
            *flag = 0;
        if ((rd_line[i] == '<' || rd_line[i] == '>') && *flag == 0)
            *flag = 4;
        else if (rd_line[i] == '"' && *flag == 0)
            *flag = 1;
        else if (rd_line[i] == '"' && *flag == 1)
        {
            if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
                compt++;
            new_flag = 0;
            *flag = 0;
        }
        else if (rd_line[i] == '\'' && *flag == 0)
            *flag = 2;
        else if (rd_line[i] == '\'' && *flag == 2)
        {
            if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
                compt++;
            new_flag = 0;
            *flag = 0;
        }
        else if (rd_line[i] == '\n' && (*flag == 1 || *flag == 2))
            new_flag = 1;
        else if (rd_line[i] == ' ' && *flag == 0 && i > 0)
            compt++;
        if (i > 0 && rd_line[i] == '|' && rd_line[i - 1] != ' ' && *flag == 0) // attention changement ! i > 0
            compt++;
        if (rd_line[i] == '|' && rd_line[i + 1] != ' ' && *flag == 0)
            compt++;
        if (rd_line[i] == ' ' && *flag == 0)
            *flag = 3;
        i++;
    }
    // Créer le tableau de chaine de caractères et le tableau permettant de compter le nombre de caractères dans chaque arguments
    spt = (char **) malloc(sizeof(char *) * (compt + 1));
    if (spt == NULL)
        return (error(MALLOC, NULL), annihilation(ret, free, DEBUG));
    spt[compt] = NULL;
    tab = (int *) ft_calloc(compt, sizeof(int));
    if (tab == NULL)
        return (free(spt), error(MALLOC, NULL), annihilation(ret, free, DEBUG));
    i = 0;
    compt = 0;
    *flag = save_flag;
    new_flag = 0;
    tmp_flag = 0;
    while (rd_line[i]) // compte le nombre de caractère par arguments
    {
        if (rd_line[i] == '\n' && *flag == 0)
            *flag = 5;
        if (tmp_flag == 0 && rd_line[i] != ' ')
            tmp_flag = 1;
        if (rd_line[i] != ' ' && *flag == 3)
            *flag = 0;
        if (*flag == 4 && rd_line[i] == '\n')
        {
            tab[compt] += 1;
            compt++;
            tab[compt] -= 1;
            *flag = 0;
        }
        if (*flag == 4 && rd_line[i] == ' ')
            *flag = 0;
        if ((rd_line[i] == '<' || rd_line[i] == '>') && *flag == 0)
            *flag = 4;
        else if (rd_line[i] == '"' && *flag == 0)
            *flag = 1;
        else if (rd_line[i] == '"' && *flag == 1)
        {
            if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
                compt++;
            new_flag = 0;
            *flag = 0;
        }
        else if (rd_line[i] == '\'' && *flag == 0)
            *flag = 2;
        else if (rd_line[i] == '\'' && *flag == 2)
        {
            if (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1)
                compt++;
            new_flag = 0;
            *flag = 0;
        }
        else if (rd_line[i] == '\n' && (*flag == 1 || *flag == 2))
            new_flag = 1;
        else if (rd_line[i] == ' ' && *flag == 0 && tmp_flag == 1) // i > 0
        {
            compt++;
            *flag = 3;
        }
        if (i > 0 && rd_line[i] == '|' && rd_line[i - 1] != ' ' && *flag == 0) // i > 0
            compt++;
        if (*flag != 3)
            tab[compt] += 1;
        if (rd_line[i] == '|' && rd_line[i + 1] != ' ' && *flag == 0)
            compt++;
        i++;
    }
    i = 0;
    if (*flag != 0 && *flag == save_flag) // permet de compter le \n si jamais on doit en rajouter un a la fin de la chaine pasque l'argument n'est pas finis
        tab[compt] += 1;
    if (*flag == 3) // s'il n'y a que des espaces, réajuste compt
        compt--;
    while (i <= compt) // créer les chaines de caractères
    {
        spt[i] = (char *) malloc(sizeof(char) * (tab[i] + 1));
        if (spt[i] == NULL)
        {
            while (i >= 0)
                free(spt[i--]);
            return (free(tab), free(spt), error(MALLOC, NULL), annihilation(ret, free, DEBUG));
        }
        i++;
    }
    free(tab);
    i = 0;
    compt = 0;
    *flag = save_flag;
    j = 0;
    tmp_flag = 0;
    while (rd_line[i]) // remplis les chaines de caractères
    {
        if (rd_line[i] == '\n' && *flag == 0)
        {
            // ft_printf_fd(2, "compt = %d\nj = %d\n", compt, j);
            tmp_flag = -1;
            *flag = 5;
            spt[compt][j] = '\n';
            spt[compt][j + 1] = '\0';
            compt++;
            j = 0;
            i++;
        }
        if ((rd_line[i] == '<' || rd_line[i] == '>') && tmp_flag == 0 && *flag != 5)
            tmp_flag = 1;
        else if (rd_line[i] == '\n' && tmp_flag == 1 && *flag != 5)
        {
            spt[compt][j + 1] = '\0';
            j = 0;
            i++;
            compt++;
            tmp_flag = 0;
        }
        if (rd_line[i] != ' ' && *flag == 3)
            *flag = 0;
        if (rd_line[i] == '"' && *flag == 0)
            *flag = 1;
        else if (rd_line[i] == '"' && *flag == 1)
        {
            *flag = 0;
            spt[compt][j] = '"';
            if (rd_line[i + 1] == ' ' || (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1))
            {
                spt[compt][j + 1] = '\0';
                j = 0;
                i++;
                compt++;
            }
            new_flag = 0;
        }
        else if (rd_line[i] == '\'' && *flag == 0)
            *flag = 2;
        else if (rd_line[i] == '\'' && *flag == 2)
        {
            *flag = 0;
            spt[compt][j] = '\'';
            if (rd_line[i + 1] == ' ' || (rd_line[i + 1] != '\0' && rd_line[i + 1] != ' ' && new_flag == 1))
            {
                spt[compt][j + 1] = '\0';
                j = 0;
                i++;
                compt++;
            }
            new_flag = 0;
        }
        else if (rd_line[i] == ' ' && *flag == 0 && i > 0)
        {
            spt[compt][j] = '\0';
            j = 0;
            compt++;
        }
        else if (i > 0 && rd_line[i] == '|' && *flag == 0 \
        && (rd_line[i - 1] != ' ' || \
        (rd_line[i - 1] != ' ' && rd_line[i + 1] != ' '))) // i > 0
        {
            spt[compt][j] = '\0';
            j = 0;
            compt++;
            spt[compt][j] = '|';
            j++;
        }
        else if (i > 0 && rd_line[i] == '|' && *flag == 0 && rd_line[i - 1] == ' ') // i > 0
        {
            spt[compt][j] = '|';
            j++;
        }
        else if (rd_line[i] == '\n' && (*flag == 1 || *flag == 2))
            new_flag = 1;
        if (rd_line[i] == '|' && *flag == 0 && rd_line[i + 1] != ' ')
        {
            spt[compt][j] = '\0';
            j = 0;
            compt++;
        }
        if (rd_line[i] == ' ' && *flag == 0)
            *flag = 3;
        if ((*flag != 3 && rd_line[i] != '|') || (*flag != 0 && rd_line[i] == '|') || *flag == 5)
        {
            spt[compt][j] = rd_line[i];
            j++;
        }
        i++;
    }
    if (*flag != 0 && *flag == save_flag) // Rajoute le \n si c'est un rappel de la fonction et qu'on est toujours pas sortie
        spt[compt][j++] = '\n';
    spt[compt][j] = '\0';
    i = 0;
    while (spt[i] && spt[i][0] != '\0') // ajoute les arguments a la liste
    {
        if (tailing_list(ret, spt[i++], STRING, DEBUG) == 0)
        {
            anihilation(spt);
            error(MALLOC, NULL);
        }
    }
    if (j == 0)
        free(spt[compt]);
    free(spt);
    // if (compt == 0 && j == 0)
    //     anihilation(spt);
    // else if (j == 0)
    // {
    //     free(spt[compt]);
    //     free(spt);
    // }
    // else
    //     free(spt);
}
