/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pretty_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewurstei <ewurstei@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:55:52 by ewurstei          #+#    #+#             */
/*   Updated: 2022/12/15 15:56:09 by ewurstei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lred(void)
{
	printf("\033[1;91m");
}

void	lyellow(void)
{
	printf("\033[1;93m");
}

void	lcyan(void)
{
	printf("\33[1;96m");
}

void	reset(void)
{
	printf("\033[1;0m");
}
