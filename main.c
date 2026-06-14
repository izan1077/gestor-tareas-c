#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAREAS 100
#define MAX_TEXTO 100

// Definimos la estructura de una tarea
typedef struct {
    char descripcion[MAX_TEXTO];
    int completada; // 0 = Pendiente, 1 = Completada
} Tarea;

int main() {
    Tarea lista[MAX_TAREAS];
    int total_tareas = 0;

    // Intentamos abrir el archivo de tareas
    FILE *archivo = fopen("tareas.txt", "r");
    
    if (archivo == NULL) {
        printf("No se encontró el archivo tareas.txt. ¡Crea uno para empezar!\n");
        return 1;
    }

    // Leemos el archivo línea por línea
    // El formato del archivo será: 0,Hacer el paso 3 de Linux
    while (fscanf(archivo, "%d,%[^\n]\n", &lista[total_tareas].completada, lista[total_tareas].descripcion) != EOF) {
        total_tareas++;
        if (total_tareas >= MAX_TAREAS) break;
    }

    fclose(archivo);

    // Mostramos las tareas de forma bonita
    printf("\n=== MI LISTA DE TAREAS DE VERANO ===\n");
    for (int i = 0; i < total_tareas; i++) {
        char *estado = lista[i].completada ? "[X]" : "[ ]";
        printf("%s %s\n", estado, lista[i].descripcion);
    }
    printf("====================================\n\n");

    return 0;
}