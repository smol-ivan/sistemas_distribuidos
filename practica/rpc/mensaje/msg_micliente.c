/* msg_micliente.c */

#include <stdio.h>
#include <rpc/rpc.h> /* Siempre se requiere para rpc*/
#include "msg.h"     /* msg.h será generado por rpcgen */

int main(int argc, char *argv[])
{ 
   CLIENT *cl;  /* referencia hacia el servidor */
   
   int *result;
   char *server;
   char *message;
      
   if (argc != 3)
   { 
      fprintf(stderr,"Uso: %s <nombre_host> <mensaje>\n", argv[0]);
      exit(1);
   }
   
   /* Salva los valores de la línea de comandos*/
   server = argv[1];
   message = argv[2];
   
   
   /* Crea la referencia del Cliente para llamar MESSAGEPROG en el
    * servidor pasado como parámetro. Indicamos que el protocolo a
    * utilizar es "tcp" */
   
   cl = clnt_create(server, MESSAGEPROG, MESSAGEVERS, "tcp");
   
   if (cl == NULL)
   { /* No se estableció comunicación con el cliente */
      clnt_pcreateerror (server);
      exit(1);
   }
   
   /* Invocación al procedimiento remoto "printmessage" */
   result = printmessage_1(&message, cl);
   
   if (result == NULL)
   { /* Error ocurrido durante la invocación */
      clnt_perror (cl, server);
      exit(1);
   }
   
   /* Llamado exitoso */
   if (*result == 0)
   { /* No se pudo imprimir el mensaje */
      fprintf(stderr, "%s: %s no pudo imprimir su mensaje\n",
              argv[0], server);
      exit(1);
   }
   
   /* El mensaje se imprimió en la consola del servidor */
   printf("Mensaje entregado a %s!\n", server);
   return 0;
}
