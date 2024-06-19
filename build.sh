#!/bin/bash
g++ -g main.cpp -o launch -lcurlpp -ldl -lcurl -lpthread -lwiringPi -ljsoncpp -lwiringPi
echo "Build completed successfully."