#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "practica.h"

struct sockaddr_in serv, cli;

void print(char *text)
{
    printf("\n%s\n", text);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        print("Sintaxis -> servidor <puerto>");
        // exit(-1);
    }
    // int PORT = atoi(argv[1]);
    int PORT = 12345;

    int fd, idb, cli_len, size_recv;

    char buff_peticion[ETHSIZE];
    char buff_respuesta[ETHSIZE];

    print("Se creará el socket");

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        print("Error, no se puedo crear el socket");
        exit(-1);
    }
    print("Socket creado");

    print("Asignando atributos al socket");
    memset(&serv, sizeof(serv), 0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if ((idb = bind(fd, (struct sockaddr *)&serv, sizeof(serv))) < 0)
    {
        print("Atributos no asignados");
        exit(-1);
    }

    printf("\nEstoy escuchando al puerto -> %d\n", PORT);

    cli_len = ETHSIZE;

    // Inicar aplicacion

    struct Servidor servidor;

    inicializarServidor(&servidor);

    char texto[50];
    int numero;

    // Ciclo para escuchar peticiones
    while (1)
    {

        // Recibir peticion
        if ((size_recv = recvfrom(fd, (void *)buff_peticion, ETHSIZE, 0, (struct sockaddr *)&cli, (socklen_t *)&cli_len) < 0))
        {
            print("Hubo un problema con el ercvfrom");
        }
        printf("\nMensaje recivido\n");
        printf("Desde -> %s", inet_ntoa(cli.sin_addr));

        printf("\nMensaje -> %s\n", buff_peticion);

        // Decodificar la peticion
        s_decodificar(buff_peticion, texto, &numero);

        printf("\nTexto -> %s\n", texto);
        printf("Numero -> %d\n", numero);

        if (strcmp(texto, "viaje") == 0)
        // Solicitud de un taxi
        {
            int taxi;
            // Si no hay taxis disponibles
            if ((taxi = buscar_taxi(&servidor)) == -1)
            {
                s_codificar(buff_respuesta, "No hay conductores", -1);
                printf("No hay taxis disponibles");
            }
            else
            {

                printf("Taxi asignado -> %d", taxi);

                char placa[2];

                sprintf(placa, "%d", taxi);

                s_codificar(buff_respuesta, placa, PRECIO);
            }
        }
        else if (strcmp(texto, "viaje terminado") == 0)
        // Solicitar terminacion de un viaje
        {
            terminar_viaje(&servidor, numero);
            printf("Viaje terminado");
        }
        else if (strcmp(texto, "estado") == 0)
        // Solicitar el estado del servidor
        {
            // Obtener el estado del servidor
            int ganancia;
            int viajes;
            estado(&servidor, &ganancia, &viajes);

            char ganancia_str[10];

            sprintf(ganancia_str, "%d", ganancia);

            s_codificar(buff_respuesta, ganancia_str, viajes);
        }
        else
        {
            // Peticion invalida
            s_codificar(buff_respuesta, "Peticion invalida", -1);
        }

        // Enviar respuesta

        if (sendto(fd, buff_respuesta, ETHSIZE, 0, (struct sockaddr *)&cli, sizeof(cli)) < 0)
        {
            print("Hubo un problema al enviar la respuesta");
        }

        printf("\nRespuesta enviada\n");

        // Limpiar los buffers
        memset(buff_peticion, 0, ETHSIZE);
        memset(buff_respuesta, 0, ETHSIZE);
    }
    return 0;
}
