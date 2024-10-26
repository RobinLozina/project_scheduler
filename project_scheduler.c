#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLIGNE 256

int Dimension(FILE* pfich);
int LectureFichier(int* tabduree, char** tabPrecedent, FILE* pfich);
int TableauVecDesPrecedents(int** TableauVecDesPrecedents, char** tabPrecedent, int dim);
int* Tabniveau(int** TableauVecDesPrecedents, int dim);
int calculerDatesAuPlusTot(int* tabduree, int** TableauVecDesPrecedents, int dim, int** tabDatesAuPlusTot);
int calculerDatesAuPlusTard(int* tabduree, int** TableauVecDesPrecedents, int dim, int** tabDatesAuPlusTard, int* tabDatesAuPlusTot);
int* calculerCheminCritique(int dim, int* tabDatesAuPlusTard, int* tabDatesAuPlusTot, int* nbrTaches);
int dessinerTachesGraphiques(int dim, int* tabduree, int* tabDatesAuPlusTard, int* tabDatesAuPlusTot, char** tabGraphique);

main()
{
	int* tabduree;
	char** tabPrecedent;
	int** tabVecteurDesPrecedents;
	int dim, i, j;
	FILE* pfich;
	int* tabniveau;
	int* tabDatesAuPlusTot;
	int* tabDatesAuPlusTard;
	int* tabCheminCritique;
	int nbrTaches=0;
	char** tabGraphique;

	pfich = fopen("Exemple1.csv", "r");
	if (pfich == NULL) { printf("Impossible d'ouvrir le fichier !\n"); return -1; }

	dim = Dimension(pfich);
	

	// ZONE MALLOC


	tabduree = (int*)malloc(dim * (sizeof(int)));       // tableau d'int (une etoile=une dimension)
	if (tabduree == NULL)
	{
		printf("Erreur d'allocation");
		return;
	}

	tabPrecedent = (char**)malloc(dim * (sizeof(char*)));    // tableau de chaine de carac
	if (tabPrecedent == NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		return;
	}

	tabVecteurDesPrecedents = (int**)malloc((dim + 2) * (sizeof(int*)));      // tableau de vecteurs, dim+2 car tache fictive initiale et finale
	if (tabPrecedent == NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		return;
	}

	tabDatesAuPlusTot = (int*)malloc((dim+2)*(sizeof(int)));       // tableau d'int
	if (tabDatesAuPlusTot == NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		for (i = 0; i < dim + 2; i++)
		{
			free(tabVecteurDesPrecedents[i]);
			tabVecteurDesPrecedents[i] = NULL;
		}
		free(tabVecteurDesPrecedents);
		return;
	}

	tabDatesAuPlusTard = (int*)malloc((dim + 2)*(sizeof(int)));     // tableau d'int
	if (tabDatesAuPlusTard == NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		for (i = 0; i < dim + 2; i++)
		{
			free(tabVecteurDesPrecedents[i]);
			tabVecteurDesPrecedents[i] = NULL;
		}
		free(tabVecteurDesPrecedents);
		free(tabDatesAuPlusTot);
		return;
	}

	tabGraphique = (char**)malloc(dim*(sizeof(char*)));     // tab 2dimensions de caracteres
	if(tabGraphique==NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		for (i = 0; i < dim + 2; i++)
		{
			free(tabVecteurDesPrecedents[i]);
			tabVecteurDesPrecedents[i] = NULL;
		}
		free(tabVecteurDesPrecedents);
		free(tabDatesAuPlusTot);
		free(tabDatesAuPlusTard);
		return;
	}


	// ZONE AFFICHAGE


	if (LectureFichier(tabduree, tabPrecedent, pfich)==1)
	{
		printf("Affichage duree\n");
		for (i = 0; i < dim; i++)
		{
			printf("Tache %d : ", i + 1);
			printf("%d\n", tabduree[i]);
		}
		printf("\n");
		printf("Affichage anteriorite\n");
		for (i = 0; i < dim; i++)
		{
			printf("Tache %d : ", i + 1);
			puts(tabPrecedent[i]);
		}
	}
	else 
	{
		printf("Erreur d'allocation");
		return;
	}
	printf("\n");

	if (TableauVecDesPrecedents(tabVecteurDesPrecedents, tabPrecedent, dim) == 1)
	{
		printf("Affichage tableau de vecteurs contenant les anteriorites : \n");
		for (i = 1; i <=( dim); i++)
		{
			printf("Tache %d : ", i);
			for (j = 0; j < (tabVecteurDesPrecedents[i][0]) + 1; j++)
			{
				printf("%d ", tabVecteurDesPrecedents[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}

	else if(TableauVecDesPrecedents(tabVecteurDesPrecedents, tabPrecedent, dim)==0)
	{
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		for (i = 0; i < dim + 2; i++)
		{
			free(tabVecteurDesPrecedents[i]);
			tabVecteurDesPrecedents[i] = NULL;
		}
		free(tabVecteurDesPrecedents);
		printf("Erreur d'allocation");
		return;
	}

	tabniveau = Tabniveau(tabVecteurDesPrecedents, dim);  
	if (tabniveau == NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		for (i = 0; i < dim + 2; i++)
		{
			free(tabVecteurDesPrecedents[i]);
			tabVecteurDesPrecedents[i] = NULL;
		}
		free(tabVecteurDesPrecedents);
		return;
	}
	printf("Affichage niveau\n");
	for (i = 1; i < dim+1; i++)
	{
		printf("Niveau de la tache %d : ", i);
		printf("%d\n", tabniveau[i]);
	}

	if (calculerDatesAuPlusTot(tabduree, tabVecteurDesPrecedents, dim, &tabDatesAuPlusTot) == 1)
	{
		printf("\nDates au plus tot \n");
		for (i = 1; i < dim + 1; i++)
			printf("tache %d : %d \n",i, tabDatesAuPlusTot[i]);
	}

	if (calculerDatesAuPlusTard(tabduree, tabVecteurDesPrecedents, dim, &tabDatesAuPlusTard, tabDatesAuPlusTot) == 1)
	{
		printf("\nDates au plus tard \n");
		for (i = 1; i < dim + 1; i++)
			printf("tache %d : %d \n", i, tabDatesAuPlusTard[i]);
	}
	
	tabCheminCritique =calculerCheminCritique(dim, tabDatesAuPlusTard, tabDatesAuPlusTot, &nbrTaches);
	if (tabCheminCritique == NULL)
	{
		printf("Erreur d'allocation");
		free(tabduree);
		for (i = 0; i < dim; i++)
		{
			free(tabPrecedent[i]);
			tabPrecedent[i] = NULL;
		}
		free(tabPrecedent);
		for (i = 0; i < dim + 2; i++)
		{
			free(tabVecteurDesPrecedents[i]);
			tabVecteurDesPrecedents[i] = NULL;
		}
		free(tabVecteurDesPrecedents);
		free(tabDatesAuPlusTot);
		free(tabDatesAuPlusTard);
		printf("Erreur d'allocation");
		return;
	}
	{
		printf("\nChemin critique\n");
		for (i = 1; i < nbrTaches-1; i++)
			printf("tache %d \n", tabCheminCritique[i]);
	}

	if (dessinerTachesGraphiques(dim, tabduree, tabDatesAuPlusTard, tabDatesAuPlusTot, tabGraphique) == 1)
	{
		printf("\nGraphique \n");
		for (i = 0; i < dim; i++)
			puts(tabGraphique[i]);
	}

	free(tabduree);
	for (i = 0; i < dim; i++)
	{
		free(tabPrecedent[i]);            
		tabPrecedent[i] = NULL;
	} 
	free(tabPrecedent);
	for (i = 0; i < dim+2; i++)
	{
		free(tabVecteurDesPrecedents[i]);
		tabVecteurDesPrecedents[i] = NULL;
	}
	free(tabVecteurDesPrecedents);
	free(tabniveau);
	free(tabDatesAuPlusTot);
	free(tabDatesAuPlusTard);
	free(tabCheminCritique);
	for (i = 0; i < dim; i++)
	{
		free(tabGraphique[i]);
		tabGraphique[i] = NULL;
	}
	free(tabGraphique);
	printf("Fin du fichier\n\n");
	fclose(pfich);
}


// ZONE FONCTION


int Dimension(FILE* pfich)                                         // On balaye chaque ligne du tableau avec une condition afin de trouver le nbr de taches
{
	char ligne[MAXLIGNE + 1];
	int codes, duree;
	char car1, car2;
	char anteriorite[MAXLIGNE];
	int dim = 0;
	while (fgets(ligne, MAXLIGNE + 1, pfich) != NULL)
	{
		if (sscanf(ligne, "%d%c%d%c%s", &codes, &car1, &duree, &car2, anteriorite) == 5)
			dim++;
	}
	rewind(pfich);
	return dim;
}

int LectureFichier(int* tabduree, char** tabPrecedent, FILE* pfich)      // Le but est de trouver le tabduree et le tabPrecedent 
{
	char ligne[MAXLIGNE + 1];
	int codes, duree, i = 0, j;
	char car1, car2;
	char anteriorite[MAXLIGNE];
	while (fgets(ligne, MAXLIGNE + 1, pfich) != NULL)
	{              
		if (sscanf(ligne, "%d%c%d%c%s", &codes, &car1, &duree, &car2, anteriorite) == 5)       
		{
			for (j = 0; j < strlen(anteriorite); j++)
			{
				if (anteriorite[j] == ';')                                                        // Permet de determiner la fin de la ligne de carac
					anteriorite[j] = '\0';
			}
		    tabduree[i] = duree;
			tabPrecedent[i] = (char*)malloc((strlen(anteriorite) + 1) * (sizeof(char)));          // +1 car strlen ne lit pas le \0
			if (tabPrecedent[i] == NULL)
			{
				free(tabduree);
				free(tabPrecedent);
				return 0;
			}
			strcpy(tabPrecedent[i], anteriorite);
			i++;
		}
	}
	return 1;
}

int TableauVecDesPrecedents(int** TableauVecDesPrecedents, char** tabPrecedent, int dim)     // On cherche a transformer le tab de chaine de carac en tab a 2 dim de int
{
	int i, j = 0, k = 0, Z;
	int nombre;
	char virgule;
	char suite[MAXLIGNE];
	for (i = 0; i < dim; i++)
	{
		j = 0;
		for (Z = 0;Z < strlen(tabPrecedent[i]); Z++)
		{
			if (tabPrecedent[i][Z] == ',')
			{
				j++;                                               // Nbr virgules 
			}
		}
		(TableauVecDesPrecedents)[i+1] = (int*)malloc((j +2) * (sizeof(int)));    // On commence a i+1 car le cas 0 et le cas final sont geres par apres, +2 car besoin nbr d'ante et virgules+1=chiffres
		if ((TableauVecDesPrecedents)[i+1] == NULL)
		{
			return 0;
		}
		(TableauVecDesPrecedents)[i+1][0] = j+1 ;                                  // si y a 2 virgules y a 3 int d'ou le +1
		for (k = 0; k < j + 1; k++)
		{
			if (sscanf(tabPrecedent[i], "%d%c%s", &nombre, &virgule, suite) == 3)           // nous permet de diviser la ligne en set (x,...)
			{
				(TableauVecDesPrecedents)[i+1 ][k + 1] = nombre;                       // k+1 car on a deja ecrit dans le 0
				strcpy(tabPrecedent[i], suite);                                        // on passe au set (x,...) suivant
			}
			else if (sscanf(tabPrecedent[i], "%d", &nombre) == 1)                      // pour le dernier set qui ne contient que x
			{
				(TableauVecDesPrecedents)[i + 1][j + 1] = nombre;                      // on peut aussi utiliser k+1
			}
		}
	}
	(TableauVecDesPrecedents)[0] = (int*)malloc(1 * sizeof(int));                       // pour la tache initiale 
	(TableauVecDesPrecedents)[0][0] = 0;
	(TableauVecDesPrecedents)[i + 1] = (int*)malloc(2 * sizeof(int));                   // pour la tache finale
	(TableauVecDesPrecedents)[i + 1][0] = 1;                            
	(TableauVecDesPrecedents)[i + 1][1] = i;
	return 1;
}

int* Tabniveau(int** TableauVecDesPrecedents, int dim)      // permet de trouver le niveau d'une tache via ses precedents
{
	int i, j, n, max=0;
	int* tabniveau;
	tabniveau = (int*)malloc((dim + 2) * (sizeof(int)));
	if (tabniveau == NULL)
	{
		return NULL;
	}
	tabniveau[0] = 0;
	for (i = 1; i < dim + 2; i++)
	{
		for (j = 1; j <= TableauVecDesPrecedents[i][0]; j++)            // on regarde le nbr de prec
		{
			n = TableauVecDesPrecedents[i][j];                          
			if (tabniveau[n] > max)                                     // on compare chaque niveau via un max
				max = tabniveau[n];
		}
		tabniveau[i] = max + 1;
		max = 0;
	}
	return tabniveau;
}

int calculerDatesAuPlusTot(int* tabduree, int** TableauVecDesPrecedents, int dim, int** tabDatesAuPlusTot)    // attention ici decalage car tabduree[0]=tache 1, une * de plus pour datesauplustot car pointeur
{
	int i,j=1,n,max=0;
	(*tabDatesAuPlusTot)[0] = 0;
	for (i = 1; i < dim + 2; i++)
	{
		for (j = 1; j <= TableauVecDesPrecedents[i][0]; j++)         
		{
			n = TableauVecDesPrecedents[i][j];						     // Pour regarder chaque anteriorite et chercher le max 
			if(n == 0)
			{
				continue;
			}
	     	if (tabduree[n-1] + (*tabDatesAuPlusTot)[n] > max)           // On regarde si le resultat est plus grand que le max precedent, utile seulement si plusieurs ante
			{
				max = tabduree[n-1]+ (*tabDatesAuPlusTot)[n];            // La formule est duree+dates au plus tot
			}
		}	
	(*tabDatesAuPlusTot)[i] = max;  max = 0; 
	}
	return 1;
}

int calculerDatesAuPlusTard(int* tabduree, int** TableauVecDesPrecedents, int dim, int** tabDatesAuPlusTard, int* tabDatesAuPlusTot)
{
	int i,ligne,nbrColonnes=0,colonne,m,datemin=0,duree,stockage=0;
	(*tabDatesAuPlusTard)[dim + 1] = tabDatesAuPlusTot[dim+1];
	(*tabDatesAuPlusTard)[0] = 0;
	for (i = dim; i >= 1; i--)														  // On part de la fin pour creer le tableau de taille dim+2
	{
		for (ligne = 1; ligne <= dim + 1; ligne++)									  // On balaie chaque ligne du tab contenant les precedents
		{
			nbrColonnes = TableauVecDesPrecedents[ligne][0];
			for (colonne = 1; colonne <= nbrColonnes; colonne++)					  // On balaie les colonnes des dites lignes afin de chercher la tache i dans cells-ci
			{
				if (TableauVecDesPrecedents[ligne][colonne] == i)              
				{
					stockage++;     
						if (stockage == 1)                                             // si la tache i n'apparait qu'une fois dans toutes les lignes et colonnes
						{                                                              
							datemin = (*tabDatesAuPlusTard)[ligne];                    // on injecte la valeur date au plus tard de la ligne ou on a trouv� i pour l'avoir quand stockage>1
							duree = tabduree[i-1];                                     // on injecte la duree de la tache i (decalage car tabduree commence a 0=tache 1)
							m = ligne;                                                 // on injecte le numero de la ligne du tab precedent car on aura besoin de son tab dates plus tard
						}
						else                                                           // si la tache apparait une n �me fois
						{
							if (datemin > (*tabDatesAuPlusTard)[ligne])                // on compare datemin et la nouvelle date au plus tard afin de garder la plus petite
								m = ligne;
						}
				}
			}
		}
		(*tabDatesAuPlusTard)[i] = (*tabDatesAuPlusTard)[m] - duree; stockage = 0; datemin = 0;    
	}
	return 1;
} 

int* calculerCheminCritique(int dim, int* tabDatesAuPlusTard, int* tabDatesAuPlusTot, int* nbrTaches)     // nbrTaches en pointeur pour faciliter l'affichage
{
	int* tabCheminCritique;
	int i;
	for (i = 0; i < dim + 2; i++)
	{
		if (tabDatesAuPlusTard[i] == tabDatesAuPlusTot[i])                            // permet de calculer le nbr de taches dans le chemin critique
			*nbrTaches = *nbrTaches + 1;
	}
	tabCheminCritique = (int*)malloc((*nbrTaches)*(sizeof(int)));
	if (tabCheminCritique == NULL)
	{
		return NULL;
	}
	*nbrTaches = 0;
	for (i = 0; i < dim + 2; i++)                                                     // on refait la meme boucle qu'au dessus c'est pas tres opti
	{
		if (tabDatesAuPlusTard[i] == tabDatesAuPlusTot[i])
		{
			tabCheminCritique[*nbrTaches] = i;
			*nbrTaches=*nbrTaches + 1;
		}
	}
	return tabCheminCritique;
}

int dessinerTachesGraphiques(int dim, int* tabduree, int* tabDatesAuPlusTard, int* tabDatesAuPlusTot, char** tabGraphique)
{
	int i, a, colonnes, espace, interogation, chiffre;
	char tampon[MAXLIGNE];
	for (i = 0; i < dim; i++)
	{
		colonnes = tabduree[i] + tabDatesAuPlusTard[i + 1];        // decalage entre tabduree et tabDatesAuPlusTard
		tabGraphique[i] = (char*)malloc((colonnes*2+1) * (sizeof(char)));    // longueur des chaines de carac dynamique, x2 car on met un espace entre tout, +1 pour le /0
	}
	for (i = 0; i < dim; i++)
	{
		espace = tabDatesAuPlusTot[i + 1];
		interogation = (tabDatesAuPlusTard[i + 1] - tabDatesAuPlusTot[i + 1]);
		chiffre = tabduree[i];
		tabGraphique[i][0]='\0';                                       // permet de cloturer la chaine via les strcat
		for (a = 0; a < espace; a++)
			strcat(tabGraphique[i], "  ");                              // on concatene le nbr d'espace
		for (a = espace; a < (espace + chiffre); a++)
		{
			sprintf(tampon, "%2d", i+1);                               // tous les nbr ont 2 places 
			strcat(tabGraphique[i], tampon);                           // on concatene tampon qui contient le chiffre n fois dans tabGraph
		}
		for (a = (espace + chiffre); a < (espace + chiffre + interogation); a++)
		{
			strcat(tabGraphique[i], " ?");                              // on concatene le nbr de ?
		}
	}
	return 1;
}	