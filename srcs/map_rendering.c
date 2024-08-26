/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_rendering.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:53:28 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 15:03:13 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Check if the new position is within the map boundaries,
// then perform the corresponding action based on the key pressed
int	ft_handle_input(int keysym, t_game *game)
{
	if (keysym == KEY_W || keysym == KEY_UP)
		ft_player_move(game, game->map.player.y - 1, game->map.player.x, BACK);
	else if (keysym == KEY_A || keysym == KEY_LEFT)
		ft_player_move(game, game->map.player.y, game->map.player.x - 1, LEFT);
	else if (keysym == KEY_S || keysym == KEY_DOWN)
		ft_player_move(game, game->map.player.y + 1, game->map.player.x, FRONT);
	else if (keysym == KEY_D || keysym == KEY_RIGHT)
		ft_player_move(game, game->map.player.y, game->map.player.x + 1, RIGHT);
	else if (keysym == KEY_ESC)
		ft_close_game(game);
	return (0);
}

// Check if the player can move to the specified new position,
// if so, update the player's position, map status, and re-render the map.
void	ft_player_move(t_game *game, int new_y, int new_x, int player_sprite)
{
	int	old_x;
	int	old_y;

	game->player_sprite = player_sprite;
	old_x = game->map.player.x;
	old_y = game->map.player.y;
	if (game->map.full[new_y][new_x] == MAP_EXIT && game->map.coins == 0)
		ft_victory(game);
	else if ((game->map.full[new_y][new_x] == FLOOR)
		|| (game->map.full[new_y][new_x] == COINS))
	{
		game->map.full[old_y][old_x] = FLOOR;
		if (game->map.full[new_y][new_x] == COINS)
			game->map.coins--;
		game->map.player.x = new_x;
		game->map.player.y = new_y;
		game->map.full[new_y][new_x] = PLAYER;
		game->movements++;
		ft_render_map(game);
	}
}

// Display the player's movement count in the game window using MiniLibX
void	ft_print_movements(t_game *game)
{
	char	*movements;
	char	*phrase;

	movements = ft_itoa(game->movements);
	if (movements == NULL)
		return ;
	phrase = ft_strjoin("Movements : ", movements);
	free(movements);
	if (phrase == NULL)
		return ;
	mlx_string_put(game->mlx_ptr, game->win_ptr, 30, 20, 16777215, phrase);
	free(phrase);
}

// Traverse the map, render the images, and print the movements
int	ft_render_map(t_game *game)
{
	int	row;
	int	col;

	row = 0;
	while (row < game->map.rows)
	{
		col = 0;
		while (col < game->map.columns)
		{
			ft_identify_sprite(game, row, col);
			col++;
		}
		row++;
	}
	ft_print_movements(game);
	return (0);
}
