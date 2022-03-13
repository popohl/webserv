SHELL = /bin/sh

CXX = clang++
CXXFLAGS = -Wall -Wextra

NAME = gtests

INCLUDE = -I../../incs/ -Igtest_incs/ -pthread
LIBGTEST = gtest_incs/libgtest.a

CXXFLAGS += $(INCLUDE)

MAIN = gtest_main.cpp

FILES = lexer_gtests

WEBSERV_OBJS_PATH = ../../objs/

WEBSERV_DIR = parsing
WEBSERV_OBJS = $(wildcard $(addprefix $(WEBSERV_OBJS_PATH), $(WEBSERV_DIR))/*.o) #taking all the objects without the main

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

