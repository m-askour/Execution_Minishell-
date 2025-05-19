
#include "../../minishell.h"

void	print_error(t_token *head, char *val)
{
	ft_putstr_fd("minishell: syntax error near unexpected token`'\n", 2, 0);
	if (head)
		free_tokens(head, NULL);
	if (val)
		free(val);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	ft_intlen(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	long	nb = (long)n;

	len = ft_intlen(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nb == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[--len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}
