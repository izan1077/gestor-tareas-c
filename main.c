#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> // <-- La nueva librería mágica

// Función auxiliar para inicializar la base de datos y crear la tabla si no existe
void inicializar_db() {
    sqlite3 *db;
    char *err_msg = 0;
    
    // Abre (o crea) el archivo de la base de datos
    int rc = sqlite3_open("tareas.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Código SQL para crear la tabla
    char *sql = "CREATE TABLE IF NOT EXISTS tareas("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "descripcion TEXT NOT NULL,"
                "completada INTEGER DEFAULT 0);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error de SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    sqlite3_close(db);
}

// Función para mostrar las tareas ejecutando un SELECT
void mostrar_tareas() {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    sqlite3_open("tareas.db", &db);
    
    char *sql = "SELECT id, descripcion, completada FROM tareas;";
    
    // Preparamos la consulta SQL
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al consultar: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== MI LISTA DE TAREAS (SQL) ===\n");
    // Recorremos los resultados fila por fila
    while (sqlite3_step(res) == SQLITE_ROW) {
        int id = sqlite3_column_int(res, 0);
        const unsigned char *desc = sqlite3_column_text(res, 1);
        int comp = sqlite3_column_int(res, 2);
        
        printf("[%d] %s %s\n", id, comp ? "[X]" : "[ ]", desc);
    }
    printf("=================================\n");

    sqlite3_finalize(res);
    sqlite3_close(db);
}

// Función para añadir una tarea usando INSERT
void añadir_tarea() {
    sqlite3 *db;
    sqlite3_stmt *res;
    char nueva_descripcion[100];
    
    printf("\nEscribe la nueva tarea: ");
    getchar(); 
    fgets(nueva_descripcion, 100, stdin);
    nueva_descripcion[strcspn(nueva_descripcion, "\n")] = 0;

    sqlite3_open("tareas.db", &db);
    
    // Usamos '?' para evitar ataques de inyección SQL (buenas prácticas)
    char *sql = "INSERT INTO tareas (descripcion) VALUES (?);";
    
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_text(res, 1, nueva_descripcion, -1, SQLITE_STATIC);
    
    sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);

    printf("¡Tarea guardada en la base de datos!\n");
}

// Función para completar una tarea usando UPDATE
void completar_tarea() {
    sqlite3 *db;
    sqlite3_stmt *res;
    int id_tarea;

    mostrar_tareas();
    printf("\nIntroduce el ID [número entre corchetes] de la tarea completada: ");
    scanf("%d", &id_tarea);

    sqlite3_open("tareas.db", &db);
    
    char *sql = "UPDATE tareas SET completada = 1 WHERE id = ?;";
    
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_int(res, 1, id_tarea);
    
    sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);

    printf("¡Base de datos actualizada!\n");
}

int main() {
    int opcion;
    
    // Inicializamos la base de datos antes de mostrar el menú
    inicializar_db();

    do {
        printf("\n--- GESTOR DE TAREAS (EDICIÓN SQLITE) ---\n");
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