#!/bin/bash

cd ~/Documents/SFML/box_game

g++ box_main.cpp box.cpp box_map.cpp -o box -lsfml-graphics -lsfml-window -lsfml-system

./box

