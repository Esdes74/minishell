#include "minishell.h"

int *counting_arg(t_cell *cell, int count, t_list *spt)
{
    int *tab;
    int i;
    int j;

    i = 0;
    j = 0;
    tab = malloc(sizeof(int *) * count + 1); // le +1 c'est parce que count comence à 0
    if (tab == NULL)
        return (NULL);
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
    for_cmd = malloc(sizeof(char **) * tab[i] + 1);
    if (!for_cmd)
        return (NULL);
    while (tab[i] > j)
    {
        for_cmd[j] = ft_strdup(cell->data_cell->data);
        if (!for_cmd[j])
            return (NULL);
        j++;
        cell = cell->next;
    }
    for_cmd[j] = '\0';
    cell = spt->head;
    return (for_cmd);
}
