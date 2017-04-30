#ifndef TP_SIMU_H
#define TP_SIMU_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define HV LONG_MAX   // Definimos HV como el valor máximo de unsigned int
#define SECRETARIO 0
#define MEDICO 1
#define PRIORIDAD 2

#define NEGRO  "\x1B[0m"
#define ROJO  "\x1B[31m"

long  tf, t;

//Eventos futuros
long     TPLL,          // Tiempo proxima llegada
        *TPSM,          // Tiempo proxima salida medico
        *TPSS,          // Tiempo proxima salida secretario
        *TPSSP;         // Tiempo proxima salida secretario prioridad

// Datos
long 	IA(),		// Intervalo entre arribos
        TAM(),		// Tiempo de atención del medico
        TAS();		// Tiempo de atención del secretario

// Var. Control
int     M,              // Cantidad de médicos
        N;              // Cantidad de secretarios

// Var. Estado
int     NSS,            // Cantidad de personas para el secretario
        NSM,            // Cantidad de personas para el Médico
        NSP,            // Cantidad de personas para el secretario con prioridad
        *ITOM,          //
        *ITOS,          //
        STAM,          //
        *STOM,          //
        STAS,          //
        *STOS,          //
        SS,             //
        SSM,            //
        SSS,            //
        SLLM,           //
        SLLS,           //
        NTM,            //
        NTS,            //
        SA5,
        TOTAL;

// Var. Resultado
double 	PECM,		// Promedio de Espera en Cola para el Médico
        PECS,		// Promedio de Espera en Cola para el Secretario
	*PTOS,		// Porcentaje de tiempo ocioso secretario
	*PTOM,		// Porcentaje de tiempo ocioso médico
	PPS,		// Promedio de Permanencia en el sistema
	PA5;		// Porcentaje de arrepentidos 5

int k;                  // Indice actual

void condicionesIniciales();    
void calcularResultados();
void mostrarResultados();
void llegada();
int motivoConsulta();
bool arrepentimiento();
void salidaSecretario();
int secretarioLibre();
int medicoLibre();
int proxima_salida();

double r();  // Random 0 < r() < 1

#endif /* TP_SIMU_H */

