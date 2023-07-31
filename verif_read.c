#include "minishell.h"

char    *verif_read(char *rd_line)
{
    int     flag;
    char    *buff;
    char    *buf;
    t_list  *spt;

    flag = 0;
    spt = (t_list *) malloc(sizeof(t_list));
	if (spt == NULL)
		return (error(MALLOC, NULL), NULL);
    init_list(spt);
    second_parsing_check(rd_line, &flag, spt);
    buff = rd_line;
    while (flag == 1 || flag == 2)
    {
        rd_line = readline("> ");
        second_parsing_check(rd_line, &flag, spt);
        buf = ft_strjoin(buff, "\n");
        free(buff);
        buff = ft_strjoin(buf, rd_line);
        free(buf);
    }
    t_cell *cell = spt->head;// liste -> main_exec -> pipes, forks, dups, cmd_center
    while (cell != NULL)
    {
        ft_printf(cell->data_cell->data);
        ft_printf("\n");
        cell = cell->next;
    }
	annihilation(spt, free, DEBUG);
    return (buff);
}