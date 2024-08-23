# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuxchen <yuxchen@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/23 14:21:06 by yuxchen           #+#    #+#              #
#    Updated: 2024/08/23 20:16:51 by yuxchen          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= so_long

LIBFT 			= ./libft/libft.a
GET_NEXT_LINE	= ./get_next_line/get_next_line.a
FT_PRINTF		= ./ft_printf/libftprintf.a

CC 				= clang

STANDARD_FLAGS 	= -Wall -Werror -Wextra
MINILIBX_FLAGS	= -lmlx -lXext -lX11

VALGRIND		= @valgrind --leak-check=full --show-leak-kinds=all \
--track-origins=yes --quiet --tool=memcheck --keep-debuginfo=yes

REMOVE 			= rm -f

SRCS_DIR		= ./srcs/
OBJS_DIR		= ./objs/

SRCS 			= $(addprefix $(SRCS_DIR),\
				so_long.c)

OBJS			= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)

all:			${LIBFT} ${GET_NEXT_LINE} ${FT_PRINTF} ${NAME}

${NAME}: 		${OBJS}
				${CC} ${OBJS} ${LIBFT} ${GET_NEXT_LINE} ${FT_PRINTF} ${STANDARD_FLAGS} ${MINILIBX_FLAGS} -o ${NAME}
				@echo "$(NAME) was compiled"

${OBJS_DIR}%.o:	${SRCS_DIR}%.c
				@mkdir -p $(OBJS_DIR)
				${CC} ${STANDARD_FLAGS} -c $< -o $@

${LIBFT}:
				make -C ./libft

${GET_NEXT_LINE}:
				make -C ./get_next_line

${FT_PRINTF}:
				make -C ./ft_printf

clean:
				make clean -C ./libft
				make clean -C ./get_next_line
				make clean -C ./ft_printf
				${REMOVE} -rf $(OBJS_DIR)

fclean:			clean
				make fclean -C ./libft
				make fclean -C ./get_next_line
				make fclean -C ./ft_printf
				${REMOVE} ${NAME}
				@echo "${NAME} were deleted"

re:				fclean all

run:			${NAME}
				${VALGRIND} ./${NAME} assets/maps/valid/map4.ber

.PHONY:			all clean fclean re valgrind run
