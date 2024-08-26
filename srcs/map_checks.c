/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:55:09 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 12:55:44 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Check if the map file is empty
void	ft_check_empty_file(char *map, int rows, t_game *game)
{
	if (rows == 0 || map[0] == '\0')
	{
		free(map);
		ft_error_msg("Oops! The map file is empty! 😢", game);
	}
}

// Check if the map string contains any illegal empty lines
void	ft_check_for_empty_line(char *map, t_game *game)
{
	int	i;

	if (map[0] == '\n')
	{
		free(map);
		ft_error_msg("Uh-oh! The map has a empty line at the start! 😯", game);
	}
	if (map[ft_strlen(map) - 1] == '\n')
	{
		free(map);
		ft_error_msg("Uh-oh! There's an empty line at the end of the map. 🧹",
			game);
	}
	i = 0;
	while (map[i])
	{
		if (map[i] == '\n' && map[i + 1] == '\n')
		{
			free(map);
			ft_error_msg("Uh-oh! The map has a empty line in the middle! 😮",
				game);
		}
		i++;
	}
}
