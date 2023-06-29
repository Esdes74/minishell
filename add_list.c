#include "minishell.h"

int add_list(pid_t data, t_list *list)
{
    pid_t *new;

    new = (pid_t *) malloc(sizeof(pid_t));
    if (new == NULL)
        error(MALLOC);
    *new = data;
    if (tailing_list(list, new, PID, DEBUG) == 0)
        return (free(new), error(TAILING), 1);
    return (0);
}