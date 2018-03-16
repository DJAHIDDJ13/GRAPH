#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct { //graphe représenté par une matrice d'adjacence
  	int nombre_sommet;
  	int ** matrice_adjacence;
} graphe;

typedef struct pile pile; 
 
struct pile{
	int val;
	pile* suiv;
};

pile* push(pile* p, int n){
	pile* m = malloc(sizeof(pile));
	m->val = n;
	m->suiv = p;
	return m;
}
pile* pop(pile* p){
	if(p == NULL){
		return NULL;
	}
	if(p->suiv == NULL){
		free(p);
		return NULL;
	}
	pile* m = p->suiv;
	free(p);
	return m;
}
void affichePile(pile* p){
	if(p == NULL){
		printf("vide\n");
		return;
	}
	while(p){
		printf("%d ",p->val);
		p = p->suiv;
	}
	printf("\n");
}
void free_pile(pile* p){
	while(p){
		p = pop(p);
	}
}
void affiche_graphe(graphe G){
	int i,j;
	printf("Graphe avec %d sommets \n",G.nombre_sommet);
	for(i = 0; i<G.nombre_sommet; i++){
		printf("Voisins de %d: ",i);
		for(j = 0; j < G.nombre_sommet; j++){
			if(G.matrice_adjacence[i][j]) printf("%d ",j);
		}
		printf("\n");
	}
}

int degre(graphe G, int i){
	int som = 0;
	for(int j=0; j<G.nombre_sommet ;j++){
		som += G.matrice_adjacence[i][j];
	}
	return som;
}


graphe init_graphe(int n){//créé un graphe dont tous les sommets sont isolés
	graphe G; 
	G.nombre_sommet = n;
	G.matrice_adjacence = malloc(sizeof(int*) * n);
	for(int i=0; i<n; i++){
		G.matrice_adjacence[i] = malloc(sizeof(int) * n);
		for(int j=0; j<n; j++){
			G.matrice_adjacence[i][j] = 0;
		}
	}
	return G;
}

void libere_graphe(graphe G){
	for(int i=0; i<G.nombre_sommet; i++){
		free(G.matrice_adjacence[i]);
	}
	free(G.matrice_adjacence);
}

graphe cycle_graphe(int n){//créé un cycle
	graphe G = init_graphe(n);
	for(int i=0; i<n-1; i++){
		G.matrice_adjacence[i][i+1] = 1;
		G.matrice_adjacence[i+1][i] = 1;
	}
	G.matrice_adjacence[n-1][0] = 1;
	G.matrice_adjacence[0][n-1] = 1;
	return G;
}


graphe complet_graphe(int n){//créé un graphe complet
	graphe G = init_graphe(n);
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i != j)
				G.matrice_adjacence[i][j] = 1;
		}
	}
	return G;
}


graphe alea_graphe(int n, float p){
	graphe G = init_graphe(n);
	for(int i=0; i<n; i++){
		for(int j=i+1; j<n; j++){
			_Bool r = (double)rand() / ((double)RAND_MAX + 1) <= p;
			G.matrice_adjacence[i][j] = r;
			G.matrice_adjacence[j][i] = r;
		}
	}
	return G;
}

//~ graphe graphe_arbre(int *pere, int n){//créer un arbre couvrant à partir du tableau des pères représenté comme un graphe
	//~ graphe G;
	//~ return G;
//~ }

int findRoot(graphe G, int *pere, int i){
	while(pere[i] !=-1){
		i = pere[i];
	}
	return i;
}
void parcours_sommet(graphe G, int s, int *couleur, int *pere){
	couleur[s] = 1;
	//~ printf("%d\n", s);
	for(int v=0; v<G.nombre_sommet; v++){
		if(G.matrice_adjacence[v][s] == 1 && couleur[v] == 0){
			pere[v] = s;
			parcours_sommet(G,v,couleur,pere);
		}
	}
	couleur[s] = 2;
}

void parcours_graphe(graphe G){
	int *couleur = calloc(G.nombre_sommet,sizeof(int)); // 0 est blanc, 1 gris et 2 noir
	int *pere = malloc(sizeof(int)*G.nombre_sommet);
	for(int i=0; i<G.nombre_sommet; i++){
		pere[i] = -1;
	}
	for(int s=0; s<G.nombre_sommet; s++){
		if(couleur[s] == 0){
			parcours_sommet(G, s, couleur, pere); 
		}
	}

	pile **compoConnex = malloc(sizeof(pile*) * G.nombre_sommet);
	for(int i=0; i<G.nombre_sommet; i++){
		compoConnex[i] = NULL;
	}
	for(int i=0; i<G.nombre_sommet; i++){
		int root = findRoot(G,pere,i);
		compoConnex[root] = push(compoConnex[root], i);
	}
	for(int i=0; i<G.nombre_sommet; i++){
		affichePile(compoConnex[i]);
	}
	int** foretCouvrante = malloc(sizeof(int*)*G.nombre_sommet);
	for(int i=0; i<G.nombre_sommet; i++){
		foretCouvrante[i] = malloc(sizeof(int)*G.nombre_sommet);
		for(int j=0; j<G.nombre_sommet; j++){
			foretCouvrante[i][j] = -2;
		}
		while(compoConnex[i]){
			int n = compoConnex[i]->val;
			foretCouvrante[i][n] = pere[n];
			compoConnex[i] = pop(compoConnex[i]);
		}
	}
	for(int i=0; i<G.nombre_sommet; i++){
		for(int j=0; j<G.nombre_sommet; j++){
			printf("%d ",foretCouvrante[i][j]);
		}
		printf("\n");
	}

	free(couleur);
	free(pere);
}


void parcours_sommet2(graphe G, int s, int *couleur, int *pere){
	couleur[s] = 1;
	//~ printf("%d\n", s);
	for(int v=0; v<G.nombre_sommet; v++){
		if(G.matrice_adjacence[v][s] == 1 && couleur[v] == 0){
			pere[v] = s;
			parcours_sommet(G,v,couleur,pere);
		}
	}
	couleur[s] = 2;
}
void cycle_eulerien(graphe G){//on suppose le graphe connexe
	int *couleur = calloc(G.nombre_sommet,sizeof(int)); // 0 est blanc, 1 gris et 2 noir
	int *pere = malloc(sizeof(int)*G.nombre_sommet);
	for(int i=0; i<G.nombre_sommet; i++){
		pere[i] = -1;
	}
	for(int s=0; s<G.nombre_sommet; s++){
		if(couleur[s] == 0){
			parcours_sommet(G, s, couleur, pere); 
		}
	}

	pile **compoConnex = malloc(sizeof(pile*) * G.nombre_sommet);
	for(int i=0; i<G.nombre_sommet; i++){
		compoConnex[i] = NULL;
	}
	for(int i=0; i<G.nombre_sommet; i++){
		int root = findRoot(G,pere,i);
		compoConnex[root] = push(compoConnex[root], i);
	}
	for(int i=0; i<G.nombre_sommet; i++){
		affichePile(compoConnex[i]);
	}
	int times = 1;
	for(int i=0; i<G.nombre_sommet; i++){
		if(compoConnex[i]){
			times++;
		}
	}
	if(times != 1){
		printf("ce graph ne contient pas un circuit eulerien car il n'est pas connexe\n");
		return;
	}
	for(int i=0; i<G.nombre_sommet; i++){
		if(degre(G,i)%2){
			printf("ce graph ne contient pas un circuit eulerien car il existe des/un sommet/s avec un degre impair\n");
		}
	}
	graphe AC = init_graphe(G.nombre_sommet);
	for(int i=0; i<G.nombre_sommet; i++){
		if(pere[i] != -1){
			AC.matrice_adjacence[i][pere[i]];
			AC.matrice_adjacence[pere[i]][i];
		}
	}
	
}


int main(){
	/* Tests pour vérifier si vos implémentations sont correctes*/
	
	srand(time(NULL));
	graphe G = alea_graphe(10,0.2);
	affiche_graphe(G);
	parcours_graphe(G);
	libere_graphe(G);
  	//~ graphe G = cycle_graphe(10);
  	//~ affiche_graphe(G);
	//~ graphe H = complet_graphe(5);
	//~ affiche_graphe(H);
	//~ graphe L = alea_graphe(10,0.2);
	//~ affiche_graphe(L);
	//~ graphe A = parcours_graphe(L);
	//~ affiche_graphe(A);
	//~ cycle_eulerien(L);
	//~ cycle_eulerien(H);
	//~ libere_graphe(G);
	//~ libere_graphe(H);
	//~ libere_graphe(L);
	//~ libere_graphe(A);
}
