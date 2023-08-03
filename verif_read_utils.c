#include "minishell.h"

int *counting_arg(int count, t_list *spt)
{
    int *tab;
    int i;
    int j;
    t_cell *tmp;

    i = 0;
    j = 0;
    tmp = spt->head;
    tab = malloc(sizeof(int *) * count);
    if (tab == NULL)
        return (error(MALLOC, NULL), NULL);
    while (tmp != NULL)
    {
        if (((char *)(tmp->data_cell->data))[0] == '|')
        {
            tab[j] = i;
            j++;
            i = -1;
        }
        i++;
        tmp = tmp->next;
    }
    tab[j] = i;
    return (tab);
}

char    **string_for_cmd_center(int *tab, int i, t_list *spt)
{
    int     j;
    char    **for_cmd;
    char    *buf;
    t_cell  *tmp;

    j = 0;
    tmp = spt->head;
    while (j < i)
    {
        tmp = tmp->next;
        j++;
    }
    buf = (char *)tmp->data_cell->data;
    if (buf[0] == '|')
        tmp = tmp->next; // il faudra protéger dans le ca ou le pipe est en derniere position
    buf = (char *)tmp->data_cell->data;
    for_cmd = (char **)malloc(sizeof(char *) * (tab[i] + 1));
    if (!for_cmd)
        return (error(MALLOC, NULL), NULL);
    while (tab[i] > j)
    {
        buf = (char *)tmp->data_cell->data;
        if (buf[0] == '|')
        {
            tmp = tmp->next;
            printf ("detection du pipe\n");
        }
        for_cmd[j] = (char *)tmp->data_cell->data;
        if (!for_cmd[j])
            return (anihilation(for_cmd), NULL);
        j++;
        tmp = tmp->next;
    }
    for_cmd[j] = 0;
    return (for_cmd);
}
