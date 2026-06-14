#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "db.h" // Importamos nuestra propia cabecera

void inicializar_db() {
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("tareas.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

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

void mostrar_tareas() {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    sqlite3_open("tareas.db", &db);
    char *sql = "SELECT id, descripcion, completada FROM tareas;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al consultar: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== MI LISTA DE TAREAS (MODULAR) ===\n");
    while (sqlite3_step(res) == SQLITE_ROW) {
        int id = sqlite3_column_int(res, 0);
        const unsigned char *desc = sqlite3_column_text(res, 1);
        int comp = sqlite3_column_int(res, 2);
        
        printf("[%d] %s %s\n", id, comp ? "[X]" : "[ ]", desc);
    }
    printf("=====================================\n");

    sqlite3_finalize(res);
    sqlite3_close(db);
}

void añadir_tarea() {
    sqlite3 *db;
    sqlite3_stmt *res;
    char nueva_descripcion[100];
    
    printf("\nEscribe la nueva tarea: ");
    getchar(); 
    fgets(nueva_descripcion, 100, stdin);
    nueva_descripcion[strcspn(nueva_descripcion, "\n")] = 0;

    sqlite3_open("tareas.db", &db);
    char *sql = "INSERT INTO tareas (descripcion) VALUES (?);";
    
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_text(res, 1, nueva_descripcion, -1, SQLITE_STATIC);
    
    sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);

    printf("¡Tarea guardada en la base de datos!\n");
}

void completar_tarea() {
    sqlite3 *db;
    sqlite3_stmt *res;
    int id_tarea;

    mostrar_tareas();
    printf("\nIntroduce el ID de la tarea completada: ");
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