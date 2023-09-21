#!/bin/sh
valgrind --show-leak-kinds=all --leak-check=full --show-mismatched-frees=yes --suppressions=valgrind_ignore_leaks.txt --log-file=valgrind_out.txt ./minishell
#--trace-children=yes