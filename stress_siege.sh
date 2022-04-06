#!/bin/bash

# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37

BLUE='\033[0;34m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "server {
	   listen :7000;
	   server_name localhost;
	   index	doesnotexist index.html;

	   location / {
		  root ./www;
	   }
	   error_page 404 ./404.html;
	   error_page 403 ./403.html;
}
" > file_temp_1649785.conf

gnome-terminal -x sh -c "./webserv ./file_temp_1649785.conf"
siege -b --time=10s  http://localhost:7000/
echo -e "\n${RED}ATTACK GET - PORT 7000 ${NC}"
sleep 10
siege -b --time=10s  http://localhost:7000/huge.png
echo -e "\n${RED}ATTACK GET - HUGE PICTURE ${NC}"

rm file_temp_1649785.conf;
