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
	struct Nod* st;
	struct Nod* dr;
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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		}
		else if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		*radacina = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaMasinaInArbore(&radacina, citireMasinaDinFisier(f));
		}
		fclose(f);
	}
	return radacina;
}

void afisarePreordine(Nod* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisarePreordine(radacina->st);
		afisarePreordine(radacina->dr);
	}
}

void afisareInordine(Nod* radacina) {
	if (radacina) {
		afisareInordine(radacina->st);
		afisareMasina(radacina->info);
		afisareInordine(radacina->dr);
	}
}

void afisarePostordine(Nod* radacina) {
	if (radacina) {
		afisarePostordine(radacina->st);
		afisarePostordine(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("\n--- Parcurgere SRD ---\n");
	afisareInordine(radacina);

	printf("\n--- Parcurgere SRD ---\n");
	afisarePreordine(radacina);

	printf("\n--- Parcurgere SDR ---\n");
	afisarePostordine(radacina);
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeMasini(&((*radacina)->st));
		dezalocareArboreDeMasini(&((*radacina)->dr));
		if ((*radacina)->info.model) {
			free((*radacina)->info.model);
		}
		if ((*radacina)->info.numeSofer) {
			free((*radacina)->info.numeSofer);
		}
		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	m.id = -1;
	if (radacina) {
		if (radacina->info.id == id) {
			m = radacina->info;
			m.model = malloc(strlen(radacina->info.model) + 1);
			strcpy_s(m.model, strlen(radacina->info.model) + 1, radacina->info.model);
			m.numeSofer = malloc(strlen(radacina->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(radacina->info.numeSofer) + 1, radacina->info.numeSofer);
			return m;
		}
		else if (radacina->info.id > id) {
			return getMasinaByID(radacina->st, id);
		}
		else {
			return getMasinaByID(radacina->dr, id);
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		if (inaltimeSt > inaltimeDr) {
			return 1 + inaltimeSt;
		}
		else {
			return 1 + inaltimeDr;
		}
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina) {
		float pret = 0;
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			pret = radacina->info.pret;
		}
		return pret + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
	}
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini.txt");

	afisareMasiniDinArbore(radacina);

	printf("Numarul de noduri din arbore: %d\n", determinaNumarNoduri(radacina));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("Pret total masini: %.2f\n", calculeazaPretTotal(radacina));

	const char* sofer = "Ion";
	printf("Pret total masini pentru soferul %s: %.2f\n\n", sofer, calculeazaPretulMasinilorUnuiSofer(radacina, sofer));

	int idDeCautat = 2;
	printf("Cautare masina cu id %d:\n", idDeCautat);
	Masina m = getMasinaByID(radacina, idDeCautat);
	if (m.id != -1) {
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}
	else {
		printf("Masina nu a fost gasita\n");
	}

	dezalocareArboreDeMasini(&radacina);

	return 0;
}