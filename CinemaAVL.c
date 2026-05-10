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

struct NodAVL {
    Film film;
    int inaltime;
    struct NodAVL* stanga;
    struct NodAVL* dreapta;
};
typedef struct NodAVL NodAVL;

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

NodAVL* initializareNodAVL(Film film) {
    NodAVL* nod = (NodAVL*)malloc(sizeof(NodAVL));
    nod->film = film;
    nod->inaltime = 1;
    nod->stanga = NULL;
    nod->dreapta = NULL;
    return nod;
}

int getInaltime(NodAVL* nod) {
    if (nod == NULL) {
        return 0;
    }
    return nod->inaltime;
}

int getMax(int a, int b) {
    return (a > b) ? a : b;
}

int getFactorEchilibru(NodAVL* nod) {
    if (nod == NULL) {
        return 0;
    }
    return getInaltime(nod->stanga) - getInaltime(nod->dreapta);
}

NodAVL* rotatieDreapta(NodAVL* y) {
    NodAVL* x = y->stanga;
    NodAVL* T2 = x->dreapta;
    x->dreapta = y;
    y->stanga = T2;
    y->inaltime = getMax(getInaltime(y->stanga), getInaltime(y->dreapta)) + 1;
    x->inaltime = getMax(getInaltime(x->stanga), getInaltime(x->dreapta)) + 1;
    return x;
}

NodAVL* rotatieStanga(NodAVL* x) {
    NodAVL* y = x->dreapta;
    NodAVL* T2 = y->stanga;
    y->stanga = x;
    x->dreapta = T2;
    x->inaltime = getMax(getInaltime(x->stanga), getInaltime(x->dreapta)) + 1;
    y->inaltime = getMax(getInaltime(y->stanga), getInaltime(y->dreapta)) + 1;
    return y;
}

NodAVL* inserareFilmInAVL(NodAVL* radacina, Film film) {
    if (radacina == NULL) {
        return initializareNodAVL(film);
    }
    if (film.rating < radacina->film.rating) {
        radacina->stanga = inserareFilmInAVL(radacina->stanga, film);
    }
    else {
        radacina->dreapta = inserareFilmInAVL(radacina->dreapta, film);
    }
    radacina->inaltime = getMax(getInaltime(radacina->stanga), getInaltime(radacina->dreapta)) + 1;
    int fe = getFactorEchilibru(radacina);

    // caz stanga-stanga
    if (fe > 1 && film.rating < radacina->stanga->film.rating) {
        return rotatieDreapta(radacina);
    }
    // caz dreapta-dreapta
    if (fe < -1 && film.rating >= radacina->dreapta->film.rating) {
        return rotatieStanga(radacina);
    }
    // caz stanga-dreapta
    if (fe > 1 && film.rating >= radacina->stanga->film.rating) {
        radacina->stanga = rotatieStanga(radacina->stanga);
        return rotatieDreapta(radacina);
    }
    // caz dreapta-stanga
    if (fe < -1 && film.rating < radacina->dreapta->film.rating) {
        radacina->dreapta = rotatieDreapta(radacina->dreapta);
        return rotatieStanga(radacina);
    }
    return radacina;
}

void afisareInOrdine(NodAVL* radacina) {
    if (radacina != NULL) {
        afisareInOrdine(radacina->stanga);
        afisareFilm(radacina->film);
        afisareInOrdine(radacina->dreapta);
    }
}

NodAVL* cautaFilmDupaRating(NodAVL* radacina, float ratingCautat) {
    if (radacina == NULL) {
        return NULL;
    }
    if (radacina->film.rating == ratingCautat) {
        return radacina;
    }
    else if (ratingCautat < radacina->film.rating) {
        return cautaFilmDupaRating(radacina->stanga, ratingCautat);
    }
    else {
        return cautaFilmDupaRating(radacina->dreapta, ratingCautat);
    }
}

void dezalocareAVL(NodAVL** radacina) {
    if (*radacina != NULL) {
        dezalocareAVL(&(*radacina)->stanga);
        dezalocareAVL(&(*radacina)->dreapta);
        if ((*radacina)->film.titlu != NULL) {
            free((*radacina)->film.titlu);
        }
        if ((*radacina)->film.regizor != NULL) {
            free((*radacina)->film.regizor);
        }
        free(*radacina);
        *radacina = NULL;
    }
}

NodAVL* citireAVLDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodAVL* radacina = NULL;
    if (f) {
        while (!feof(f)) {
            radacina = inserareFilmInAVL(radacina, citireFilmDinFisier(f));
        }
        fclose(f);
    }
    return radacina;
}

int main() {
    NodAVL* radacina = citireAVLDinFisier("filme.txt");

    printf("--- Afisare in ordine (rating crescator) ---\n");
    afisareInOrdine(radacina);

    float ratingCautat = 8.80f;
    NodAVL* nodGasit = cautaFilmDupaRating(radacina, ratingCautat);
    if (nodGasit != NULL) {
        printf("--- Film cu ratingul %.2f gasit ---\n", ratingCautat);
        afisareFilm(nodGasit->film);
    }
    else {
        printf("Nu exista niciun film cu ratingul %.2f\n", ratingCautat);
    }

    dezalocareAVL(&radacina);
    return 0;
}