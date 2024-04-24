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
ifndef LINUX
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g
else
CFLAGS		=	-Wall -Wextra -Werror -std=c++11 -g
endif
RM			=	rm -rf

# Sources are all .cpp files
SRCS		=	main.cpp \
				Server.cpp \
				User.cpp \
				CommandHandler.cpp \
				Command.cpp \
				Pass.cpp \
				Nickname.cpp \
				Username.cpp \
				Ping.cpp \
				Channel.cpp \
				PrivMsg.cpp \
				Join.cpp \
				Mode.cpp \
				Topic.cpp \
				Part.cpp \
				Kick.cpp \
				Invite.cpp \
				List.cpp \

OBJS_DIR	=	obj/
OBJS_LST	=	$(patsubst %.cpp, %.o, $(SRCS))
OBJS		=	$(addprefix $(OBJS_DIR), $(OBJS_LST))

#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: dir $(NAME)

linux:
	@make -s LINUX=1 all iplinux

# Generates output file
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(ERASE_LINE)$(GREEN)✔️ $(ITALIC)$(NAME) successfully compile.$(RESET)\
	$(GREEN) ✔️ $(RESET)"

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

relinux: fclean linux
	@echo "$(ERASE_LINE)$(GREEN)✔️ $(ITALIC)Remake complete$(RESET)\
	$(GREEN) ✔️$(RESET)"

ip:
	@printf "$(GREEN) $(ITALIC) Your adress ip is "
	@ipconfig getifaddr en0
	@printf "$(RESET)"

iplinux:
	@printf "$(GREEN) $(ITALIC) Your adress ip is "
	@hostname -I
	@printf "$(RESET)"

meurt:
	@echo "🦃🍎🥓🧀🐿️"
