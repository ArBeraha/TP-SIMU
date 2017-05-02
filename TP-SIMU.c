#include "TP-SIMU.h"

int main(int argc, char** argv) {
    printf("INICIANDO\n");
    condicionesIniciales(argc,argv);

    // MODELO
    do {
        int PS = proxima_evento();

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
    printf("%sRESULTADOS%s (M=%d, N=%d)\n", VERDE, NEGRO, M,N);

    printf("PTOS:\n");
    for (i = 0; i < N; i++) {
        printf("\tPTOS[%d]=%f\n", i, PTOS[i]);
    }
    printf("PTOM:\n");
    for (i = 0; i < M; i++) {
        printf("\tPTOM[%d]=%f\n", i, PTOM[i]);
    }
    printf("PECM=%f\n", PECM);
    printf("PECS=%f\n", PECS);
}

void llegada() {
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

void condicionesIniciales(int argc, char** argv) {
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
        N=2;
        M=2;
    }


    // Var. Estado
    ITOM = calloc(M, sizeof(int));
    ITOS = calloc(N, sizeof(int));

    ITOM = calloc(M, sizeof(int));
    ITOS = calloc(N, sizeof(int));
    STOM = calloc(M, sizeof(int));
    STOS = calloc(N, sizeof(int));

    NSS = NSM = SS = SSM = SSS = SLLM = SLLS = NTM = NTS = STAS = STAM = 0;

    // Var. Resultado
    PECM = PECS = 0;

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
        if (r() < 0.3)
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
            printf("ATENDIENDO UN PRIORITARIO QUE ESTABA EN COLA\n");
        } else {
            TPSS[k] = t + tas;
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

int proxima_evento() {
    int i;
    long menorTiempo = HV;
    int proximaEvento;
    for (i = 0; i < N; i++) {
        if (TPSS[i] < menorTiempo) {
            menorTiempo = TPSS[i];
            k = i;
            proximaEvento = SECRETARIO;
        }
        if (TPSSP[i] < menorTiempo) {
            menorTiempo = TPSSP[i];
            k = i;
            proximaEvento = PRIORIDAD;
        }
    }
    for (i = 0; i < M; i++) {
        if (TPSM[i] < menorTiempo) {
            menorTiempo = TPSM[i];
            k = i;
            proximaEvento = MEDICO;
        }
    }
    if (TPLL < menorTiempo)
        return LLEGADA;
    else
        return proximaEvento;
}

long IA() {
    double R = 0.07 + r() * (0.93 - 0.07);
    return -5.81775 * log(-0.374763 * log(R));
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