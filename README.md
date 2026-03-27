# Neptune OS
Neptune OS is a fake Operating System.

I call it a fake os because it has the same structure as an os. It has a bootloader and kernel.

# How to use
## Linux/Mac
### Normal
Download the latest release and run it (lol)

There are not builds for mac so you will have to compile it yourself

### Compiling
Download the source and then install gcc and then go into your terminal and run `make run` in the directory you put neptune in.

If it doesnt work then install makefile I think it comes preinstalled with linux tho.

## Windows
### Normal
Same as Linux

### Compiling
Mostly the same as linux but you need to make sure you run the makefile in git bssh (or something similar)

# Makefile
Basically the makefile has 3 options.

- Run
- Build
- Clean

Run well it runs Neptune OS and build compiles the os without running it and clean deletes all the .o files and stuff.

To use the makefile just run `make [option]` also make sure you have makefile installed.
