#include "minishell.h"
#define ERR STDERR_FILENO

void    error(t_error err)
{
    if (err == ADD_LIST)
        ft_printf_fd(ERR, "Error : Problem when adding pid to list.\n");
    if (err == MALLOC)
        ft_printf_fd(ERR, "Error : Problem with malloc function.\n");
    if (err == TAILING)
        ft_printf_fd(ERR, "Error : Problem tailing PID tailing in list.\n");
    exit(1);
}