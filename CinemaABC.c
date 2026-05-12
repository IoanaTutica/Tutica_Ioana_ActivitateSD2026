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

struct NodABC {
    int nrFilme;
    Film filme[3];
    struct NodABC* fii[4];
    int esteFrunza;
};
typedef struct NodABC NodABC;

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

NodABC* initializareNodABC() {
    NodABC* nod = (NodABC*)malloc(sizeof(NodABC));
    nod->nrFilme = 0;
    nod->esteFrunza = 1;
    for (int i = 0; i < 4; i++) {
        nod->fii[i] = NULL;
    }
    return nod;
}

void inserareInNod(NodABC* nod, Film film, NodABC* fiuDreapta) {
    int i = nod->nrFilme - 1;
    while (i >= 0 && nod->filme[i].rating > film.rating) {
        nod->filme[i + 1] = nod->filme[i];
        nod->fii[i + 2] = nod->fii[i + 1];
        i--;
    }
    nod->filme[i + 1] = film;
    nod->fii[i + 2] = fiuDreapta;
    nod->nrFilme++;
}

Film splitNod(NodABC* nod, NodABC** nodDrept) {
    *nodDrept = initializareNodABC();
    (*nodDrept)->esteFrunza = nod->esteFrunza;

    Film filmMijloc = nod->filme[1];

    (*nodDrept)->filme[0] = nod->filme[2];
    (*nodDrept)->fii[0] = nod->fii[2];
    (*nodDrept)->fii[1] = nod->fii[3];
    (*nodDrept)->nrFilme = 1;

    nod->nrFilme = 1;
    nod->fii[2] = NULL;
    nod->fii[3] = NULL;

    return filmMijloc;
}

int inserareRecursiva(NodABC* nod, Film film, Film* filmPromovat, NodABC** fiuDrept) {
    int splitFacut = 0;

    if (nod->esteFrunza) {
        inserareInNod(nod, film, NULL);
    }
    else {
        int i = nod->nrFilme - 1;
        while (i > 0 && film.rating < nod->filme[i].rating) {
            i--;
        }
        if (film.rating >= nod->filme[i].rating) {
            i++;
        }

        Film filmSub;
        NodABC* fiuSub = NULL;
        splitFacut = inserareRecursiva(nod->fii[i], film, &filmSub, &fiuSub);

        if (splitFacut) {
            inserareInNod(nod, filmSub, fiuSub);
            splitFacut = 0;
        }
    }

    if (nod->nrFilme == 3) {
        *filmPromovat = splitNod(nod, fiuDrept);
        return 1;
    }
    return 0;
}

void inserareFilmInABC(NodABC** radacina, Film film) {
    Film filmPromovat;
    NodABC* fiuDrept = NULL;
    int splitFacut = inserareRecursiva(*radacina, film, &filmPromovat, &fiuDrept);

    if (splitFacut) {
        NodABC* radacinaNoua = initializareNodABC();
        radacinaNoua->esteFrunza = 0;
        radacinaNoua->filme[0] = filmPromovat;
        radacinaNoua->fii[0] = *radacina;
        radacinaNoua->fii[1] = fiuDrept;
        radacinaNoua->nrFilme = 1;
        *radacina = radacinaNoua;
    }
}

void afisareInOrdine(NodABC* radacina) {
    if (radacina != NULL) {
        if (radacina->esteFrunza) {
            for (int i = 0; i < radacina->nrFilme; i++) {
                afisareFilm(radacina->filme[i]);
            }
        }
        else {
            afisareInOrdine(radacina->fii[0]);
            afisareFilm(radacina->filme[0]);
            afisareInOrdine(radacina->fii[1]);
            if (radacina->nrFilme == 2) {
                afisareFilm(radacina->filme[1]);
                afisareInOrdine(radacina->fii[2]);
            }
        }
    }
}

NodABC* cautaFilmDupaRating(NodABC* radacina, float ratingCautat) {
    if (radacina == NULL) {
        return NULL;
    }
    for (int i = 0; i < radacina->nrFilme; i++) {
        if (radacina->filme[i].rating == ratingCautat) {
            return radacina;
        }
    }
    if (radacina->esteFrunza) {
        return NULL;
    }
    int i = 0;
    while (i < radacina->nrFilme && ratingCautat > radacina->filme[i].rating) {
        i++;
    }
    return cautaFilmDupaRating(radacina->fii[i], ratingCautat);
}

void dezalocareABC(NodABC** radacina) {
    if (*radacina != NULL) {
        if (!(*radacina)->esteFrunza) {
            for (int i = 0; i <= (*radacina)->nrFilme; i++) {
                dezalocareABC(&(*radacina)->fii[i]);
            }
        }
        for (int i = 0; i < (*radacina)->nrFilme; i++) {
            if ((*radacina)->filme[i].titlu != NULL) {
                free((*radacina)->filme[i].titlu);
            }
            if ((*radacina)->filme[i].regizor != NULL) {
                free((*radacina)->filme[i].regizor);
            }
        }
        free(*radacina);
        *radacina = NULL;
    }
}

NodABC* citireABCDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodABC* radacina = initializareNodABC();
    if (f) {
        while (!feof(f)) {
            inserareFilmInABC(&radacina, citireFilmDinFisier(f));
        }
        fclose(f);
    }
    return radacina;
}

int main() {
    NodABC* radacina = citireABCDinFisier("filme.txt");

    printf("=== Afisare in ordine (rating crescator) ===\n");
    afisareInOrdine(radacina);

    float ratingCautat = 8.70f;
    NodABC* nodGasit = cautaFilmDupaRating(radacina, ratingCautat);
    if (nodGasit != NULL) {
        printf("=== Film cu ratingul %.2f gasit ===\n", ratingCautat);
        for (int i = 0; i < nodGasit->nrFilme; i++) {
            if (nodGasit->filme[i].rating == ratingCautat) {
                afisareFilm(nodGasit->filme[i]);
            }
        }
    }
    else {
        printf("Nu exista niciun film cu ratingul %.2f\n", ratingCautat);
    }

    dezalocareABC(&radacina);
    return 0;
}