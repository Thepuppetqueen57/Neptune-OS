./build
mkdir Neptune
cp ./out/Neptune-OS ./Neptune
cp -r ./config ./Neptune

# Making the docker image
sudo docker build -t neptune ./
sudo docker save -o ./Neptune/neptune-docker.tar neptune:latest