#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
    int id;
    int nrPagini;
    float pret;
    char* titlu;
    char* autor;
    char gen;
};
typedef struct StructuraCarte Carte;

struct NodBST {
    Carte carte;
    struct NodBST* stanga;
    struct NodBST* dreapta;
};
typedef struct NodBST NodBST;

Carte citireCarteDinFisier(FILE* file) {
    char buffer[200];
    char sep[3] = ",\n";
    fgets(buffer, 200, file);
    char* aux;
    Carte c;
    aux = strtok(buffer, sep);
    c.id = atoi(aux);
    c.nrPagini = atoi(strtok(NULL, sep));
    c.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    c.titlu = malloc(strlen(aux) + 1);
    strcpy_s(c.titlu, strlen(aux) + 1, aux);
    aux = strtok(NULL, sep);
    c.autor = malloc(strlen(aux) + 1);
    strcpy_s(c.autor, strlen(aux) + 1, aux);
    c.gen = *strtok(NULL, sep);
    return c;
}

void afisareCarte(Carte carte) {
    printf("Id: %d\n", carte.id);
    printf("Nr. pagini: %d\n", carte.nrPagini);
    printf("Pret: %.2f RON\n", carte.pret);
    printf("Titlu: %s\n", carte.titlu);
    printf("Autor: %s\n", carte.autor);
    printf("Gen: %c\n\n", carte.gen);
}

NodBST* initializareNodBST(Carte carte) {
    NodBST* nod = (NodBST*)malloc(sizeof(NodBST));
    nod->carte = carte;
    nod->stanga = NULL;
    nod->dreapta = NULL;
    return nod;
}

void inserareCarteInBST(NodBST** radacina, Carte carte) {
    if (*radacina == NULL) {
        *radacina = initializareNodBST(carte);
    }
    else if (carte.pret < (*radacina)->carte.pret) {
        inserareCarteInBST(&(*radacina)->stanga, carte);
    }
    else {
        inserareCarteInBST(&(*radacina)->dreapta, carte);
    }
}

void afisareInOrdine(NodBST* radacina) {
    if (radacina != NULL) {
        afisareInOrdine(radacina->stanga);
        afisareCarte(radacina->carte);
        afisareInOrdine(radacina->dreapta);
    }
}

void afisarePreOrdine(NodBST* radacina) {
    if (radacina != NULL) {
        afisareCarte(radacina->carte);
        afisarePreOrdine(radacina->stanga);
        afisarePreOrdine(radacina->dreapta);
    }
}

NodBST* cautaCarteDupaPret(NodBST* radacina, float pretCautat) {
    if (radacina == NULL) {
        return NULL;
    }
    if (radacina->carte.pret == pretCautat) {
        return radacina;
    }
    else if (pretCautat < radacina->carte.pret) {
        return cautaCarteDupaPret(radacina->stanga, pretCautat);
    }
    else {
        return cautaCarteDupaPret(radacina->dreapta, pretCautat);
    }
}

NodBST* getCarteaScumpaDinBST(NodBST* radacina) {
    if (radacina == NULL) {
        return NULL;
    }
    if (radacina->dreapta == NULL) {
        return radacina;
    }
    return getCarteaScumpaDinBST(radacina->dreapta);
}

NodBST* getCarteaIeftinaDinBST(NodBST* radacina) {
    if (radacina == NULL) {
        return NULL;
    }
    if (radacina->stanga == NULL) {
        return radacina;
    }
    return getCarteaIeftinaDinBST(radacina->stanga);
}

void dezalocareBST(NodBST** radacina) {
    if (*radacina != NULL) {
        dezalocareBST(&(*radacina)->stanga);
        dezalocareBST(&(*radacina)->dreapta);
        if ((*radacina)->carte.titlu != NULL) {
            free((*radacina)->carte.titlu);
        }
        if ((*radacina)->carte.autor != NULL) {
            free((*radacina)->carte.autor);
        }
        free(*radacina);
        *radacina = NULL;
    }
}

NodBST* citireBSTDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodBST* radacina = NULL;
    if (f) {
        while (!feof(f)) {
            inserareCarteInBST(&radacina, citireCarteDinFisier(f));
        }
        fclose(f);
    }
    return radacina;
}

int main() {
    NodBST* radacina = citireBSTDinFisier("carti.txt");

    printf("--- Afisare in ordine (pret crescator) ---\n");
    afisareInOrdine(radacina);

    printf("--- Afisare in preordine ---\n");
    afisarePreOrdine(radacina);

    float pretCautat = 44.00f;
    NodBST* nodGasit = cautaCarteDupaPret(radacina, pretCautat);
    if (nodGasit != NULL) {
        printf("--- Cartea cu pretul %.2f RON gasita ---\n", pretCautat);
        afisareCarte(nodGasit->carte);
    }
    else {
        printf("Nu exista nicio carte cu pretul %.2f RON in BST\n", pretCautat);
    }

    NodBST* nodScump = getCarteaScumpaDinBST(radacina);
    if (nodScump != NULL) {
        printf("--- Cartea cea mai scumpa ---\n");
        afisareCarte(nodScump->carte);
    }

    NodBST* nodIeftin = getCarteaIeftinaDinBST(radacina);
    if (nodIeftin != NULL) {
        printf("--- Cartea cea mai ieftina ---\n");
        afisareCarte(nodIeftin->carte);
    }

    dezalocareBST(&radacina);
    return 0;
}