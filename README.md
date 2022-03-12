# webserv
webserv project at 42 school

## How to run
To run normally, run `make` and start `./webserv` with a config file provided in
`./config_files/`, there is a shortcut to this: `make run`
To run in test mode, run `make --eval="CFLAGS += -D TEST" && ./webserv`

## Infos

**Parser:** https://ruslanspivak.com/lsbasi-part1/

**HTTP:** https://beej.us/guide/bgnet/html/
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa 

Check my BNF for config files and parser structure in `./research/`

Check out my [Notion page for the project](https://pohl.notion.site/Parser-060000caaff047509900210372d06f26)
