/*
	Simple AG
	Author: Bruno Vieira - github.com/brunovieira97
	Artificial Intelligence - Unisnios - 2018/1
*/

// Dependencies
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Constants
#define population_size 10
#define generations 10

double mutation = 0.0;             // percentual (0..1)
double ag_range = 20.0;            // teste com 1?
double ag_range_offset = 10.0;     // teste com 0?

/*
	0, 1: 	function variables
	2: 		fitness storage
*/
float population[population_size][3];   

// Creates population
int CreatePopulation() {
	for (int i = 0; i < population_size; i++) {
		/*
			pop[i][0] = (float) rand();  
			pop[i][1] = (float) rand(); 
			pop[i][0] = (float) rand() / (float) RAND_MAX; 
			pop[i][1] = (float) rand() / (float) RAND_MAX; 
	 	*/

		population[i][0] = (((float) rand() / (float) RAND_MAX) * ag_range) - ag_range_offset;
		population[i][1] = (((float) rand() / (float) RAND_MAX) * ag_range) - ag_range_offset;
	}
	return 0;
}

// Prints population on screen
int ShowPopulation() {
	for (int i = 0; i < population_size; i++) {
		printf("%d %.3f %.3f\n",i, population[i][0],population[i][1]);
	}
	return 0;
}

// Prints population and fitness on screen
int ShowPopulationWithFitness() {
	for (int i = 0; i < population_size; i++) {
		printf("%d %.3f %.3f fit: %.3f\n", i, population[i][0], population[i][1], population[i][2]);
	}
	return 0;
}

// Calculates fitness
int CalculateFitness() {
	for (int i = 0; i < population_size; i++) {
		population[i][2] = (population[i][0] * population[i][0]) + (population[i][1] * population[i][1]); 
	}
	return 0;
}

int GetMinor() {
	float minor = (float) RAND_MAX;
	int minor_index = 0;

	for (int i = 0; i < population_size; i++) {
		if (minor > population[i][2]) {
			minor = population[i][2];
			minor_index = i;
		}
	}
	return minor_index;
}

int Crossover(int minor_index) {
	for (int i = 0; i < population_size; i++) {
		population[i][0] = (population[i][0] + population[minor_index][0]) / 2.0;
		population[i][1] = (population[i][1] + population[minor_index][1]) / 2.0;
	}
	return 0;
}

int UniformMutationWithoutElitism() {
	for (int i = 0; i < population_size; i++) {
		for (int j = 0; j < 2; j++) {
			float r = rand() / (float) RAND_MAX;
			if (r < mutation) {
				population[i][j] = (((float) rand() / (float) RAND_MAX) * ag_range) - ag_range_offset;
			}
		}
	}
	return 0;
}

int UniformMutationWithElitism(int minor_index) {
	for (int i = 0; i < population_size; i++) {
		if (i != minor_index) {
			for (int j = 0; j < 2; j++) {
				float r = rand() / (float) RAND_MAX;
				if (r < mutation) {
					population[i][j] = (((float) rand() / (float) RAND_MAX) * ag_range) - ag_range_offset;
				}
			}
		}
	}
	return 0;
}

int GaussMutationWithoutElitism() {
	for (int i = 0; i < population_size; i++) {
		for (int j = 0; j < 2;j++) {
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
	return 0;
}

int GaussMutationWithElitism(int minor_index) {
	for (int i = 0; i < population_size; i++) {
		if (i != minor_index) {
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
	return 0;
}

int main() {
	srand(time(NULL));       

	CreatePopulation();

	printf("Created population:\n");
	ShowPopulation();
	getchar();

	CalculateFitness();

	printf("Created population with fitness:\n");
	ShowPopulationWithFitness();
	getchar();

	int g = 1;  // discutir

	while (g < generations) {
		// Selection
		int minor_index = GetMinor(); 
			
		// Crossover
		Crossover(minor_index);

		// Mutation
		//UniformMutationWithElitism(minor_index);
		//UniformMutationWithoutElitism();
		GaussMutationWithElitism(minor_index);
		//GaussMutationWithoutElitism();

		// Fitness
		CalculateFitness();

		printf("Generation: %d \n\n",g);
		ShowPopulationWithFitness();
		
		// View only
		minor_index = GetMinor(); 
		printf("\nBest: %d %.3f\n", minor_index, population[minor_index][2]);
		
		getchar();

		g++;
	}
	printf("That's all, Folks!");
	getchar();
}