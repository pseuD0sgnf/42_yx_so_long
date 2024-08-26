/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_initialization.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:54:30 by yuxchen           #+#    #+#             */
/*   Updated: 2024/08/25 12:57:49 by yuxchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
