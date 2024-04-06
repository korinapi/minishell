NAME		:= minishell
CC			:= gcc -std=gnu99
CFLAGS		:= -Wall -Wextra -Werror -g
LIBFT_DIR	:= libft
#PIPEX_DIR	:= pipex
LIBFT		:= $(LIBFT_DIR)/libft.a
#PIPEX		:= $(PIPEX_DIR)/pipex

HEADERS		:= -I ./include
LIBS		:= -L$(LIBFT_DIR) -lft -lreadline
SRCS		:= src/main.c src/builtins/echo.c src/builtins/exit.c src/environment/environment.c \
src/execution/exec_built.c src/execution/executor.c src/execution/redirections.c src/input/lex.c \
src/parser/parser.c src/signals/signals.c src/errors/errors.c src/utils/print_checks.c src/free/free.c \
src/builtins/unset.c src/builtins/export.c src/builtins/env.c src/builtins/cd.c src/builtins/pwd.c \
src/utils/utilities.c src/builtins/builtins_utils.c src/environment/environment_utils.c src/execution/exec_pipe.c \
src/execution/exec_utils.c src/parser/parser_utils_1.c src/parser/parser_utils_2.c src/parser/parser_utils_3.c \
src/parser/parser_simple_command.c src/parser/parser_redirection.c src/parser/parser_pipeline.c 

OBJS		:= $(SRCS:.c=.o)

COLOR_RESET			=	\033[0m
COLOR_CYAN			=	\033[36m
COLOR_GREEN			=	\033[32m
COLOR_RED			=	\033[31m

all: $(LIBFT) $(NAME)

$(PIPEX):
	@$(MAKE) -C $(PIPEX_DIR)

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
#	@$(MAKE) -C $(PIPEX_DIR) clean
	@echo "$(COLOR_GREEN)Cleanup completed.$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_CYAN)Full compiled clean initiated$(COLOR_RESET)"
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
#	@$(MAKE) -C $(PIPEX_DIR) fclean
	@echo "$(COLOR_GREEN)Full-Clean completed.$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re
