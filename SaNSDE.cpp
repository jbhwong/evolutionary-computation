#include <iostream>
#include <random>
#include <fstream>
#include <float.h>
#include "random.h"
#include "Header.h"
#include <vector>
#include "simple_sop_benchmark.h"

using namespace std;

default_random_engine e;	//分布引擎
normal_distribution<double> normal_F(0.5, 0.3);
cauchy_distribution<double> cauchy_F(1);

#define NP 100
#define DIM 1000

#define GEN_UPDATE_CR 5
#define GEN_UPDATE_CRm 25
#define GEN_UPDATE_P 50

int gen_max = 5000;

int func_num = 1;

int gen = 1;	//generation value
 
typedef struct {
	double variable[DIM];
	double fitness;
}Individual;

Individual population[NP];
Individual newPopulation[NP];

double p = 0.5;

double F;

double CR[NP];

vector<double> CRrec;
vector<double> delta_F;

Benchmarks* fp = NULL;

double CRm = 0.5;

double minX, maxX;

int ns1 = 0, ns2 = 0, nf1 = 0, nf2 = 0;

bool mutation_flag[NP];

Individual best_individual;

int permutation[NP];

void swap(int *array, int a, int b) {
	int tmp = array[a];
	array[a] = array[b];
	array[b] = tmp;
}

double generate_variable() {
	return random_in_interval(minX, maxX, 1);
}

void initialize_permutaion() {
	//初始化序列
	for (int i = 0; i < NP; i++)
		permutation[i] = i;
}


double get_fitness(Individual indi) {
	/*return get_simple_sop_fitness(indi.variable, DIM);*/
	return fp->compute(indi.variable);
}

double new_CR() {
	normal_distribution<double> normal_CR(CRm, 0.1);
	double res = normal_CR(e);
	while (res <= 0 || res >=1)
		res = normal_CR(e);
	return res;
}

void generate_new_CR() {
	for (int i = 0; i < NP; i++)
		CR[i] = new_CR();
}

void initialize() {
	start_up_random();
	set_main_test_parameter(1);
	simple_sop_init();
	cout.setf(ios::scientific);
	
	fp = generateFuncObj(func_num);
	fp->setDimension(DIM);
	/*minX = get_simple_sop_search_space_low();
	maxX = get_simple_sop_search_space_high();*/

	minX = fp->getMinX();
	maxX = fp->getMaxX();

	best_individual.fitness = DBL_MAX;

	//初始化种群
	for (int i = 0; i < NP; i++) {
		for (int j = 0; j < DIM; j++)
			population[i].variable[j] = generate_variable();
		population[i].fitness = get_fitness(population[i]);
		if (population[i].fitness < best_individual.fitness)
			best_individual = population[i];
	}
	generate_new_CR();
}

void mutation() {
	
	for (int i = 0; i < NP; i++) {
		int divisor = NP - 1;
		initialize_permutaion();
		swap(permutation, i, NP - 1);
		for (int j = 0; j < 3; j++) {
			int tmp = rand() % divisor;
			swap(permutation, --divisor, tmp);
		}
		int i1 = permutation[NP - 2];
		int i2 = permutation[NP - 3];
		int i3 = permutation[NP - 4];

		if (random_in_interval(0, 1, 1) < p)
			F = normal_F(e);
		else
			F = cauchy_F(e);

		double tmp_variable;

		if (random_in_interval(0, 1, 1) < p) {
			//选择变异策略1
			mutation_flag[i] = true;
			for (int j = 0; j < DIM; j++) {
				tmp_variable = population[i1].variable[j] + F * (population[i2].variable[j] - population[i3].variable[j]);
				if (tmp_variable < minX)
					tmp_variable = minX;
				if (tmp_variable > maxX)
					tmp_variable = maxX;
				newPopulation[i].variable[j] = tmp_variable;
			}
		}
		else {
			//选择变异策略2
			mutation_flag[i] = false;
			for (int j = 0; j < DIM; j++) {
				tmp_variable = population[i].variable[j] + F * (best_individual.variable[j] - population[i].variable[j]) + F * (population[i1].variable[j] - population[i2].variable[j]);
				if (tmp_variable < minX)
					tmp_variable = minX;
				if (tmp_variable > maxX)
					tmp_variable = maxX;
				newPopulation[i].variable[j] = tmp_variable;
			}
		}
	}
}

void crossover() {
	
	for (int i = 0; i < NP; i++) {
		
		int index_j = rand() % DIM;
		for (int j = 0; j < DIM; j++) {
			if (!(j == index_j || random_in_interval(0, 1, 1) < CR[i]))
				newPopulation[i].variable[j] = population[i].variable[j];
		}
		newPopulation[i].fitness = get_fitness(newPopulation[i]);
	}
}

void selection() {
	
	for (int i = 0; i < NP; i++) {
		if (newPopulation[i].fitness < population[i].fitness) {
			if (mutation_flag[i]) {
				ns1++;
			}
			else {
				ns2++;
			}
			CRrec.push_back(CR[i]);
			delta_F.push_back(population[i].fitness - newPopulation[i].fitness);
			population[i] = newPopulation[i];
			if (population[i].fitness < best_individual.fitness)
				best_individual = population[i];
		}
		else {
			if (mutation_flag[i]) {
				nf1++;
			}
			else {
				nf2++;
			}
		}
	}
}
double sum_delta_F = 0;

void SaNSDE() {
	initialize();

	while (gen <= gen_max) {
		cout << gen << ": ";
		if (gen % GEN_UPDATE_P == 0) {
			p = (double)(ns1 * (ns2 + nf2)) / (double)(ns2 * (ns1 + nf1) + ns1 * (ns2 + nf2));
			ns1 = 0;
			ns2 = 0;
			nf1 = 0;
			nf2 = 0;
		}
		if (gen % GEN_UPDATE_CR == 0)
			generate_new_CR();
		if (gen % GEN_UPDATE_CRm == 0) {
			int end_index = CRrec.size();
			CRm = 0;
			sum_delta_F = 0;
			if (end_index != 0) {
				for (int i = 0; i < end_index; i++)
					sum_delta_F += delta_F[i];
				for (int i = 0; i < end_index; i++)
					CRm += (delta_F[i] / sum_delta_F) * CRrec[i];
			}

			CRrec.clear();
			delta_F.clear();
		}
		mutation();
		crossover();
		selection();
		gen++;
		cout << best_individual.fitness << endl;
	}
}

int main() {

	SaNSDE();
	
	system("pause");
	return 0;
}


Benchmarks* generateFuncObj(int funcID) {
	Benchmarks *fp;
	// run each of specified function in "configure.ini"
	if (funcID == 1) {
		fp = new F1();
	}
	else if (funcID == 2) {
		fp = new F2();
	}
	else if (funcID == 3) {
		fp = new F3();
	}
	else if (funcID == 4) {
		fp = new F4();
	}
	else if (funcID == 5) {
		fp = new F5();
	}
	else if (funcID == 6) {
		fp = new F6();
	}
	else if (funcID == 7) {
		fp = new F7();
	}
	else if (funcID == 8) {
		fp = new F8();
	}
	else if (funcID == 9) {
		fp = new F9();
	}
	else if (funcID == 10) {
		fp = new F10();
	}
	else if (funcID == 11) {
		fp = new F11();
	}
	else if (funcID == 12) {
		fp = new F12();
	}
	else if (funcID == 13) {
		fp = new F13();
	}
	else if (funcID == 14) {
		fp = new F14();
	}
	else if (funcID == 15) {
		fp = new F15();
	}
	else {
		cerr << "Fail to locate Specified Function Index" << endl;
		exit(-1);
	}
	return fp;
}
