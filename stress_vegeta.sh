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

tfile=$(mktemp /tmp/XXXXX.conf) #Create a temp file .conf

#listen : port you want to listen
#server_name : Name of the domain (e.g amazon.fr / amazon.fr)
#index : name of the index . If the URI is /www then it will check at /www/index.html
#location : all the rules to access files / img ...
#E.G : location / { root ./www } if you go to amazon.be you can access all the files in directory ./www
#client_max_body_size : limit the size of the client body
#allowed method : if no method allowed you only have GET

echo "server {
	   listen :9000;
	   server_name localhost;
	   index	doesnotexist index.html;
	   location / {
			allowed_method GET;
			root ./www;
	   }
#	   error_page 404 ./404.html;
}

server {
	   listen :8000;
	   server_name localhost;
	   index	doesnotexist index.html;

	   location / {
		  root ./www;
	   }
#	   error_page 404 ./404.html;
}

server {
	   listen :7000;
	   server_name localhost;
	   index	doesnotexist index.html;

	   location / {
		  allowed_method POST GET;
		  root ./www;
	   }
	   error_page 404 ./404.html;
	   error_page 403 ./403.html;
}
" > $tfile

gnome-terminal -x sh -c "./webserv $tfile"

echo -e "\n${RED}ATTACK GET - PORT 9000 ${NC}"
echo "GET http://localhost:9000" | stress_test/vegeta attack -duration=10s | tee results.bin | stress_test/vegeta report

echo -e "\n${RED}ATTACK GET - PORT 8000 ${NC}"
echo "GET http://localhost:8000/" | stress_test/vegeta attack -duration=10s| tee results.bin | stress_test/vegeta report

echo -e "\n${BLUE}SIMULTANEOUS ATTACK GET - PORT 9000 & 7000${NC}"
stress_test/vegeta attack -duration=10s -targets=stress_test/target.list | tee results.bin | stress_test/vegeta report

rm $tfile;
rm results.bin
