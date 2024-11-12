
#include "suma_struct.h"


int main (int argc, char *argv[])
{
    char *host;

    if (argc < 4) {
        printf ("usage: %s server_host num1 num2\n", argv[0]);
        exit (1);
    }
    host = argv[1];

    struct operandos args;
    sscanf(argv[2], "%d", &(args.primero));
    sscanf(argv[3], "%d", &(args.segundo));

    CLIENT *clnt;
    int  *result_1;

    clnt = clnt_create (host, CALC_PROG, CALC_VER, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        exit (1);
    }

    /* Acá se hace la invocación REMOTA */	
    result_1 = suma_1(&args, clnt);

    if (result_1 == (int *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    else {
        printf("Operación: %d + %d = %d\n", 
                args.primero, args.segundo, *result_1); 
    }

    clnt_destroy (clnt);

    exit (0);
}
