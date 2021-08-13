# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/03 13:31:12 by rvan-aud          #+#    #+#              #
#    Updated: 2021/08/13 14:09:07 by rvan-aud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= 	srcs/main.c				\
			srcs/cut_cmd.c			\
			srcs/free.c				\
			srcs/parse_cmd.c		\
			srcs/path_handling.c	\
			srcs/pipex.c			\
			srcs/utils.c			\

NAME	= pipex

CC		= gcc

RM		= rm -f

CFLAGS	= -I includes -Wall -Wextra -Werror

OBJS	= $(SRCS:.c=.o)

LIBFT	= libft/libft.a

$(NAME):	$(OBJS)
			@$(MAKE) -C ./libft
			@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

all:		$(NAME)

clean:
			@$(MAKE) -C libft fclean
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re