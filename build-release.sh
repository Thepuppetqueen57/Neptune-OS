#!/bin/usr/env bash

# variables
RESET = \e[0m
GREEN = \e[92m

# main
echo "########################################################"
echo "#   build_release.sh - contributed by naturelessGit    #"
echo "########################################################"
echo "[i] Running build.. (verbose: ./build)"
./build
echo "[i] Creating directory \"Neptune\"... (verbose: mkdir Neptune)"
mkdir Neptune
echo "[i] Copying files... (verbose: cp ./out/Neptune-OS ./Neptune && cp -r ./config ./Neptune)"
cp ./out/Neptune-OS ./Neptune
cp -r ./config ./Neptune

# Making the docker image
echo "[i] Making the docker image... (verbose: sudo docker build -t neptune ./ && sudo docker save -o ./Neptune/neptune-docker.tar neptune:latest)"
sudo docker build -t neptune ./
sudo docker save -o ./Neptune/neptune-docker.tar neptune:latest

echo "${GREEN}[+] Script done!${RESET}"
