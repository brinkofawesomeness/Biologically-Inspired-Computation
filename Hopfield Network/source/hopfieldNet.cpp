/* 
 * Conner Brinkley
 * March 23, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 3: Hopfield Network
 * 
 * USAGE> ./hopfieldNet
 * 
 * This program simulates a Hopfield Network and investigates its associative 
 * memory capacity. It calculates data and averages it over 100 runs before 
 * saving it to two CSV files for easy viewing. Because of the high amount of runs 
 * it averages over, it's possible that it could take up to 2 minutes to run.
 *
 * */

#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

/* OBJECT */
class HopfieldNetwork {

	public:
		HopfieldNetwork();
		void imprintPatterns(int p);
		void testStability(int p);
		void calculateProbability(int p);
		void saveData(vector <double> &, vector <double> &);

	private:
		vector <double> stableCount;
		vector <vector<int> > patterns;
		vector <vector<double> > weights;
		vector <double> unstableProbability;
};

/* CONSTRUCTOR */
HopfieldNetwork::HopfieldNetwork() {

	// Resize the stability and probability count to 50
	stableCount.resize(50, 0);
	unstableProbability.resize(50, 0);

	// Resize the weights to be 100 x 100
	weights.resize(100);
	for (int i = 0; i < 100; i++) 
		weights[i].resize(100);

	// Resize the patterns to be 50 x 100
	patterns.resize(50);
	for (int i = 0; i < 50; i++)
		patterns[i].resize(100);

	// Randomly initialize each pattern value to either 1 or -1
	srand(time(NULL));
	for (int i = 0; i < (int) patterns.size(); i++) {
		for (int j = 0; j < (int) patterns[i].size(); j++) {
			int randNum = rand() % 100 + 1;
			if (randNum % 2 == 0) patterns[i][j] = 1;
			else patterns[i][j] = -1;
		}
 	}
}

/* IMPRINTS PATTERNS 1 THROUGH p */
void HopfieldNetwork::imprintPatterns(int p) {

	double N = 100;        // Number of neurons
	double sum = 0;        // Sum 

	// Loop through all weights[i][j]
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			// Make sure there is no self-coupling
			if (i == j) weights[i][j] = 0;
			else {

				// Loop through each pattern and sum the product
				sum = 0;
				for (int k = 0; k < p + 1; k++) 
					sum += (patterns[k][i] * patterns[k][j]);
				weights[i][j] = (sum / N);
			}
		}
	}
}

/* TESTS THE p IMPRINTED PATTERNS FOR STABILITY */
void HopfieldNetwork::testStability(int p) {

	int N = 100;                 // Number of neurons
	bool stable;                 // Stability flag 
	double sum = 0;              // Sum 
	vector <int> newNeuralNet;   // Stores the next state value   
	vector <double> localField;  // Stores local field

	// Resize the vectors to 100
	localField.resize(100, 0);
	newNeuralNet.resize(100, 0);

	// Loop through each pattern p
	for (int k = 0; k < p + 1; k++) {

		// Compute the local field of the neurons
		for (int i = 0; i < N; i++) {		
			sum = 0;
			for (int j = 0; j < N; j++) 
				sum += (((double) (weights[i][j]) * ((double) patterns[k][j])));
			localField[i] = sum;

			// Calculate the next state
			if (localField[i] < 0) newNeuralNet[i] = -1;
			else newNeuralNet[i] = 1;
		}

		// Check its stability
		stable = true;
		for (int i = 0; i < N; i++) {
			if (patterns[k][i] != newNeuralNet[i]) {
				stable = false;
				break;
			}
		} if (stable) stableCount[p] += 1;
	}
}

/* CALCULATES THE PROBABILITY OF UNSTABLE IMPRINTS FOR EACH p */
void HopfieldNetwork::calculateProbability(int p) {

	double stableProbability = (((double) stableCount[p]) / (p + 1.0));
	unstableProbability[p] = (1.0 - stableProbability);
}

/* ADDS THE DATA FROM THE CURRENT RUN TO THE TOTAL COUNT */
void HopfieldNetwork::saveData(vector <double> &stable, vector <double> &unstable) {

	for (int i = 0; i < 50; i++) {
		stable[i] += stableCount[i];
		unstable[i] += unstableProbability[i];
	}
}

/* OUTPUT THE DATA AS CSV FILES */
void saveDataAsCSV(vector <double> &stableCount, vector <double> &unstableProbability) {
	
	ofstream stable;
	ofstream unstable;
	stable.open("stableImprints.csv");
	unstable.open("unstableImprints.csv");

	stable << "p,# of stable imprints" << endl;
	for (int p = 1; p <= 50; p++) 
		stable << p << "," << stableCount[p - 1] << endl;

	unstable << "p,fraction of unstable imprints" << endl;
	for (int p = 1; p <= 50; p++) 
		unstable << p << "," << unstableProbability[p - 1] << endl;

	stable.close();
	unstable.close();
}

/* DRIVER CODE */
int main(int argc, char **argv) {

	// Used to store data
	vector <double> stableCount;
	vector <double> unstableProbability;
	stableCount.resize(50, 0);
	unstableProbability.resize(50, 0);
	
	// Average over a series of 100 runs
	for (int run = 0; run < 100; run++) {

		HopfieldNetwork *net = new HopfieldNetwork();
		
		for (int p = 0; p < 50; p++) {
			net->imprintPatterns(p);
			net->testStability(p);
			net->calculateProbability(p);
		}
		net->saveData(stableCount, unstableProbability);
		
		delete net;
	}

	// Average values
	for (int i = 0; i < 50; i++) {
		stableCount[i] /= 100;
		unstableProbability[i] /= 100;
	}

	// Save and return
	saveDataAsCSV(stableCount, unstableProbability);
	return 0;
}
