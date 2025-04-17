#include "minishell.h"

// Yeni ortam değişkenlerini oluşturup bağlı listeye ekler
void ft_create_env(t_program *prgm, char **env, t_environment **env_list)
{
    int i;
    char *key;
    char *value;
    char *delimiter;
    t_environment *new_env;

    i = 0;
    while (env[i])
    {
        delimiter = ft_strchr(env[i], '=');
        if (delimiter)
        {
            key = ft_strndup(env[i], delimiter - env[i]);
            value = ft_strdup(delimiter + 1);
            if (!key || !value)
            {
                free(key);
                free(value);
                perror("malloc");
                prgm->status_code = 1;
                return;
            }
            new_env = (t_environment *)malloc(sizeof(t_environment));
            if (!new_env)
            {
                free(key);
                free(value);
                perror("malloc");
                prgm->status_code = 1;
                return;
            }
            new_env->variable = key;
            new_env->value = value;
            new_env->next = NULL;

            // Listenin sonuna ekleme
            if (*env_list == NULL)
                *env_list = new_env;
            else
            {
                t_environment *last = *env_list;
                while (last->next)
                    last = last->next;
                last->next = new_env;
            }
        }
        i++;
    }
}

// Ortam değişkenlerinin listesini tersine çevirerek yazdır
void print_env_reverse(t_environment *env_list)
{
    t_environment *prev = NULL;
    t_environment *curr = env_list;
    t_environment *next = NULL;

    // Listeyi tersine çevir
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    // Listeyi yazdır
    while (prev)
    {
        if (prev->value)
            printf("%s=%s\n", prev->variable, prev->value);
        else
            printf("%s\n", prev->variable);
        prev = prev->next;
    }
}

// Ortam değişkenlerini yazdıran ana fonksiyon
void ft_env(t_program *prgm)
{
    if (prgm->env_list)
    {
        print_env_reverse(prgm->env_list);
    }
    else
    {
        write(1, "Environment list is empty.\n", 26);
    }
}

// Ortam değişkenlerinin listesini serbest bırak
void free_env_list(t_environment *env_list)
{
    t_environment *temp;

    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        free(temp->variable);
        free(temp->value);
        free(temp);
    }
}
