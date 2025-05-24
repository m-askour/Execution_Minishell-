CC = cc 
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
SRC_PARSING=./parsing/tools/split.c         ./parsing/tools/string_pasing.c ./parsing/tools/init_tools.c \
			./parsing/tools/string_tools.c  ./parsing/tools/free_tools.c    ./parsing/tools/print_file.c ./parsing/tools/tools.c\
			./parsing/parsing.c             ./parsing/storge_tokens.c     ./parsing/tokenizer.c ./parsing/expand.c ./parsing/command.c main.c\
			./exicution_minishell/execution.c ./exicution_minishell/redirections.c ./exicution_minishell/pipex_util.c\
			./exicution_minishell/utils/ft_split.c\
			./exicution_minishell/utils/ft_strchr.c ./exicution_minishell/utils/ft_strjoin.c\
			./exicution_minishell/utils/ft_strlcat.c ./exicution_minishell/utils/ft_isalpha.c\
			./exicution_minishell/utils/ft_strlcpy.c ./exicution_minishell/utils/ft_atoi.c\
			./exicution_minishell/utils/ft_strncmp.c ./exicution_minishell/utils/ft_strnstr.c\
			./exicution_minishell/builtin/builtin.c ./exicution_minishell/builtin/ft_cd.c \
			./exicution_minishell/builtin/ft_echo.c ./exicution_minishell/builtin/ft_env.c \
			./exicution_minishell/builtin/ft_exit.c ./exicution_minishell/builtin/ft_export.c \
			./exicution_minishell/builtin/ft_pwd.c ./exicution_minishell/builtin/ft_unset.c \
			./exicution_minishell/builtin/utils_1.c 
FLAGS= -lreadline
OBJ_PARSING = $(SRC_PARSING:.c=.o)

NAME = minishell

all : $(NAME)

$(NAME) : $(OBJ_PARSING)
	$(CC) $(CFLAGS) ${OBJ_PARSING} -o $@ -lreadline

%.o : %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -rf $(OBJ_PARSING)

fclean : clean
	rm -rf $(NAME)

re : fclean all