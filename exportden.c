#include "minishell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// export argümanlarını birleştirme fonksiyonu (sadece export için)

char *ft_quotes_clean(char *str)
{
	int i = 0, j = 0;
	char quote = 0;
	char *result = malloc(strlen(str) + 1);

	if (!str || !result)
		return (NULL);

	while (str[i])
	{
		// Eğer açılmamış bir tırnaksa
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
		{
			quote = str[i]; // tırnak aç
			i++;
			continue;
		}
		// Eğer tırnak kapanıyorsa
		else if (quote != 0 && str[i] == quote)
		{
			quote = 0; // tırnak kapat
			i++;
			continue;
		}
		// Normal karakterleri yaz
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return result;
}




int main()
{
    char *input1 = "\"\"a=a\"\"";
    char *input2 = "c=\"a\"a\"";
    char *input3 = "\"e\"\"e\"";
    char *input4 = "c=\'a\"a\'";   // Sıkıntılı örnek
    char *input5 = "c=\"a\'a\"";   // Karışık tırnak

    printf("%s\n", ft_quotes_clean(input1)); // a=a
    printf("%s\n", ft_quotes_clean(input2)); // c=a"a
    printf("%s\n", ft_quotes_clean(input3)); // ee
    printf("%s\n", ft_quotes_clean(input4)); // c=a"a
    printf("%s\n", ft_quotes_clean(input5)); // c=a'a

    return 0;
}