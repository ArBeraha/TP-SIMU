#include "TP-SIMU.h"

int main(int argc, char** argv) {
    printf("INICIANDO\n");
    condicionesIniciales();

    // MODELO
    do {

        //TABLA DE EVENTOS PARA DEBUG
        printf("TPLL=%ld\n", TPLL);
        printf("TPSM[0]=%ld,TPSS[0]=%ld,TPSSP[0]=%ld\n", TPSM[0], TPSS[0],
            TPSSP[0]);
        printf("TPSM[1]=%ld,TPSS[1]=%ld,TPSSP[1]=%ld\n", TPSM[1], TPSS[1],
            TPSSP[1]);

        int PS = proxima_salida();

        switch (PS) {
            case MEDICO:
            printf("SALIDA MEDICO\n");
            t = TPSM[k];
            NSM--;
            if (NSM < M) {
                ITOM[k] = t;
                TPSM[k] = HV;
            } else {
                long tam = TAM();
                TPSM[k] = t + tam;
                STAM += tam;
            }
            SSM = SSM + t;
            NTM++;
            if (r() < 0.3) {
                printf("PASE A PRIORIDAD\n");
                NSP++;
                if (NSP + NSS <= N) {
                    int l = secretarioLibre();
                    STOS[l] += t - ITOS[l];
                    long tas = TAS();
                    TPSSP[l] = t + tas;
                }
            }
            break;
            case SECRETARIO:
            printf("SALIDA SECRETARIO\n");
            t = TPSS[k];
            NSS--;
            salidaSecretario(k);
            SSS += t;
            NTS++;
            break;
            case PRIORIDAD:
            printf("SALIDA SECRETARIO PRIORITARIO\n");
            t = TPSSP[k];
            NSP--;
            salidaSecretario(k);
            break;
            default:
            llegada();
            break;
        }

        if (t > tf && TPLL != HV) {
            printf("%sINICIANDO VACIAMIENTO%s\n", AMARILLO, NEGRO);
            TPLL = HV;
        }

        printf("Fin ciclo con t=%ld, [NSM=%d, NSS=%d, NSP=%d]\n", t, NSM, NSS,
            NSP);
        //usleep(100); // Sleep para Debug
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
        PECM = (SSM - SLLM - STAM) / NTM;
    else
        PECM = 0;

    if (NTS != 0)
        PECS = (SSS - SLLS - STAS) / NTS;
    else
        PECS = 0;
}

void mostrarResultados() {
    int i;
    printf("%sRESULTADOS%s\n", VERDE, NEGRO);

    printf("PTOS:\n");
    for (i = 0; i < N; i++) {
        printf("\tPTOS[%d]=%f\n", i, PTOS[i]);
    }
    printf("PTOM:\n");
    for (i = 0; i < M; i++) {
        printf("\tPTOM[%d]=%f\n", i, PTOM[i]);
    }
    printf("PA5=%f\n", PA5);
    printf("PECM=%f\n", PECM);
    printf("PECS=%f\n", PECS);
}

void llegada() {
    int k;
    t = TPLL;
    TPLL = t + IA();
    switch (motivoConsulta()) {
        case MEDICO:
        printf("LLEGADA MOTIVO MEDICO\n");
        k = medicoLibre();
        NSM++;
        SLLM += t;
        if (NSM <= M) {
            STOM[k] += t - ITOM[k];
            long tam = TAM();
            TPSM[k] = t + tam;
            STAM += tam;
        }
        break;
        case SECRETARIO:
        printf("LLEGADA MOTIVO SECRETARIO\n");
        k = secretarioLibre();
        int a = arrepentimiento();
        if (!a) {
            NSS++;
            SLLS += t;
            if (NSS + NSP <= N) {
                STOS[k] += t - ITOS[k];
                long tas = TAS();
                TPSS[k] = t + tas;
                STAS += tas;
            }
        }
        break;
    }
}

void condicionesIniciales() {
    printf("Condiciones Iniciales\n");
    // Condiciones iniciales
    t = 0;
    tf = 100000;

    // Var. Control
    if (argc==3){
        M=strtol(argv[1],NULL,10);
        N=strtol(argv[2],NULL,10);
    }
    else {
        N=1;
        M=1;
    }


    // Var. Estado
    ITOM = calloc(M, sizeof(int));
    ITOS = calloc(N, sizeof(int));

    ITOM = malloc(sizeof(int) * M);
    ITOS = malloc(sizeof(int) * N);
    STOM = malloc(sizeof(int) * M);
    STOS = malloc(sizeof(int) * N);

    NSS = NSM = SS = SSM = SSS = SLLM = SLLS = NTM = NTS = STAS = STAM = 0;

    // Var. Resultado
    PECM = PECS = PPS = PA5 = 0;

    PTOS = calloc(N, sizeof(double));
    PTOM = calloc(M, sizeof(double));

    //Eventos futuros
    TPLL = 0;
    TPSM = malloc(sizeof(long) * M);
    TPSS = calloc(N, sizeof(long));
    TPSSP = malloc(sizeof(long) * N);
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
        TPSS[k] = HV;
        TPSSP[k] = HV;
        if (NSP + NSS < N) {
            ITOS[k] = t;

        } else {
            long tas = TAS();
        if (prioritariosAtendidos() < NSP) { // HAY ALGUIEN CON PRIORIDAD SIN ATENDER
            TPSSP[k] = t + tas;
            TPSS[k] = HV;
            printf("ATENDIENDO UN PRIORITARIO QUE ESTABA EN COLA\n");
        } else {
            TPSS[k] = t + tas;
            TPSSP[k] = HV;
            STAS += tas;
        }

    }
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

int proxima_salida() {
    int i;
    long menorTiempo = HV;
    int proximaSalida;
    for (i = 0; i < N; i++) {
        if (TPSS[i] < menorTiempo) {
            menorTiempo = TPSS[i];
            k = i;
            proximaSalida = SECRETARIO;
        }
        if (TPSSP[i] < menorTiempo) {
            menorTiempo = TPSSP[i];
            k = i;
            proximaSalida = PRIORIDAD;
        }
    }
    for (i = 0; i < M; i++) {
        if (TPSM[i] < menorTiempo) {
            menorTiempo = TPSM[i];
            k = i;
            proximaSalida = MEDICO;
        }
    }
    if (TPLL < menorTiempo)
        return -1;
    else
        return proximaSalida;
}

long IA() {
    double R = 0.07 + r() * (0.93 - 0.07);
    return -3.8785 * log(-0.374763 * log(R));
}

long TAM() {
    return 15 + r() * (30 - 15);
}

long TAS() {
    return -32.5182 * (pow(1 - r(), 0.38114) - 1.06189);
}

double r() {
    // Random 0 < r() < 1
    return (double) rand() / RAND_MAX;
}

int prioritariosAtendidos() {
    int i, a = 0;
    for (i = 0; i < N; i++) {
        if (TPSSP[i] != HV)
            a++;
    }
    return a;
}