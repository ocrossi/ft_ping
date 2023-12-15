NAME = ft_ping

# les routes
SRC_PATH	= sources
OBJ_PATH	= objects
INC_PATH	= includes
LIB_PATH	= libraries

# nom des fichier code source (*.c)
NAME_SRCS	= $(shell cd $(SRC_PATH); ls *.c)
NAME_OBJS	= $(NAME_SRCS:.c=.o)
NAME_LIB	= $(shell cd $(LIB_PATH); ls *.a)


#les sources
SRCS		= $(addprefix $(SRC_PATH)/,$(NAME_SRCS))
HEADERS		= $(INC_PATH)/$(NAME).h

# les objects
OBJS		= $(addprefix $(OBJ_PATH)/,$(NAME_OBJS))

#compilateur + flags
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror

#librairies
#LIBFT				= $(LIB_PATH)/libft.a

all: libft $(NAME)
	@echo "\033[48;5;22m\033[38;5;15m ft_ping compiles \033[0m"

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -L $(LIB_PATH) -I $(INC_PATH) $^ -o $@ $(LIB_PATH)/$(NAME_LIB)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(HEADERS)
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) -L $(LIB_PATH) -I $(INC_PATH) -c $< -o $@ 

libft:
	@make -C $(LIB_PATH)


clean:
	@rm -f $(OBJS)
	@make -C $(LIB_PATH) clean
	@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean: clean
	@make -C $(LIB_PATH) fclean
	@rm -f $(NAME)

re: fclean all
