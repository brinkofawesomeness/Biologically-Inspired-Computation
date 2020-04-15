/*
 * Conner Brinkley
 * February 24, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 2: Activation/Inhibition Cellular Automata
 * 
 * USAGE> ./exp3
 * This program runs 38 different test cases for the AICA simulator. The 
 * test cases were taken from the project write up.
 * 
 * */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
using namespace std;

int main() {

    // Test cases to run
    char command[512];
    string commands[38] = {
        "1 2 0",
        "1 5 -4",
        "1 5 -2",
        "1 5 0",
        "1 5 2",
        "1 5 4",
        "1 9 -6",
        "1 9 -3",
        "1 9 0",
        "1 9 3",
        "1 9 6",
        "1 14 -6",
        "1 14 -3",
        "1 14 0",
        "1 14 3",
        "1 14 6",
        "3 5 -1",
        "3 5 0",
        "3 5 1",
        "3 9 -6",
        "3 9 -3",
        "3 9 0",
        "3 9 3",
        "3 9 6",
        "3 14 -6",
        "3 14 -3",
        "3 14 0",
        "3 14 3",
        "3 14 6",
        "7 9 -1",
        "7 9 0",
        "7 9 1",
        "7 14 -3",
        "7 14 0",
        "7 14 3",
        "12 14 -2",
        "12 14 0",
        "12 14 2"
    };

    // Iterate through each case and pass it to the AICA simulator
    for (int i = 0; i < 38; i++) {
        sprintf(command, "./aica %s %d", commands[i].c_str(), i);
        system(command);
    }

    return 0;
}