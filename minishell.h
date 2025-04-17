/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekarada <bekarada@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:39:41 by bekarada          #+#    #+#             */
/*   Updated: 2025/03/25 18:14:06 by bekarada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
#include <sys/wait.h>
# include "libft/libft.h"

#define BUF_SIZE 42

typedef struct s_token
{
    char    *value;  // Token'ın değeri
    int     dollar_flag;
    int     pipe_flag;
    int     redirect_flag;
    struct s_token *next; // Bir sonraki token
} t_token;

typedef struct s_environment
{
    char                *variable;     // Çevre değişkeninin adı
    char                *value;        // Çevre değişkeninin değeri
    struct s_environment *next;         // Bir sonraki çevre değişkenine işaretçi
}                       t_environment;

typedef struct s_program
{
    int                 id;
    int                 quote_state;   // 0: Kapalı, 1: Tek tırnak, 2: Çift tırnak
    int                 single_quote;  // Tek tırnak sayacı
    int                 double_quote;  // Çift tırnak sayacı
    int                 pipe_count;    // Pipe sayısı
    int                 status_code;   // Çıkış durumu
    char                *input_program; // Orijinal girdi satırı
    char                *parsed_program; // İşlenmiş girdi (bazen temizlenmiş veya formatlanmış)
    t_token              *tokens;      // Komutları ayıran tokenlar
    char                **full_path;   // Yönlendirilmiş komut yolları
    int                 env_len;       // Çevre değişkenlerinin uzunluğu
    struct s_environment *env_list;     // Çevre değişkenleri listesi
    struct s_environment *exp_list;     // Expansiyon (değişkenler ve parametreler) listesi
    char                **command_args; // Komut argümanları (örneğin "ls", "-l", "/home" gibi)
    int                 bg_process;     // Arka planda çalışan işlem olup olmadığını belirtir (1: evet, 0: hayır)
}                       t_program;


int	    dollar_syntax_check(char *str);
void	ft_create_env(t_program *prgm, char **env, t_environment **env_list);
//void	ft_lexer(char *input, t_token **token_list);
void	ft_lexer(char *input, t_token **token_list, int status_code);
void    pipe_handle_quotes_check(char temp, int *squotes, int *dquotes);
int     pipe_check(char *line, int sq, int dq);
int     handle_quotes(t_program *prgm);
int     validate_redirects(t_program *prgm);
void	ft_signal(void);
void	signal_handler(int sig);
int     history_check(t_program *prgm);
char	*ft_strndup(char *str, size_t n);
int     ft_isspace(int c);
int     ft_strcmp(const char *s1, const char *s2);
void	ft_parser(t_program *prgm);
void	ft_executor(t_program *prgm, int exec_flag);
char    *ft_strcpy(char *dest, const char *src);
//builtin
void	ft_exit(t_program *program);
void	ft_cd(t_program *prgm);
void    ft_echo(t_program *prgm);
void	ft_pwd(t_program *prgm);
void	ft_env(t_program *prgm);
void	ft_export(t_program *prgm, char *arg);
void	ft_unset(t_program *prgm, char **arg);
char *ft_quotes_clean(char *str);
//




////
char *reconstruct_export_argument(t_token *starts);
void sort_env(t_environment **env, int size);
void print_sorted_export(t_environment *exp_list);
void	ft_redirect_lexer(char *input, t_token **token_list, int quote_flag, int status_code);
void	print_tokens(t_token *token);
char	**ft_free_split(char **arr, int i);
char	*get_env_value(const char *var);
void	ft_expand_and_set_token(t_token *new_token, char *add_token, int k, int last_exit_code);
void	ft_exec_not_built(t_program *prgm);
char *ft_strcat(char *dest, const char *src);
void	ft_redirect_token(char *input, t_token **token_list, int i, int status_code);

#endif