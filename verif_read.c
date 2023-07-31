#include "minishell.h"

int verif_read(char *rd_line)
{
    int     flag;
    char    **spt;

    flag = 0;
    spt = second_parsing_check(rd_line, &flag);
    free(rd_line);
    if (spt == NULL)
        return (1);
    while (flag == 1 || flag == 2)
    {
        anihilation(spt);
        rd_line = readline("> ");
        spt = second_parsing_check(rd_line, &flag);
        free(rd_line);
        int i = 0;
        while (spt[i])
        {
            ft_printf(spt[i++]);
            ft_printf("\n");
        }
    }
    int i = 0;
    while (spt[i])
    {
        ft_printf(spt[i++]);
        ft_printf("\n");
    }
	anihilation(spt);
    return (0);
}