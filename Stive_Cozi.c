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

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
void pushStack(Nod** varf, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = *varf;
	*varf = nou;
}

Masina popStack(Nod** varf) {
	if (*varf) {
		Masina m = (*varf)->info;
		Nod* temp = *varf;
		*varf = (*varf)->next;
		free(temp);
		return m;
	}
}

int emptyStack(Nod* varf) {
	if (varf == NULL) {
		return 1;
	}
	return 0;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	Nod* stiva = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			pushStack(&stiva, citireMasinaDinFisier(file));
		}
		fclose(file);
	}
	return stiva;
}

void dezalocareStivaDeMasini(Nod** varf) {
	while (!emptyStack(*varf)) {
		Masina m = popStack(varf);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Nod* varf) {
	int contor = 0;
	while (varf) {
		contor++;
		varf = varf->next;
	}
	return contor;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
void enqueue(Nod** cap, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = NULL;
	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

Masina dequeue(Nod** cap) {
	if (*cap) {
		Masina m = (*cap)->info;
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
		return m;
	}
}

Nod* citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	Nod* coada = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			enqueue(&coada, citireMasinaDinFisier(file));
		}
		fclose(file);
	}
	return coada;
}

void dezalocareCoadaDeMasini(Nod** cap) {
	while (*cap) {
		Masina m = dequeue(cap);
		free(m.model);
		free(m.numeSofer);
	}
}


//metode de procesare
Masina getMasinaByID(Nod* cap, int id) {
	Masina m;
	while (cap) {
		if (cap->info.id == id) {
			m = cap->info;
			m.model = malloc(strlen(cap->info.model) + 1);
			strcpy_s(m.model, strlen(cap->info.model) + 1, cap->info.model);
			m.numeSofer = malloc(strlen(cap->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(cap->info.numeSofer) + 1, cap->info.numeSofer);
			return m;
		}
		cap = cap->next;
	}
	return m;
}

float calculeazaPretTotal(Nod* cap) {
	float suma = 0;
	while (cap) {
		suma += cap->info.pret;
		cap = cap->next;
	}
	return suma;
}

int main() {
	printf("=== TESTARE STIVA (LIFO) ===\n");
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	
	printf("Numarul de masini din stiva este: %d\n", size(stiva));
	printf("Pretul total al masinilor din stiva: %.2f\n\n", calculeazaPretTotal(stiva));
	
	printf("---- Extragem 2 elemente din STIVA (pop) ----\n");
	afisareMasina(popStack(&stiva));
	afisareMasina(popStack(&stiva));

	dezalocareStivaDeMasini(&stiva);

	printf("=== TESTARE COADA (FIFO) ===\n");
	Nod* coada = citireCoadaDeMasiniDinFisier("masini.txt");

	int idCautat = 3;
	printf("Cautam in coada masina cu id %d:\n", idCautat);
	Masina m = getMasinaByID(coada, idCautat);
	if (m.model != NULL) { 
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}

	printf("--- Extragem 2 elemente din COADA (dequeue) ---\n");
	afisareMasina(dequeue(&coada));
	afisareMasina(dequeue(&coada));

	dezalocareCoadaDeMasini(&coada);

	return 0;
}