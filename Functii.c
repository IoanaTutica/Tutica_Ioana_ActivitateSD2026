#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

struct Vagon {
	int id;
	char* tip;
	float capacitate;
	char serie;
};

struct Vagon initializare(int id, const char* tip, float capacitate, char serie) {
	struct Vagon v;
	v.id = id;
	v.tip = (char*)malloc(sizeof(char) * (strlen(tip) + 1));
	strcpy(v.tip, tip);
	v.capacitate = capacitate;
	v.serie = serie;
	return v;
}

void afisare(struct Vagon v) {
	if (v.tip != NULL) {
		printf("Vagonul %s (ID: %d), seria %c, are o capacitate de %.2f tone\n", 
			v.tip, v.id, v.serie, v.capacitate);
	}
	else {
		printf("Vagonul cu ID: %d nu are tipul definit\n", v.id);
	}
}

void modifica_Capacitate(struct Vagon* v, float capacitateNoua) {
	if (capacitateNoua > 0) {
		v->capacitate = capacitateNoua;
	}
}

void dezalocare(struct Vagon* v) {
	if (v->tip != NULL) {
		free(v->tip);
		v->tip = NULL;
	}
}

int main() {
	struct Vagon v1;
	v1 = initializare(10, "Marfa", 50.5, 'M');
	afisare(v1);

	modifica_Capacitate(&v1, 55.0);
	printf("Dupa modificare:\n");
	afisare(v1);

	dezalocare(&v1);

	return 0;
}