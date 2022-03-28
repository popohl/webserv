# Source files directory
S		= srcs/
# Object files directory
O		= objs/
# Include files directory
I		= incs/
# Dependency files directory
D		= deps/

# Name of your program
NAME	= webserv

# list of your source files
SRCS	= cgi/cgi.cpp cgi/fork.cpp cgi/pipe.cpp configParsing/AST/ConfigFileNode.cpp \
		  configParsing/AST/LocationRules.cpp \
		  configParsing/AST/ServerNode.cpp configParsing/AST/ServerRules.cpp \
		  configParsing/Lexer.cpp configParsing/Parser.cpp \
		  configParsing/Rules.cpp configParsing/Token.cpp \
		  configParsing/helperFunctions.cpp main.cpp

# Compiler
CC		= c++
# Compiler flags
CFLAGS	+= -Wall -Wextra -g -std=c++98
# Linker flags
LDFLAGS	=

# Run command
RUN_COMMAND	= # For the moment, make run is alias to make test

# The rest is automatic

CFLAGS	+= $(addprefix -I,$I)

SRCS	:= $(addprefix $S,$(SRCS))
OBJS	:= $(SRCS:$S%=$O%.o)
DEPS	:= $(SRCS:$S%=$D%.d)
DFLAGS	= -MT $(@:$O%.o=$S%) -MMD -MP -MF $(@:$O%.o=$D%.d)

RM		= /bin/rm -rf

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)"
	@$(CC) $^ -o $@

# Object directory generation
$O:
	@mkdir $O

# Dependency directory generation
$D:
	@mkdir $D

# Object files generation
$(OBJS): $O%.o:$S% | $O $D
	@mkdir -p $(@D)
	@mkdir -p $(@D:$O%=$D%)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) $(DFLAGS) -c $(@:$O%.o=$S%) -o $@

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@$(RM) $D $O

.PHONY: fclean
fclean: clean
	@echo "Everything!"
	@$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: run
run: test
	$(RUN_COMMAND)

valgrind: $(NAME)
	valgrind --leak-check=full $(RUN_COMMAND)

test: all
	-@make -C srcs/testing all #- before a command allows make to ignore error code
	# @make -C srcs/testing fclean

