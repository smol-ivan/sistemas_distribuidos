/*
 * Esta es la manera estándar de pasar más de un valor
 * en la función.
 */
struct operandos {
	int primero;
	int segundo;
};
program CALC_PROG {				/* número de programa */
	version CALC_VER {		        /* número de versión  */
		int suma(operandos) = 1;		/* función remota */
	} = 1;
} = 0x20000199;
