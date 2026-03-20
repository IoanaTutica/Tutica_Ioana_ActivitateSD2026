#define _CRT_SECURE_NO_WARNINGS
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
/*Sa se implementeze o structura Proiect care sa contina id (int), nr_zile (int), buget (float), denumire (char), nume_manager (char*) si cod_categorie (unsigned char). Datele vor fi citite dintr-un fisier text. Sa se stocheze datele intr-o lista simplu inlantuita. Sa se realizeze functii pentru: afisarea listei, calculul bugetului mediu, calculul bugetului total pentru un anumit manager si stergerea proiectelor care apartin unei categorii transmise ca parametru (tratand cazurile de inceput, mijloc si sfarsit de lista).*/

 struct StructuraProiect {
 	int id;
 	int nrZile;
 	float buget;
 	char* denumire;
 	char* numeManager;
 	unsigned char codCategorie;
 };
 typedef struct StructuraProiect Proiect;
 
 struct Nod {
 	Proiect info;
 	struct Nod* next;
 };
 typedef struct Nod Nod;
 
 Proiect citireProiectDinFisier(FILE* file) {
 	char buffer[100];
 	char sep[3] = ",\n";
 	fgets(buffer, 100, file);
 	char* aux;
 	Proiect p1;
 	aux = strtok(buffer, sep);
 	p1.id = atoi(aux);
 	p1.nrZile = atoi(strtok(NULL, sep));
 	p1.buget = atof(strtok(NULL, sep));
 	aux = strtok(NULL, sep);
 	p1.denumire = malloc(strlen(aux) + 1);
 	strcpy_s(p1.denumire, strlen(aux) + 1, aux);
 
 	aux = strtok(NULL, sep);
 	p1.numeManager = malloc(strlen(aux) + 1);
 	strcpy_s(p1.numeManager, strlen(aux) + 1, aux);
 
 	p1.codCategorie = *strtok(NULL, sep);
 	return p1;
 }
 
 void afisareProiect(Proiect p) {
 	printf("Id: %d\n", p.id);
 	printf("Nr. zile : %d\n", p.nrZile);
 	printf("Buget: %.2f\n", p.buget);
 	printf("Denumire: %s\n", p.denumire);
 	printf("Nume manager: %s\n", p.numeManager);
 	printf("Categorie: %c\n\n", p.codCategorie);
 }
 
 void afisareListaProiecte(Nod* cap) {
 	while (cap) {
 		afisareProiect(cap->info);
 		cap = cap->next;
 	}
 }
 
 void adaugaProiectInLista(Nod* *cap, Proiect proiectNou) {
 	Nod* nou = malloc(sizeof(Nod));
 	nou->info = proiectNou;
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
 
 Nod* citireListaProiecteDinFisier(const char* numeFisier) {
 	Nod* cap = NULL;
 	FILE* file = fopen(numeFisier, "r");
 	if (file) {
 		while (!feof(file)) {
 			Proiect p = citireProiectDinFisier(file);
 			if (p.id > 0) {
 				adaugaProiectInLista(&cap, p);
 			}
 		}
 		fclose(file);
 	}
 	return cap;
 }
 
 void dezalocareListaProiecte(Nod* *cap) {
 	while ((*cap)) {
 		Nod* aux = (*cap);
 		(*cap) = (*cap)->next;
 		if (aux->info.numeManager) {
 			free(aux->info.numeManager);
 		}
 		if (aux->info.denumire) {
 			free(aux->info.denumire);
 		}
 		free(aux);
 	}
 }
 
 float calculeazaBugetMediu(Nod* cap) {
 	float suma = 0;
 	int count = 0;
 	while (cap) {
 		suma += cap->info.buget;
 		count++;
 		cap = cap->next;
 	}
 	if (count > 0) {
 		return suma / count;
 	}
 	else {
 		return 0;
 	}
 }
 
 void stergeProiecteDinCategoria(Nod* *cap, char codCautat) {
 	while ((*cap) && (*cap)->info.codCategorie == codCautat) {
 		Nod* aux = *cap;
 		(*cap) = aux->next;
 		if (aux->info.numeManager) {
 			free(aux->info.numeManager);
 		}
 		if (aux->info.denumire) {
 			free(aux->info.denumire);
 		}
 		free(aux);
 	}
 	if ((*cap)) {
 		Nod* p = *cap;
 		while (p) {
 			while (p->next && p->next->info.codCategorie != codCautat) {
 				p = p->next;
 			}
 			if (p->next) {
 				Nod* aux = p->next;
 				p->next = aux->next;
 				if (aux->info.numeManager) {
 					free(aux->info.numeManager);
 				}
 				if (aux->info.denumire) {
 					free(aux->info.denumire);
 				}
 				free(aux);
 			}
 			else {
 				p = NULL;
 			}
 		}
 	}
 }
 
 float calculeazaBugetManager(Nod* cap, const char* numeManager) {
 	float suma = 0;
 	while (cap) {
 		if (strcmp(cap->info.numeManager, numeManager) == 0) {
 			suma += cap->info.buget;
 		}
 		cap = cap->next;
 	}
 	return suma;
 }
 
 
 int main() {
 	Nod* cap = citireListaProiecteDinFisier("proiecte.txt");
 	afisareListaProiecte(cap);
 	printf("Bugetul mediu este:%.2f\n", calculeazaBugetMediu(cap));
 	printf("Buget manager Popescu:%.2f\n", calculeazaBugetManager(cap, "Popescu"));
 	
 	stergeProiecteDinCategoria(&cap, 'B');
 	printf("Lista dupa stergere categoria B:\n");
 	afisareListaProiecte(cap);
 
 	dezalocareListaProiecte(&cap);
 
 	return 0;
 }