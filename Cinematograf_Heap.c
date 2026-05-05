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

struct Heap {
    int lungime;
    Film* vector;
    int nrFilme;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
    Heap heap;
    heap.lungime = lungime;
    heap.nrFilme = 0;
    heap.vector = (Film*)malloc(sizeof(Film) * lungime);
    return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int pozFiuSt = 2 * pozitieNod + 1;
    int pozFiuDr = 2 * pozitieNod + 2;
    int pozMax = pozitieNod;
    if (pozFiuSt < heap.nrFilme && heap.vector[pozMax].rating < heap.vector[pozFiuSt].rating) {
        pozMax = pozFiuSt;
    }
    if (pozFiuDr < heap.nrFilme && heap.vector[pozMax].rating < heap.vector[pozFiuDr].rating) {
        pozMax = pozFiuDr;
    }
    if (pozMax != pozitieNod) {
        Film aux = heap.vector[pozMax];
        heap.vector[pozMax] = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = aux;
        if (pozMax <= (heap.nrFilme - 2) / 2) {
            filtreazaHeap(heap, pozMax);
        }
    }
}

Heap citireHeapDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Heap heap = initializareHeap(10);
    while (!feof(file)) {
        heap.vector[heap.nrFilme++] = citireFilmDinFisier(file);
    }
    fclose(file);
    for (int i = (heap.nrFilme - 2) / 2; i >= 0; i--) {
        filtreazaHeap(heap, i);
    }
    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrFilme; i++) {
        afisareFilm(heap.vector[i]);
    }
}

void afisareHeapAscuns(Heap heap) {
    for (int i = heap.nrFilme; i < heap.lungime; i++) {
        afisareFilm(heap.vector[i]);
    }
}

Film extrageFilm(Heap* heap) {
    Film extras = heap->vector[0];
    heap->vector[0] = heap->vector[heap->nrFilme - 1];
    heap->vector[heap->nrFilme - 1] = extras;
    heap->nrFilme--;
    for (int i = (heap->nrFilme - 2) / 2; i >= 0; i--) {
        filtreazaHeap(*heap, i);
    }
    return extras;
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->lungime; i++) {
        if (heap->vector[i].titlu != NULL) {
            free(heap->vector[i].titlu);
        }
        if (heap->vector[i].regizor != NULL) {
            free(heap->vector[i].regizor);
        }
    }
    free(heap->vector);
    heap->vector = NULL;
    heap->lungime = 0;
    heap->nrFilme = 0;
}

int main() {
    Heap heap = citireHeapDinFisier("filme.txt");

    printf("--- Heap initial (Max-Heap dupa rating) ---\n");
    afisareHeap(heap);

    printf("--- Extragere filme in ordinea ratingului ---\n");
    while (heap.nrFilme > 0) {
        afisareFilm(extrageFilm(&heap));
    }

    printf("--- Zona ascunsa (filme sortate descrescator) ---\n");
    afisareHeapAscuns(heap);

    dezalocareHeap(&heap);
    return 0;
}