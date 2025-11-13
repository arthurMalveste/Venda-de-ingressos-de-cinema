
#include <stdio.h> 
#include <stdlib.h>   
#include <string.h>   
#include "sessoes.h"  

// --- Constantes ---
#define ARQUIVO_SESSOES "sessoes.dat"
#define ARQUIVO_CONTADOR "contador_sessao.dat"

// (Declaração da função de limpeza, o corpo está no final)
void limparBufferTecladoSessoes();

// Obtém um ID único para a nova sessão.
int obterProximoIdSessao() {
    int proximo_id = 1;
    FILE* f = fopen(ARQUIVO_CONTADOR, "rb");

    if (f != NULL) {
        fread(&proximo_id, sizeof(int), 1, f);
        fclose(f);
    }
    int id_para_retornar = proximo_id;
    int novo_proximo_id = proximo_id + 1;
    f = fopen(ARQUIVO_CONTADOR, "wb");
    if (f == NULL) {
        printf("ERRO FATAL: Nao foi possivel escrever no arquivo contador!\n");
        exit(1); 
    }
    fwrite(&novo_proximo_id, sizeof(int), 1, f);
    fclose(f); 
    return id_para_retornar;
}


// --- Implementação das Funções Públicas ---

// Agendar uma nova sessão
 
void agendarSessao() {
    Sessao nova_sessao; 

    printf("--- Agendamento de Nova Sessao ---\n");
    
   
    printf("Digite o NOME do Filme: ");

    // Lê o nome com espaços, até 49 caracteres
     scanf("%49[^\n]", nova_sessao.nome_filme); 
    limparBufferTecladoSessoes(); // Limpa o buffer DEPOIS de ler a string
    

    printf("Digite o ID da Sala: ");
    scanf("%d", &nova_sessao.id_sala);
    printf("Digite a Data (DD MM AAAA): ");
    scanf("%d %d %d", &nova_sessao.data_sessao.dia, &nova_sessao.data_sessao.mes, &nova_sessao.data_sessao.ano);
    printf("Digite o Horario (HH MM): ");
    scanf("%d %d", &nova_sessao.hora_sessao.hora, &nova_sessao.hora_sessao.minuto);
    printf("Digite o Preco (ex: 25.50): ");
    scanf("%f", &nova_sessao.preco);

    // Pega o ID único e define o status como "Ativa"
    nova_sessao.id_sessao = obterProximoIdSessao();
    nova_sessao.status = 1;

    // --- Parte de Escrita no Arquivo ---
    FILE* f = fopen(ARQUIVO_SESSOES, "ab");
    if (f == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo %s\n", ARQUIVO_SESSOES);
        return;
    }

    // Escreve a "ficha" inteira (agora com o nome_filme) no arquivo.
    fwrite(&nova_sessao, sizeof(Sessao), 1, f);
    fclose(f);

    printf("\n>>> Sessao (ID: %d) para o filme '%s' agendada com sucesso!\n\n", nova_sessao.id_sessao, nova_sessao.nome_filme);
}


// Exibir todas as sessões disponíveis

void exibirSessoesDisponiveis() {
    Sessao temp_sessao; 
    int encontrou_alguma = 0;

    printf("--- Sessoes Disponiveis ---\n");

    FILE* f = fopen(ARQUIVO_SESSOES, "rb");
    if (f == NULL) {
        printf("Nenhuma sessao cadastrada ainda.\n\n");
        return;
    }

    while (fread(&temp_sessao, sizeof(Sessao), 1, f) == 1) {
        
        if (temp_sessao.status == 1) {
            printf("----------------------------------\n");
            printf("ID da Sessao: %d\n", temp_sessao.id_sessao);
            
            printf("  Filme: %s | Sala (ID): %d\n", temp_sessao.nome_filme, temp_sessao.id_sala);
            
            printf("  Data: %02d/%02d/%d\n", temp_sessao.data_sessao.dia, temp_sessao.data_sessao.mes, temp_sessao.data_sessao.ano);
            printf("  Hora: %02d:%02d\n", temp_sessao.hora_sessao.hora, temp_sessao.hora_sessao.minuto);
            printf("  Preco: R$ %.2f\n", temp_sessao.preco);
            encontrou_alguma = 1;
        }
    }

    if (encontrou_alguma == 0) {
        printf("Nenhuma sessao ATIVA encontrada.\n");
    }
    printf("----------------------------------\n\n");

    fclose(f); 
}


// Cancelar uma sessão agendada

void cancelarSessao() {
    int id_para_cancelar;
    int encontrou = 0;
    int n_sessoes = 0;
    Sessao* array_de_sessoes = NULL; 

    printf("--- Cancelar Sessao ---\n");
    printf("Digite o ID da sessao que deseja cancelar: ");
    scanf("%d", &id_para_cancelar);

    FILE* f = fopen(ARQUIVO_SESSOES, "rb");
    if (f == NULL) {
        printf("Nenhuma sessao cadastrada para cancelar.\n\n");
        return;
    }

    fseek(f, 0, SEEK_END); 
    long tamanho_total = ftell(f); 
    rewind(f); 
    n_sessoes = tamanho_total / sizeof(Sessao); 

    if (n_sessoes == 0) {
        printf("Nenhuma sessao cadastrada.\n\n");
        fclose(f);
        return;
    }
    array_de_sessoes = (Sessao*) malloc(n_sessoes * sizeof(Sessao));
    if (array_de_sessoes == NULL) {
        printf("ERRO FATAL: Falha ao alocar memoria!\n");
        fclose(f);
        exit(1);
    }
    fread(array_de_sessoes, sizeof(Sessao), n_sessoes, f);
    fclose(f); 

    int i;
    for ( i = 0; i < n_sessoes; i++) {
        if (array_de_sessoes[i].id_sessao == id_para_cancelar && array_de_sessoes[i].status == 1) {
            array_de_sessoes[i].status = 0; 
            encontrou = 1;
            break; 
        }
    }

    if (encontrou) {
        f = fopen(ARQUIVO_SESSOES, "wb");
        if (f == NULL) {
            printf("ERRO FATAL: Nao foi possivel reescrever o arquivo!\n");
            free(array_de_sessoes); 
            exit(1);
        }
        fwrite(array_de_sessoes, sizeof(Sessao), n_sessoes, f);
        fclose(f); 
        printf("\n>>> Sessao (ID: %d) cancelada com sucesso!\n\n", id_para_cancelar);
    } else {
        printf("\n>>> Sessao (ID: %d) nao encontrada ou ja estava cancelada.\n\n", id_para_cancelar);
    }
    free(array_de_sessoes);
}

// (Função original)
void limparBufferTecladoSessoes() { 
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// (Função original, sem alterações)
void menuGerenciarSessoes() {
    int opcao;

    do {
        printf("\n--- GESTAO DE SESSOES ---\n");
        printf("1. Agendar Nova Sessao\n");
        printf("2. Exibir Sessoes Disponiveis\n");
        printf("3. Cancelar Sessao\n");
        printf("0. Voltar ao Menu Principal\n"); 
        printf("----------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoSessoes();
            opcao = -1;
            continue;
        }
        limparBufferTecladoSessoes();

        switch (opcao) {
            case 1:
                agendarSessao();
                break;
            case 2:
                exibirSessoesDisponiveis();
                break;
            case 3:
                cancelarSessao();
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