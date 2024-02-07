NAME = philo
SRC = main.c utils.c init.c philo.c philo_functions.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
# CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread
RM = rm -f

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all