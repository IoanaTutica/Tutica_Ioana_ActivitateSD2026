#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct Sablon {
	int id;
	char* titlu;
	float pret;
	int nrPagini;
};
struct Sablon initializare(int id, const char* titlu, float pret, int nrPagini) {
	struct Sablon s;
	s.id = id;
	s.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
	strcpy(s.titlu, titlu);
	s.pret = pret;
	s.nrPagini = nrPagini;
	return s;
}

void afisare(struct Sablon s) {
	if (s.titlu != NULL) {
		printf("cartea %s cu id %d are %d pagini si costa %.2f ron\n", s.titlu, s.id, s.nrPagini, s.pret);
	}
}

void afisareVector(struct Sablon* vector, int nrElemente) {
	if (vector != NULL && nrElemente > 0) {
		for (int i = 0; i < nrElemente; i++) {
			afisare(vector[i]);
		}
	}
}

struct Sablon* copiazaPrimeleNElemente(struct Sablon* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate <= 0 || nrElementeCopiate > nrElemente) {
		return NULL;
	}
	struct Sablon* vectorNou = (struct Sablon*)malloc(sizeof(struct Sablon) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = initializare(vector[i].id, vector[i].titlu, vector[i].pret, vector[i].nrPagini);
	}
	return vectorNou;
}

void dezalocare(struct Sablon** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].titlu);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Sablon* vector, char nrElemente, float prag, struct Sablon** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret > prag) {
			(*dimensiune)++;
		}
	}
	if (*dimensiune > 0) {
		*vectorNou = (struct Sablon*)malloc(sizeof(struct Sablon) * (*dimensiune));
		int k = 0;
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].pret > prag) {
				(*vectorNou)[k] = initializare(vector[i].id, vector[i].titlu, vector[i].pret, vector[i].nrPagini);
				k++;
			}
		}
	}
}

struct Sablon getPrimulElementConditionat(struct Sablon* vector, int nrElemente, const char* conditie) {
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].titlu, conditie) == 0) {
			return initializare(vector[i].id, vector[i].titlu, vector[i].pret, vector[i].nrPagini);
		}
	}
	struct Sablon s;
	s.id = 1;
	s.titlu = NULL;
	s.pret = 0;
	s.nrPagini = 0;

	return s;
}



int main() {
	int n = 3;
	struct Sablon* carti = (struct Sablon*)malloc(sizeof(struct Sablon) * n);
	carti[0] = initializare(1, "baltagul", 25.5, 200);
	carti[1] = initializare(2, "enigma otiliei", 40.0, 350);
	carti[2] = initializare(3, "ion", 30.0, 400);

	printf("vector initial:\n");
	afisareVector(carti, n);

	struct Sablon* cartiIeftine = copiazaPrimeleNElemente(carti, n, 2);
	printf("\nprimele 2 carti:\n");
	afisareVector(cartiIeftine, 2);

	struct Sablon* cartiScumpe = NULL;
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