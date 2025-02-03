# Neptune OS
Neptune OS is a fake Operating System.

I call it a fake os because it has the same structure as an os. It has a bootloader and kernel.

# How to use
## Linux
### Normal
Download the latest release and run it (lol)

### Compiling
Download the source and then install gcc and nasm and then go into your terminal and run `./build` in the directory you put neptune in.

If it doesnt work then install cmake I think it comes preinstalled with linux tho.

## Windows/Mac
Windows and mac are not supported and will almost definitely never be.

This is just a simple hobby project I am making for fun. I dont wanna bother myself with that stuff.

Although theres a chance it already works but me and my friend who is on windows just havent figured out how to get it to compile correctly yet.

## Docker
To run on docker you need to run `sudo docker build -t neptune ./`

Then run `sudo docker run -it neptune`

The -it is very important because if you dont put it there well..

Input handling gets a little silly. Thats all I'm gonna say.

I plan on adding a docker image to every new release (if thats even possible)

Cuz its kinda pointless if you have to make it yourself

# Credits
 Thanks to [my friend Shayna](https://github.com/PsychedelicShayna) for making the library used in the calculator
