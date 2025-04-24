/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 04:59:25 by ahari             #+#    #+#             */
/*   Updated: 2025/04/23 18:55:00 by maskour          ###   ########.fr       */
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

/*---------------function for free--------------------*/
void			free_tokens(t_token *tokens, char *input);
void    		free_cmd(t_cmd *cmd);
void    		free_cmd_array(char **cmd);
void    		free_files(t_file *files, int file_count);
void			free_cmd_list(t_cmd **cmd_list, int count);

/*-----------------for print--------------------------*/
void			print_tokens(t_token *head);
void			print_command_with_files(t_cmd *cmd);

/*---------------function for create------------------*/
t_cmd    		*init_cmd(void);
t_file			*init_mfile(void);

/*---------------cmd----------------------------------*/
t_cmd			**parse_commands(t_token *tokens);
int				ft_isredirect(t_token_type type);
int				count_args(t_token *token);

/*------------ tools for parsing ----------------*/
void			ft_putstr_fd(char *s, int fd, char c);
int     		ft_isspace(char c);
int     		is_operator(const char s);
char    		**ft_split(char *str);
char			*ft_strndup(const char *s1, size_t size);
char			*ft_strdup(const char *s1);
char			*ft_strncpy(char *dest, const char *src, size_t n);

/*--------------this function for tockens------------*/
void			add_token(t_token **head, t_token *new);
t_token_type	get_token_type(const char *s);
t_token			*new_token(char *val, t_token_type type);

/*---------------parsing parts-----------------------*/
t_token			*string_tokens(char *str);




#endif