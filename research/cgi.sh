#!/bin/bash

# export urlReceived="/cgi-bin/ubuntu_cgi_tester/mnt/nfs/homes/pohl/Documents/tmp"

export REQUEST_METHOD="POST"
export SERVER_PROTOCOL="HTTP/1.1"
export PATH_INFO="/"

./ubuntu_cgi_tester	./config.bnf
