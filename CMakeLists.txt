cmake_minimum_required(VERSION 3.10)
project(ScrabbleJunior)

set(CMAKE_CXX_STANDARD 14)

add_executable(ScrabbleJunior main.cpp Board.h Board.cpp Player.h Player.cpp Pool.h Pool.cpp Colors.h LettersBag.h LettersBag.cpp MenuIO.cpp MenuIO.h)