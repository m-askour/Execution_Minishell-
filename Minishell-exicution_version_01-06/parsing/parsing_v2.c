
#include "../minishell.h"


char *check_export_syntax(char *str)
{
    int i = 0;
    char quote = 0;

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && (quote == 0))
            quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        if (!quote)
        {
            if (str[0] == '|'
                || (str[i - 1] == '|' && str[i] == '|'))
                return (write( 2 , "syntax error near unexpected token `|'\n", 40), NULL);
            else if (str[i - 1] == ';' && str[i] == ';')
                return (write(2, "syntax error near unexpected token `;;'\n",41), NULL);
            else if (str[i] == '\\')
                return (write(2, "syntax error near unexpected token `\\'\n",40), NULL);
            else if (str[i] == ';')
                return (write(2, "syntax error near unexpected token `;'\n",40), NULL);
            else if (str[i] == '!')
                return (write(2, "syntax error near unexpected token `!'\n",40), NULL);
            else if (str[i] == '&')
                return (write(2, "syntax error near unexpected token `&'\n",40), NULL);
            else if (str[i - 1] == ':' && str[i] == ':')
                return (write(2, "syntax error near unexpected token `::'\n",41), NULL);
            else if (str[i] == '(' || str[i] == ')')
                return (write(2, "syntax error near unexpected token `()'\n",41), NULL);
        }
        i++;
    }
    return (str);
}
