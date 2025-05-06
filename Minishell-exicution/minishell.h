/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 04:59:25 by ahari             #+#    #+#             */
/*   Updated: 2025/05/06 10:44:22 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
///the headar of all file that we will use in this project 
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>  // perror()
#include <readline/readline.h> //for 
#include <readline/history.h> // for readline
#include <unistd.h> //  for getcwd() , chdir() , isatty()   q
#include <sys/stat.h> // stat() & lstat() & fstat()
#include <sys/types.h> // opendir() closedir()
#include <dirent.h> // opendir() readdir() closedir()
#include <string.h> // strerror() && 
#include <sys/ioctl.h> //
#include <stdlib.h> //
#include <termios.h> //
#include <curses.h> //
#include <term.h> //
#include <string.h>

// #define PATH_MAX 4096

typedef enum e_token_type
{
	TOKEN_WORD,      // command or argument
	TOKEN_PIPE,      // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_NULL,        // for empty or errors
} t_token_type;


typedef struct s_token
{
	char            *value;         // the actual string (e.g. "ls", ">", "file.txt")
	t_token_type    type;           // type of token
	struct s_token  *next;          // pointer to next token
}   t_token;

typedef struct s_file
{
	char			*name; // name of the file
	t_token_type	type;// file type
}	t_file;

typedef struct s_cmd
{
	char		**cmd; // command name
	t_file		*files; // files associated with the command
	int			file_count; // number of files
}	t_cmd;

//this struct where i str all the infermation of env
typedef struct s_env
{
	char *data_env;//this the path like use/bin....
	struct s_env *next; /* data */
}t_env;

/*---------------function for free--------------------*/
void                    free_tokens(t_token *tokens, char *input);
void                    free_cmd(t_cmd *cmd);
void                    free_cmd_array(char **cmd);
void                    free_files(t_file *files, int file_count);
void                    free_cmd_list(t_cmd **cmd_list, int count);
void                    print_error(t_token *head, char *val);
/*-----------------Tokenizer --------------------------*/
t_token                 *string_tokens(char *str);


/*-----------------for print--------------------------*/
void                    print_tokens(t_token *head);
void                    print_command_with_files(t_cmd *cmd);

/*---------------function for create------------------*/
t_cmd                   *init_cmd(void);
t_file                  *init_mfile(void);

/*---------------cmd----------------------------------*/
t_cmd                   **parse_commands(t_token *tokens);
int                             ft_isredirect(t_token_type type);
int                             count_args(t_token *token);
t_token                 *check_quoted(char *str);

/*------------ tools for parsing ----------------*/
void                    ft_putstr_fd(char *s, int fd, char c);
int                     ft_isspace(char c);
int                     is_operator(const char s);
char                    **ft_split(char *str);
char                    *ft_strndup(const char *s1, size_t size);
char                    *ft_strdup(const char *s1);
char                    *ft_strncpy(char *dest, const char *src, size_t n);
size_t                  ft_strlen(const char *s);

/*--------------this function for tockens------------*/
void                    add_token(t_token **head, t_token *new);
t_token_type    get_token_type(const char *s);
t_token                 *new_token(char *val, t_token_type type);

/*---------------parsing parts-----------------------*/
t_token                 *string_tokens(char *str);








/*---------------exicution_util-----------------------*/
char	*ft_itoa(int n);
char	**ft_split_up(char const *s, char c);
char	*ft_strchr(const char *src, int c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
size_t	ft_strlen(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putstr_fd_up(char *s, int fd);
int	ft_atoi(const char *str);
int	ft_isdigit(int c);
int	ft_isalpha_up(int c);
int	ft_isalnum(int c);


/*---------------exicution_util-----------------------*/
int exicut(t_cmd **cmd, char **env);
// int execute_single_command(t_cmd **cmd, char **envp);
void redirections(t_cmd *cmd);
char	*find_path(char *cmd, char **env);

/*---------------------builtins-----------------------*/
void execut_bultin(t_cmd **cmd, char **env);
int is_builtin(char *command);
void ft_cd(t_cmd **cmd, t_env *data_env);
void ft_echo(t_cmd **cmd);
void ft_env(t_env *env_list);
void ft_exit(t_cmd **cmd);
void ft_pwd();
void ft_unset(t_cmd **cmd, t_env *env);
void ft_export(t_cmd **cmd, t_env *envp);


/*---------------builtins-utils-----------------------*/
char *search_env(t_env *env, const char *key);
t_env *new_env(char *data_env);
void add_env(t_env **env_list, t_env *new_node);
t_env *file_inv(char **env);

#endif