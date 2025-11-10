/*
 * Arquivo: sessoes.c
 * Descrição: Implementação (código) das funções do módulo de sessões.
 * Contém toda a lógica de manipulação de arquivos e dados.
 */

#include <stdio.h>    // Para printf, scanf, FILE, fopen, fclose, etc.
#include <stdlib.h>   // Para malloc, free, exit
#include "sessoes.h"  

// --- Constantes ---
// Definir nomes de arquivos como constantes é uma boa prática.
#define ARQUIVO_SESSOES "sessoes.dat"
#define ARQUIVO_CONTADOR "contador_sessao.dat"

/*
 * Obtém um ID único para a nova sessão.
 * Lê o contador do ARQUIVO_CONTADOR, retorna o valor atual
 * e salva o próximo valor (valor + 1) de volta no arquivo.
 */
int obterProximoIdSessao() {
    int proximo_id = 1; // Começa em 1 se o arquivo não existir

    // 1. Tenta ABRIR o arquivo do contador para LEITURA BINÁRIA ("rb")
    FILE* f = fopen(ARQUIVO_CONTADOR, "rb");

    if (f != NULL) {
        // Se o arquivo existe, lê o ID atual
        fread(&proximo_id, sizeof(int), 1, f);
        fclose(f); // Fecha o arquivo de leitura
    }

    // 2. Prepara o PRÓXIMO ID e SALVA de volta
    int id_para_retornar = proximo_id;
    int novo_proximo_id = proximo_id + 1;

    // Abre o arquivo para ESCRITA BINÁRIA ("wb")
    // "wb" apaga o conteúdo anterior e escreve do zero.
    f = fopen(ARQUIVO_CONTADOR, "wb");
    if (f == NULL) {
        printf("ERRO FATAL: Nao foi possivel escrever no arquivo contador!\n");
        exit(1); // Aborta o programa se não conseguir salvar o ID
    }

    // Escreve o ID incrementado
    fwrite(&novo_proximo_id, sizeof(int), 1, f);
    fclose(f); // Fecha o arquivo de escrita

    // 3. Retorna o ID que deve ser usado agora
    return id_para_retornar;
}


// --- Implementação das Funções Públicas ---

/*
 * REQUISITO 1: Agendar uma nova sessão
 */
void agendarSessao() {
    Sessao nova_sessao; // Cria uma "ficha" em branco na memória

    printf("--- Agendamento de Nova Sessao ---\n");
    printf("Digite o ID do Filme: ");
    scanf("%d", &nova_sessao.id_filme);
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
    // Abre o arquivo de sessões no modo "ab" (Append Binary / Anexar Binário)
    // "ab" adiciona no FINAL do arquivo, sem apagar nada.
    FILE* f = fopen(ARQUIVO_SESSOES, "ab");
    if (f == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo %s\n", ARQUIVO_SESSOES);
        return;
    }

    // Escreve a "ficha" inteira (a struct) de uma vez no arquivo.
    fwrite(&nova_sessao, sizeof(Sessao), 1, f);

    // Fecha o arquivo
    fclose(f);

    printf("\n>>> Sessao (ID: %d) agendada com sucesso!\n\n", nova_sessao.id_sessao);
}


/*
 * REQUISITO 2: Exibir todas as sessões disponíveis
 */
void exibirSessoesDisponiveis() {
    Sessao temp_sessao; // Uma "ficha" temporária para carregar os dados
    int encontrou_alguma = 0;

    printf("--- Sessoes Disponiveis ---\n");

    // Abre o arquivo para LEITURA BINÁRIA ("rb")
    FILE* f = fopen(ARQUIVO_SESSOES, "rb");
    if (f == NULL) {
        printf("Nenhuma sessao cadastrada ainda.\n\n");
        return;
    }

    // Loop de Leitura:
    // O fread tenta ler UMA struct Sessao do arquivo para a 'temp_sessao'.
    // Se ele conseguir (retorna 1), o loop continua.
    // Se chegar ao fim do arquivo (retorna 0), o loop para.
    while (fread(&temp_sessao, sizeof(Sessao), 1, f) == 1) {
        
        // Verifica se a sessão lida está ATIVA
        if (temp_sessao.status == 1) {
            printf("----------------------------------\n");
            printf("ID da Sessao: %d\n", temp_sessao.id_sessao);
            printf("  Filme (ID): %d | Sala (ID): %d\n", temp_sessao.id_filme, temp_sessao.id_sala);
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

    fclose(f); // Fecha o arquivo de leitura
}


/*
 * REQUISITO 3: Cancelar uma sessão agendada
 * (Implementação com Alocação Dinâmica)
 */
void cancelarSessao() {
    int id_para_cancelar;
    int encontrou = 0;
    int n_sessoes = 0;
    Sessao* array_de_sessoes = NULL; // Ponteiro para nosso array dinâmico

    printf("--- Cancelar Sessao ---\n");
    printf("Digite o ID da sessao que deseja cancelar: ");
    scanf("%d", &id_para_cancelar);

    // --- FASE 1: Carregar TUDO do Disco para a RAM ---

    FILE* f = fopen(ARQUIVO_SESSOES, "rb");
    if (f == NULL) {
        printf("Nenhuma sessao cadastrada para cancelar.\n\n");
        return;
    }

    // 1a. Descobrir o tamanho do arquivo para saber quantas sessões temos
    fseek(f, 0, SEEK_END); // Move o cursor para o FIM do arquivo
    long tamanho_total = ftell(f); // Pega a posição atual (tamanho total em bytes)
    rewind(f); // Move o cursor de volta para o INÍCIO do arquivo
    
    n_sessoes = tamanho_total / sizeof(Sessao); // Calcula quantas structs cabem no arquivo

    if (n_sessoes == 0) {
        printf("Nenhuma sessao cadastrada.\n\n");
        fclose(f);
        return;
    }

    // 1b. Alocar memória RAM para todas as sessões
    array_de_sessoes = (Sessao*) malloc(n_sessoes * sizeof(Sessao));
    if (array_de_sessoes == NULL) {
        printf("ERRO FATAL: Falha ao alocar memoria!\n");
        fclose(f);
        exit(1);
    }

    // 1c. Ler TODAS as sessões do arquivo para o array na RAM
    fread(array_de_sessoes, sizeof(Sessao), n_sessoes, f);
    fclose(f); // Fecha o arquivo, todos os dados já estão na RAM.

    // --- FASE 2: Modificar os dados na RAM ---
    int i;
    for ( i = 0; i < n_sessoes; i++) {
        // Se acharmos o ID e ele estiver ATIVO
        if (array_de_sessoes[i].id_sessao == id_para_cancelar && array_de_sessoes[i].status == 1) {
            array_de_sessoes[i].status = 0; // MUDAMOS O STATUS (aqui está o cancelamento)
            encontrou = 1;
            break; // Já achamos, podemos parar o loop
        }
    }

    // --- FASE 3: Reescrever TUDO da RAM para o Disco ---
    if (encontrou) {
        // Abre o arquivo em modo "wb" (ESCRITA, apaga o anterior)
        f = fopen(ARQUIVO_SESSOES, "wb");
        if (f == NULL) {
            printf("ERRO FATAL: Nao foi possivel reescrever o arquivo!\n");
            free(array_de_sessoes); // Libera a memória antes de sair
            exit(1);
        }

        // Salva o array INTEIRO (com a sessão modificada) de volta no disco
        fwrite(array_de_sessoes, sizeof(Sessao), n_sessoes, f);
        fclose(f); // Fecha o arquivo de escrita
        printf("\n>>> Sessao (ID: %d) cancelada com sucesso!\n\n", id_para_cancelar);

    } else {
        printf("\n>>> Sessao (ID: %d) nao encontrada ou ja estava cancelada.\n\n", id_para_cancelar);
    }

    free(array_de_sessoes);
}
	void limparBufferTecladoSessoes() { 
    	int c;
    	while ((c = getchar()) != '\n' && c != EOF);
}
	void menuGerenciarSessoes() {
    int opcao;

    do {
        printf("\n--- GESTAO DE SESSOES ---\n");
        printf("1. Agendar Nova Sessao\n");
        printf("2. Exibir Sessoes Disponiveis\n");
        printf("3. Cancelar Sessao\n");
        printf("0. Voltar ao Menu Principal\n"); // <<< MUDANÇA IMPORTANTE
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
                break; // Simplesmente sai do loop e a função termina
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0); // O loop termina quando o usuário digita 0
}

