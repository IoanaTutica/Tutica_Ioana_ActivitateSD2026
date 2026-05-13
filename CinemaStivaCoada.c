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

struct NodStiva {
    Film film;
    struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

struct NodCoada {
    Film film;
    struct NodCoada* next;
};
typedef struct NodCoada NodCoada;

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

void pushFilm(NodStiva** varf, Film film) {
    NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
    nou->film = film;
    nou->next = *varf;
    *varf = nou;
}

Film popFilm(NodStiva** varf) {
    Film filmGol;
    filmGol.id = -1;
    filmGol.titlu = NULL;
    filmGol.regizor = NULL;
    if (*varf == NULL) {
        printf("Stiva este goala\n");
        return filmGol;
    }
    NodStiva* temp = *varf;
    Film extras = temp->film;
    *varf = (*varf)->next;
    free(temp);
    return extras;
}

void peekStiva(NodStiva* varf) {
    if (varf == NULL) {
        printf("Stiva este goala\n");
        return;
    }
    printf("Varful stivei:\n");
    afisareFilm(varf->film);
}

int esteGoalaStiva(NodStiva* varf) {
    return varf == NULL;
}

void dezalocareStiva(NodStiva** varf) {
    while (*varf != NULL) {
        NodStiva* temp = *varf;
        *varf = (*varf)->next;
        if (temp->film.titlu != NULL) {
            free(temp->film.titlu);
        }
        if (temp->film.regizor != NULL) {
            free(temp->film.regizor);
        }
        free(temp);
    }
}

void enqueueFilm(NodCoada** cap, NodCoada** coada, Film film) {
    NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
    nou->film = film;
    nou->next = NULL;
    if (*cap == NULL) {
        *cap = nou;
        *coada = nou;
    }
    else {
        (*coada)->next = nou;
        *coada = nou;
    }
}

Film dequeueFilm(NodCoada** cap, NodCoada** coada) {
    Film filmGol;
    filmGol.id = -1;
    filmGol.titlu = NULL;
    filmGol.regizor = NULL;
    if (*cap == NULL) {
        printf("Coada este goala\n");
        return filmGol;
    }
    NodCoada* temp = *cap;
    Film extras = temp->film;
    *cap = (*cap)->next;
    if (*cap == NULL) {
        *coada = NULL;
    }
    free(temp);
    return extras;
}

void peekCoada(NodCoada* cap) {
    if (cap == NULL) {
        printf("Coada este goala\n");
        return;
    }
    printf("Fata cozii:\n");
    afisareFilm(cap->film);
}

int esteGoalaCoada(NodCoada* cap) {
    return cap == NULL;
}

void dezalocareCoada(NodCoada** cap, NodCoada** coada) {
    while (*cap != NULL) {
        NodCoada* temp = *cap;
        *cap = (*cap)->next;
        if (temp->film.titlu != NULL) {
            free(temp->film.titlu);
        }
        if (temp->film.regizor != NULL) {
            free(temp->film.regizor);
        }
        free(temp);
    }
    *coada = NULL;
}

int main() {
    FILE* f = fopen("filme.txt", "r");
    NodStiva* stiva = NULL;
    NodCoada* capCoada = NULL;
    NodCoada* coadaCoada = NULL;

    if (f) {
        while (!feof(f)) {
            Film film = citireFilmDinFisier(f);
            pushFilm(&stiva, film);
            enqueueFilm(&capCoada, &coadaCoada, film);
        }
        fclose(f);
    }

    printf("--- Peek stiva (ultimul film adaugat) ---\n");
    peekStiva(stiva);

    printf("--- Peek coada (primul film adaugat) ---\n");
    peekCoada(capCoada);

    printf("--- Extragere din STIVA (LIFO) ---\n");
    while (!esteGoalaStiva(stiva)) {
        afisareFilm(popFilm(&stiva));
    }

    printf("--- Extragere din COADA (FIFO) ---\n");
    while (!esteGoalaCoada(capCoada)) {
        afisareFilm(dequeueFilm(&capCoada, &coadaCoada));
    }

    return 0;
}