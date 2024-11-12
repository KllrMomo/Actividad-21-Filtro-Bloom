#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BIT_ARRAY_SIZE 1000 // Tamaño del filtro de Bloom (ajústalo según la cantidad de datos)

// Estructura del filtro de Bloom
struct FiltroBloom {
    unsigned char *array_bit;
};

// Función hash 1
int hashPrimary(char *str) {
    unsigned int sum = 0;
    while (*str) {
        sum = (sum << 5) + *str;
        str++;
    }
    return sum % BIT_ARRAY_SIZE;
}

// Función hash 2
int hashSecond(char *str) {
    unsigned int sum = 0;
    while (*str) {
        sum = (sum << 3) + *str;
        str++;
    }
    return sum % BIT_ARRAY_SIZE;
}

// Función hash 3
int hashThird(char *str) {
    unsigned int sum = 0;
    while (*str) {
        sum = (sum << 2) + *str;
        str++;
    }
    return sum % BIT_ARRAY_SIZE;
}

// Inicializar el filtro de Bloom (arreglo de bits)
void initArrayBits(struct FiltroBloom* filter) {
    filter->array_bit = (unsigned char*) calloc(BIT_ARRAY_SIZE / 8, sizeof(unsigned char));
}

// Agregar una cadena al filtro de Bloom
void addArrayBits(struct FiltroBloom* filter, char *str) {
    int indexHash1 = hashPrimary(str);
    int indexHash2 = hashSecond(str);
    int indexHash3 = hashThird(str);

    filter->array_bit[indexHash1 / 8] |= (1 << (indexHash1 % 8));
    filter->array_bit[indexHash2 / 8] |= (1 << (indexHash2 % 8));
    filter->array_bit[indexHash3 / 8] |= (1 << (indexHash3 % 8));
}

// Verificar si una cadena posiblemente está en el filtro
bool findString(struct FiltroBloom* filter, char* str) {
    int hash1 = hashPrimary(str);
    int hash2 = hashSecond(str);
    int hash3 = hashThird(str);

    return (filter->array_bit[hash1 / 8] & (1 << (hash1 % 8))) &&
           (filter->array_bit[hash2 / 8] & (1 << (hash2 % 8))) &&
           (filter->array_bit[hash3 / 8] & (1 << (hash3 % 8)));
}

// Imprimir el arreglo de bits (para depuración)
void printArrayBits(struct FiltroBloom* filter) {
    for (int i = 0; i < (BIT_ARRAY_SIZE / 8); i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", (filter->array_bit[i] >> (7 - j)) & 1);
        }
    }
    printf("\n");
}

// Liberar la memoria del filtro de Bloom
void freeFilter(struct FiltroBloom* filter) {
    free(filter->array_bit);
    free(filter);
}

int main() {
    // Crear e inicializar el filtro de Bloom
    struct FiltroBloom* filter = static_cast<struct FiltroBloom *>(malloc(sizeof(struct FiltroBloom)));
    initArrayBits(filter);

    // Agregar cadenas al filtro
    addArrayBits(filter, "Andrea");
    addArrayBits(filter, "Angela");
    addArrayBits(filter, "Angel");

    // Verificar si las cadenas están en el filtro
    char* stringsToCheck[] = {"Andrea", "Ana", "Angela", "Carlos"};
    for (int i = 0; i < 4; i++) {
        if (findString(filter, stringsToCheck[i])) {
            printf("La cadena \"%s\" probablemente está en el filtro.\n", stringsToCheck[i]);
        } else {
            printf("La cadena \"%s\" NO está en el filtro.\n", stringsToCheck[i]);
        }
    }

    // Imprimir el estado del filtro de Bloom
    printf("Estado del arreglo de bits del filtro de Bloom:\n");
    printArrayBits(filter);

    // Liberar memoria
    freeFilter(filter);

    return 0;
}
