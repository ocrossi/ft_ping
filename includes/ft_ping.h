#ifndef FT_PING_H
# define FT_PING_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>


#include <netdb.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "../libraries/libft.h"
//#include "libft.h"

#define CPT_FLAGS 2

typedef struct 	s_pingData {
		bool options[CPT_FLAGS];
		char *destination;	
}				t_pingData;

#endif
