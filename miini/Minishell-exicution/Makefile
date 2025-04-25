CC = cc 
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
SRC_PARSING=./parsing/tools/split.c         ./parsing/tools/string_pasing.c ./parsing/tools/init_tools.c \
			./parsing/tools/string_tools.c  ./parsing/tools/free_tools.c    ./parsing/tools/print_file.c\
			./parsing/parsing.c             ./parsing/storge_tokens.c     ./parsing/tokenizer.c   main.c\
			./exicution_minishell/execution.c ./exicution_minishell/redirections.c ./exicution_minishell/pipex_util.c\
			./exicution_minishell/utils/ft_itoa.c ./exicution_minishell/utils/ft_split.c\
			./exicution_minishell/utils/ft_strchr.c ./exicution_minishell/utils/ft_strjoin.c\
			./exicution_minishell/utils/ft_strlcat.c\
			./exicution_minishell/utils/ft_strlcpy.c \
			./exicution_minishell/utils/ft_strncmp.c 
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