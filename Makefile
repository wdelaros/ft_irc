#------------------------------------------------------------------------------#
#                                VARIABLES                                     #
#------------------------------------------------------------------------------#

NAME		=	ircserv

# Style
BOLD		=	\033[1m
ITALIC		=	\033[3m
UNDERLINE	=	\033[4m

# Color text
RESET		=	\033[0m
BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
PURPLE		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
DEFAULT		=	\033[39m
ERASE_LINE	=	\033[2K\r

# Compiler and flags
CC			=	c++
CFLAGS		=	-std=c++98
RM			=	rm -rf

# Sources are all .cpp files
SRCS		=	main.cpp \
				Server.cpp \
				User.cpp \

OBJS_DIR	=	obj/
OBJS_LST	=	$(patsubst %.cpp, %.o, $(SRCS))
OBJS		=	$(addprefix $(OBJS_DIR), $(OBJS_LST))

#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: dir $(NAME)

# Generates output file
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(ERASE_LINE)$(GREEN)✔️ $(ITALIC)$(NAME) successfully compile.$(RESET)\
	$(GREEN) ✔️ $(RESET)"
	@printf "$(ITALIC)	Your ip is: "
	@hostname -I

# Compiles sources into objects
$(OBJS_DIR)%.o: src/%.cpp
	@printf "$(ERASE_LINE)⚙️ $(PURPLE)Compiling $(DEFAULT)⚙️$(YELLOW) $(notdir $<)\
	$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# create objects directory
dir:
	@mkdir -p $(OBJS_DIR)

# Removes objects
clean:
	@printf "💣 $(RED)Removing $(NAME) objects...$(RESET) 💥\n"
	@$(RM) $(OBJS_DIR)

# Removes objects and executables
fclean: clean
	@printf "💣 $(RED)Removing $(NAME) executable$(RESET) 💥\n"
	@$(RM) $(NAME)

# Removes objects and executables and remakes
re: fclean all
	@echo "$(ERASE_LINE)$(GREEN)✔️ $(ITALIC)Remake complete$(RESET)\
	$(GREEN) ✔️$(RESET)"

ip:
	@printf "$(ITALIC)	Your ip is: "
	@hostname -I
