#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct Carte {
	int id;
	char* titlu;
	float pret;
	int nrPagini;
};
struct Carte initializare(int id, const char* titlu, float pret, int nrPagini) {
	struct Carte c;
	c.id = id;
	c.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
	strcpy(c.titlu, titlu);
	c.pret = pret;
	c.nrPagini = nrPagini;
	return c;
}

void afisare(struct Carte c) {
	if (c.titlu != NULL) {
		printf("cartea %s cu id %d are %d pagini si costa %.2f ron\n", c.titlu, c.id, c.nrPagini, c.pret);
	}
}

void afisareVector(struct Carte* vector, int nrElemente) {
	if (vector != NULL && nrElemente > 0) {
		for (int i = 0; i < nrElemente; i++) {
			afisare(vector[i]);
		}
	}
}

struct Carte* copiazaPrimeleNElemente(struct Carte* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate <= 0 || nrElementeCopiate > nrElemente) {
		return NULL;
	}
	struct Carte* vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = initializare(vector[i].id, vector[i].titlu, vector[i].pret, vector[i].nrPagini);
	}
	return vectorNou;
}

void dezalocare(struct Carte** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].titlu);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Carte* vector, char nrElemente, float prag, struct Carte** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret > prag) {
			(*dimensiune)++;
		}
	}
	if (*dimensiune > 0) {
		*vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * (*dimensiune));
		int k = 0;
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].pret > prag) {
				(*vectorNou)[k] = initializare(vector[i].id, vector[i].titlu, vector[i].pret, vector[i].nrPagini);
				k++;
			}
		}
	}
}

struct Carte getPrimulElementConditionat(struct Carte* vector, int nrElemente, const char* conditie) {
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].titlu, conditie) == 0) {
			return initializare(vector[i].id, vector[i].titlu, vector[i].pret, vector[i].nrPagini);
		}
	}
	struct Carte c;
	c.id = 1;
	c.titlu = NULL;
	c.pret = 0;
	c.nrPagini = 0;

	return c;
}



int main() {
	int n = 3;
	struct Carte* carti = (struct Carte*)malloc(sizeof(struct Carte) * n);
	carti[0] = initializare(1, "baltagul", 25.5, 200);
	carti[1] = initializare(2, "enigma otiliei", 40.0, 350);
	carti[2] = initializare(3, "ion", 30.0, 400);

	printf("vector initial:\n");
	afisareVector(carti, n);

	struct Carte* cartiIeftine = copiazaPrimeleNElemente(carti, n, 2);
	printf("\nprimele 2 carti:\n");
	afisareVector(cartiIeftine, 2);

	struct Carte* cartiScumpe = NULL;
	int nrScumpe = 0;
	copiazaAnumiteElemente(carti, n, 35.0, &cartiScumpe, &nrScumpe);
	printf("\ncartile peste 35 ron:\n");
	afisareVector(cartiScumpe, nrScumpe);

	dezalocare(&carti, &n);
	int nrIeftine = 2;
	dezalocare(&cartiIeftine, &nrIeftine);
	dezalocare(&cartiScumpe, &nrScumpe);

	return 0;
}