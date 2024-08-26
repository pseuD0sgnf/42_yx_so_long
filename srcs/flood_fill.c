/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:49:51 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 12:50:43 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Traverse each tile of the map, check for unreachable exits and collectibles
// and update the count of unvisited coins
void	check_map_for_coins_and_exit(char **map_copy, int *coins_count,
		t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.rows)
	{
		j = 0;
		while (map_copy[i][j])
		{
			if (map_copy[i][j] == COINS)
				(*coins_count)++;
			j++;
		}
		free(map_copy[i]);
		i++;
	}
}

// Traverse the entire map from the specified starting point
// and mark reachable areas
void	flood_fill(char **map, int y, int x, int *exit_reached)
{
	if (map[y][x] == WALL || map[y][x] == 'F')
		return ;
	if (map[y][x] == MAP_EXIT)
	{
		*exit_reached = 1;
		return ;
	}
	if (map[y][x] == COINS || map[y][x] == FLOOR || map[y][x] == PLAYER)
		map[y][x] = 'F';
	flood_fill(map, y + 1, x, exit_reached);
	flood_fill(map, y - 1, x, exit_reached);
	flood_fill(map, y, x + 1, exit_reached);
	flood_fill(map, y, x - 1, exit_reached);
}

// Create a copy of the map for flood fill validation
char	**copy_map(t_game *game)
{
	int		i;
	char	**map_copy;

	map_copy = malloc(game->map.rows * sizeof(char *));
	if (!map_copy)
		return (NULL);
	i = 0;
	while (i < game->map.rows)
	{
		map_copy[i] = ft_strdup(game->map.full[i]);
		if (!map_copy[i])
			return (NULL);
		i++;
	}
	return (map_copy);
}

// Ensure that all exits and collectibles are reachable from
// the player's starting position. If not, the map is invalid
void	ft_check_map_validity(t_game *game)
{
	char	**map_copy;
	int		y;
	int		x;
	int		coins_count;
	int		exit_reached;

	coins_count = 0;
	exit_reached = 0;
	map_copy = copy_map(game);
	y = game->map.player.y;
	x = game->map.player.x;
	flood_fill(map_copy, y, x, &exit_reached);
	check_map_for_coins_and_exit(map_copy, &coins_count, game);
	free(map_copy);
	if (coins_count != 0)
		ft_error_msg("Invalid Map! Not all collectibles are reachable.", game);
	if (!exit_reached)
		ft_error_msg("Invalid Map! Exit is not reachable.", game);
}
