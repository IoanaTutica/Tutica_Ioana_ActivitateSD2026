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
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** tabela;
};
typedef struct HashTable HashTable;

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
	printf("Masina %d: %s, %d usi, %.2f RON, sofer: %s, serie: %c\n", 
		masina.id, masina.model, masina.nrUsi, masina.pret, masina.numeSofer, masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->masina);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	if ((*cap)) {
		Nod* aux = (*cap);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		(*cap) = nou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* numeSofer, int dimensiune) {
	int suma = 0;
	if (numeSofer != NULL) {
		for (int i = 0; i < strlen(numeSofer); i++) {
			suma += numeSofer[i];
		}
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	if (hash.dim > 0) {
		int poz = calculeazaHash(masina.numeSofer, hash.dim);
		adaugaMasinaInLista(&(hash.tabela[poz]), masina);
	}
}

HashTable citireMasiniDinFisierTabela(const char* numeFisier, int dimensiune) {
	HashTable ht = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id > 0) {
				inserareMasinaInTabela(ht, m);
			}
		}
		fclose(f);
	}
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			printf("Clusterul de pe pozitia %d:\n", i);
			afisareListaMasini(ht.tabela[i]);
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* p = ht->tabela[i];
		while (p) {
			Nod* aux = p;
			p = p->next;
			free(aux->masina.model);
			free(aux->masina.numeSofer);
			free(aux);
		}
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* medii = (float*)malloc(sizeof(float) * ht.dim);
	(*nrClustere) = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			float suma = 0;
			int count = 0;
			Nod* p = ht.tabela[i];
			while (p) {
				suma += p->masina.pret;
				count++;
				p = p->next;
			}
			medii[(*nrClustere)] = suma / count;
			(*nrClustere)++;
		}
	}
	return medii;
}

Masina getMasinaDupaNumeSofer(HashTable ht, const char* numeCautat) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;

	int poz = calculeazaHash(numeCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim) {
		Nod* p = ht.tabela[poz];
		while (p && strcmp(p->masina.numeSofer, numeCautat) != 0) {
			p = p->next;
		}
		if (p) {
			m.id = p->masina.id;
			m.nrUsi = p->masina.nrUsi;
			m.pret = p->masina.pret;
			m.serie = p->masina.serie;
			
			m.model = malloc(strlen(p->masina.model) + 1);
			strcpy_s(m.model, strlen(p->masina.model) + 1, p->masina.model);
			
			m.numeSofer = malloc(strlen(p->masina.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(p->masina.numeSofer) + 1, p->masina.numeSofer);
		}
	}
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisierTabela("masini.txt", 7);
	afisareTabelaDeMasini(ht);

	int nrClustere = 0;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("\nPreturile medii pe clusterele ocupate:\n");
	for (int i = 0; i < nrClustere; i++) {
		printf("Media cluster %d: %.2f\n", i, medii[i]);
	}
	free(medii);

	const char* cautat = "Ion";
	printf("\nCautam masina soferului %s:\n", cautat);
	Masina m = getMasinaDupaNumeSofer(ht, cautat);
	if (m.id != -1) {
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	} else {
		printf("Masina nu a fost gasita\n");
	}

	dezalocareTabelaDeMasini(&ht);

	return 0;
}