# Neptune OS
Neptune OS is a fake Operating System.

I call it a fake os because it has the same structure as an os. It has a bootloader and kernel.

Bootl is the boot file for linux and bootw is the boot file for windows. I dont know if bootw works though.

# How to use
## Linux
### Normal
Download the latest release and run it (lol)

### Compiling
Download the source and then install gcc and nasm and then go into your terminal and run `make run` in the directory you put neptune in.

If it doesnt work then install makefile I think it comes preinstalled with linux tho.

## Windows/Mac
Windows and mac are not supported and will almost definitely never be.

This is just a simple hobby project I am making for fun. I dont wanna bother myself with that stuff.

# Makefile
Basically the makefile has 3 options.

- Run
- Build
- Clean

Run well it runs Neptune OS and build compiles the os without running it and clean deletes all the .o files and stuff.

To use the makefile just run `make [option]` also make sure you have makefile installed.