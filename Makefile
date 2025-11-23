NAME			= $(BUILD_PATH)/cub3D
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -Wno-error=cast-function-type
BUILD_PATH		?= ./build

DEPENDENCIES		= libft libftprintf libftsscanf libgc libgnl
DEPENDENCIES_DIR	= ./dependencies
DEPENDENCIES_FLAGS	= $(foreach lib,$(DEPENDENCIES),-L$(BUILD_PATH)/$(lib)) \
						$(foreach lib,$(patsubst lib%,%, $(DEPENDENCIES)),-l$(lib)) \
						-L$(DEPENDENCIES_DIR)/minilibx-linux -lmlx -lXext -lX11 -lm
INCLUDE_FLAGS		= -Iinclude $(foreach lib,$(DEPENDENCIES),-Iinclude/$(lib))
MINI_LIBX			= $(DEPENDENCIES_DIR)/minilibx-linux/libmlx_Linux.a

SRCS			=	src/main.c \
					src/player.c \
					src/rendering.c \
					src/raycasting.c \
					src/timing.c \
					src/parsing/parsing.c \
					src/parsing/utils_parsing.c \
					src/parsing/utils_parsing2.c \
					src/parsing/map_parsing.c \
					src/parsing/map_reading.c \
					src/parsing/map_validation.c \
					src/parsing/map_validation2.c \
					src/parsing/map_utils.c
OBJS			= $(SRCS:src/%.c=$(BUILD_PATH)/obj/%.o)
HEADERS			= include/cub3d.h

all: $(NAME)

$(NAME): $(OBJS) $(foreach lib,$(DEPENDENCIES),$(BUILD_PATH)/$(lib)/$(lib).a) $(MINI_LIBX) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) $(DEPENDENCIES_FLAGS) $(INCLUDE_FLAGS) -o $(NAME)

$(foreach lib,$(DEPENDENCIES),$(BUILD_PATH)/$(lib)/$(lib).a):
	$(MAKE) -C $(DEPENDENCIES_DIR)/$(basename $(@F)) BUILD_PATH=../../$(BUILD_PATH)/$(basename $(@F)) all

$(BUILD_PATH)/obj/%.o: src/%.c $(HEADERS)
	@mkdir -p $(BUILD_PATH)/obj/parsing
	$(CC) $(CFLAGS) $(DEPENDENCIES_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(MINI_LIBX):
	$(MAKE) -C $(DEPENDENCIES_DIR)/minilibx-linux CC=gcc-14

clean:
	@rm -rf $(BUILD_PATH)/obj
	@rm -rf $(BUILD_PATH)/lib*/obj
	$(MAKE) -C $(DEPENDENCIES_DIR)/minilibx-linux clean

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(BUILD_PATH)

re: fclean all

.PHONY: all clean re fclean
