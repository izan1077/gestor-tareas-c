#include <stdio.h>
#include "db.h" // <-- Solo necesitamos nuestra cabecera para que funcione todo

int main() {
    int opcion;
    
    // Llamamos a la función del módulo db
    inicializar_db();

    do {
        printf("\n--- GESTOR DE TAREAS MODULAR ---\n");
        printf("1. Ver tareas\n");
        printf("2. Añadir tarea\n");
        printf("3. Completar tarea\n");
        printf("4. Salir\n");
        printf("Elige una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: mostrar_tareas(); break;
            case 2: añadir_tarea(); break;
            case 3: completar_tarea(); break;
            case 4: printf("¡Hasta luego!\n"); break;
            default: printf("Opción no válida.\n");
        }
    } while (opcion != 4);

    return 0;
}