# Source files
MY_SOURCES = 	src/main.c \
				src/parsing.c \
				src/printing.c \
				src/raw_socket.c \
             # src/ft_utils.c \
             # src/ft_init.c \
             
             # src/ft_print.c

# Object files
MY_OBJECTS = $(MY_SOURCES:.c=.o)

# Include directory for header files
INCLUDES = include/

# Output executable name
NAME = ft_ping

# Compilation flags
FLAGS = -g -Wall -Wextra -Werror -I $(INCLUDES)

# Default target
all: $(NAME)

# Rule to compile .c files to .o files
.c.o:
	@gcc $(FLAGS) -c $< -o $@
	@echo "Compiling $(NAME): $@"

# Rule to link object files into the final executable
$(NAME): $(MY_OBJECTS)
	@gcc $(FLAGS) $(MY_OBJECTS) -o $(NAME)
	@echo "$(NAME) compiled"

# Rule to clean up object files
clean:
	@rm -f $(MY_OBJECTS)
	@echo ".o files cleaned."

# Rule to clean up all generated files
fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

# Rule to recompile everything
re: fclean all

# Mark these targets as not real files
.PHONY: all clean fclean re
