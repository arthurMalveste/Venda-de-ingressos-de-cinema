#include <stdio.h>
#include <stdlib.h> 
#include "salas.h"

// --- Constantes ---
#define ARQUIVO_SALAS "salas.dat"
#define ARQUIVO_CONTADOR_SALA "contador_sala.dat"


// Função interna para obter o ID 
int obterProximoIdSala() {
    int proximo_id = 1;
    FILE* f = fopen(ARQUIVO_CONTADOR_SALA, "rb");

    if (f != NULL) {
        fread(&proximo_id, sizeof(int), 1, f);
        fclose(f);
    }

    int id_para_retornar = proximo_id;
    int novo_proximo_id = proximo_id + 1;

    f = fopen(ARQUIVO_CONTADOR_SALA, "wb");
    if (f == NULL) {
        printf("ERRO FATAL: Nao foi possivel escrever no arquivo contador de sala!\n");
        exit(1);
    }
    fwrite(&novo_proximo_id, sizeof(int), 1, f);
    fclose(f);

    return id_para_retornar;
}

// Função para limpar o buffer
void limparBufferTecladoSalas() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Cadastrar Salas
void cadastrarSala() {
    Sala nova_sala;

    printf("\n--- Cadastro de Nova Sala ---\n");
    printf("Digite o Numero da Sala (ex: 5): ");
    scanf("%d", &nova_sala.numero_sala);

    printf("Digite a Capacidade Maxima de Assentos: ");
    scanf("%d", &nova_sala.capacidade);

    // Limpa o buffer para o menu principal
    limparBufferTecladoSalas();

    // Define os padrões
    nova_sala.id_sala = obterProximoIdSala();
    nova_sala.status = 1; // 1 = Ativa

    // --- Parte de Escrita no Arquivo ---
    FILE* f = fopen(ARQUIVO_SALAS, "ab"); 
    if (f == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo %s\n", ARQUIVO_SALAS);
        return;
    }

    fwrite(&nova_sala, sizeof(Sala), 1, f);
    fclose(f);

    printf("\n>>> Sala (ID: %d, Numero: %d) cadastrada com sucesso!\n\n", nova_sala.id_sala, nova_sala.numero_sala);
}



// Listar Salas
void listarSalas() {
    Sala temp_sala;
    int encontrou_alguma = 0;

    printf("\n--- Salas Cadastradas ---\n");

    FILE* f = fopen(ARQUIVO_SALAS, "rb");
    if (f == NULL) {
        printf("Nenhuma sala cadastrada ainda.\n\n");
        return;
    }

    // Loop de leitura
    while (fread(&temp_sala, sizeof(Sala), 1, f) == 1) {
        
        // Só exibe se estiver ativa
        if (temp_sala.status == 1) {
            printf("----------------------------------\n");
            printf(" ID da Sala: %d\n", temp_sala.id_sala);
            printf("   Numero: Sala %d\n", temp_sala.numero_sala);
            printf("   Capacidade: %d assentos\n", temp_sala.capacidade);
            encontrou_alguma = 1;
        }
    }

    if (encontrou_alguma == 0) {
        printf("Nenhuma sala ativa foi encontrada.\n");
    }
    printf("----------------------------------\n\n");

    fclose(f);
}


// Função do Menu
void menuGerenciarSalas() {
    int opcao;

    do {
        printf("\n--- GESTAO DE SALAS ---\n");
        printf("1. Cadastrar Nova Sala\n");
        printf("2. Listar Salas Cadastradas\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoSalas();
            opcao = -1; 
            continue;
        }

        limparBufferTecladoSalas();

        switch (opcao) {
            case 1:
                cadastrarSala();
                break;
            case 2:
                listarSalas();
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}