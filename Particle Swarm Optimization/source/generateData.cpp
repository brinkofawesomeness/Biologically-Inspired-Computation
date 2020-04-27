/* 
 * Conner Brinkley
 * April 24, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 5: Particle Swarm Optimization
 * 
 * USAGE> ./generateData
 * 
 * This program generates data for the Particle Swarm Optimization (PSO) 
 * algorithm simulator by storing an array of parameters and executing the 
 * PSO with each of them one by one.
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
    string commands[16] = {

        // -------- PROBLEM 1 ---------
        
        // Testing number of particles
        "300 30 0.6 2 2",
        "300 100 0.6 2 2",
        "300 200 0.6 2 2",
        
        // Testing inertia
        "300 30 0.9 2 2",
        
        // Testing cognition param
        "300 30 0.6 1 2",
        "300 30 0.6 3 2",

        // Testing social param
        "300 30 0.6 2 1",
        "300 30 0.6 2 3",

        // -------- PROBLEM 2 ---------

        // Testing number of particles
        "300 30 0.6 2 2 2",
        "300 100 0.6 2 2 2",
        "300 200 0.6 2 2 2",
        
        // Testing inertia
        "300 30 0.9 2 2 2",
        
        // Testing cognition param
        "300 30 0.6 1 2 2",
        "300 30 0.6 3 2 2",

        // Testing social param
        "300 30 0.6 2 1 2",
        "300 30 0.6 2 3 2"
    };

    // Iterate through each case and pass it to the PSO simulator
    for (int i = 0; i < 16; i++) {
        sprintf(command, "./pso %s", commands[i].c_str());
        system(command);
    }

    return 0;
}