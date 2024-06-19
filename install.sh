#!/bin/bash
sudo apt-get update
sudo apt-get install -y libcurlpp-dev libcurl4-openssl-dev libjsoncpp-dev wiringpi

git clone https://github.com/WiringPi/WiringPi.git

cd WiringPi
./build

cd ..
rm -rf WiringPi

echo "Installation completed successfully."
