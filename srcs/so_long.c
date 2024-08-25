/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 01:00:40 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 02:37:44 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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

// Append the string to_append to the end of the string original,
// and return the new string
// The memory is dynamically allocated for the new string,
// and the old original memory is freed
char	*ft_strappend(char **original, const char *to_append)
{
	char	*str;
	size_t	total_len;

	if (!*original || !to_append)
		return (NULL);
	total_len = ft_strlen(*original) + ft_strlen(to_append) + 1;
	str = (char *)ft_calloc(total_len, sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, *original, ft_strlen(*original));
	ft_memcpy(str + ft_strlen(*original), to_append, ft_strlen(to_append) + 1);
	free(*original);
	return (str);
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

// Check if the map file is empty
void	ft_check_empty_file(char *map, int rows, t_game *game)
{
	if (rows == 0 || map[0] == '\0')
	{
		free(map);
		ft_error_msg("Oops! The map file is empty! 😢", game);
	}
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
	while ((line = get_next_line(fd)) != NULL)
	{
		map_content = ft_strappend(&map_content, line);
		free(line);
		game->map.rows++;
	}
	close(fd);
	ft_check_empty_file(map_content, game->map.rows, game);
	ft_check_for_empty_line(map_content, game);
	game->map.full = ft_split(map_content, '\n');
	free(map_content);
	game->map.columns = ft_strlen(game->map.full[0]);
	// ft_printf("map's rows is %d, map's columns is %d\n", game->map.rows,
	//	game->map.columns);
	game->map_alloc = true;
	// int i;
	// i = 0;
	// while (i < game->map.rows)
	// {
	// 	ft_printf("%s\n", game->map.full[i]);
	// 	i++;
	// }
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

// Ensure that each row of the map is enclosed by walls on both ends
void	ft_check_rows(t_game *game)
{
	int	row;
	int	last_col;

	row = 0;
	last_col = game->map.columns - 1;
	while (row < game->map.rows)
	{
		// ft_printf("Checking row %d, first_col: 0, last_char: %c\n", row,
		//	game->map.full[row][0]);
		// ft_printf("Checking row %d, last_col: %d, last_char: %c\n", row,
		//	last_col, game->map.full[row][last_col]);
		if (game->map.full[row][0] != WALL)
			ft_error_msg("Invalid Map! Check the (left) wall again. 🧱", game);
		if (game->map.full[row][last_col] != WALL)
			ft_error_msg("Invalid Map! Check the (right) wall again. 🧱", game);
		row++;
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
		// ft_printf("Checking col %d, top_row: 0, top_char: %c\n", col,
		//	game->map.full[0][col]);
		// ft_printf("Checking col %d, last_row: %d, last_char: %c\n", col,
		//	last_row, game->map.full[last_row][col]);
		if (game->map.full[0][col] != WALL)
			ft_error_msg("Invalid Map! Check the (top) wall again. 🧱", game);
		if (game->map.full[last_row][col] != WALL)
			ft_error_msg("Invalid Map! Check the (bottom) wall again. 🧱", game);
		col++;
	}
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

// Check if each row of the map file has the same length
void	ft_check_row_lengths(t_game *game)
{
	int		i;
	size_t	first_row_length;

	first_row_length = ft_strlen(game->map.full[0]);
	i = 1;
	while (i < game->map.rows)
	{
		// if (i == game->map.rows - 1)
		// {
		// 	if (ft_strlen(game->map.full[i]) != first_row_length - 1)
		// 		ft_error_msg("Invalid Map! Check the walls again. 🧱", game);
		// }
		// else
		// {
		if (ft_strlen(game->map.full[i]) != first_row_length)
			ft_error_msg("Invalid Map! Check the walls again. 🧱", game);
		// }
		i++;
	}
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
			// 	if (map_copy[i][j] == MAP_EXIT)
			// 		ft_error_msg("Invalid Map! Exit is not reachable.", game);
			if (map_copy[i][j] == COINS)
				(*coins_count)++;
			j++;
		}
		free(map_copy[i]);
		i++;
	}
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

// Initialize MiniLibX and create a window
void	ft_init_mlx(t_game *game)
{
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == NULL)
	{
		free(game->mlx_ptr);
		ft_error_msg("Failed to initialize MLX.", game);
	}
	game->win_ptr = mlx_new_window(game->mlx_ptr, game->map.columns * IMG_WIDTH,
			game->map.rows * IMG_HEIGHT, "so_long");
	if (game->win_ptr == NULL)
	{
		free(game->mlx_ptr);
		ft_error_msg("Failed to create the window.", game);
	}
}

// Call MiniLibX to draw
void	ft_render_sprite(t_game *game, t_image sprite, int row, int col)
{
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, sprite.xpm_ptr, col
		* sprite.x, row * sprite.y);
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

// Display a victory message, free all allocated memory, and exit the game
int	ft_victory(t_game *game)
{
	ft_printf("\n		Movements: %d\n", ++game->movements);
	ft_free_all_allocated_memory(game);
	ft_printf("\n\
 Y   Y  OOO  U   U  W     W  OOO  N   N \n\
  Y Y  O   O U   U  W     W O   O NN  N \n\
   Y   O   O U   U  W  W  W O   O N N N \n\
   Y   O   O U   U   W W W  O   O N  NN \n\
   Y    OOO   UUU     W W    OOO  N   N \n\
\n\
        CONGRATULATIONS! YOU WON!\n\
");
	exit(EXIT_SUCCESS);
}

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
