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

gnome-terminal -x sh -c "./webserv ./config_files/basic_file.conf"

# echo -e "\n${RED}ATTACK GET - PORT 9000 ${NC}"
# echo "GET http://localhost:9000/" | stress_test/vegeta attack -duration=2s | tee results.bin | stress_test/vegeta report
echo -e "\n${RED}ATTACK GET - PORT 9000 ${NC}"
echo "GET http://localhost:9000/basic.html" | stress_test/vegeta attack -duration=5s | tee results.bin | stress_test/vegeta report
echo -e "\n${RED}ATTACK GET - PORT 8000 ${NC}"
echo "GET http://localhost:8000/" | stress_test/vegeta attack -duration=2s | tee results.bin | stress_test/vegeta report
echo -e "\n${BLUE}SIMULTANEOUS ATTACK GET - PORT 9000 & 8000${NC}"
stress_test/vegeta attack -duration=5s -targets=stress_test/target.list | tee results.bin | stress_test/vegeta report

gnome-terminal -x sh -c "./webserv ./config_files/pierre_basic_file.conf"
echo -e "\n${RED}ATTACK POST - PORT 9042 ${NC}"
echo "POST http://localhost:7000/" | stress_test/vegeta attack -duration=10s | tee results.bin | stress_test/vegeta report
