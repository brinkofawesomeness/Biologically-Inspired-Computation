/*
 * Conner Brinkley
 * February 24, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 2: Activation/Inhibition Cellular Automata
 * 
 * USAGE> ./aica R1 R2 H ID#
 * 
 * This program simulates the effect of activation and inhibition 
 * on cellular automata and produces a .csv data file along with .pgm and .jpg 
 * image files to view the results.
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


/* GLOBAL DATA VALUES */
int H, R1, R2, ID;
double J1, J2;

/* GLOBAL DATA AVERAGES FOR EACH EXPERIMENT */
double avgCorrelation[15];
double avgEntropy;
double avgJointEntropy[15];
double avgMutualInfo[15];

/*  AICA OBJECT */
class AICA {
    public:  
        // Public methods
        void initCA();
        void convergeCA(int run);
        void printCA();
        void createImage(char *filename1, char *filename2);
        void saveData(char *filename);
        void generateHTML();
        void updateAvgs();
        void calculateAvgs();
        void resetAvgs();
    private:
        // Helper methods
        void updateCA();
        void doUpdate(int x, int y);
        int distance(int ix, int iy, int jx, int jy);
        int beta(int s);
        void doCorrelation();
        void doEntropy();
        void doJointEntropy();
        void doMutualInfo();

        // Local data for each run
        double correlation[15];
        double entropy;
        double jointEntropy[15];
        double mutualInfo[15];
        vector <vector <int> > CA;
};


/* RANDOMIZES THE CA WITH 1's AND -1's */
void AICA::initCA() {

    // Resize the vector to be 30x30
    CA.resize(30);
    for (int i = 0; i < (int) CA.size(); i++) {
        CA[i].resize(30);
    }

    // Randomize the CA based on even or odd random num
    srand(time(NULL));

    for (int i = 0; i < (int) CA.size(); i++) {
        for (int j = 0; j < (int) CA[i].size(); j++) {

            int randNum = rand() % 100 + 1;
            if (randNum % 2 == 0) CA[i][j] = 1;
            else CA[i][j] = -1;
        }
    }

    // Make sure to 0 out all initial values
    entropy = 0.0;
    for (int i = 0; i < 15; i++) {
        correlation[i] = 0.0;
        jointEntropy[i] = 0.0;
        mutualInfo[i] = 0.0;
    }
}

/* STABILIZES THE CA USING THE UPDATE RULE */
void AICA::convergeCA(int run) {

    // Variables
    int step = 0;
    bool notStable = true;
    vector <vector <int> > prevCA;

    // Keep looping until we've reached a stable state
    while (notStable) {

        // Create a PGM of the current state
        char pgmFilename[512];
        char jpgFilename[512];
        sprintf(pgmFilename, "images/experiment%d_run%d_step%d.pgm", ID, run, step);
        sprintf(jpgFilename, "images/experiment%d_run%d_step%d.jpg", ID, run, step);
        createImage(pgmFilename, jpgFilename);

        // Update the CA
        prevCA = CA;
        updateCA();
        notStable = false;

        // Check if the new state is different from the last (if it's stable or not)
        for (int i = 0; i < (int) CA.size(); i++) {
            for (int j = 0; j < (int) CA[i].size(); j++) {
                if (prevCA[i][j] != CA[i][j]) {
                    notStable = true;
                    break;
                }
            }
            if (notStable) break;
        }
        step++;  
    }

    // Calculate some stuff once it's stable
    doCorrelation();
    doEntropy();
    doJointEntropy();
    doMutualInfo();
}

/* CHOOSES A RANDOM CELL TO UPDATE */
void AICA::updateCA() {
    
    // Variables
    int nextIndex, toUpdate;
    vector <int> remaining;
    remaining.resize(0);

    // Create a list of numbers from 0-899
    for (int i = 0; i < 30*30; i++) {
        remaining.push_back(i);
    }

    // Pick a random index and update it
    srand(time(NULL));
    while (remaining.size() > 0) {
        nextIndex = rand() % remaining.size();
        toUpdate = remaining[nextIndex];
        doUpdate(toUpdate / 30, toUpdate % 30);
        remaining.erase(remaining.begin() + nextIndex);
    }
}

/* UPDATES THE RANDOM CELL WE PREVIOUSLY CHOSE */
void AICA::doUpdate(int x, int y) {
    
    // Variables
    double s1 = 0, s2 = 0;
    int far = 0, near = 0;
    int d = 0;

    // Calculate near and far cells
    for (int j = 0; j < 30; j++) {
        for (int i = 0; i < 30; i++) {
            d = distance(i, j, x, y);
            if (d < R1) {
                near += CA[i][j];
            } else if (d < R2) {
                far += CA[i][j];
            }
        }
    }

    s1 = far * J2;
    s2 = near * J1;
    
    // If new value is nonnegative, it's a 1
    if (H + s1 + s2 >= 0) {
        CA[x][y] = 1;
    } 
    // Else it's a -1
    else {
        CA[x][y] = -1;
    }
}

/* CALCULATES THE DISTANCE BETWEEN TWO CELLS */
int AICA::distance(int ix, int iy, int jx, int jy) {
    
    int a = abs(ix - jx);
    int b = abs(iy - jy);

    // Wraps around if it's greater than the bound
    if (a > 15) {
        a = 30 - a;
    } if (b > 15) {
        b = 30 - b;
    }

    return a + b;
}

/* RETURN THE BETA */
int AICA::beta(int s) {
    return (s + 1) / 2;
}

/* CALCULATES CORRELATION */
void AICA::doCorrelation() {
    int sum1[15], sum2[15];
    double c1[15];
    double c2 = (1.0 / (30 * 30));
    
    // Loop through all possible distances
    for (int a = 0; a < 15; a++) {
        sum1[a] = 0;
        sum2[a] = 0;
        c1[a] = (2.0 / (30 * 30 * 4 * (a + 1)));
        
        // Iterate through every cell of the CA (x2)
        for (int i = 0; i < 30; i++) {
            for (int j = 0; j < 30; j++) {
                for (int m = 0; m < 30; m++) {
                    for (int n = 0; n < 30; n++) {
                        if ((i < m) || ((i == m) && (j < n))) {
                            if (distance(i, j, m, n) == (a + 1)) {
                                sum1[a] += CA[i][j] * CA[m][n];
                            }
                        }
                    }
                }
                sum2[a] += CA[i][j];
            }
        }
        // Formula given on write up
        correlation[a] = abs((c1[a] * sum1[a]) - (pow((c2 * sum2[a]), 2)));
    }
}

/* CALCULATES ENTROPY */
void AICA::doEntropy() {
    int sum = 0;
    double p_alive;
    double p_dead;
    
    // Calculate sum of 1's on grid
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            if (CA[i][j] == 1)
                sum++;
        }
    }
    
    // Calculate the probability of 1 and -1 cells
    p_alive = ((1.0 * sum) / (30 * 30));
    p_dead = 1.0 - p_alive;
    
    // Entropy formula given on write up
    if (p_alive == 0.0) {
        entropy = 0.0 - (p_dead * log(p_dead));
    } else if (p_dead == 0.0) {
        entropy = 0.0 - (p_alive * log(p_alive));
    } else {
        entropy = 0.0 - (p_alive * log(p_alive)) - (p_dead * log(p_dead));
    }
}

/* CALCULATES JOINT ENTROPY */
void AICA::doJointEntropy() {
    int sum1[15], sum2[15];
    double c[15];
    double p_ll[15], p_dd[15], p_ld[15];
    
    // Same nested for loop as we did in calculating correlation
    for (int a = 0; a < 15; a++) {
        sum1[a] = 0;
        sum2[a] = 0;
        
        if (a == 0) {
            c[1] = (1.0 / (30 * 30));
        } else {
            c[a] = (2.0 / (30 * 30 * 4 * a));
        }
        
        for (int i = 0; i < 30; i++) {
            for (int j = 0; j < 30; j++) {
                for (int m = 0; m < 30; m++) {
                    for (int n = 0; n < 30; n++) {
                        if ((i < m) || ((i == m) && (j < n))){
                            if (distance(i, j, m, n) == a) {
                                sum1[a] += beta(CA[i][j]) * beta(CA[m][n]);
                                sum2[a] += beta(-CA[i][j]) * beta(-CA[m][n]);
                            }
                        }
                    }
                }
            }
        }
        p_ll[a] = 1.0 * c[a] * sum1[a];
        p_dd[a] = 1.0 * c[a] * sum2[a];
        p_ld[a] = 1.0 - p_ll[a] - p_dd[a];
        
        // Make sure we're NOT taking the log of 0
        if (p_ll[a] != 0.0) {
            p_ll[a] *= log(p_ll[a]);
        }
        if (p_dd[a] != 0.0) {
            p_dd[a] *= log(p_dd[a]);
        }
        if (p_ld[a] != 0.0) {
            p_ld[a] *= log(p_ld[a]);
        }

        // Formula given on write up
        jointEntropy[a] = -(p_ll[a] + p_dd[a] + p_ld[a]);
    }
}

/* CALCUALTES MUTUAL INFORMATION */
void AICA::doMutualInfo() {

    // Formula given on write up
    for (int a = 0; a < 15; a++) {
		mutualInfo[a] = (2.0 * entropy) - jointEntropy[a];	
	}
}

/* ADD THIS RUN'S DATA TO THE GLOBAL SUM */
void AICA::updateAvgs() {		
    avgEntropy += entropy;
    for (int i = 0; i < 15; i++) {
        avgCorrelation[i] += correlation[i];
        avgJointEntropy[i] += jointEntropy[i];
        avgMutualInfo[i] += mutualInfo[i];
    }
}

/* DIVIDE THE GLOBALS BY 4 (THE AMOUNT OF RUNS WE AVERAGE) */
void AICA::calculateAvgs() {
	avgEntropy /= 4.0;
	for (int i = 0; i < 15; i++) {
		avgCorrelation[i] /= 4.0;
        avgJointEntropy[i] /= 4.0;
		avgMutualInfo[i] /= 4.0;
	}
}

/* ZERO OUT THE GLOBALS FOR THE NEXT RUN */
void AICA::resetAvgs() {
    avgEntropy = 0.0;
	for (int i = 0; i < 15; i++) {
        avgCorrelation[i] = 0.0;
		avgJointEntropy[i] = 0.0;
		avgMutualInfo[i] = 0.0;
	}
}

/* PRINTS OUT THE CURRENT CA / USED FOR TESTING */
void AICA::printCA() {
    
    for (int i = 0; i < (int) CA.size(); i++) {
        for (int j = 0; j < (int) CA[i].size(); j++) {
            cout << setw(2) << CA[i][j] << "  ";
        }
        cout << endl;
    }
}

/* CREATES A NEW PGM FILE FROM THE CA */
void AICA::createImage(char *filename1, char *filename2) {
    
    // Open a new pgm file and write the header
    ofstream pgm;
    pgm.open(filename1);

    pgm << "P2" << endl;
    pgm << "30 30" << endl;
    pgm << "255" << endl;

    // Print out the current CA values to the file
    for (int i = 0; i < (int) CA.size(); i++) {
        for (int j = 0; j < (int) CA[i].size(); j++) {
            if (CA[i][j] == 1) pgm << 0 << " ";
            else pgm << 255 << " ";
        }
        pgm << endl;
    }

    pgm.close();

    // Now convert to jpg using the linux convert tool
    char command[512];
    sprintf(command, "convert %s %s", filename1, filename2);
    system(command);
}

/* SAVE THE AVERAGE VALUES TO A CSV FILE */
void AICA::saveData(char *filename) {

    ofstream csv;
    csv.open(filename);

    // Print out our calculated values for each distance
    csv << "Run,Correlation (rho_l),Joint Entropy (H_l),Mutual Information (I_l)" << endl;
    for (int i = 0; i < 15; i++) {
        csv << i+1 << "," << avgCorrelation[i] << "," << avgJointEntropy[i] << "," << avgMutualInfo[i] << endl;
    }

    // Print out our initial J1, J2, R1, R2, and H values for reference
    csv << ",,,," << endl << ",,,," << endl;
    csv << "J1,J2,R1,R2,H" << endl;
    csv << J1 << "," << J2 << "," << R1 << "," << R2 << "," << H << endl;
}

/* GENERATES AN HTML DOCUMENT SO WE CAN VIEW THE RESULTS */
void AICA::generateHTML() {
    // Does nothing for now    
} 

/* DRIVER CODE */
int main(int argc, char **argv) {

    // Exit if we don't have the correct amount of arguments
    if (argc != 5) {
        cout << "USAGE: ./aica R1 R2 H ID" << endl;
        exit(0);
    }

    // Set global information from command line
    J1 = 1;
    J2 = -0.1;
    R1 = atoi(argv[1]);
    R2 = atoi(argv[2]);
    H = atoi(argv[3]);
    ID = atoi(argv[4]);

    // Create a new object and zero out globals
    AICA obj;
    obj.resetAvgs();

    // Loops through and averages results over 4 runs
    for (int i = 0; i < 4; i++) {
        obj.initCA();
        obj.convergeCA(i+1);
        obj.updateAvgs();
    }

    // Create a CSV file of the averaged data
    char filename[512];
    sprintf(filename, "csv/experiment%d.csv", ID);
    obj.calculateAvgs();
    obj.saveData(filename);

    return 0;
}