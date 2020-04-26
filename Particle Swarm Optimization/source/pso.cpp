/* 
 * Conner Brinkley
 * April 24, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 5: Particle Swarm Optimization
 * 
 * USAGE> ./pso epochs numParticles inertia cognitionParam socialParam problemNum(optional)
 * 
 * This program simulates the particle swarm optimization algorithm...
 * 
 * */

#include <algorithm>
#include <iostream>
//#include <fstream>
#include <cstdlib>
//#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;


/* GLOBAL DATA */
vector <class Particle> population;
int problem = 1;
double xGlobalBest, yGlobalBest;
double inertia, cognition, social;
double maxVelocity = 1.0,
       width = 100.0,
       height = 100.0;


/* GLOBAL METHOD PROTOTYPES */
double fitness(double x, double y);
void printScatterPlotData(char* filename);


/* PARTICLE OBJECT */
class Particle {
    public:

        // Methods 
        Particle();
        void update();
        
        // Data
        double xpos, ypos;
        double xvelocity, yvelocity;
        double xbest, ybest;
        
};


/* PARTICLE CONSTRUCTOR */
Particle::Particle() {

    double rand1 = fmod(rand(), width) + 1.0;
    double rand2 = fmod(rand(), height) + 1.0;

    xbest = xpos = (rand1 - (width / 2.0));
    ybest = ypos = (rand2 - (height / 2.0));
    xvelocity = yvelocity = 0.0;

    cout << "xpos = " << xpos << endl;
    cout << "ypos = " << ypos << endl << endl;
}


/* UPDATE PARTICLE */
void Particle::update() {

    // Get random numbers between 0 and 1
    double rand1 = ((double) rand() / (RAND_MAX));
    double rand2 = ((double) rand() / (RAND_MAX));
    double rand3 = ((double) rand() / (RAND_MAX));
    double rand4 = ((double) rand() / (RAND_MAX));

    // Update velocity
    xvelocity = inertia * xvelocity + cognition * rand1 * (xbest - xpos) + social * rand2 * (xGlobalBest - xpos);
    yvelocity = inertia * yvelocity + cognition * rand3 * (ybest - ypos) + social * rand4 * (yGlobalBest - ypos);

    // Update position
    xpos += xvelocity;
    ypos += yvelocity;

    // Update personal best
    if (fitness(xpos, ypos) > fitness(xbest, ybest)) {
        xbest = xpos;
        ybest = ypos;
    }

    // Update global best
    if (fitness(xpos, ypos) > fitness(xGlobalBest, yGlobalBest)) {
        xGlobalBest = xpos;
        yGlobalBest = ypos;
    }
}


/* DRIVER CODE */
int main(int argc, char** argv) {

    int epochLimit;
    srand(time(NULL));

    // Read in the parameters
    switch (argc) {

        case 7: 
            problem = atoi(argv[6]);

        case 6:
            epochLimit = atoi(argv[1]);
            population.resize(atoi(argv[2]));
            inertia = atof(argv[3]);
            cognition = atof(argv[4]);
            social = atof(argv[5]);
            break;

        default:
            cout << "USAGE> ./pso epochs numParticles inertia cognitionParam socialParam problemNum(optional)" << endl;
            return -1;
    }

    // Init the global best with the best initial particle
    double curr, bestFitness = 0.0;
    for (int i = 0; i < population.size(); i++) {
        curr = fitness(population[i].xpos, population[i].ypos);
        if (curr > bestFitness) {
            xGlobalBest = population[i].xpos;
            yGlobalBest = population[i].ypos;
            bestFitness = curr;
        }
    }

    // Iterate while we are under epoch limit or error threshold
    int epoch = 0;
    double x_error, y_error;
    double threshold_error = 0.01;
    do {

        // Update the population
        for (int i = 0; i < population.size(); i++) {
            population[i].update();
        }

        // Calculate error
        x_error = y_error = 0.0;
        for (int i = 0; i < population.size(); i++) {
            x_error += pow(population[i].xpos - xGlobalBest, 2.0);
            y_error += pow(population[i].ypos - yGlobalBest, 2.0);
        }
        x_error = sqrt((1.0 / (2.0 * population.size())) * x_error);
        y_error = sqrt((1.0 / (2.0 * population.size())) * y_error);
        epoch++;

        // printScatterPlotData(filename);


    } while (epoch < epochLimit && (x_error > threshold_error || y_error > threshold_error));

    cout << "Convergence at epoch: " << epoch << endl;
    cout << "Error: X(" << x_error << ") Y(" << y_error << ")" << endl << endl;

    return 0;
}


/* FITNESS FUNCTION */
double fitness(double x, double y) {

    double mdist = ((sqrt(pow((width / 2.0), 2.0) + pow((height / 2.0), 2.0))) / 2.0);
    double pdist = sqrt(pow((x - 20.0), 2.0) + pow((y - 7.0), 2.0));
    double ndist = sqrt(pow((x + 20.0), 2.0) + pow((y + 7.0), 2.0));

    double fitness = 0.0;
    switch (problem) {

        case 2:
            fitness = (9.0 * max(0.0, (10.0 - pow(pdist, 2.0)))) + (10.0 * (1.0 - (pdist / mdist))) + (70.0 * (1.0 - (ndist / mdist)));
            break;
        case 1:
        default:
            fitness = 100.0 * (1.0 - (pdist / mdist));
            break;
    }

    return fitness;
}


/* PRINT DATA */
void printScatterPlotData(char* filename) {

}