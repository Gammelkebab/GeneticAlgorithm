#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <cstring>
#include <omp.h>

int nitems = 290;
int nbins = 200;
int popsize = 550;
int COPY = 50;
int COMBINE = 450;
int fib = 30;

//chromosome holding a possible solution
struct chrom {
	int* genes;
	int fit;
	int random;

	bool operator<(const chrom &chrm) const {
		return fit < chrm.fit;
	}
	static bool compare(const chrom *c1, const chrom *c2) {
		return (*c1) < (*c2);
	}
};

//DO NOT CHANGE THIS FUNCTION!
int spentSomeTime(int a) {
	if(a>fib) a = fib;
	if(a<=2) return a;
	return (spentSomeTime(a-1))+(spentSomeTime(a-2));
}


//adds item x in the bin that is defined in genes[x]
//then count the number of bins that have at least 1 item
//if a bin is overfilled, the overhead is added to the fitness
//fitness should be as small as possible -> use less bins that are not overfilled
//at the end a function increases the runtime to simulate a more complex fitness function
//may be important for parallelisation!!!
int fitness(int *genes, int *vbins, int* vitems) {
	int fit = 0;
	int *bins = new int[nbins]();
	for (int i=0; i<nitems; ++i) {
		if(bins[genes[i]]+vitems[i] < vbins[genes[i]]) {
			bins[genes[i]] += vitems[i];
		} else {
			genes[i] = (genes[i]+1)%nbins;
			--i;
		}
	}
    #pragma omp parallel for 
    for (int i=0; i<nbins; ++i) {
	    if(bins[i] > vbins[i]) {
		    fit += 1+(bins[i]-vbins[i]);
	    } else if (bins[i] > 0) {
		    fit ++;
	    }
    }
	 

	/****************************
	***DO NOT CHANGE THIS PART***
	****************************/
	int TIMEY = rand()%100+30;
	int a = spentSomeTime(TIMEY);
	if(rand()%1000==-1) printf("a: %d\n",a);
	/***************************
	**********PART END**********
	****************************/

	delete[] bins;
	return fit;
}

//compare function for qsort
int comp(const void *a,const void *b) {
	struct chrom *x = (struct chrom *) a;
	struct chrom *y = (struct chrom *) b;
	return x->fit - y->fit;
}

//returns random genes
int* create_random_genes() {
	int* genes = new int[nitems];
	#pragma omp parallel for
	for(int i=0; i<nitems; ++i) {
		genes[i] = rand()%(nbins);
	}
	return genes;
}

//combines 2 chromosomes by randomly picking a value from parent1 or parent2
struct chrom* combine(struct chrom* parent1, struct chrom* parent2) {
	struct chrom* child = new struct chrom;
	child->genes = new int[nitems];
	#pragma omp parallel for
	for(int i=0; i<nitems; ++i) {
		if(rand()%2==0) {
			child->genes[i] = parent1->genes[i];
		} else {
			child->genes[i] = parent2->genes[i];
		}
	}
	return child;
}

//choose 6 random chromosomes
//returns the chromosomes with the best fitness out of those 6
struct chrom* pick_best_of(struct chrom* old_pop) {
	struct chrom* chrm1 = &old_pop[rand()%popsize];
	for(int i=0; i<5; ++i) {
		struct chrom* chrm2 = &old_pop[rand()%popsize];
		if(chrm2->fit < chrm1->fit) {
			chrm1 = chrm2;
		}
	}
	return chrm1;
}

//with 2% probability: assign new random value to gene
void mutate(int* genes) {
    #pragma omp parallel for
	for (int i=0; i<nitems; ++i) {
		if(rand()%100 < 2) {
			genes[i] = rand()%nbins;
		}
	}
}

//simple print to check if data is valid
void print_gene(struct chrom* chrm) {
	for(int i=0;i<nitems; ++i) {
		printf("%d ", chrm->genes[i]);
	}
	printf("--- %d\n",chrm->fit);
}

void genetic_algorithm(int *vbins, int* vitems) {
	struct chrom* population = new struct chrom[popsize];

	//create first random population
	#pragma omp parallel for
	for (int i=0; i<popsize; ++i) {
		population[i].genes = create_random_genes();
		population[i].random = 1;
		population[i].fit = fitness(population[i].genes, vbins, vitems);
	}
	
/************************************ Parallel quicksort evtl. implementieren? ************************************/
	qsort(population, popsize, sizeof(chrom), comp);

	int iteration = 0;

	//fixed value to see speedup of parallelisation!
	while(iteration < 500) {
		//keep the previous population
		struct chrom* old_pop = population;
		population = new struct chrom[popsize];

		//elitism
		//copy the best chromosomes to the new population
		#pragma omp parallel for schedule(static)
		for (int i=0; i<COPY; ++i) {
			population[i].genes = new int[nitems];
			memcpy(population[i].genes, old_pop[i].genes, sizeof(int)*nitems);
			population[i].fit = old_pop[i].fit;
			population[i].random = old_pop[i].random;
		}
		//combine and mutate 2 chromosomes
		#pragma omp parallel for schedule(static)
		for (int i=0; i<COMBINE; ++i) {
			struct chrom* parent1 = pick_best_of(old_pop);
			struct chrom* parent2 = pick_best_of(old_pop);
			while(parent1 == parent2) {
				parent2 = pick_best_of(old_pop);
			}
			struct chrom *child = combine(parent1, parent2);
			mutate(child->genes);
			population[i+COPY].genes = child->genes;
			population[i+COPY].random = 0;
			population[i+COPY].fit = fitness(child->genes, vbins, vitems);
			delete child;
		}
		//add some new random chromosomes
		#pragma omp parallel for
		for (int i=COPY+COMBINE; i<popsize; i++) {
			population[i].genes = create_random_genes();
			population[i].random = 1;
			population[i].fit = fitness(population[i].genes, vbins, vitems);
		}

		qsort(population, popsize, sizeof(chrom), comp);
		iteration++;

		//free memory!
		#pragma omp parallel for
		for(int i=0; i<popsize; ++i) {
			delete[] old_pop[i].genes;
		}
		delete[] old_pop;
	}

	//printf("best overall fitness: %d \n",population[0].fit);
	#pragma omp parallel for
	for(int i=0; i<popsize; ++i) {
			delete[] population[i].genes;
	}
	delete[] population;
}

//assigns random volumes (depending on random state) to each bin/item
void create_bin_packing_problem(int* vbins, int* vitems, int random_state) {
	srand(random_state);
	
    #pragma omp parallel for
	for(int i=0; i<nbins; ++i) {
		vbins[i] = rand()%500+40;
	}
    #pragma omp parallel for
	for(int i=0; i<nitems; ++i) {
		vitems[i] = rand()%200+20;
	}
	srand(time(NULL));
}

int main(int argc, char** argv) {

    omp_set_num_threads(4);
    //time measurement
    double elapsed = 0;
    struct timeval begin, end;
    
	int *vbins = new int[nbins];
	int *vitems = new int[nitems];

    gettimeofday(&begin, NULL);

	for(int i=2; i<3; ++i) {
		create_bin_packing_problem(vbins, vitems, i);
		genetic_algorithm(vbins, vitems);
	}

    gettimeofday(&end, NULL);
    elapsed += (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("Runtime: %.5fs\n",elapsed);

}
