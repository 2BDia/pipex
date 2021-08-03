# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/03 13:31:12 by rvan-aud          #+#    #+#              #
#    Updated: 2021/08/03 14:13:59 by rvan-aud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= 	main.c			\
			free.c			\
			path_handling.c	\
			utils.c			\

NAME	= pipex

CC		= gcc

RM		= rm -f

CFLAGS	= -I includes -Wall -Wextra -Werror

OBJS	= $(SRCS:.c=.o)

LIBFT	= libft.a

$(NAME):	$(OBJS)
			@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re