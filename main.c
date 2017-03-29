#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

extern uint64_t rdtsc(void);

void baseline(int n, float m[n][n], float v1[n], float v2[20])
{
	int i,j,k;
	
	for(i=0; i<n; i++)
		v1[i]=0;
	for (k=0; k<20; k++)
		for(j=0; j<n; j++)
			for(i=0; i<n; i++)
				v1[i] += m[i][j] * v2[k];
}

void initTab1D(int taille, float* tab)
{
	//srand(2);
	for (int i = 0; i < taille; ++i)
		tab[i]=0.0f; // Pour pas se faire chier, sinon --> tab[i]=rand();
}

void initTab2D(int n, float tab[n][n])
{	
	for (int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			tab[i][j]=0; // Pareil que pour l'init à 1 dimension
}

// Il faut 3 arguments :
// - Le 1er le nombre de tour de warmup à faire
// - Le 2ème le nbr de mesures de notre baseline
// - Le 3ème la taille de nos tableaux
int main(int argc, char const *argv[])
{	 
	if(argc <= 3) {
		printf("Pas assez d'argument, arrêt.\n");
		return EXIT_FAILURE;
	}
	else
	{
		int n = atoi(argv[3]);
		float (*m)[n]=malloc(n*n*sizeof(float));
		float v1[n];
		float v2[20];
		uint64_t start, stop;
		FILE* mesures=fopen("mesures.csv", "a");
	
		// Initialisations
		initTab1D(n,v1);
		initTab2D(n,m);
		
		// Boucle du warmup
		printf("Nombre de warmup à faire : %d\n", atoi(argv[1]));
		for (int i = 0; i < atoi(argv[1]); ++i) {		
			baseline(n, m, v1, v2);
		}
		
		// Partie d'analyse
		printf("Nombre de boucle à analyser : %d\n", atoi(argv[2]));
		start=rdtsc();
		for (int i = 0; i < atoi(argv[2]); ++i)
		{
			baseline(n, m, v1, v2);
		}
		stop=rdtsc();
		
		printf("Voici le temps obtenu : %f\n\n", (float)(stop-start));
		
		fprintf(mesures,"%f;", (float)(stop-start));
		fclose(mesures);
	}
	
	return EXIT_SUCCESS;
}
