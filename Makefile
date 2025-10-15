CC := g++
CCF := -std=c++17 -O1
SOURCE := main.cpp
OUTPUT := exe

compile:
    $(CC) $(CCF) $(SOURCE) -o $(OUTPUT)
