#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAREAS 100
#define MAX_TEXTO 100

typedef struct {
    char descripcion[MAX_TEXTO];
    int completada;
} Tarea;

void mostrar_tareas() {
    Tarea lista[MAX_TAREAS];
    int total_tareas = 0;

    FILE *archivo = fopen("tareas.txt", "r");
    if (archivo == NULL) {
        printf("\n[i] No hay tareas guardadas aún.\n");
        return;
    }

    while (fscanf(archivo, "%d,%[^\n]\n", &lista[total_tareas].completada, lista[total_tareas].descripcion) != EOF) {
        total_tareas++;
        if (total_tareas >= MAX_TAREAS) break;
    }
    fclose(archivo);

    printf("\n=== MI LISTA DE TAREAS ===\n");
    for (int i = 0; i < total_tareas; i++) {
        // Añadimos el número de índice (i + 1) para que el usuario pueda elegirla
        printf("%d. %s %s\n", i + 1, lista[i].completada ? "[X]" : "[ ]", lista[i].descripcion);
    }
    printf("==========================\n");
}

void añadir_tarea() {
    char nueva_descripcion[MAX_TEXTO];
    
    printf("\nEscribe la nueva tarea: ");
    getchar(); 
    fgets(nueva_descripcion, MAX_TEXTO, stdin);
    nueva_descripcion[strcspn(nueva_descripcion, "\n")] = 0;

    FILE *archivo = fopen("tareas.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fprintf(archivo, "0,%s\n", nueva_descripcion);
    fclose(archivo);

    printf("¡Tarea añadida con éxito!\n");
}

// NUEVA FUNCIÓN: Modificar una tarea existente
void completar_tarea() {
    Tarea lista[MAX_TAREAS];
    int total_tareas = 0;

    // 1. LEER EL ARCHIVO COMPLETO A LA RAM
    FILE *archivo_lectura = fopen("tareas.txt", "r");
    if (archivo_lectura == NULL) {
        printf("No hay tareas para completar.\n");
        return;
    }

    while (fscanf(archivo_lectura, "%d,%[^\n]\n", &lista[total_tareas].completada, lista[total_tareas].descripcion) != EOF) {
        total_tareas++;
        if (total_tareas >= MAX_TAREAS) break;
    }
    fclose(archivo_lectura);

    if (total_tareas == 0) {
        printf("La lista está vacía.\n");
        return;
    }

    // Mostrar las tareas para que el usuario elija
    mostrar_tareas();
    int numero_tarea;
    printf("\nIntroduce el número de la tarea que has completado: ");
    scanf("%d", &numero_tarea);

    // Validar que el número sea correcto
    if (numero_tarea < 1 || numero_tarea > total_tareas) {
        printf("Número de tarea no válido.\n");
        return;
    }

    // 2. MODIFICAR EL DATO EN LA MEMORIA (Restamos 1 porque los arrays empiezan en 0)
    lista[numero_tarea - 1].completada = 1;

    // 3. SOBRESCRIBIR EL ARCHIVO CON LOS NUEVOS DATOS (Modo "w" de write)
    FILE *archivo_escritura = fopen("tareas.txt", "w");
    if (archivo_escritura == NULL) {
        printf("Error al actualizar el archivo.\n");
        return;
    }

    for (int i = 0; i < total_tareas; i++) {
        fprintf(archivo_escritura, "%d,%s\n", lista[i].completada, lista[i].descripcion);
    }
    fclose(archivo_escritura);

    printf("¡Tarea marcada como completada!\n");
}

int main() {
    int opcion;

    do {
        printf("\n--- GESTOR DE TAREAS ---\n");
        printf("1. Ver tareas\n");
        printf("2. Añadir tarea\n");
        printf("3. Completar tarea\n"); // Nueva opción
        printf("4. Salir\n");
        printf("Elige una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrar_tareas();
                break;
            case 2:
                añadir_tarea();
                break;
            case 3:
                completar_tarea();
                break;
            case 4:
                printf("¡Pasa un buen verano!\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 4);

    return 0;
}