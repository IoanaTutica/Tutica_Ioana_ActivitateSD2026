#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraFilm {
    int id;
    int durata;
    float rating;
    char* titlu;
    char* regizor;
    char gen;
};
typedef struct StructuraFilm Film;

typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;

struct NodPrincipal {
    Film info;
    NodSecundar* vecini;
    struct NodPrincipal* next;
};

struct NodSecundar {
    NodPrincipal* nodInfo;
    NodSecundar* next;
};

Film citireFilmDinFisier(FILE* file) {
    char buffer[200];
    char sep[3] = ",\n";
    fgets(buffer, 200, file);
    char* aux;
    Film f;

    aux = strtok(buffer, sep);
    f.id = atoi(aux);
    f.durata = atoi(strtok(NULL, sep));
    f.rating = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    f.titlu = malloc(strlen(aux) + 1);
    strcpy_s(f.titlu, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    f.regizor = malloc(strlen(aux) + 1);
    strcpy_s(f.regizor, strlen(aux) + 1, aux);

    f.gen = *strtok(NULL, sep);
    return f;
}

void afisareFilm(Film film) {
    printf("Id: %d\n", film.id);
    printf("Durata: %d min\n", film.durata);
    printf("Rating: %.2f\n", film.rating);
    printf("Titlu: %s\n", film.titlu);
    printf("Regizor: %s\n", film.regizor);
    printf("Gen: %c\n\n", film.gen);
}

void inserareListaPrincipala(NodPrincipal** cap, Film f) {
    NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
    nou->info = f;
    nou->next = NULL;
    nou->vecini = NULL;
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

void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodInfo) {
    NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
    nou->nodInfo = nodInfo;
    nou->next = *cap;
    *cap = nou;
}

NodPrincipal* cautaNodDupaID(NodPrincipal* cap, int idCautat) {
    while (cap && cap->info.id != idCautat) {
        cap = cap->next;
    }
    return cap;
}

void inserareMuchie(NodPrincipal* graf, int idStart, int idStop) {
    NodPrincipal* nodStart = cautaNodDupaID(graf, idStart);
    NodPrincipal* nodStop = cautaNodDupaID(graf, idStop);
    if (nodStart && nodStop) {
        inserareListaSecundara(&(nodStart->vecini), nodStop);
        inserareListaSecundara(&(nodStop->vecini), nodStart);
    }
}

NodPrincipal* citireNoduriFilmeDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodPrincipal* graf = NULL;
    if (f) {
        while (!feof(f)) {
            inserareListaPrincipala(&graf, citireFilmDinFisier(f));
        }
        fclose(f);
    }
    return graf;
}

void citireMuchiiDinFisier(NodPrincipal* graf, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    while (f && !feof(f)) {
        int idStart = 0;
        int idStop = 0;
        fscanf(f, "%d %d", &idStart, &idStop);
        inserareMuchie(graf, idStart, idStop);
    }
    fclose(f);
}

void dezalocareListaSecundara(NodSecundar** cap) {
    while (*cap) {
        NodSecundar* temp = (*cap);
        (*cap) = (*cap)->next;
        free(temp);
    }
}

void dezalocareNoduriGraf(NodPrincipal** graf) {
    while (*graf) {
        NodPrincipal* temp = (*graf);
        (*graf) = (*graf)->next;
        dezalocareListaSecundara(&(temp->vecini));
        if (temp->info.titlu) {
            free(temp->info.titlu);
        }
        if (temp->info.regizor) {
            free(temp->info.regizor);
        }
        free(temp);
    }
}

// parcurgere in adancime

typedef struct NodStiva NodStiva;
struct NodStiva {
    int id;
    NodStiva* next;
};

void push(NodStiva** cap, int id) {
    NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
    nou->id = id;
    nou->next = *cap;
    *cap = nou;
}

int pop(NodStiva** cap) {
    if (*cap) {
        NodStiva* temp = *cap;
        *cap = (*cap)->next;
        int id = temp->id;
        free(temp);
        return id;
    }
    else {
        return -1;
    }
}

int calculeazaNrNoduriGraf(NodPrincipal* listaPrincipala) {
    int count = 0;
    while (listaPrincipala) {
        count++;
        listaPrincipala = listaPrincipala->next;
    }
    return count;
}

void afisareGrafInAdancime(NodPrincipal* graf, int idPlecare) {
    int nrNoduri = calculeazaNrNoduriGraf(graf);
    unsigned char* vizitate = (unsigned char*)malloc(sizeof(unsigned char) * nrNoduri);
    for (int i = 0; i < nrNoduri; i++) {
        vizitate[i] = 0;
    }
    NodStiva* stiva = NULL;
    vizitate[idPlecare - 1] = 1;
    push(&stiva, idPlecare);
    while (stiva) {
        int idNod = pop(&stiva);
        NodPrincipal* nodCurent = cautaNodDupaID(graf, idNod);
        afisareFilm(nodCurent->info);
        NodSecundar* p = nodCurent->vecini;
        while (p) {
            if (vizitate[p->nodInfo->info.id - 1] == 0) {
                push(&stiva, p->nodInfo->info.id);
                vizitate[p->nodInfo->info.id - 1] = 1;
            }
            p = p->next;
        }
    }
    free(vizitate);
}

// parcurgere in latime

typedef struct NodCoada NodCoada;
struct NodCoada {
    int id;
    NodCoada* next;
};

void enqueue(NodCoada** cap, int id) {
    NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
    nou->id = id;
    nou->next = NULL;
    if (*cap) {
        NodCoada* p = *cap;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
    else {
        *cap = nou;
    }
}

int dequeue(NodCoada** cap) {
    if (*cap) {
        NodCoada* temp = *cap;
        *cap = (*cap)->next;
        int id = temp->id;
        free(temp);
        return id;
    }
    else {
        return -1;
    }
}

void afisareGrafInLatime(NodPrincipal* graf, int idPlecare) {
    int nrNoduri = calculeazaNrNoduriGraf(graf);
    unsigned char* vizitate = (unsigned char*)malloc(sizeof(unsigned char) * nrNoduri);
    for (int i = 0; i < nrNoduri; i++) {
        vizitate[i] = 0;
    }
    NodCoada* coada = NULL;
    vizitate[idPlecare - 1] = 1;
    enqueue(&coada, idPlecare);
    while (coada) {
        int idNod = dequeue(&coada);
        NodPrincipal* nodCurent = cautaNodDupaID(graf, idNod);
        afisareFilm(nodCurent->info);
        NodSecundar* p = nodCurent->vecini;
        while (p) {
            if (vizitate[p->nodInfo->info.id - 1] == 0) {
                enqueue(&coada, p->nodInfo->info.id);
                vizitate[p->nodInfo->info.id - 1] = 1;
            }
            p = p->next;
        }
    }
    free(vizitate);
}

int main() {
    NodPrincipal* graf = NULL;
    graf = citireNoduriFilmeDinFisier("filme.txt");
    citireMuchiiDinFisier(graf, "muchii_filme.txt");

    printf("--- Parcurgere in adancime (DFS) pornind din nodul 1 ---\n");
    afisareGrafInAdancime(graf, 1);

    printf("--- Parcurgere in latime (BFS) pornind din nodul 1 ---\n");
    afisareGrafInLatime(graf, 1);

    dezalocareNoduriGraf(&graf);
    return 0;
}