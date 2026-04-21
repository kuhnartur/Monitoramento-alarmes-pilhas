#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_ALARMS 10

struct Alarme {
    char dataHora[30];
    char idEquipamento[30];
    float valorLido;
};

struct dispositivo {
    char IP[15];
    char ID[30];
    float temperatura;
    float umidade;
};

struct PilhaAlarmes {
    struct Alarme alarmes[MAX_ALARMS];
    int topo;
};

void inicializarPilha(struct PilhaAlarmes *pilha) {
    pilha->topo = -1;
}

void empilharAlarme(struct PilhaAlarmes *pilha, const char *idEquipamento, float valorLido) {
    if (pilha->topo == MAX_ALARMS - 1) {
        for (int i = 0; i < MAX_ALARMS - 1; i++) {
            pilha->alarmes[i] = pilha->alarmes[i + 1];
        }
        pilha->topo--;
    }

    pilha->topo++;

    time_t agora;
    time(&agora);
    struct tm *t = localtime(&agora);
    strftime(pilha->alarmes[pilha->topo].dataHora,
             sizeof(pilha->alarmes[pilha->topo].dataHora),
             "%d/%m/%Y %H:%M:%S", t);

    strcpy(pilha->alarmes[pilha->topo].idEquipamento, idEquipamento);
    pilha->alarmes[pilha->topo].valorLido = valorLido;
}

void mostrarUltimoAlarme(struct PilhaAlarmes *pilha) {
    if (pilha->topo >= 0) {
        printf("\n------Ultimo Alarme------\n");
        printf("Data e Hora     : %s\n", pilha->alarmes[pilha->topo].dataHora);
        printf("ID Equipamento  : %s\n", pilha->alarmes[pilha->topo].idEquipamento);
        printf("Valor Lido      : %.2f\n", pilha->alarmes[pilha->topo].valorLido);
        printf("-------------------------\n");
    } else {
        printf("Nenhum alarme registrado.\n");
    }
}

void mostrarTodosOsAlarmes(struct PilhaAlarmes *pilha) {
    if (pilha->topo < 0) {
        printf("Nenhum alarme registrado.\n");
        return;
    }
    printf("\n------Ultimos %d Alarmes (mais recente ao mais antigo)------\n", pilha->topo + 1);
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%d] Data e Hora    : %s\n", pilha->topo - i + 1, pilha->alarmes[i].dataHora);
        printf("    ID Equipamento : %s\n", pilha->alarmes[i].idEquipamento);
        printf("    Valor Lido     : %.2f\n", pilha->alarmes[i].valorLido);
        printf("    ------------------------------\n");
    }
}

void gerarLeiturasAleatorias(struct dispositivo *dev, FILE *logfile, struct PilhaAlarmes *pilha) {
    dev->temperatura = (float)(rand() % 100) + ((float)(rand() % 100) / 100);
    dev->umidade     = (float)(rand() % 100) + ((float)(rand() % 100) / 100);

    time_t agora;
    time(&agora);
    char dataHora[30];
    struct tm *t = localtime(&agora);
    strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", t);

    if (dev->temperatura > 50.0) {
        fprintf(logfile, "Erro de temperatura! ID: %s, Temperatura: %.2f graus C, Umidade: %.2f%%, Data e Hora: %s\n",
                dev->ID, dev->temperatura, dev->umidade, dataHora);
        fflush(logfile);
        empilharAlarme(pilha, dev->ID, dev->temperatura);
    }

    if (dev->umidade > 50.0) {
        fprintf(logfile, "Erro de umidade! ID: %s, Temperatura: %.2f graus C, Umidade: %.2f%%, Data e Hora: %s\n",
                dev->ID, dev->temperatura, dev->umidade, dataHora);
        fflush(logfile);
        empilharAlarme(pilha, dev->ID, dev->umidade);
    }
}

int main() {
    srand(time(NULL));

    struct dispositivo *dispositivos = malloc(10 * sizeof(struct dispositivo));
    if (dispositivos == NULL) {
        fprintf(stderr, "Falha na alocacao de memoria.\n");
        return 1;
    }

    FILE *logfile = fopen("Alarmes.txt", "a");
    if (logfile == NULL) {
        fprintf(stderr, "Falha ao abrir o arquivo de log.\n");
        free(dispositivos);
        return 1;
    }

    struct PilhaAlarmes pilha;
    inicializarPilha(&pilha);

    strcpy(dispositivos[0].IP, "10.1.1.2");  strcpy(dispositivos[0].ID, "Dispositivo 1");
    strcpy(dispositivos[1].IP, "10.1.1.3");  strcpy(dispositivos[1].ID, "Dispositivo 2");
    strcpy(dispositivos[2].IP, "10.1.1.4");  strcpy(dispositivos[2].ID, "Dispositivo 3");
    strcpy(dispositivos[3].IP, "10.1.1.5");  strcpy(dispositivos[3].ID, "Dispositivo 4");
    strcpy(dispositivos[4].IP, "10.1.1.6");  strcpy(dispositivos[4].ID, "Dispositivo 5");
    strcpy(dispositivos[5].IP, "10.1.1.7");  strcpy(dispositivos[5].ID, "Dispositivo 6");
    strcpy(dispositivos[6].IP, "10.1.1.8");  strcpy(dispositivos[6].ID, "Dispositivo 7");
    strcpy(dispositivos[7].IP, "10.1.1.9");  strcpy(dispositivos[7].ID, "Dispositivo 8");
    strcpy(dispositivos[8].IP, "10.1.1.10"); strcpy(dispositivos[8].ID, "Dispositivo 9");
    strcpy(dispositivos[9].IP, "10.1.1.11"); strcpy(dispositivos[9].ID, "Dispositivo 10");

    while (1) {
        printf("\n---Leituras dos Dispositivos---\n");
        for (int i = 0; i < 10; i++) {
            gerarLeiturasAleatorias(&dispositivos[i], logfile, &pilha);
            printf("ID          : %s\n", dispositivos[i].ID);
            printf("IP          : %s\n", dispositivos[i].IP);
            printf("Temperatura : %.2f graus C\n", dispositivos[i].temperatura);
            printf("Umidade     : %.2f%%\n", dispositivos[i].umidade);
            printf("-------------------------------\n");
        }

        printf("\nMenu:\n");
        printf("1. Mostrar o ultimo alarme\n");
        printf("2. Mostrar os ultimos 10 alarmes\n");
        printf("3. Nao mostrar\n");
        printf("Escolha uma opcao: ");

        int escolha;
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                mostrarUltimoAlarme(&pilha);
                break;
            case 2:
                mostrarTodosOsAlarmes(&pilha);
                break;
            case 3:
                printf("Continuando monitoramento...\n");
                break;
            default:
                printf("Opcao invalida. Continuando monitoramento...\n");
                break;
        }

        sleep(10);
    }

    fclose(logfile);
    free(dispositivos);
    dispositivos = NULL;

    return 0;
}