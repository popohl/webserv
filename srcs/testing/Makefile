SHELL = /bin/sh

CXX = clang++
CXXFLAGS = -Wall -Wextra

NAME = gtests

INCLUDE = -I../../incs/ -Igtest_incs/ -pthread
LIBGTEST = gtest_incs/libgtest.a

CXXFLAGS += $(INCLUDE)

MAIN = gtest_main.cpp

FILES = lexer_gtests requests_gtests

WEBSERV_OBJS_PATH = ../../objs/

WEBSERV_DIR = config_parsing requests
WEBSERV_PATH = $(addprefix $(WEBSERV_OBJS_PATH), $(WEBSERV_DIR))
WEBSERV_OBJS = $(foreach dir, $(WEBSERV_PATH) ,$(wildcard $(dir)/*.o))
SRCS = $(addsuffix .cpp, $(FILES))
OBJS = $(addsuffix .o, $(FILES))


all: $(NAME)
	./$(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(MAIN) $(WEBSERV_OBJS) $(OBJS) $(LIBGTEST) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
