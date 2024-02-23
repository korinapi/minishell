# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 12:47:45 by cpuiu             #+#    #+#              #
#    Updated: 2024/02/06 21:25:17 by cpuiu            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -I./Libft -I./include

LIBFTDIR = ./Libft
LIBFT = $(LIBFTDIR)/libft.a 
LIBFT_PATH = ./Libft

OBJSDIR			=	objs
SRCSDIR			=	srcs
INCLUDES		=	include/pipex.h $(LIBFT_PATH)/libft.h

PIPEX_BONUS =	$(SRCSDIR)/fd_bonus.c $(SRCSDIR)/free_all_bonus.c $(SRCSDIR)/get_next_line.c \
				$(SRCSDIR)/here_doc.c $(SRCSDIR)/process_bonus.c
OBJ_BONUS_O =	$(OBJSDIR)/fd_bonus.o $(OBJSDIR)/free_all_bonus.o $(OBJSDIR)/get_next_line.o \
				$(OBJSDIR)/here_doc.o $(OBJSDIR)/process_bonus.o

PIPEX_MAIN = $(SRCSDIR)/main.c
OBJ_PIPEX_MAIN = $(OBJSDIR)/main.o
SRC_FILES = $(SRCSDIR)/error_management.c $(SRCSDIR)/free_all.c $(SRCSDIR)/process.c $(SRCSDIR)/split.c $(SRCSDIR)/utils.c
OBJ_FILES = $(OBJSDIR)/error_management.o $(OBJSDIR)/free_all.o $(OBJSDIR)/process.o $(OBJSDIR)/split.o $(OBJSDIR)/utils.o

OBJ_FILES += $(OBJ_BONUS_O)

NAME = pipex

$(OBJSDIR)/%.o: $(SRCSDIR)/%.c $(INCLUDES) | $(OBJSDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(NAME) : $(OBJ_FILES) $(OBJ_PIPEX_MAIN) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ -L$(LIBFTDIR) -lft
	
$(LIBFT):
	make -C $(LIBFTDIR) && make clean -C $(LIBFTDIR)

all : $(NAME)

clean:
	$(RM) -r $(OBJSDIR)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFTDIR)

re: fclean all

bonus: all
$(OBJSDIR):
	mkdir -p $(OBJSDIR)

.PHONY: all clean fclean re