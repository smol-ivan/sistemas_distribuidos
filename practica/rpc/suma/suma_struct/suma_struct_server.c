/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "suma_struct.h"

int *suma_1_svc(operandos *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	 result = argp->primero + argp->segundo;

	return &result;
}
