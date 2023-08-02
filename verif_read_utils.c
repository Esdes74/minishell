#include "minishell.h"

int *counting_arg(t_cell *cell, int count, t_list *spt)
{
    int *tab;
    int i;
    int j;

    i = 0;
    j = 0;
    cell = spt->head;
    tab = malloc(sizeof(int *) * count);
    if (tab == NULL)
        return (error(MALLOC, NULL), NULL);
    while (cell != NULL)
    {
        if (((char *)(cell->data_cell->data))[0] == '|')
        {
            tab[j] = i;
            j++;
            i = -1;
        }
        i++;
        cell = cell->next;
    }
    tab[j] = i;
    cell = spt->head;
    return (tab);
}

char    **string_for_cmd_center(int *tab, t_cell *cell, int i, t_list *spt)
{
    int     j;
    char    **for_cmd;

    j = 0;
    cell = spt->head;
    for_cmd = malloc(sizeof(char **) * (tab[i] + 1));
    if (!for_cmd)
        return (error(MALLOC, NULL), NULL);
    while (tab[i] > j)
    {
        for_cmd[j] = (char *)cell->data_cell->data;
        if (!for_cmd[j])
            return (NULL);
        j++;
        cell = cell->next;
    }
    for_cmd[j] = 0;
    cell = spt->head;
    return (for_cmd);
}
