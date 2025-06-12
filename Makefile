########## COLORS ##########
GREEN := \033[32m
RED := \033[31m
BLUE := \033[34m
YELLOW := \033[33m
RESET := \033[0m
BOLD := \033[1m

########## FLAGS ##########
CC := clang
CFLAGS := -Wall -Wextra -Werror \
		-MMD -MP -Wshadow \
		-fno-omit-frame-pointer -g3 -O0 \
		-fPIC -fvisibility=hidden \
		-D_GNU_SOURCE
LDFLAGS := -ldl
INCLUDES := -Iincludes

########## FILES ##########
SRC_DIR := srcs
OBJ_DIR := objs
SRC_FILES := $(SRC_DIR)/mallocsanitize.c
			 
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
NAME := mallocsanitizer.so
########## RULES ##########
all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CC) -shared -o $@ $^ $(LDFLAGS)
	@printf "$(GREEN)$(BOLD)[✓] $@ compiled successfully!$(RESET)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(BLUE)Compiling $(BOLD)$<$(RESET)\n"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(RED)[x] Object files cleaned.$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)[x] Shared library $(NAME) removed.$(RESET)\n"

re: fclean all

.PHONY: all clean fclean test re

-include $(OBJ_FILES:.o=.d)
