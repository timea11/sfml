#!/bin/bash

g++ box_main.cpp box.cpp box_map.cpp -o box -lsfml-graphics -lsfml-window -lsfml-system

./box

