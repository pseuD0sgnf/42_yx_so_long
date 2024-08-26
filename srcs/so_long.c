/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:45:54 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 13:01:45 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Check the validity of the game map. Check the width and height,
// count and verify the number of elements
void	ft_check_map(t_game *game)
{
	ft_check_rows(game);
	ft_check_columns(game);
	ft_count_map_parameters(game);
	ft_verify_map_parameters(game);
	ft_check_row_lengths(game);
	ft_check_map_validity(game);
}

// Initialize the number of coins, exits, players, and movements
// the initial direction of the player
void	ft_init_vars(t_game *game)
{
	game->map.coins = 0;
	game->map.exit = 0;
	game->map.players = 0;
	game->movements = 0;
	game->player_sprite = RIGHT;
}

// Initialize the game map, read the contents of the map file into memory,
// perform necessary checks and processing, calculating the width and height
void	ft_init_map(t_game *game, char *file_name)
{
	char	*map_content;
	char	*line;
	int		fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		ft_error_msg("Uh-oh! I can't open the map. Is it really there? 🧐",
			game);
	map_content = ft_strdup("");
	game->map.rows = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		map_content = ft_strappend(&map_content, line);
		free(line);
		game->map.rows++;
		line = get_next_line(fd);
	}
	close(fd);
	ft_check_empty_file(map_content, game->map.rows, game);
	ft_check_for_empty_line(map_content, game);
	game->map.full = ft_split(map_content, '\n');
	free(map_content);
	game->map.columns = ft_strlen(game->map.full[0]);
	game->map_alloc = true;
}

// Check if the command-line arguments passed at the program's start are
// correct, and terminate the program with an error message if they are not
void	ft_check_args(int argc, char **argv, t_game *game)
{
	int	args_len;

	game->map_alloc = false;
	if (argc > 2)
		ft_error_msg("Uh-oh! That's too many arguments! I only need two. 😊",
			game);
	if (argc < 2)
		ft_error_msg("Uh-oh! Where's the map file? I can't find it! 😢", game);
	args_len = ft_strlen(argv[1]);
	if (!ft_strnstr(&argv[1][args_len - 4], ".ber", 4))
		ft_error_msg("Uh-oh! That file extension should be .ber! 🧐", game);
	if (ft_strnstr(&argv[1][args_len - 5], "/.ber", 5))
		ft_error_msg("Uh-oh! It's a hidden file, not extension.ber! 🥺", game);
}

// mlx_hook associates an event with a callback function
int	main(int argc, char **argv)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	ft_check_args(argc, argv, game);
	ft_init_map(game, argv[1]);
	ft_init_vars(game);
	ft_check_map(game);
	ft_init_mlx(game);
	ft_init_sprites(game);
	ft_render_map(game);
	mlx_hook(game->win_ptr, KeyPress, KeyPressMask, ft_handle_input, game);
	mlx_hook(game->win_ptr, DestroyNotify, ButtonPressMask, ft_close_game,
		game);
	mlx_hook(game->win_ptr, Expose, ExposureMask, ft_render_map, game);
	mlx_loop(game->mlx_ptr);
	ft_free_all_allocated_memory(game);
}
