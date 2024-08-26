/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:48:30 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 12:49:46 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Verify if the number of elements is correct
void	ft_verify_map_parameters(t_game *game)
{
	if (game->map.coins == 0)
		ft_error_msg("Invalid Map! The map has no collectible. 🍬", game);
	if (game->map.exit != 1)
		ft_error_msg("Invalid Map! It must have 1 exit. 🚪", game);
	if (game->map.players != 1)
		ft_error_msg("Invalid Map! It must have 1 player. 🎮", game);
}

// Record the position and number of elements
// check if there are any unexpected characters in the map
void	ft_count_map_parameters(t_game *game)
{
	int	row;
	int	col;

	row = 0;
	while (row < game->map.rows)
	{
		col = 0;
		while (col < game->map.columns)
		{
			if (!ft_strchr("10CPE", game->map.full[row][col]))
				ft_error_msg("Invalid Map! Something odd in the map. ❓", game);
			if (game->map.full[row][col] == PLAYER)
			{
				game->map.players++;
				game->map.player.x = col;
				game->map.player.y = row;
			}
			else if (game->map.full[row][col] == COINS)
				game->map.coins++;
			else if (game->map.full[row][col] == MAP_EXIT)
				game->map.exit++;
			col++;
		}
		row++;
	}
}

// Check if each row of the map file has the same length
void	ft_check_row_lengths(t_game *game)
{
	int		i;
	size_t	first_row_length;

	first_row_length = ft_strlen(game->map.full[0]);
	i = 1;
	while (i < game->map.rows)
	{
		if (ft_strlen(game->map.full[i]) != first_row_length)
			ft_error_msg("Invalid Map! Check the walls again. 🧱", game);
		i++;
	}
}

// Check if the top and bottom of the map (i.e., the first
// and last rows) are completely surrounded by walls
void	ft_check_columns(t_game *game)
{
	int	col;
	int	last_row;

	col = 0;
	last_row = game->map.rows - 1;
	while (col < game->map.columns)
	{
		if (game->map.full[0][col] != WALL)
			ft_error_msg("Invalid Map! Check the (top) wall again. 🧱", game);
		if (game->map.full[last_row][col] != WALL)
			ft_error_msg("Invalid Map! Check the (bottom) wall again. 🧱", game);
		col++;
	}
}

// Ensure that each row of the map is enclosed by walls on both ends
void	ft_check_rows(t_game *game)
{
	int	row;
	int	last_col;

	row = 0;
	last_col = game->map.columns - 1;
	while (row < game->map.rows)
	{
		if (game->map.full[row][0] != WALL)
			ft_error_msg("Invalid Map! Check the (left) wall again. 🧱", game);
		if (game->map.full[row][last_col] != WALL)
			ft_error_msg("Invalid Map! Check the (right) wall again. 🧱", game);
		row++;
	}
}
