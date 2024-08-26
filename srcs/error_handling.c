/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:50:52 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 12:52:00 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Clean up, display information, and terminate the program
int	ft_close_game(t_game *game)
{
	if (game)
	{
		ft_printf("Movements: %d\n", game->movements);
		ft_free_all_allocated_memory(game);
		ft_printf("CLOSED\n");
	}
	exit(EXIT_FAILURE);
}

// Destroy all image resources
void	ft_destroy_images(t_game *game)
{
	mlx_destroy_image(game->mlx_ptr, game->wall.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->floor.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->coins.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->player_front.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->player_left.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->player_right.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->player_back.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->exit_closed.xpm_ptr);
	mlx_destroy_image(game->mlx_ptr, game->open_exit.xpm_ptr);
}

// Free all dynamically allocated memory and resources.
// mlx_ptr is a memory block dynamically allocated when calling mlx_init(),
// and needs to be manually freed.
// win_ptr is a resource managed internally by the MLX library,
// and is handled when calling mlx_destroy_window
void	ft_free_all_allocated_memory(t_game *game)
{
	ft_destroy_images(game);
	ft_free_map(game);
	if (game->win_ptr)
		mlx_destroy_window(game->mlx_ptr, game->win_ptr);
	if (game->mlx_ptr)
	{
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
	free(game);
}

// Traverses and frees each row in the 2D array storing the map row
// frees the array storing these row pointers
void	ft_free_map(t_game *game)
{
	int	row;

	row = game->map.rows;
	while (row > 0)
		free(game->map.full[--row]);
	free(game->map.full);
}

// Output an error message, clean up resources,
// and terminate the program in case of an error
int	ft_error_msg(char *msg, t_game *game)
{
	if (game && game->map_alloc)
		ft_free_map(game);
	free(game);
	ft_printf("Error\n%s\n", msg);
	exit(EXIT_FAILURE);
}
