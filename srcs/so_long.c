/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 01:00:40 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/24 03:58:39 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// 释放与游戏地图相关的内存资源。具体来说，它遍历并释放
// 存储地图行的二维数组中的每一行，然后释放存储这些行指针的数组
void	ft_free_map(t_game *game)
{
	int	row;

	row = game->map.rows;
	while (row > 0)
		free(game->map.full[--row]);
	free(game->map.full);
}

// 在发生错误时输出错误信息、清理资源，并终止程序
int	ft_error_msg(char *msg, t_game *game)
{
	if (game && game->map_alloc)
		ft_free_map(game);
	free(game);
	ft_printf("Error\n%s\n", msg);
	exit(EXIT_FAILURE);
}

// 检查程序启动时传递的命令行参数是否正确，并且在发现错误时终止程序并给出错误信息
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
		ft_error_msg("Uh-oh! That file extension should be .ber! 🧐",
			game);
	if (ft_strnstr(&argv[1][args_len - 5], "/.ber", 5))
		ft_error_msg("Uh-oh! It's a hidden file, not extension.ber! 🥺",
			game);
}

// 将字符串 to_append 附加到字符串 original 之后，并返回新字符串
// 动态分配内存给新字符串，然后释放旧的 original 内存
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

// 检查地图字符串是否包含不合法的空行（即全为 \n 的行）
void	ft_check_for_empty_line(char *map, t_game *game)
{
	int	i;

	if (map[0] == '\n')
	{
		free(map);
		ft_error_msg("Uh-oh! The map has a empty line at the start! 😯",
			game);
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

void	ft_check_empty_file(char *map, int rows, t_game *game)
{
	if (rows == 0 || map[0] == '\0')
	{
		free(map);
		ft_error_msg("Oops! The map file is empty! 😢", game);
	}
}

// 初始化游戏地图，将地图文件的内容读取到内存中，并进行必要的检查和处理、计算长宽
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
	// ft_printf("map's rows is %d, map's columns is %d\n", game->map.rows, game->map.columns);
	game->map_alloc = true;

	// int i;
	// i = 0;
	// while (i < game->map.rows)
	// {
	// 	ft_printf("%s\n", game->map.full[i]);
	// 	i++;
	// }

}

// 初始化game中map的金币数、出口数、玩家数，移动次数和玩家的初始朝向
void	ft_init_vars(t_game *game)
{
	game->map.coins = 0;
	game->map.exit = 0;
	game->map.players = 0;
	game->movements = 0;
	game->player_sprite = RIGHT;
}

// 确保地图的每一行的两端都有墙壁
void	ft_check_rows(t_game *game)
{
	int	row;
	int	last_col;

	row = 0;
	last_col = game->map.columns - 1;
	while (row < game->map.rows)
	{
		// ft_printf("Checking row %d, first_col: 0, last_char: %c\n", row, game->map.full[row][0]);
		// ft_printf("Checking row %d, last_col: %d, last_char: %c\n", row, last_col, game->map.full[row][last_col]);
		if (game->map.full[row][0] != WALL)
			ft_error_msg("Invalid Map! Check the (left) wall again. 🧱",
				game);
		if (game->map.full[row][last_col] != WALL)
			ft_error_msg("Invalid Map! Check the (right) wall again. 🧱",
				game);
		row++;
	}
}

// 检查地图的顶端和底端（即第一行和最后一行）是否完全由墙壁包围
void	ft_check_columns(t_game *game)
{
	int	col;
	int	last_row;

	col = 0;
	last_row = game->map.rows - 1;
	while (col < game->map.columns)
	{
		// ft_printf("Checking col %d, top_row: 0, top_char: %c\n", col, game->map.full[0][col]);
		// ft_printf("Checking col %d, last_row: %d, last_char: %c\n", col, last_row, game->map.full[last_row][col]);
		if (game->map.full[0][col] != WALL)
			ft_error_msg("Invalid Map! Check the (top) wall again. 🧱", game);
		if (game->map.full[last_row][col] != WALL)
			ft_error_msg("Invalid Map! Check the (bottom) wall again. 🧱", game);
		col++;
	}
}

// 记录元素的位置和数量，检查地图中是否有不期望的字符
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

// 验证元素数量是否正确
void	ft_verify_map_parameters(t_game *game)
{
	if (game->map.coins == 0)
		ft_error_msg("Invalid Map! The map has no collectible. 🍬", game);
	if (game->map.exit != 1)
		ft_error_msg("Invalid Map! It must have 1 exit. 🚪", game);
	if (game->map.players != 1)
		ft_error_msg("Invalid Map! It must have 1 player. 🎮", game);
}

// 检查地图文件的每一行是否具有相同的长度
void	ft_check_row_lengths(t_game *game)
{
	int	i;
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

// 创建一个地图的副本用洪水填充检验
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

// 从指定的起点遍历整个地图，并标记可以到达的区域
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

// 遍历地图的每个块，检查是否有不可达的出口和收藏品，并更新未访问的金币计数
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

// 确保从玩家的起始位置可以到达所有出口和收藏品。如果不能，则地图无效。
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

// 检查游戏地图的有效性。检查长宽，计数和核对要素数量
void	ft_check_map(t_game *game)
{
	ft_check_rows(game);
	ft_check_columns(game);
	ft_count_map_parameters(game);
	ft_verify_map_parameters(game);
	ft_check_row_lengths(game);
	ft_check_map_validity(game);
}

// 加载指定路径的sprite，成功MiniLibX则返回包含图像指针、宽度和高度的t_image
t_image	ft_load_sprite(void *mlx, char *path, t_game *game)
{
	t_image	sprite;

	sprite.xpm_ptr = mlx_xpm_file_to_image(mlx, path, &sprite.x, &sprite.y);
	if (sprite.xpm_ptr == NULL)
		ft_error_msg("Couldn't find the sprite. Did it run away? 🕵️‍♂️", game);
	return (sprite);
}

// 初始化所有sprites并加载到内存
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

// 初始化MiniLibX并创建窗口
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

// 调用MiniLibX绘制
void	ft_render_sprite(t_game *game, t_image sprite, int row, int col)
{
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, sprite.xpm_ptr, col
		* sprite.x, row * sprite.y);
}

// 根据玩家朝向渲染玩家图像
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

// 用MiniLibX将玩家的移动次数显示在游戏窗口中
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

// 识别字符，决定要渲染的图像。墙、地板、金币直接渲染，出口判断是否还有金币，角色渲染需要进一步判定
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

// 遍历地图，渲染图像，打印移动
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

// 清除所有图像资源
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

// 释放所有动态内存和资源，关闭所有与图形界面相关的资源
// mlx_ptr是在调用 mlx_init() 时动态分配的内存块，需要手动释放
// win_ptr 是由 MLX 库内部管理的资源，调用 mlx_destroy_window 时已经处理了其释放
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

// 显示胜利消息，释放所有分配的内存，并退出游戏
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

// 清理，显示信息，并终止程序
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

// 检查玩家是否可以移动到指定的新位置，并在可能的情况下更新玩家的位置、地图状态，并重新渲染地图
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

// 检查新位置是否在地图范围内，然后根据按键的不同执行相应的动作
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

// mlx_hook将一个事件（如按键、鼠标点击等）与一个回调函数关联起来。当这个事件发生时，MLX 会调用相应的回调函数
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
