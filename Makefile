NAME		:= minishell
CC			:= gcc -std=gnu99
CFLAGS		:= -Wall -Wextra -Werror -g -O3
LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

HEADERS		:= -I ./include
LIBS		:= -L$(LIBFT_DIR) -lft -lreadline
SRCS		:= src/main.c src/builtins/echo.c src/builtins/exit.c src/builtins/builtins_utils.c src/environment/environment.c \
src/execution/exec_built.c src/execution/executor.c src/execution/redirections.c src/input/lex.c \
src/parser/parser.c src/signals/signals.c src/errors/errors.c src/utils/print_checks.c src/free/free.c \
src/builtins/unset.c src/builtins/export/export.c src/builtins/export/export_utils.c src/builtins/env.c \
src/builtins/cd.c src/builtins/pwd.c src/environment/env_mem_funcs.c src/execution/exec_utils.c \
src/utils/utilities.c src/execution/pipelines.c src/utils/append_utils.c src/parser/variables.c \
src/parser/parse_pipes.c src/parser/parse_quotes.c src/parser/parse_redirect.c src/parser/parse_simple.c src/parser/parse_utils.c

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
	@echo "$(COLOR_GREEN)Cleanup completed.$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_CYAN)Full compiled clean initiated$(COLOR_RESET)"
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(COLOR_GREEN)Full-Clean completed.$(COLOR_RESET)"

re: fclean all
test:
	@cd tests && bash tester.sh a

.PHONY: all clean fclean re