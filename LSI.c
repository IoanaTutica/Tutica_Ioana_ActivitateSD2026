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
 	Masina info;
 	struct Nod* next;
 };
 typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*cap)) {
		Nod* p = (*cap);
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		(*cap) = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = (*cap);
	(*cap) = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			Masina m = citireMasinaDinFisier(file);
			if (m.id > 0) {
				adaugaMasinaInLista(&cap, m);
			}
		}
		fclose(file);
	}
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	while ((*cap)) {
		Nod* aux = (*cap);
		(*cap) = (*cap)->next;
		if (aux->info.model) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		free(aux);
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int count = 0;
	while (cap) {
		suma += cap->info.pret;
		count++;
		cap = cap->next;
	}
	if (count > 0) {
		return suma / count;
	}
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		Nod* aux = (*cap);
		(*cap) = aux->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
	if ((*cap)) {
		Nod* p = (*cap);
		while (p->next) {
			if (p->next->info.serie == serieCautata) {
				Nod* aux = p->next;
				p->next = aux->next;
				free(aux->info.model);
				free(aux->info.numeSofer);
				free(aux);
			}
			else {
				p = p->next;
			}
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* cap = NULL;
	cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);

	printf("pret mediu: %.2f\n", calculeazaPretMediu(cap));

	char sofer[] = "Ion";
	printf("pret total pentru soferul %s: %.2f\n", sofer, calculeazaPretulMasinilorUnuiSofer(cap, sofer));

	stergeMasiniDinSeria(&cap, 'A');
	printf("lista dupa stergere serie A:\n");
	afisareListaMasini(cap);

	dezalocareListaMasini(&cap);

	return 0;
}