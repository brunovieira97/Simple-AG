/*
	Simple AG
	Author: Bruno Vieira - github.com/brunovieira97
	Artificial Intelligence - Unisinos - 2018/1
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define POPULATION_SIZE 5
#define MAXIMUM_GENERATIONS 5

double mutation = 0.2;
double range = 100.0;
double rangeOffset = 50.0;

/*
	0, 1: 	function variables
	2: 		fitness storage
*/
float population[POPULATION_SIZE][3];   

void CreatePopulation() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		population[i][0] = (((float) rand() / (float) RAND_MAX) * range) - rangeOffset;
		population[i][1] = (((float) rand() / (float) RAND_MAX) * range) - rangeOffset;
	}
}

void ShowPopulation() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		printf("%d %.3f %.3f\n", i, population[i][0], population[i][1]);
	}
}

void ShowFitness() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		printf("%.3f\n", population[i][2]);
	}
}

void ShowPopulationWithFitness() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		printf("%d %.3f %.3f fit: %.3f\n", i, population[i][0], population[i][1], population[i][2]);
	}
}

/* deprecated
	void CalculateFitness() {
		for (int i = 0; i < POPULATION_SIZE; i++) {
			population[i][2] = (population[i][0] * population[i][0]) + (population[i][1] * population[i][1]); 
		}
	}
*/

// New Fitness function, provided by Gustavo Pessin
void CalculateFitness() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		population[i][2] = (
			(
				sin(pow(population[i][0], 3)) 
				+ sin(pow(population[i][0], 2)) 
				+ sin(population[i][1])
			) * 3
		) + (
			sqrt((
				pow(population[i][0], 2)
				+ pow(population[i][1], 2)
			))
		);
	}
}

int FindMinor() {
	float minor = (float) RAND_MAX;
	int minorIndex = 0;

	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (minor > population[i][2]) {
			minor = population[i][2];
			minorIndex = i;
		}
	}
	return minorIndex;
}

int FindMinorForTournament(int indexes[3]) {
	float minor = (float) RAND_MAX;
	int minorIndex = 0;

	for (int i = 0; i < 3; i++) {
		if (minor > population[indexes[i]][2]) {
			minor = population[indexes[i]][2];
			minorIndex = indexes[i];
		}
	}
	return minorIndex;
}

// deprecated
void Crossover(int minorIndex) {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		population[i][0] = (population[i][0] + population[minorIndex][0]) / 2.0;
		population[i][1] = (population[i][1] + population[minorIndex][1]) / 2.0;
	}
}

void UniformMutationWithoutElitism() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		for (int j = 0; j < 2; j++) {
			float r = rand() / (float) RAND_MAX;
			if (r < mutation) {
				population[i][j] = (((float) rand() / (float) RAND_MAX) * range) - rangeOffset;
			}
		}
	}
}

void UniformMutationWithElitism(int minorIndex) {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (i != minorIndex) {
			for (int j = 0; j < 2; j++) {
				float r = rand() / (float) RAND_MAX;
				if (r < mutation) {
					population[i][j] = (((float) rand() / (float) RAND_MAX) * range) - rangeOffset;
				}
			}
		}
	}
}

void GaussMutationWithoutElitism() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		for (int j = 0; j < 2;j++) {
			float r = rand() / (float) RAND_MAX;
			if (r < mutation) {
				float q;
				q = rand() / (float) RAND_MAX;     // 0 to 1
				q = (q * 2.0) - 1.0;               // -1 to 1
				q = q / 4.0;                       // -0.25 to 0.25
				q = 1.0 + q;                       // 0.75 to 1.25  

				population[i][j] = population[i][j] * q;
			}
		}
	}
}

void GaussMutationWithElitism(int minorIndex) {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (i != minorIndex) {
			for (int j = 0; j < 2; j++) {
				float r = rand() / (float) RAND_MAX;
				if (r < mutation) {
					float q;
					q = rand() / (float) RAND_MAX;     // valor entre 0 e 1
					q = (q * 2.0) - 1.0;               // valor entre -1 e 1
					q = q / 4.0;                       // valor entre + 0.25 e -0.25
					q = 1.0 + q;                       // valor entre 0.75 e 1.25  

					population[i][j] = population[i][j] * q;
				}
			}
		}
	}
}

void OverwritePopulation(float new_population[][3]) {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		population[i][0] = new_population[i][0];
		population[i][1] = new_population[i][1];
	} 
}

void TournamentSelectionWithCrossover() {
	float tempPopulation[POPULATION_SIZE][3];
	
	for (int i = 0; i < POPULATION_SIZE; i++) {
		int firstIndexes[3] = {
			(rand() % POPULATION_SIZE),
			(rand() % POPULATION_SIZE),
			(rand() % POPULATION_SIZE)
		};
		int secondIndexes[3] = {
			(rand() % POPULATION_SIZE),
			(rand() % POPULATION_SIZE),
			(rand() % POPULATION_SIZE)
		};

		auto firstElement = FindMinorForTournament(firstIndexes);
		auto secondElement = FindMinorForTournament(secondIndexes);

		// crossover
		tempPopulation[i][0] = (population[firstElement][0] + population[secondElement][0]) / 2.0;
		tempPopulation[i][1] = (population[firstElement][1] + population[secondElement][1]) / 2.0;
	}

	OverwritePopulation(tempPopulation);
}

int main() {
	srand(time(NULL));       

	CreatePopulation();

	CalculateFitness();

	printf("Created population with fitness:\n");
	ShowPopulationWithFitness();

	getchar();

	int currentGeneration = 1;

	while (currentGeneration < MAXIMUM_GENERATIONS) {
		int minorIndex = FindMinor(); 

		TournamentSelectionWithCrossover();
		
		// Mutation
		/*
			UniformMutationWithElitism(minorIndex);
			UniformMutationWithoutElitism();
			GaussMutationWithElitism(minorIndex);
		*/
		
		GaussMutationWithoutElitism();

		CalculateFitness();

		printf("Generation: %d \n\n", currentGeneration);
		ShowPopulationWithFitness();
		
		// View only
		minorIndex = FindMinor(); 
		printf("\nBest: %d %.3f\n", minorIndex, population[minorIndex][2]);
		
		getchar();
		currentGeneration++;
	}
	
	printf("Final population:\n");
	ShowPopulation();

	//ShowFitness();

	printf("\nThe end.\n");
	getchar();
}