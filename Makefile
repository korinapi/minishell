NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g -O3
LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

HEADERS		:= -I ./include
LIBS		:= -L$(LIBFT_DIR) -lft -lreadline
SRCS		:= main.c lex.c signals.c builtins/exit.c parsing.c hash.c
# builtins/cd.c builtins/echo.c builtins/env.c  builtins/export.c builtins/pwd.c builtins/unset.c
OBJS		:= $(SRCS:.c=.o)

COLOR_RESET			=	\033[0m
COLOR_CYAN			=	\033[36m
COLOR_GREEN			=	\033[32m
COLOR_RED			=	\033[31m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): compile_message $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "$(COLOR_GREEN)Compilation for $(NAME) complete$(COLOR_RESET)"

compile_message:
	@echo "$(COLOR_CYAN)Beginning compilation for: $(NAME)$(COLOR_RESET)"

clean:
	@echo "$(COLOR_CYAN)Cleaning compiled files$(COLOR_RESET)"
	@rm -rf $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(COLOR_GREEN)Cleanup completed.$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_CYAN)Full compiled clean initiated$(COLOR_RESET)"
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(COLOR_GREEN)Full-Clean completed.$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re
