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

//Constante
#define HV LONG_MAX   // Definimos HV como el valor máximo de long
#define MEDICO 0
#define SECRETARIO 1
#define PRIORIDAD 2
#define LLEGADA 3

//Colores para Debug
#define NEGRO  "\x1B[0m"
#define ROJO  "\x1B[31m"
#define VERDE  "\x1B[32m"
#define AMARILLO  "\x1B[33m"

long  tf, t;

//Eventos futuros
long     TPLL,          // Tiempo proxima llegada
        *TPSM,          // Tiempo proxima salida medico
        *TPSS,          // Tiempo proxima salida secretario
        *TPSSP;         // Tiempo proxima salida secretario prioridad

// Datos
long    IA(),           // Intervalo entre arribos
        TAM(),          // Tiempo de atención del medico
        TAS();          // Tiempo de atención del secretario

// Var. Control
int     M,              // Cantidad de médicos
        N;              // Cantidad de secretarios

// Var. Estado
int     NSS,            // Cantidad de personas para el secretario
        NSM,            // Cantidad de personas para el Médico
        NSP,            // Cantidad de personas para el secretario con prioridad
        *ITOM,          // Inicio tiempo ocioso médico
        *ITOS,          // Inicio tiempo ocioso secretario
        STAM,           // Sumatoria tiempo de atencion médico
        *STOM,          // Sumatoria tiempo ocioso médico
        STAS,           // Sumatoria tiempo de atencion secretario
        *STOS,          // Sumatoria tiempo ocioso secretario
        SS,             // Sumatoria salida
        SSM,            // Sumatoria salida médico
        SSS,            // Sumatoria salida secretario
        SLLM,           // Sumatoria llegada médico
        SLLS,           // Sumatoria llegada secretario
        NTM,            // Total de personas atendidas por médicos
        NTS;            // Total de personas atendidas por secretarios

// Var. Resultado
double  PECM,           // Promedio de Espera en Cola para el Médico
        PECS,           // Promedio de Espera en Cola para el Secretario
        *PTOS,          // Porcentaje de tiempo ocioso secretario
        *PTOM;          // Porcentaje de tiempo ocioso médico


int k;                  // Indice actual

void condicionesIniciales(int argc, char** argv);
void calcularResultados();
void mostrarResultados();
void llegada();
int motivoConsulta();
bool arrepentimiento();
void salidaSecretario();
int secretarioLibre();
int medicoLibre();
int proxima_salida();
int prioritariosAtendidos();

double r();  // Random 0 < r() < 1

#endif /* TP_SIMU_H */

