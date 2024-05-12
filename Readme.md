# Asaphus Coding Challenge

## Project Description

This project implements the mechanics of a simple game involving green and blue boxes that absorb tokens of different weights and calculate scores based on specific rules. The implementation includes test cases to verify the correctness of game mechanics using the Catch v2.13.9 test framework.

## Prerequisites

To build and run this project, you will need:
- C++14 compliant compiler such as GCC, Clang, or MSVC
- CMake version 3.10 or higher (optional, for building with CMake)

## Repository Setup

Clone the repository to your local machine using:
```bash
git clone https://github.com/Iamminal/asaphus_coding_challenge.git
```

## Building the Project

You can build the project using CMake or directly through the command line if you prefer not to use CMake.

## Using CMake

### Navigate to the project directory:
Navigate to the directory where you have the project files:

```bash
cd path/to/asaphus_coding_challenge
```

### Set up a build directory to hold the compilation output:

```bash
mkdir build
cd build
```
### Generate the build configuration:
Run CMake to configure the build system:

```bash
cmake ..
```
### Compile the project:
Build the project using the generated Makefile or build system configuration:

```bash
cmake --build .
```
### Without CMake
If you choose not to use CMake, compile the project directly from the command line:

```bash
g++ --std=c++14 ../asaphus_coding_challenge.cpp -o challenge
```

### Running Tests
After successfully building the project, you can run the executable to execute all tests:

```bash
./challenge
```
This command runs the Catch2 test cases defined in the asaphus_coding_challenge.cpp file.

## Game Mechanics
Box Types: Green and blue, each with unique scoring rules.
Token Absorption: Boxes absorb tokens altering their weights and scores.
Players: Two players compete by choosing boxes to absorb tokens in turn.
End Condition: The game ends when all tokens are absorbed, and the player with the highest score wins.