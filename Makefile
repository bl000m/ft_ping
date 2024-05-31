MY_SOURCES = src/main.c \
             src/parsing.c \
             src/printing.c \
             src/raw_socket.c \
             src/icmp_packet.c \
             src/signals.c \
             src/init.c \
             src/utils.c

OBJ_DIR = obj

MY_OBJECTS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(MY_SOURCES))

INCLUDES = include/

NAME = ft_ping

FLAGS = -g -Wall -Wextra -Werror -I$(INCLUDES)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@echo "Compiling $(NAME): $@" > $(OBJ_DIR)/compile.log
	@gcc $(FLAGS) -c $< -o $@
	@rm -f $(OBJ_DIR)/compile.log

$(NAME): $(MY_OBJECTS)
	@gcc $(FLAGS) $(MY_OBJECTS) -o $(NAME) -lm
	@echo "$(NAME) compiled"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files cleaned."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

re: fclean all

# Install target with setuid bit
install: $(NAME)
	@echo "Installing $(NAME) to /usr/local/bin with setuid bit set"
	@sudo cp $(NAME) /usr/local/bin/
	@sudo chown root:root /usr/local/bin/$(NAME)
	@sudo chmod 4755 /usr/local/bin/$(NAME)
	@echo "$(NAME) installed with setuid bit set"

.PHONY: all clean fclean re install
