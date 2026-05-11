#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIMENSIUNE 10

struct StructuraFilm {
    int id;
    int durata;
    float rating;
    char* titlu;
    char* regizor;
    char gen;
};
typedef struct StructuraFilm Film;

struct Nod {
    Film film;
    struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
    Nod* elemente[DIMENSIUNE];
};
typedef struct HashTable HashTable;

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

void initializareHashTable(HashTable* ht) {
    for (int i = 0; i < DIMENSIUNE; i++) {
        ht->elemente[i] = NULL;
    }
}

int hash(float rating) {
    return (int)(rating * 100) % DIMENSIUNE;
}

void inserareFilmInHashTable(HashTable* ht, Film film) {
    int pozitie = hash(film.rating);
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->film = film;
    nodNou->next = ht->elemente[pozitie];
    ht->elemente[pozitie] = nodNou;
}

Nod* cautaFilmDupaRating(HashTable ht, float ratingCautat) {
    int pozitie = hash(ratingCautat);
    Nod* curent = ht.elemente[pozitie];
    while (curent != NULL) {
        if (curent->film.rating == ratingCautat) {
            return curent;
        }
        curent = curent->next;
    }
    return NULL;
}

void afisareHashTable(HashTable ht) {
    for (int i = 0; i < DIMENSIUNE; i++) {
        printf("-- Pozitia %d --\n", i);
        Nod* curent = ht.elemente[i];
        if (curent == NULL) {
            printf("(goala)\n\n");
        }
        else {
            while (curent != NULL) {
                afisareFilm(curent->film);
                curent = curent->next;
            }
        }
    }
}

void dezalocareHashTable(HashTable* ht) {
    for (int i = 0; i < DIMENSIUNE; i++) {
        Nod* curent = ht->elemente[i];
        while (curent != NULL) {
            Nod* urmator = curent->next;
            if (curent->film.titlu != NULL) {
                free(curent->film.titlu);
            }
            if (curent->film.regizor != NULL) {
                free(curent->film.regizor);
            }
            free(curent);
            curent = urmator;
        }
        ht->elemente[i] = NULL;
    }
}

HashTable citireHashTableDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    HashTable ht;
    initializareHashTable(&ht);
    if (f) {
        while (!feof(f)) {
            inserareFilmInHashTable(&ht, citireFilmDinFisier(f));
        }
        fclose(f);
    }
    return ht;
}

int main() {
    HashTable ht = citireHashTableDinFisier("filme.txt");

    printf("=== Continutul Hash Table-ului ===\n");
    afisareHashTable(ht);

    float ratingCautat = 9.00f;
    Nod* nodGasit = cautaFilmDupaRating(ht, ratingCautat);
    if (nodGasit != NULL) {
        printf("=== Film cu ratingul %.2f gasit ===\n", ratingCautat);
        afisareFilm(nodGasit->film);
    }
    else {
        printf("Nu exista niciun film cu ratingul %.2f\n", ratingCautat);
    }

    dezalocareHashTable(&ht);
    return 0;
}