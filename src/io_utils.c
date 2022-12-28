/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momo <momo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:38:02 by ewurstei          #+#    #+#             */
/*   Updated: 2022/12/28 12:41:37 by momo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// penser au code de retour
void	io_redirection(t_vault *data, int input, int output)
{
	(void)data;
	if (dup2(input, STDIN_FILENO) == -1)
		ft_putstr_fd("error 54\n", 2);
	if (dup2(output, STDOUT_FILENO) == -1)
		ft_putstr_fd("error 55\n", 2);
}

int	dup_fds(t_vault *data, int line)
{
	int	len;

	if (data->flag->pipe_count == 0)
		return (0);
	len = ft_dbl_ptr_len(data->tab_arg);
	if (line == 0)
	{
		if (dup2(data->flag->pipe[line][p_write], STDOUT_FILENO) == -1)
			ft_putstr_fd("error 56\n", 2);
	}
	else if (line != len - 1)
		io_redirection(data, data->flag->pipe[line - 1][p_read],
			data->flag->pipe[line][p_write]);
	else
	{
		if (dup2(data->flag->pipe[line - 1][p_read], STDIN_FILENO) == -1)
			ft_putstr_fd("error 2\n", 2);
	}
	return (0);
}

void	close_pipe(t_vault *data)
{
	int	x;

	x = 0;
	if (data->flag->pipe_count == 0)
		return ;
	while (x < data->flag->pipe_count)
	{
		close (data->flag->pipe[x][p_write]);
		close (data->flag->pipe[x][p_read]);
		free (data->flag->pipe[x]);
		x++;
	}
	free(data->flag->pipe);
	return ;
}

void	reset_io(t_vault *data)
{
	dup2(data->flag->stdout_backup, STDOUT_FILENO);
	dup2(data->flag->stdin_backup, STDIN_FILENO);
}
