#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Masina are id-ul %d si un numar de %d usi\n", masina.id, masina.nrUsi);
	printf("Pretul este de %.2f iar seria este %c.\n", masina.pret, masina.serie);
	printf("Modelul %s este condus de %s.\n\n", masina.model, masina.numeSofer);
}

void afisareListaMasini(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = lista->first;
	nou->prev = NULL;
	if (lista->first) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			Masina m = citireMasinaDinFisier(file);
			if (m.id > 0) {
				adaugaMasinaInLista(&lista, m);
			}
		}
		fclose(file);
	}
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer) {
			free(aux->masina.numeSofer);
		}
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	float suma = 0;
	Nod* p = lista.first;
	while (p) {
		suma += p->masina.pret;
		p = p->next;
	}
	if (lista.nrNoduri > 0) {
		return suma / lista.nrNoduri;
	}
	return 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	Nod* p = lista->first;
	while (p && p->masina.id != id) {
		p = p->next;
	}
	if (p) {
		if (p->prev) {
			p->prev->next = p->next;
		}
		else {
			lista->first = p->next;
		}
		if (p->next) {
			p->next->prev = p->prev;
		}
		else {
			lista->last = p->prev;
		}
		free(p->masina.model);
		free(p->masina.numeSofer);
		free(p);
		lista->nrNoduri--;
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->masina.pret > max->masina.pret) {
				max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(max->masina.numeSofer) + 1);
		strcpy_s(nume, strlen(max->masina.numeSofer) + 1, max->masina.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	printf("Pretul mediu pentru masinile din lista: %.2f\n", calculeazaPretMediu(lista));

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	if (numeSofer) {
		printf("Numele soferului cu cea mai scumpa masina: %s\n", numeSofer);
		free(numeSofer);
	}

	int idCautat = 2;
	printf("Stergem masina cu id-ul %d...\n", idCautat);
	stergeMasinaDupaID(&lista, idCautat);

	printf("Lista de masini dupa stergere:\n");
	afisareListaMasini(lista);

	dezalocareLDMasini(&lista);

	return 0;
}