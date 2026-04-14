#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct NodSecundar {
	struct NodPrincipal* nodInfo;
	struct NodSecundar* next;
};
typedef struct NodSecundar NodSecundar;

struct NodPrincipal {
	Masina info;
	NodSecundar* vecini;
	struct NodPrincipal* next;
};
typedef struct NodPrincipal NodPrincipal;

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
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserarePrincipala(NodPrincipal** cap, Masina masina) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = masina;
	nou->vecini = NULL;
	nou->next = NULL;

	if (*cap) {
		NodPrincipal* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareSecundara(NodSecundar** cap, NodPrincipal* nodVecin) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->nodInfo = nodVecin;
	nou->next = NULL;

	if (*cap) {
		NodSecundar* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

//3.
//functie de cautarea in lista principala dupa ID
NodPrincipal* cautaNodDupaID(NodPrincipal* listaPrincipala, int id) {
	while (listaPrincipala) {
		if (listaPrincipala->info.id == id) {
			return listaPrincipala;
		}
		listaPrincipala = listaPrincipala->next;
	}
	return NULL;
}

//4.
//inserare muchie
void inserareMuchie(NodPrincipal* listaPrincipala, int idStart, int idStop) {
	NodPrincipal* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodPrincipal* nodStop = cautaNodDupaID(listaPrincipala, idStop);

	if (nodStart && nodStop) {
		inserareSecundara(&(nodStart->vecini), nodStop);
		inserareSecundara(&(nodStop->vecini), nodStart);
	}
}

NodPrincipal* citireNoduriMasiniDinFisier(const char* numeFisier) {
	NodPrincipal* graf = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			inserarePrincipala(&graf, citireMasinaDinFisier(file));
		}
		fclose(file);
	}
	return graf;
}

void citireMuchiiDinFisier(NodPrincipal* graf, const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		char buffer[20];
		char sep[3] = ",\n";
		while (fgets(buffer, 20, file)) {
			char* aux = strtok(buffer, sep);
			if (aux) {
				int idStart = atoi(aux);
				aux = strtok(NULL, sep);
				if (aux) {
					int idStop = atoi(aux);
					inserareMuchie(graf, idStart, idStop);
				}
			}
		}
		fclose(file);
	}
}

void afisareGraf(NodPrincipal* graf) {
	while (graf) {
		printf("Masina %d (%s) are conexiuni cu:\n", graf->info.id, graf->info.model);
		NodSecundar* vecin = graf->vecini;
		if (!vecin) {
			printf("-> Niciun vecin\n");
		}
		while (vecin) {
			printf("-> Masina %d (%s)\n", vecin->nodInfo->info.id, vecin->nodInfo->info.model);
			vecin = vecin->next;
		}
		printf("\n");
		graf = graf->next;
	}
}

void dezalocareNoduriGraf(NodPrincipal** listaPrincipala) {
	while (*listaPrincipala) {
		free((*listaPrincipala)->info.model);
		free((*listaPrincipala)->info.numeSofer);

		NodSecundar* vecin = (*listaPrincipala)->vecini;
		while (vecin) {
			NodSecundar* temp = vecin;
			vecin = vecin->next;
			free(temp);
		}

		NodPrincipal* tempPrincipal = *listaPrincipala;
		*listaPrincipala = (*listaPrincipala)->next;
		free(tempPrincipal);
	}
}

int main() {
	printf("=== Creare Graf-Masini ====\n\n");
	NodPrincipal* graf = citireNoduriMasiniDinFisier("masini.txt");

	citireMuchiiDinFisier(graf, "muchii.txt");

	printf("---- Structura Grafului (Lista de adiacenta) ----\n");
	afisareGraf(graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}