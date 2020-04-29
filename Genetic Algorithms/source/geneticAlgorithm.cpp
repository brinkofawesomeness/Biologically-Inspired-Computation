/* 
 * Conner Brinkley
 * April 29, 2020
 * 
 * The University of Tennessee
 * Biologically-Inspired Computation
 * Project 4: Genetic Algorithms
 * 
 * USAGE> ./geneticAlgorithm id numGenes populationSize generations mutationProb crossoverProb
 * 
 * This program simulates a basic genetic algorithm using the fitness 
 * function F(s) = (x / 2^l)^10.
 *
 * */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#include <map>
using namespace std;

/* BIT STRING OBJECT */
class Individual {
	public:
		Individual(string g);
		void update();
		double fitness, normalized, runningSum;
		string genes;
};

/* INIT THE BIT STRING */
Individual::Individual(string g) { genes = g; }

/* UPDATE USING THE FITNESS FUNCTION */
void Individual::update() {
	unsigned long long x = 0;
	for (int i = 0; i < genes.size(); i++) {
		if (genes[genes.size() - 1 - i] == '1') {
			x += (1 << i);
		}
	}
	fitness = pow((double(x) / pow(2, genes.size())), 10);
}

/* SIMULATOR */
class GeneticAlgorithm {
	public:
		GeneticAlgorithm(int, int, int, int, double, double);
		void evolve();
		void appendToCSV();
	private:
		void mating();
		vector <Individual *> population, nextGeneration;
		vector <double> bestFit, avgFit, numCorrectBits;
		double mutProbability, crossProbability;
		int generations;
		char filepath[512];
};

/* INIT */
GeneticAlgorithm::GeneticAlgorithm(int ID, int numGenes, int popSize, int g, double P_m, double P_c) {

	// Initialize the population
	string init;
	for (int i = 0; i < popSize; i++) {

		// Randomize the bit string based on even or odd random number
		init.clear();
		for (int j = 0; j < numGenes; j++) {
			int randNum = rand() % 100 + 1;
			if (randNum % 2 == 0) init.push_back('1');
			else init.push_back('0');
		}

		Individual *tmp = new Individual(init);
		population.push_back(tmp);
	}

	// Initialize the data
	bestFit.resize(g);
	avgFit.resize(g);
	numCorrectBits.resize(g);

	// Initialize the input values
	generations = g;
	mutProbability = P_m * 1000;
	crossProbability = P_c * 1000;
	sprintf(filepath, "../data/sim%d.csv", ID);
}

/* MATING LOOP */
void GeneticAlgorithm::mating() {
	
	// Get it on
	for (int i = 0; i < (((int) population.size()) / 2); i++) {

		// Pick two parents
		int idx1, idx2;
		int randNum1 = rand() % 1000 + 1, randNum2;
		double rand1 = 1.0 / double(randNum1), rand2;
		
		for (idx1 = 0; idx1 < (int) population.size() - 1; idx1++) {
			if (population[idx1]->runningSum > rand1) break;
		}
		
		do {
			randNum2 = rand() % 1000 + 1;
			rand2 = 1.0 / double(randNum2);
			for (idx2 = 0; idx2 < (int) population.size() - 1; idx2++) {
				if (idx1 != idx2 && population[idx2]->runningSum > rand2) break;
			}
		} while (idx1 == idx2);

		// Collab (crossover genes)
		int crossover = rand() % 1000 + 1;
		if (crossover < crossProbability) {
	
			int point = rand() % (population[idx1]->genes).length() + 1;
			string tmp1 = population[idx1]->genes;
			string tmp2 = population[idx2]->genes;
			
			for (int j = 0; j < point; j++) {
				tmp1[j] = population[idx2]->genes[j];
				tmp2[j] = population[idx1]->genes[j];
			}

			nextGeneration.push_back(new Individual(tmp1));
			nextGeneration.push_back(new Individual(tmp2));

		} else {
			nextGeneration.push_back(new Individual(population[idx1]->genes));
			nextGeneration.push_back(new Individual(population[idx1]->genes));
		}

		// Mutate, baby
		for (int j = 0; j < nextGeneration[nextGeneration.size() - 1]->genes.length(); j++) {
			int mutate = rand() % 100 + 1;
			
			if (mutate % 2 == 0) {
				if (nextGeneration[nextGeneration.size() - 1]->genes[j] == '0') {
					nextGeneration[nextGeneration.size() - 1]->genes[j] = '1';
				} else {
					nextGeneration[nextGeneration.size() - 1]->genes[j] = '0';
				}
			}
		}
		for (int j = 0; j < nextGeneration[nextGeneration.size() - 2]->genes.length(); j++) {
			int mutate = rand() % 100 + 1;
			
			if (mutate % 2 == 0) {
				if (nextGeneration[nextGeneration.size() - 2]->genes[j] == '0') {
					nextGeneration[nextGeneration.size() - 2]->genes[j] = '1';
				} else {
					nextGeneration[nextGeneration.size() - 2]->genes[j] = '0';
				}
			}
		}
	}
}

/* EVOLVES THE POPULATION */
void GeneticAlgorithm::evolve() {

	// Keep track of
	double totalFit, maxFit, sum, maxIdx;

	// Do for each generation
	for (int g = 0; g < generations; g++) {
		nextGeneration.clear();
		totalFit = 0.0;
		maxFit = 0.0;
		sum = 0.0;

		// Calculate fitness of the population and find the fittest individual
		for (int i = 0; i < (int) population.size(); i++) {
			population[i]->update();
			totalFit += population[i]->fitness;
			if (population[i]->fitness > maxFit) {
				maxFit = population[i]->fitness;
				maxIdx = i;
			}
		}

		// Record data
		avgFit[g] = (totalFit / population.size());
		bestFit[g] = population[maxIdx]->fitness;
		for (int i = 0; i < (int) population[maxIdx]->genes.length(); i++) {
			if (population[maxIdx]->genes[i] == '1') {
				numCorrectBits[g] += 1;
			}
		}

		// Keep a running sum
		for (int i = 0; i < (int) population.size(); i++) {
			population[i]->normalized = population[i]->fitness / totalFit;
			sum += population[i]->normalized;
			population[i]->runningSum = sum;
		}

		// Mate
		mating();

		// Copy the next generation to the current population
		population.clear();
		for (int i = 0; i < (int) nextGeneration.size(); i++) {
			population.push_back(nextGeneration[i]);
		}
	}
}

/* WRITES THE DATA TO A FILE */
void GeneticAlgorithm::appendToCSV() {

	ofstream csv;
	csv.open(filepath, std::ofstream::out | std::ofstream::app);
	csv << endl;
	for (int gen = 0; gen < generations; gen++) {
		csv << gen << "," << bestFit[gen] << "," << avgFit[gen] << "," << numCorrectBits[gen] << endl;
	}
	csv.close();
}

/* DRIVER CODE */
int main(int argc, char **argv) {

	// Error checking
	if (argc != 7) {
		printf("USAGE: ./geneticAlgorithm id numGenes populationSize generations mutationProb crossoverProb\n");
		exit(0);
	}

	// Read in data
	int id = atoi(argv[1]);
	int numGenes = atoi(argv[2]);
	int popSize = atoi(argv[3]);
	int generations = atoi(argv[4]);
	double mutProbability = atof(argv[5]);
	double crossProbability = atof(argv[6]);

	// Initialize the data file
	char filepath[512];
	sprintf(filepath, "../data/sim%d.csv", id);
	ofstream csv;
	csv.open(filepath);
	csv << "Number of Genes (l),Population Size (N),Generations (G),Mutation Probability (P_m),Crossover Probability (P_c)" << endl;
	csv << numGenes << "," << popSize << "," << generations << "," << mutProbability << "," << crossProbability << endl << endl;
	csv << "Generation,Avg Fitness,Best Fitness,# of Correct Bits in Best" << endl;
	csv.close();

	// Repeat over 10 runs to find typical behavior
	srand(time(NULL));
	for (int run = 0; run < 10; run++) {

		// Initialize the population and let it run its course
		GeneticAlgorithm *ga = new GeneticAlgorithm(id, numGenes, popSize, generations, mutProbability, crossProbability);
		ga->evolve();
		ga->appendToCSV();
		delete ga;
	}

	return 0;
}
