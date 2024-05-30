# Source files
MY_SOURCES = src/main.c \
             src/parsing.c \
             src/printing.c \
             src/raw_socket.c \
             src/icmp_packet.c \
             src/signals.c \
             src/init.c \
             src/utils.c

# Object directory
OBJ_DIR = obj

# Object files
MY_OBJECTS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(MY_SOURCES))

# Include directory for header files
INCLUDES = include/

# Output executable name
NAME = ft_ping

# Compilation flags
FLAGS = -g -Wall -Wextra -Werror -I$(INCLUDES)

# Default target
all: $(NAME)

# Rule to create the object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Rule to compile .c files to .o files
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@gcc $(FLAGS) -c $< -o $@
	@echo "Compiling $(NAME): $@"

# Rule to link object files into the final executable
$(NAME): $(MY_OBJECTS)
	@gcc $(FLAGS) $(MY_OBJECTS) -o $(NAME) -lm
	@echo "$(NAME) compiled"

# Rule to clean up object files
clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files cleaned."

# Rule to clean up all generated files
fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

# Rule to recompile everything
re: fclean all

# Install target with setuid bit
install: $(NAME)
	@echo "Installing $(NAME) to /usr/local/bin with setuid bit set"
	@sudo cp $(NAME) /usr/local/bin/
	@sudo chown root:root /usr/local/bin/$(NAME)
	@sudo chmod 4755 /usr/local/bin/$(NAME)
	@echo "$(NAME) installed with setuid bit set"

.PHONY: all clean fclean re install
