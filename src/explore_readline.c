/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore_readline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momo <momo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:55:29 by momo              #+#    #+#             */
/*   Updated: 2023/01/07 18:46:39 by momo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	explore_readline(t_vault *data)
{
	data->rl_dec = ft_split(data->read_line, ' ');
	if (rl_prio_n_qty(data, 0, '\0') == TRUE)
	{
		find_str_quote(data);
		check_for_pipe(data);
		if (pipe_check(data) == 1)
			return ;
		flag_count(data, 0, 0);
		dollar_parsing(data);
		create_tab_arg(data, -1, 0);
		if (!(data->tab_arg[0]))
		{
			error_message(data, "putain con, regarde ce que tu ecris", "1\0");
			return ;
		}
		piping(data);
		reset_io(data);
	}
	return ;
}

void	piping(t_vault *data)
{
	int	i;

	i = 0;
	data->flag->pipe = ft_calloc(sizeof(int *), (data->flag->pipe_count) + 1);
	while (i < data->flag->pipe_count)
	{
		data->flag->pipe[i] = ft_calloc(sizeof(int), 2);
		if (pipe(data->flag->pipe[i]) == -1)
			error_message(data, "pipe creation error", "1\0");
		i++;
	}
	launching_exec(data);
	i = 0;
	close_pipe(data);
	while (i < data->flag->pipe_count + 1)
	{
		data->child_id = waitpid(data->pid[i], &data->status, 0);
		i++;
	}
	free(data->pid);
}

void	launching_exec(t_vault *data)
{
	int		line;

	line = -1;
	data->pid = ft_calloc(sizeof(int), data->flag->pipe_count + 1);
	while (data->tab_arg[++line] && data->tab_arg[line][0])
	{
		if (data->flag->pipe_count == 0)
		{
			execute_redirection(data, line, 0);
			if (data->tab_arg[line][0] != '\0' && data->fail_redir == FALSE)
			{
				final_quotes_removing(data, line);
				data->cmd->name = ft_strdup(data->cmd->opt[0]);
				recompose_tab_arg(data, line);
				if (is_special_built_in(data, line) == TRUE)
					built_in(data, line);
				else
					forking(data, line, 1);
			}
		}
		else
			forking(data, line, 2);
	}
	heredoc_unlink(data);
	reset_io(data);
}

void	forking(t_vault *data, int line, int type)
{
	if (type == 1)
	{
		child_creation(data, line);
		if (data->pid[line] == 0)
		{
			find_prog(data, line);
			close_pipe(data);
			exit_process(data);
		}
	}
	else if (type == 2)
	{
		child_creation(data, line);
		if (data->pid[line] == 0)
		{
			dup_fds(data, line);
			execute_redirection(data, line, 0);
			if (data->tab_arg[line][0] != '\0' && data->fail_redir == FALSE)
				in_child_exec(data, line);
			close_pipe(data);
			exit_process(data);
		}
	}
}

void	child_creation(t_vault *data, int line)
{
	data->pid[line] = fork();
	if (data->pid[line] == -1)
	{
		error_message(data, "pid creation error", "1\0");
		exit_process(data);
	}
	// if (data->pid[line] == 0)
	// {
	// 	close (data->error_fd);
	// 	data->error_fd = open(".tmp_error", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	// 	ft_putstr_fd("0\0", data->error_fd);
	// }
}

// À corriger :
// Gestion des fd DUO
// Gestion des leaks DUO
// Probleme de fd
// Mauvais comportement avec l'historique A DEBATTRE ERIC
// Invalid read quand ctrl + c puis echo ?$ - OK
// .tmp_error leak quand on donne une commande invalid comme : salut - OK
// invalid read avec ls | ls | ls - OK
// <test grep salut | wc -l Probleme du file_descriptor
// echo salut >> file puis ctrl + D - OK
// cat + 2x ctrl-D - OK
//<test cat - Probleme de close quand test n'existe pas

//possibilite de suivre le child :
//1. ouvrir un 2e terminal
//2. lancer lldb
// lire  : https://lldb.llvm.org/use/map.html
//3a. pro at -n minishell -w
//ou 3b. pro at -p #pid
