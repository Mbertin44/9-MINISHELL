/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detached_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maison <maison@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:48:00 by mbertin           #+#    #+#             */
/*   Updated: 2022/12/28 16:39:42 by maison           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	detached_quote_tab(t_vault *data)
{
	int		len;

	data->quote->buffer = data->rl_dec;
	len = len_detached_quote_tab(data);
	data->rl_dec = ft_dbl_ptr_realloc(data->rl_dec, len + 1);
	fill_detached_quote_tab(data);
}

int	len_detached_quote_tab(t_vault *data)
{
	int	row;
	int	line;
	int	len;

	row = 0;
	len = 0;
	while (data->rl_dec[row] && data->rl_dec[row][0])
	{
		line = 0;
		while (data->rl_dec[row][line])
		{
			if (data->rl_dec[row][line] == '\''
				|| data->rl_dec[row][line] == '\"')
				quote_to_quote(data, row, &line, &len);
			line++;
		}
		row++;
		len++;
	}
	return (len);
}

void	quote_to_quote(t_vault *data, int row, int *line, int *len)
{
	data->quote->quote_priority = data->rl_dec[row][*line];
	if (*line > 0 && data->rl_dec[row][*line - 1] != ' ')
		(*len)++;
	(*line)++;
	while (data->rl_dec[row][*line] != data->quote->quote_priority)
		(*line)++;
	if (data->rl_dec[row][*line + 1]
		&& data->rl_dec[row][*line + 1] != ' ')
	{
		(*len)++;
		(*line)++;
		while (data->rl_dec[row][(*line)++])
		{
			if (data->rl_dec[row][*line] == '\''
				|| data->rl_dec[row][*line] == '\"')
			{
				data->quote->quote_priority = data->rl_dec[row][*line];
				(*line)++;
				while (data->rl_dec[row][*line]
					!= data->quote->quote_priority)
					(*line)++;
				(*len)++;
			}
		}
	}
}

void	fill_detached_quote_tab(t_vault *data)
{
	int	row;
	int	line;
	int	i;

	row = 0;
	i = 0;
	while (data->quote->buffer[row] && data->quote->buffer[row][0])
	{
		line = 0;
		if (ft_strchr(data->quote->buffer[row], '\'') == NULL
			&& ft_strchr(data->quote->buffer[row], '\"') == NULL)
			data->rl_dec[i++] = ft_strdup(data->quote->buffer[row]);
		else
			row_with_quote(data, &i, row, &line);
		row++;
	}
}

void	row_with_quote(t_vault *data, int *i, int row, int *line)
{
	while (data->quote->buffer[row][*line])
	{
		if (data->quote->buffer[row][*line] != '\''
			&& data->quote->buffer[row][*line] != '\"')
			fill_out_quote(data, i, row, line);
		else if (data->quote->buffer[row][*line] == '\''
			|| data->quote->buffer[row][*line] == '\"')
			fill_in_quote(data, i, row, line);
	}
}