/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 04:59:25 by ahari             #+#    #+#             */
/*   Updated: 2025/07/17 14:02:29 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

///the headar of all file that we will use in this project 
#include <signal.h>//this for the signals
#include <limits.h>//this for the limits
#include <fcntl.h> // Required for open() and flags
#include <stdio.h>  // perror()
#include <readline/readline.h>  // For rl_replace_line, rl_redisplay, etc.
#include <readline/history.h> // for readline
#include <unistd.h> //  for getcwd() , chdir() , isatty() ,dup2(), close()
#include <sys/stat.h> // stat() & lstat() & fstat()
#include <sys/types.h> // opendir() closedir()
#include <dirent.h> // opendir() ,readdir() ,closedir()
#include <sys/ioctl.h> // For ioctl()
#include <stdlib.h> //
#include <termios.h> //
#include <curses.h> //
#include <term.h> //
#include <string.h>
#include <sys/wait.h>

// #define PATH_MAX 4096

typedef enum e_token_type
{
	TOKEN_WORD,      // command or argument
	TOKEN_PIPE,      // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_NULL,       // for empty or errors
	TOKEN_SEMICOLON,
} t_token_type;

typedef struct s_shell
{
    int exit_status;
} t_shell;

typedef struct s_token
{
	char            *value;         // the actual string (e.g. "ls", ">", "file.txt")
	t_token_type    type;           // type of token
	int				was_quoted;  // '\"', '\'', or 0 (no quotes)
	struct s_token  *next;          // pointer to next token
}   t_token;

typedef struct s_file
{
	char			*name; // name of the file
	t_token_type	type;// file type
	int				check_expand; 
}	t_file;

typedef struct s_cmd
{
	char		**cmd; // command namel
	t_file		*files; // files associated with the command
	int			file_count; // number of files
	struct s_cmd	*next; // pointer to next command
}	t_cmd;

//this struct where i str all the infermation of env
typedef struct s_env
{
	char *data_env;//this the path like use/bin....
	struct s_env *next; /* data */
}t_env;
//export
typedef struct s_env_list
{
	char	**env_table;
	t_shell		*exit_status;
}	t_env_list;


/*---------------function for free--------------------*/
void			free_tokens(t_token *tokens, char *input);
void			free_char_array(char **array);
void			free_cmd(t_cmd *cmd);
void			free_cmd_array(char **cmd);
void			free_files(t_file *files, int file_count);
void			free_cmd_args(char **args, int count);
void			free_cmd_list(t_cmd *cmd_list);
void			print_error(t_token *head, char *val, t_shell *shell_ctx);
void			print_syntax_error(t_token *current);
void			free_array(char **array);

/*-----------------Tokenizer --------------------------*/
t_token			*string_tokens(char *str, t_shell *shell_ctx);
t_token			*find_previous_token(t_token *head, t_token *target);

/*-----------------parsing --------------------------*/
int				is_export_assignment(t_token *head, t_token *current);
char			*process_quoted_value(char *val, t_token *head, t_env_list *env);

/*-----------------for print--------------------------*/
void			print_tokens(t_token *head);
void			print_command_with_files(t_cmd *cmd);

/*---------------function for create------------------*/
t_cmd			*init_cmd(void);
t_file			*init_mfile(void);

//*---------------parsing_herdoc-----------------------*/
char			*safe_strcat_heredoc(char *dest, char *src);
char			*process_quoted_heredoc(char *val, int *i);

//*---------------parsing_v2--------------------------*/
char			*realloc_result(char *result, size_t *capacity, size_t new_len);
char			*append_to_result(char *result, char *tmp, size_t *capacity);
int				count_commands(t_token *tokens);
int				parse_arguments(t_cmd *cmd, t_token *tokens);
int				process_file_redirection(t_cmd *cmd, t_token *current);
int				validate_redirection_token(t_token *current);
t_token			*create_and_add_token(char *val, t_token **head);
/*---------------cmd----------------------------------*/
t_cmd			*parse_commands(t_token *tokens, t_shell *shell_ctx);
int				ft_isredirect(t_token_type type);
int				count_args(t_token *token);
t_token			*check_quoted(char *str, t_shell *shell_ctx, char **env_table);
char			**convert(t_env *env_list);
int				count_herdoc(t_cmd *cmd);
int				process_token(t_token *current, t_token **head, t_env_list *env_list);
char			*herdoc_parsing(char *val);
char			*remove_char(const char *str, char to_remove);
char			*remove_dollar_before_quotes(char *str, char **temp);
int				check_syntax_errors(char *str, int i, t_shell *shell_ctx);
int				validate_syntax(char *str, t_shell *shell_ctx);
char			*extract_word_token(char *str, int start, int end);
int				count_redirections(t_token *start);
int				process_quote_parsing(char *str, int *i, char *quote_type, int *in_quotes);

/*------------ tools for parsing ----------------*/
int				is_quote(char c);
int				is_char(char c);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int				ft_isspace(char c);
int				is_operator(const char s);
int				ft_isalpha(char c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
size_t			ft_strlen(const char *s);
void			ft_putstr_fd(char *s, int fd, char c);
char			*ft_strndup(const char *s1, size_t size);
char			*ft_strdup(const char *s1);
char			**ft_split(char *str);
char			*ft_strncpy(char *dest, const char *src, size_t n);
char			*ft_strcat(char *dest, const char *src);
char			*ft_substr(const char *s, unsigned int start, size_t len);
char			*ft_itoa(int n);
int				ft_strspace(char *str);
void			ft_putchar_fd(char c, int fd);
char			*ft_exit_status(t_shell *exit_stat);
int				ft_isdigit(int c);
char			*ft_delete_spaces(char *str);
int				ft_strspaces(char *str);
char			*ft_strcpy(char *dest, const char *src);
char			*found_env(char *cmd, char **env, t_shell *shell_ctx);
int				has_quotes(char *str);

//*---------------this function for expand------------*/
char			*handle_special_var(char *cmd, int pos, t_shell *exited);
char			*get_env(char **env, char *found_env);
char			*handle_env_var(char *cmd, int pos, char **env);
char			*replace_double_dollar(char *cmd);
int				should_expand_var(char *cmd, int pos);
/*--------------this function for tockens------------*/
void			add_token(t_token **head, t_token *new);
t_token_type	get_token_type(const char *s);
t_token			*new_token(char *val, t_token_type type);






/*---------------exicution_util-----------------------*/
// char	*ft_itoa(int n);
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
// int	ft_isdigit(int c);
int	ft_isalpha_up(int c);
// int	ft_isalnum(int c);
char	*ft_strstr(const char *haystack, const char *needle);

char	*ft_strtrim(char const *s1, char const *set);
/*---------------exicution_util-----------------------*/
int exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx);
// int execute_single_command(t_cmd **cmd, char **envp);
int redirections(t_cmd *cmd,int last_heredoc_index);
char	*find_path(char *cmd, char **env);

/*---------------------builtins-----------------------*/
t_env *execut_bultin(t_cmd **cmd, t_env *env_list, t_shell *shell, int i);
int is_builtin(char *command);
t_env *ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx);
void ft_echo(t_cmd **cmd, t_shell *shell_ctx);
void ft_env(t_env *env_list, t_shell *shell_ctx);
void ft_exit(t_cmd **cmd, t_shell *shell, int j);
void ft_pwd(t_shell *shell_ctx, t_env *env_list);
t_env *ft_unset(t_cmd **cmd, t_env *env, t_shell *shell_ctx);
void ft_export(t_cmd **cmd, t_env **envp, t_shell *shell);

/*---------------builtins-utils-----------------------*/
char *search_env(t_env *env, const char *key);
t_env *new_env(char *data_env);
void add_env(t_env **env_list, t_env *new_node);
t_env *file_inv(char **env);
void free_env_list(t_env *env_list);

/*---------------signals-----------------------*/
void handler_sig(int signal);

int function_herdoc(t_file *file, char ** env, t_shell *shell_ctx);
void execute_single_command(t_cmd **cmd,t_env **env_list ,char **envp, t_shell *shell_ctx);
void execute_pipeline(t_cmd **cmds, int cmd_count, char **env, t_env *env_list, t_shell *shell_ctx);
char	**convert(t_env *env_list);

#endif