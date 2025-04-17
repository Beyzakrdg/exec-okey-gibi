#include "minishell.h"

void ft_redirect_token(char *input, t_token **token_list, int i, int status_code)
{
    int start;
    char *token;
    int single_quotes = 0;
    int double_quotes = 0;

    while (input[i])
    {
        while (input[i] && ft_isspace(input[i]))
            i++;

        // Yönlendirme operatörlerini kontrol et
        if (input[i] == '>' || input[i] == '<')
        {
            start = i;
            // '>>' veya '<<' olup olmadığını kontrol et
            if (input[i] == input[i + 1])
                i++;  // Eğer çift yönlendirme (>>) veya (<<) ise, bir karakter ileri git
            token = ft_substr(input, start, i - start + 1);
            if (!token)
            {
                printf("Error: Memory allocation failed\n");
                return;
            }
            ft_trim_and_add_token(token_list, token, status_code);  // Operatörü token olarak ekle
        }
        else if (input[i] == '"' || input[i] == '\'')  // Tırnakları işleme
        {
            token = ft_handle_quotes(input, &i);
            if (!token)
            {
                printf("Error: Memory allocation failed\n");
                return;
            }
            ft_trim_and_add_token(token_list, token, status_code);
        }
        // Normal kelime işleme
        else
        {
            start = i;
            token = ft_handle_word(input, &i, start);  // Kelimeyi işleyen fonksiyon
            if (!token)
            {
                printf("Error: Memory allocation failed\n");
                return;
            }
            ft_trim_and_add_token(token_list, token, status_code);
        }
    }
}

