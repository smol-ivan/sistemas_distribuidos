/* msg_proc.c: implementación del procedimiento "printmessage" */ 

#include <stdio.h> 
#include <rpc/rpc.h>  /* Siempre necesario */ 
#include "msg.h"      /* Generado por rpcgen */ 

/* implementación de "printmessage" */ 

int *printmessage_1_svc(char **msg, struct svc_req *r)
{ 
   static int result; /* debe ser estática*/ 
   FILE *f;
   f = fopen("/dev/console", "w");
   if (f == NULL) 
   {
      result = 0;
      return (&result);
   }
   fprintf(f, "%s\n", *msg);
   fclose(f);
   result = 1;
   return (&result);
}
