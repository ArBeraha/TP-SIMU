#include "TP-SIMU.h"

int main(int argc, char** argv) {
    condicionesIniciales();
    // MODELO
    do {
        int* p;
        int PS = proxima_salida(p);
        int k = *p;
        printf("\tProxima salida K=%d\n", k);
        if (PS == MEDICO) {
            if (TPSM[k] < TPLL) {
                printf("Salida medico\n");
                t = TPSM[k];
                NSM--;
                if (NSM < M) {
                    ITOM[k] = t;
                    TPSM[k] = HV;
                } else {
                    double tam = TAM();
                    TPSM[k] = t + tam;
                    STAM[k] += tam;
                }
                SSM = SS + t;
                NTM++;
                if (r() < 0.3) {
                    printf("Se mete en la cola de prioridad\n");
                    NSP++;
                    if (NSP + NSS <= N) {
                        int l = secretarioLibre();
                        STOS[l] += t - ITOS[l];
                        int tas = TAS();
                        TPSSP[l] = t + tas;
                        STAS[l] += tas;
                    }
                }
            } else
                llegada();
        } else
            if (PS == SECRETARIO) {
            if (TPSS[k] < TPLL) {
                printf("Salida secretarios\n");
                t = TPSS[k];
                NSS--;
                salidaSecretario(k);
            } else
                llegada();
        } else {
            if (TPSSP[k] < TPLL) {
                printf("Salida secretarios prioritario\n");
                t = TPSSP[k];
                NSP--;
                salidaSecretario(k);
            } else
                llegada();
        }

        if (t > tf)
            TPLL = HV;
        printf("Fin ciclo con t=%ld\n y NS=%d", t,NSM + NSS + NSP);
        //usleep(10000);
    } while ((t < tf) || (NSM + NSS + NSP > 0));
    calcularResultados();
    mostrarResultados();
    return (EXIT_SUCCESS);
}

void calcularResultados() {
    int i;
    if (t != 0) {
        for (i = 0; i < N; i++) {
            PTOS[i] = (STOS[i] * 100) / t;
        }
        for (i = 0; i < M; i++) {
            PTOM[i] = (STOM[i] * 100) / t;
        }
    }
    if (TOTAL != 0)
        PA5 = (SA5 * 100) / TOTAL;
    else
        PA5 = 0;
    if (NTM != 0)
        PECM = (SSM - SLLM - STAM[i]) / NTM;
    else
        PECM = 0;
    if (NTS != 0)
        PECS = (SSS - SLLS - STAS[i]) / NTS;
    else
        PECS = 0;
}

void mostrarResultados() {
    int i;
    printf("Resultados\n");

    printf("PTOS:\n");
    for (i = 0; i < N; i++) {
        printf("\tPTOS[%d]=%f\n", i, PTOS[i]);
    }
    for (i = 0; i < M; i++) {
        printf("\tPTOM[%d]=%f\n", i, PTOM[i]);
    }
    printf("PA5=%f\n", PA5);
    printf("PECM=%f\n", PECM);
    printf("PECS=%f\n", PECS);
}

void llegada() {
    printf("llegada TPLL=%ld\n", TPLL);
    int k;
    t = TPLL;
    TPLL = t + IA();
    switch (motivoConsulta()) {
        case MEDICO:
            printf("Motivo medico\n");
            k = medicoLibre();
            NSM++;
            SLLM += t;
            if (NSM <= M) {
                STOM[k] += t - ITOM[k];
                double tam = TAM();
                TPSM[k] = t + tam;
                STAM[k] += tam;
                printf("Sale a las %ld\n", TPSM[k]);
            }
            break;
        case SECRETARIO:
            printf("Motivo secretario\n");
            k = secretarioLibre();
            int a = arrepentimiento();
            if (!a) {
                NSS++;
                SLLS += t;
                if (NSS + NSP <= N) {
                    STOS[k] += t - ITOS[k];
                    double tas = TAS();
                    TPSS[k] = t + tas;
                    printf("Sale a las %ld\n", TPSS[k]);
                    STAS[k] = STAS[k] + tas;
                }
            }
            break;
    }
}

void condicionesIniciales() {
    printf("Condiciones Iniciales\n");
    // Condiciones iniciales
    t = 0;
    tf = 100;

    // Var. Control
    M = 3;
    N = 3;

    // Var. Estado
    ITOM = calloc(M, sizeof (int));
    ITOS = calloc(N, sizeof (int));

    ITOM = malloc(sizeof (int)*M);
    ITOS = malloc(sizeof (int)*N);
    STAM = malloc(sizeof (int)*M);
    STOM = malloc(sizeof (int)*M);
    STAS = malloc(sizeof (int)*N);
    STOS = malloc(sizeof (int)*N);

    NSS = NSM = SS = SSM = SSS = SLLM = SLLS = NTM = NTS = 0;

    // Var. Resultado
    PECM = PECS = PPS = PA5 = 0;

    PTOS = calloc(N, sizeof (double));
    PTOM = calloc(M, sizeof (double));

    //Eventos futuros
    TPLL = 0;
    TPSM = malloc(sizeof (long)*M);
    TPSS = calloc(N, sizeof (long));
    TPSSP = malloc(sizeof (long)*N);
    int i;
    for (i = 0; i < N; i++) {
        TPSS[i] = HV;
        TPSSP[i] = HV;
    }
    for (i = 0; i < M; i++) {
        TPSM[i] = HV;
    }

    srand(time(0)); // inicializar random seed
}

bool arrepentimiento() {
    if (NSS > 5)
        if (r() < 0.3)
            return true;
    return false;
}

int motivoConsulta() {
    if (r() < 0.7)
        return MEDICO;
    else
        return SECRETARIO;
}

void salidaSecretario(int k) {
    printf("salida secretario\n");
    if (NSP + NSS < N) {
        ITOS[k] = t;
        TPSS[k] = HV;
        TPSSP[k] = HV;
    } else {
        double tas = TAS();
        if (NSP > 0) { // HAY ALGUIEN CON PRIORIDAD
            TPSSP[k] = t + tas;
            TPSS[k] = HV;
        } else {
            TPSS[k] = t + tas;
            TPSSP[k] = HV;
        }
        STAS[k] += tas;
        SSS += t;
    }
    NTS++;
}

int secretarioLibre() {
    int i;
    for (i = 0; i < N; i++) {
        if (TPSS[i] == HV && TPSSP[i] == HV) {
            return i;
        }
    }
}

int medicoLibre() {
    int i;
    for (i = 0; i < M; i++) {
        if (TPSM[i] == HV) {
            return i;
        }
    }
}

int proxima_salida(int* k) {
    int i;
    long menorTiempo = HV;
    int proximaSalida = SECRETARIO;
    for (i = 0; i < N; i++) {
        if (TPSS[i] <= TPSSP[i])
            if (TPSS[i] < menorTiempo) {
                menorTiempo = TPSS[i];
                *k = i;
            } else
                if (TPSSP[i] < menorTiempo) {
                menorTiempo = TPSSP[i];
                *k = i;
            }

    }
    for (i = 0; i < M; i++) {
        if (TPSM[i] < menorTiempo) {
            menorTiempo = TPSM[i];
            *k = i;
            proximaSalida = MEDICO;
        }
    }
    return proximaSalida;
}

long IA() {
    double R = 0.07 + r()*(0.93 - 0.07);
    return -3.8785 * log(-0.374763 * log(R));
}

long TAM() {
    return 15 + r()*(30 - 15);
}

long TAS() {
    return -32.5182 * (pow(1 - r(), 0.38114) - 1.06189);
}

double r() {
    // Random 0 < r() < 1
    return (double) rand() / RAND_MAX;
}