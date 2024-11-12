#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define ETHSIZE 400
#define PRECIO 50

struct Servidor
{
    bool taxis[5];
    int ganancias;
    int viajes;
};

void inicializarServidor(struct Servidor *servidor)
{
    servidor->taxis[0] = true;
    servidor->taxis[1] = true;
    servidor->taxis[2] = true;
    servidor->taxis[3] = true;
    servidor->taxis[4] = false;
    servidor->ganancias = 0;
    servidor->viajes = 0;
}

// Funcion para decodificar usando sscanf
// sintaxis usada: "{texto}|{numero}"

void s_decodificar(char *buffer, char *texto, int *numero)
{
    sscanf(buffer, "%[^|]|%d", texto, numero);
}

void s_codificar(char *buffer, char *texto, int numero)
{
    sprintf(buffer, "%s|%d", texto, numero);
}

int buscar_taxi(struct Servidor *servidor)
{
    for (int i = 0; i < 5; i++)
    {
        if (!servidor->taxis[i])
        {
            servidor->taxis[i] = true;

            servidor->ganancias += PRECIO;
            servidor->viajes++;
            return i;
        }
    }
    return -1;
}

void terminar_viaje(struct Servidor *servidor, int placa)
{
    // La placa es el indice del taxi
    servidor->taxis[placa] = false;
}

void estado(struct Servidor *servidor, int *ganancia, int *viajes)
{
    *ganancia = servidor->ganancias;
    *viajes = servidor->viajes;

    printf("Ganancia total: %d\n", *ganancia);
    printf("# de viajes realizados: %d\n", *viajes);

    // Imprimir la disponibilidad de los taxis
    for (int i = 0; i < 5; i++)
    {
        printf("Taxi %d: %s\n", i, servidor->taxis[i] ? "Ocupado" : "Libre");
    }
}
