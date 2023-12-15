#include "libft.h"

double get_floating_part(char *s) {
    double res;

    res = (double)ft_atoi(s);
	for (size_t i = 0; i < ft_strlen(s); i++) {
		res /= 10;
	}
    return res;
}

double ft_atolf(char *s) {
	int int_portion;
	double res;
	char** s_arr;
    int neg = 1;

    if (s[0] == '.') {
		return get_floating_part(&s[1]);
    }
	s_arr = ft_strsplit(s, '.');
    if (ft_strchr(s, '.') == NULL || s_arr[1] ==  NULL) {
		return (double)ft_atoi(s);
    }
	int_portion = ft_atoi(s_arr[0]);
    if (int_portion < 0) {
		neg = -1;
		int_portion *= -1;
	}
    res = get_floating_part(s_arr[1]);
	res += int_portion;

	return res * neg;
}
