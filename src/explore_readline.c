/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore_readline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewurstei <ewurstei@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:55:29 by momo              #+#    #+#             */
/*   Updated: 2022/12/22 11:08:14 by ewurstei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	row_parsing(t_vault *data)
{
	int	row;

	row = 0;
	data->b_in->forget_minus = 0;
	data->b_in->minus_n = 0;
	data->b_in->dont_do_minus = 0;
	while (data->rl_decomp[row] && data->rl_decomp[row][0])
	{
		data->buffer = ft_calloc(sizeof(char), 500);
		quote_priority(data, row);
		parse_row(data, row);
		free(data->buffer);
		data->b_in->forget_minus = 0;
		data->b_in->echo_first = 0;
		row++;
	}
}

void	explore_readline(t_vault *data)
{
	data->rl_decomp = ft_split(data->read_line, ' ');
	if (rl_prio_n_qty(data, 0, '\0') == TRUE)
	{
		find_str_quote(data);
		flag_count(data, 0, 0);
		execute_redirection(data, 0, 0);
		row_parsing(data);
		create_tab_arg(data, -1, 0);
		built_in(data);
		dup2(data->flag->stdout_backup, STDOUT_FILENO);
		dup2(data->flag->stdin_backup, STDIN_FILENO);
		close (data->flag->stdout_backup);
		close (data->flag->stdin_backup);
		if (data->flag->fd != 0)
			close (data->flag->fd);
	}
	return ;
}

void	built_in(t_vault *data)
{
	int	i;

	i = 1;
	if (ft_strcmp("cd", data->cmd->name) == 0)
		ft_cd(data, i);
	if (ft_strcmp("pwd", data->cmd->name) == 0)
		ft_pwd(data, i);
	if (ft_strcmp("echo", data->cmd->name) == 0)
		ft_echo(data, i);
	if (ft_strcmp("env", data->cmd->name) == 0)
		ft_env (data, 1);
	if (ft_strcmp("export", data->cmd->name) == 0)
		ft_export (data, i);
	if (ft_strcmp("unset", data->cmd->name) == 0)
		ft_unset (data, i);
	if (ft_strcmp("exit", data->cmd->name) == 0)
		ft_exit (data);
	return ;
}

int	is_built_in(char *str)
{
	if (ft_strcmp("cd", str) == 0
		|| ft_strcmp("pwd", str) == 0
		|| ft_strcmp("echo", str) == 0
		|| ft_strcmp("env", str) == 0
		|| ft_strcmp("export", str) == 0
		|| ft_strcmp("unset", str) == 0
		|| ft_strcmp("exit", str) == 0)
		return (TRUE);
	return (FALSE);
}

