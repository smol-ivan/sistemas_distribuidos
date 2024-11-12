#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "practica.h"

struct sockaddr_in serv;

int main(int argc, char *argv[])
{
    int fd, idb, result_sendto;
    char buffer[ETHSIZE];
    char solicitud[ETHSIZE];
    char respuesta[ETHSIZE];

    if (argc != 4)
    {
        printf("\n\nSintaxis: cliente <direccion IP> <# puerto> <Operacion>\n\n");
        printf("Operaciones:\n");
        printf("1. Estado del servidor\n");
        printf("2. Solicitud de viaje\n");
        exit(-1);
    }

    // Preparar la solicitud de acuerdo a la operacion
    switch (atoi(argv[3]))
    {
    case 1:
        sprintf(solicitud, "%s", "estado|0");
        break;
    case 2:
        sprintf(solicitud, "%s", "viaje|0");
        break;
    default:
        printf("\n\nOperacion no valida.\n\n");
        exit(-1);
    }

    printf("\nSolicitud: %s\n", solicitud);

    printf("\nAbriendo el socket...\n");
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        printf("\nError, no se pudo abrir el socket.\n\n");
        exit(-1);
    }

    printf("\nSocket abierto.\n");

    printf("\nAsignando atributos al socket...\n");

    memset(&serv, sizeof(serv), 0);
    serv.sin_family = AF_INET;

    serv.sin_addr.s_addr = inet_addr(argv[1]);

    serv.sin_port = htons(atoi(argv[2]));

    // Enviar la solicitud

    result_sendto = sendto(fd, solicitud, ETHSIZE, 0,
                           (const struct sockaddr *)&serv, sizeof(serv));

    if (result_sendto < 0)
    {
        printf("\nProblemas al enviar la peticion.\n\n");
        exit(-1);
    }

    printf("\nLa petición fue enviado al servidor.\n");

    // Recibir la respuesta
    recvfrom(fd, (void *)respuesta, ETHSIZE, 0,
             (struct sockaddr *)NULL, (socklen_t *)NULL);

    char texto[50];
    int numero;

    s_decodificar(respuesta, texto, &numero);

    // Mostrar la respuesta del servidor
    if (numero == -1)
    {
        printf("\nError: %s\n", texto);
        exit(0);
    }

    if (atoi(argv[3]) == 1)
    {
        printf("\nGanancia total: %s\n", texto);
        printf("# de viajes realizados: %d\n", numero);
    }
    else if (atoi(argv[3]) == 2)
    {
        printf("\nPlaca del taxi: %s\n", texto);
        printf("Precio del viaje: %d\n", numero);
        // Solicitar terminacion del viaje despues de un sleep
        sleep(5);
        s_codificar(solicitud, "viaje terminado", atoi(texto));
        sendto(fd, solicitud, ETHSIZE, 0, (const struct sockaddr *)&serv, sizeof(serv));
    }

    return 0;
}
