// NOTA: Este módulo DEPENDE dos outros. Ele precisa ler "sessoes.dat" e "salas.dat".

#include <stdio.h>
#include <stdlib.h>
#include "vendas.h"

// INCLUINDO OS HEADERS DOS OUTROS MÓDULOS
#include "../gerenciar_sessao/sessoes.h"
#include "../gerenciar_sala/salas.h"

// --- Constantes ---
#define ARQUIVO_VENDAS "vendas.dat"
#define ARQUIVO_CONTADOR_VENDA "contador_venda.dat"

// DEFINIÇÕES DOS ARQUIVOS DOS OUTROS MÓDULOS
#define ARQUIVO_SESSOES "sessoes.dat"
#define ARQUIVO_SALAS "salas.dat"


// Pega o ID para a venda
int obterProximoIdVenda() {
    int proximo_id = 1;
    FILE* f = fopen(ARQUIVO_CONTADOR_VENDA, "rb");
    if (f != NULL) {
        fread(&proximo_id, sizeof(int), 1, f);
        fclose(f);
    }
    int id_para_retornar = proximo_id;
    int novo_proximo_id = proximo_id + 1;
    f = fopen(ARQUIVO_CONTADOR_VENDA, "wb");
    if (f == NULL) {
        printf("ERRO FATAL: Nao foi possivel escrever no contador de venda!\n");
        exit(1);
    }
    fwrite(&novo_proximo_id, sizeof(int), 1, f);
    fclose(f);
    return id_para_retornar;
}

// Limpa o buffer
void limparBufferTecladoVendas() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Encontra uma Sessao específica em "sessoes.dat"
Sessao encontrarSessaoPorID(int id_sessao_procurada, int *encontrada) {
    Sessao sessao_temp;
    *encontrada = 0; // Assume que não encontrou

    FILE* f = fopen(ARQUIVO_SESSOES, "rb");
    if (f == NULL) {
        return sessao_temp; // Retorna struct vazia se o arquivo não existe
    }

    while (fread(&sessao_temp, sizeof(Sessao), 1, f) == 1) {
        if (sessao_temp.id_sessao == id_sessao_procurada && sessao_temp.status == 1) {
            *encontrada = 1;
            break;
        }
    }
    fclose(f);
    return sessao_temp;
}


// Encontra uma Sala específica em "salas.dat"
Sala encontrarSalaPorID(int id_sala_procurada, int *encontrada) {
    Sala sala_temp;
    *encontrada = 0;

    FILE* f = fopen(ARQUIVO_SALAS, "rb");
    if (f == NULL) {
        return sala_temp;
    }

    while (fread(&sala_temp, sizeof(Sala), 1, f) == 1) {
        if (sala_temp.id_sala == id_sala_procurada && sala_temp.status == 1) {
            *encontrada = 1;
            break;
        }
    }
    fclose(f);
    return sala_temp;
}


// Conta quantos ingressos já foram vendidos para uma sessão
int contarIngressosVendidos(int id_sessao) {
    Venda venda_temp;
    int total_vendido = 0;

    FILE* f = fopen(ARQUIVO_VENDAS, "rb");
    if (f == NULL) {
        return 0; // Nenhuma venda ainda
    }

    while (fread(&venda_temp, sizeof(Venda), 1, f) == 1) {
        if (venda_temp.id_sessao == id_sessao) {
            total_vendido += venda_temp.quantidade;
        }
    }
    fclose(f);
    return total_vendido;
}


// Função Principal da Venda
void realizarVenda() {
    int id_sessao_desejada;
    
    // 1. O usuário precisa saber qual sessão comprar.
    printf("\n--- Realizar Venda de Ingresso ---\n");
    printf("(Consulte o ID na listagem de sessoes)\n");
    printf("Digite o ID da Sessao desejada: ");
    
    if (scanf("%d", &id_sessao_desejada) != 1) {
        limparBufferTecladoVendas();
        printf("ID invalido.\n");
        return;
    }
    limparBufferTecladoVendas();

    // 2. Buscar a Sessão e sua Sala
    int sessao_encontrada = 0;
    Sessao sessao = encontrarSessaoPorID(id_sessao_desejada, &sessao_encontrada);

    if (!sessao_encontrada) {
        printf("ERRO: Sessao com ID %d nao encontrada ou esta cancelada.\n", id_sessao_desejada);
        return;
    }

    int sala_encontrada = 0;
    Sala sala = encontrarSalaPorID(sessao.id_sala, &sala_encontrada);

    if (!sala_encontrada) {
        printf("ERRO FATAL: A Sala (ID %d) desta sessao nao foi encontrada!\n", sessao.id_sala);
        return;
    }

    // 3. Calcular disponibilidade
    int capacidade_total = sala.capacidade;
    int ingressos_vendidos = contarIngressosVendidos(id_sessao_desejada);
    int ingressos_disponiveis = capacidade_total - ingressos_vendidos;

    printf("\n--- Detalhes da Sessao ---\n");
    printf("  Sessao ID: %d | Sala: %d\n", sessao.id_sessao, sala.numero_sala);
    printf("  Preco por Ingresso: R$ %.2f\n", sessao.preco);
    printf("  Capacidade da Sala: %d\n", capacidade_total);
    printf("  Ingressos ja vendidos: %d\n", ingressos_vendidos);
    printf("  INGRESSOS DISPONIVEIS: %d\n", ingressos_disponiveis);
    printf("-------------------------------\n");

    if (ingressos_disponiveis <= 0) {
        printf("ERRO: Esta sessao esta esgotada!\n");
        return;
    }

    // 4. Pedir a quantidade e calcular o total
    int qtd_comprar = 0;
    printf("Quantos ingressos deseja comprar? ");
    if (scanf("%d", &qtd_comprar) != 1 || qtd_comprar <= 0) {
        limparBufferTecladoVendas();
        printf("Quantidade invalida.\n");
        return;
    }
    limparBufferTecladoVendas();

    if (qtd_comprar > ingressos_disponiveis) {
        printf("ERRO: Voce tentou comprar %d, mas so ha %d disponiveis.\n", qtd_comprar, ingressos_disponiveis);
        return;
    }

    float valor_total = (float)qtd_comprar * sessao.preco;
    printf("\nValor Total: %d x R$ %.2f = R$ %.2f\n", qtd_comprar, sessao.preco, valor_total);


    // 5. Confirmação e Registro da Venda
    printf("Confirmar a compra? (1 = Sim / 0 = Nao): ");
    int confirmacao = 0;
    scanf("%d", &confirmacao);
    limparBufferTecladoVendas();

    if (confirmacao == 1) {
        Venda nova_venda;
        nova_venda.id_venda = obterProximoIdVenda();
        nova_venda.id_sessao = id_sessao_desejada;
        nova_venda.quantidade = qtd_comprar;
        nova_venda.valor_total_pago = valor_total;

        // Salva no arquivo
        FILE* f = fopen(ARQUIVO_VENDAS, "ab");
        if (f == NULL) {
            printf("ERRO: Nao foi possivel salvar a venda!\n");
            return;
        }
        fwrite(&nova_venda, sizeof(Venda), 1, f);
        fclose(f);

        printf("\n>>> Venda (ID: %d) realizada com sucesso! <<<\n", nova_venda.id_venda);
    } else {
        printf("\nVenda cancelada.\n");
    }
}


// --- Função de Menu (Chamada pelo main.c) ---
void menuRealizarVenda() {
    int opcao;

    // Mostra as sessões logo que o usuário entra no menu.
    printf("\n--- Sessoes Disponiveis no Momento ---\n");
    exibirSessoesDisponiveis(); // Chama a função do módulo de sessões

    do {
        // 2. Opções do menu ajustadas
        printf("\n--- Venda de Ingressos ---\n");
        printf("1. Comprar Ingresso\n");
        printf("2. Atualizar Lista de Sessoes\n"); // Caso o usuário queira ver de novo
        printf("0. Voltar ao Menu Principal\n");
        printf("-----------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoVendas();
            opcao = -1; 
            continue;
        }
        limparBufferTecladoVendas();

        // 3. Switch ajustado
        switch (opcao) {
            case 1:
                realizarVenda(); // Opção de compra
                break;
            case 2:
                // Se o usuário quiser recarregar a lista
                printf("\n--- Atualizando Sessoes Disponiveis ---\n");
                exibirSessoesDisponiveis();
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