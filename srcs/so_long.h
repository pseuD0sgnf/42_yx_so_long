/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:20:35 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/23 20:23:35 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <mlx.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "../libft/libft.h"
# include "../ft_printf/includes/ft_printf.h"
# include "../get_next_line/get_next_line.h"

# define IMG_HEIGHT			32
# define IMG_WIDTH			32

# define WALL				'1'
# define FLOOR 				'0'
# define COINS  			'C'
# define PLAYER				'P'
# define MAP_EXIT 		 	'E'

# define KEY_W				119
# define KEY_A				97
# define KEY_S				115
# define KEY_D				100

# define KEY_UP  			65362
# define KEY_LEFT  			65361
# define KEY_RIGHT 			65363
# define KEY_DOWN  			65364
# define KEY_ESC  			65307

# define FRONT				1
# define LEFT				2
# define RIGHT				3
# define BACK				4

# define WALL_XPM			"assets/sprites/wall.xpm"
# define FLOOR_XPM			"assets/sprites/floor.xpm"
# define COINS_XPM			"assets/sprites/coin-bag.xpm"
# define PLAYER_FRONT_XPM	"assets/sprites/player/front.xpm"
# define PLAYER_LEFT_XPM	"assets/sprites/player/left.xpm"
# define PLAYER_RIGHT_XPM	"assets/sprites/player/right.xpm"
# define PLAYER_BACK_XPM	"assets/sprites/player/back.xpm"
# define OPEN_EXIT_XPM		"assets/sprites/open-exit.xpm"
# define EXIT_CLOSED_XPM	"assets/sprites/exit-closed.xpm"

typedef struct s_position
{
	int	x;
	int	y;
}	t_positon;

typedef struct s_image
{
	void	*xpm_ptr;
	int		x;
	int		y;
}	t_image;

typedef struct s_map
{
	char		**full;
	int			rows;
	int			columns;
	int			coins;
	int			exit;
	int			players;
	t_positon	player;
}	t_map;

typedef struct s_game
{
	void		*mlx_ptr;
	void		*win_ptr;
	int			movements;
	int			player_sprite;
	t_map		map;
	bool		map_alloc;
	t_image		wall;
	t_image		floor;
	t_image		coins;
	t_image		open_exit;
	t_image		exit_closed;
	t_image		player_front;
	t_image		player_left;
	t_image		player_right;
	t_image		player_back;
}	t_game;

void	ft_free_map(t_game *game);
int		ft_error_msg(char *msg, t_game *game);
void	ft_check_args(int argc, char **argv, t_game *game);
char	*ft_strappend(char **original, const char *to_append);
void	ft_check_for_empty_line(char *map, t_game *game);
void	ft_init_map(t_game *game, char *file_name);
void	ft_init_vars(t_game *game);
void	ft_check_rows(t_game *game);
void	ft_check_columns(t_game *game);
void	ft_count_map_parameters(t_game *game);
void	ft_verify_map_parameters(t_game *game);
void	ft_check_row_lengths(t_game *game);
char	**copy_map(t_game *game);
void	flood_fill(char **map, int y, int x);
void	check_map_for_coins_and_exit(char **map_copy, int *coins_count, t_game *game);
void	ft_check_map_validity(t_game *game);
void	ft_check_map(t_game *game);
t_image	ft_load_sprite(void *mlx, char *path, t_game *game);
void	ft_init_sprites(t_game *game);
void	ft_init_mlx(t_game *game);
void	ft_render_sprite(t_game *game, t_image sprite, int row, int col);
void	ft_render_player(t_game *game, int row, int col);
void	ft_print_movements(t_game *game);
void	ft_identify_sprite(t_game *game, int row, int col);
int		ft_render_map(t_game *game);
void	ft_destroy_images(t_game *game);
void	ft_free_all_allocated_memory(t_game *game);
int		ft_victory(t_game *game);
int		ft_close_game(t_game *game);
void	ft_player_move(t_game *game, int new_y, int new_x, int player_sprite);
int		ft_handle_input(int keysym, t_game *game);

#endif
