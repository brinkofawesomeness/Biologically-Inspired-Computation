/* 
 * Conner Brinkley
 * April 29, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 4: Genetic Algorithms
 * 
 * USAGE> ./generateData
 * 
 * This program generates data for the genetic algorithm simulator by 
 * storing an array of parameters and executing the genetic algorithm 
 * with each of them one by one.
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
    string commands[14] = {
        
        // Control test case
        "20 30 10 0.033 0.6",

        // Test genetic string length parameter
        "5 30 10 0.033 0.6",
        "10 30 10 0.033 0.6",
        "30 30 10 0.033 0.6",
        "40 30 10 0.033 0.6",

        // Test population size parameter
        "20 75 10 0.0133 0.6",
        "20 200 10 0.005 0.6",
        "20 1000 10 0.001 0.6",

        // Test number of generations parameter
        "20 30 20 0.033 0.6",
        "20 30 50 0.033 0.6",
        "20 30 100 0.033 0.6",

        // Test crossover probability parameter
        "20 30 10 0.033 0.1",
        "20 30 10 0.033 0.33",
        "20 30 10 0.033 0.99"
    };

    // Iterate through each case and pass it to the PSO simulator
    for (int i = 0; i < 14; i++) {
        sprintf(command, "./geneticAlgorithm %d %s", i+1, commands[i].c_str());
        cout << command << endl;
        system(command);
    }

    return 0;
}