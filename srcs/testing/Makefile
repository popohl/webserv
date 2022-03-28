SHELL = /bin/sh

CXX = c++
CXXFLAGS = -Wall -Wextra

NAME = gtests

INCLUDE = -I../../incs/ -Igtest_incs/ -pthread
LIBGTEST = gtest_incs/libgtest.a

CXXFLAGS += $(INCLUDE)

MAIN = gtest_main.cpp

FILES = lexerTests astTests parserTests rulesTests requests_gtests

WEBSERV_OBJS_PATH = ../../objs/

WEBSERV_DIR = configParsing configParsing/AST requests

WEBSERV_PATH = $(addprefix $(WEBSERV_OBJS_PATH), $(WEBSERV_DIR))
# WEBSERV_OBJS = $(wildcard $(foreach $(WEBSERV_PATH)/*.o) #taking all the objects without the main
WEBSERV_OBJS = $(foreach dir, $(WEBSERV_PATH) ,$(wildcard $(dir)/*.o))

SRCS = $(addsuffix .cpp, $(FILES))
OBJS = $(addsuffix .o, $(FILES))

WEBSERV_GOOD = $(filter-out $(WEBSERV_PATH), $(WEBSERV_OBJS))
all: $(NAME)
	./$(NAME)

$(NAME): $(OBJS)
	@# echo $(WEBSERV_OBJS)
	@# echo $(WEBSERV_GOOD)
	@echo "Linking $(NAME)"
	@$(CXX) $(CXXFLAGS) $(MAIN) $(WEBSERV_GOOD) $(OBJS) $(LIBGTEST) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
