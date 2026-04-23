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

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodSecundar {
	NodPrincipal* nodInfo;
	NodSecundar* next;
};

struct NodPrincipal {
	Masina info;
	NodSecundar* vecini;
	NodPrincipal* next;
};

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
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
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
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
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

void dezalocareNoduriGraf(NodPrincipal** listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
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

//Parcurgere in adancime

//creare structura Stiva
//push si pop pentru stiva

struct NodStiva {
	int idNod;
	struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

void push(NodStiva** varf, int id) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->idNod = id;
	nou->next = *varf;
	*varf = nou;
}

int pop(NodStiva** varf) {
	if (*varf) {
		int id = (*varf)->idNod;
		NodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp);
		return id;
	}
	return 0;
}

int calculeazaNrNoduriGraf(NodPrincipal* listaPrincipala) {
	int contor = 0;
	while (listaPrincipala) {
		contor++;
		listaPrincipala = listaPrincipala->next;
	}
	return contor;
}

int determinaIDMaxim(NodPrincipal* graf) {
	int max = 0;
	while (graf) {
		if (graf->info.id > max) {
			max = graf->info.id;
		}
		graf = graf->next;
	}
	return max;
}

void afisareGrafInAdancime(NodPrincipal* listaPrincipala, int idPlecare) {
	/*
	1. Este luata o STIVA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in STIVA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in STIVA
	4.1 Extragem un nod din STIVA
	4.2 Procesam nodul extras din STIVA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in STIVA
	4.4 reluam procesul
	*/
	if (!cautaNodDupaID(listaPrincipala, idPlecare)) return;

	NodStiva* stiva = NULL;
	int dim = determinaIDMaxim(listaPrincipala) + 1;
	int* vizitat = (int*)calloc(dim, sizeof(int));

	vizitat[idPlecare] = 1;
	push(&stiva, idPlecare);

	while (stiva) {
		int idCurent = pop(&stiva);
		NodPrincipal* nodCurent = cautaNodDupaID(listaPrincipala, idCurent);
		afisareMasina(nodCurent->info);

		NodSecundar* vecin = nodCurent->vecini;
		while (vecin) {
			int idVecin = vecin->nodInfo->info.id;
			if (vizitat[idVecin] == 0) {
				vizitat[idVecin] = 1;
				push(&stiva, idVecin);
			}
			vecin = vecin->next;
		}
	}
	free(vizitat);
}


//Parcurgere in latime

//creare structura coada
//enqueue si dequeue pentru coada
struct NodCoada {
	int idNod;
	struct NodCoada* next;
};
typedef struct NodCoada NodCoada;

void enqueue(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->idNod = id;
	nou->next = NULL;
	if (*cap) {
		NodCoada* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		*cap = nou;
	}
}

int dequeue(NodCoada** cap) {
	if (*cap) {
		int id = (*cap)->idNod;
		NodCoada* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
		return id;
	}
	return 0;
}

void afisareGrafInLatime(NodPrincipal* listaPrincipala, int idPlecare) {
	/*
	1. Este luata o COADA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in COADA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in COADA
	4.1 Extragem un nod din stiva
	4.2 Procesam nodul extras din COADA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in COADA
	4.4 reluam procesul
	*/
	if (!cautaNodDupaID(listaPrincipala, idPlecare)) return;

	NodCoada* coada = NULL;
	int dim = determinaIDMaxim(listaPrincipala) + 1;
	int* vizitat = (int*)calloc(dim, sizeof(int));

	vizitat[idPlecare] = 1;
	enqueue(&coada, idPlecare);

	while (coada) {
		int idCurent = dequeue(&coada);
		NodPrincipal* nodCurent = cautaNodDupaID(listaPrincipala, idCurent);
		afisareMasina(nodCurent->info);

		NodSecundar* vecin = nodCurent->vecini;
		while (vecin) {
			int idVecin = vecin->nodInfo->info.id;
			if (vizitat[idVecin] == 0) {
				vizitat[idVecin] = 1;
				enqueue(&coada, idVecin);
			}
			vecin = vecin->next;
		}
	}
	free(vizitat);
}


int main() {
	NodPrincipal* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");

	printf("--- TESTARE DFS ---\n");
	afisareGrafInAdancime(graf, 1);

	printf("\n--- TESTARE BFS ---\n");
	afisareGrafInLatime(graf, 1);

	dezalocareNoduriGraf(&graf);

	return 0;
}