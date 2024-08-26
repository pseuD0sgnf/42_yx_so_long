/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:52:13 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 12:53:11 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Identify the character and decide which image to render.
// Walls, floors, and coins are rendered directly. The exit is determined
// based on whether there are remaining coins.
// Player rendering requires further determination
void	ft_identify_sprite(t_game *game, int row, int col)
{
	char	parameter;
	t_image	exit_sprite;

	parameter = game->map.full[row][col];
	if (parameter == WALL)
		ft_render_sprite(game, game->wall, row, col);
	else if (parameter == FLOOR)
		ft_render_sprite(game, game->floor, row, col);
	else if (parameter == COINS)
		ft_render_sprite(game, game->coins, row, col);
	else if (parameter == MAP_EXIT)
	{
		if (game->map.coins == 0)
			exit_sprite = game->open_exit;
		else
			exit_sprite = game->exit_closed;
		ft_render_sprite(game, exit_sprite, row, col);
	}
	else if (parameter == PLAYER)
		ft_render_player(game, row, col);
}

// Render the player's image based on the player's direction
void	ft_render_player(t_game *game, int row, int col)
{
	if (game->player_sprite == FRONT)
		ft_render_sprite(game, game->player_front, row, col);
	if (game->player_sprite == LEFT)
		ft_render_sprite(game, game->player_left, row, col);
	if (game->player_sprite == RIGHT)
		ft_render_sprite(game, game->player_right, row, col);
	if (game->player_sprite == BACK)
		ft_render_sprite(game, game->player_back, row, col);
}

// Call MiniLibX to draw
void	ft_render_sprite(t_game *game, t_image sprite, int row, int col)
{
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, sprite.xpm_ptr, col
		* sprite.x, row * sprite.y);
}

// Load the sprite from the specified path. If successful, MiniLibX returns
// a t_image containing pointers to the image, width, and height
t_image	ft_load_sprite(void *mlx, char *path, t_game *game)
{
	t_image	sprite;

	sprite.xpm_ptr = mlx_xpm_file_to_image(mlx, path, &sprite.x, &sprite.y);
	if (sprite.xpm_ptr == NULL)
		ft_error_msg("Couldn't find the sprite. Did it run away? 🕵️‍♂️", game);
	return (sprite);
}

// Initialize all sprites and load them into memory
void	ft_init_sprites(t_game *game)
{
	void	*mlx;

	mlx = game->mlx_ptr;
	game->wall = ft_load_sprite(mlx, WALL_XPM, game);
	game->floor = ft_load_sprite(mlx, FLOOR_XPM, game);
	game->coins = ft_load_sprite(mlx, COINS_XPM, game);
	game->player_front = ft_load_sprite(mlx, PLAYER_FRONT_XPM, game);
	game->player_left = ft_load_sprite(mlx, PLAYER_LEFT_XPM, game);
	game->player_right = ft_load_sprite(mlx, PLAYER_RIGHT_XPM, game);
	game->player_back = ft_load_sprite(mlx, PLAYER_BACK_XPM, game);
	game->open_exit = ft_load_sprite(mlx, OPEN_EXIT_XPM, game);
	game->exit_closed = ft_load_sprite(mlx, EXIT_CLOSED_XPM, game);
}
