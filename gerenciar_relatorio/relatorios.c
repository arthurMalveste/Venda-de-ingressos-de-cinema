#include <stdio.h>
#include <stdlib.h>
#include "relatorios.h"

// incluir TODOS os headers para entender as structs
#include "../gerenciar_filme/filmes.h"
#include "../gerenciar_sala/salas.h"
#include "../gerenciar_sessao/sessoes.h"
#include "../gerenciar_vendas/vendas.h"

// --- Constantes dos Arquivos de Dados ---
#define ARQUIVO_FILMES "filmes.dat"
#define ARQUIVO_SALAS "salas.dat"
#define ARQUIVO_SESSOES "sessoes.dat"
#define ARQUIVO_VENDAS "vendas.dat"


// Busca um Filme pelo ID
static Filme helperEncontrarFilme(int id_filme, int *encontrada) {
    Filme temp;
    *encontrada = 0;
    FILE* f = fopen(ARQUIVO_FILMES, "rb");
    if (f == NULL) return temp;
    while (fread(&temp, sizeof(Filme), 1, f) == 1) {
        if (temp.ID == id_filme) {
            *encontrada = 1;
            break;
        }
    }
    fclose(f);
    return temp;
}

// Busca uma Sala pelo ID
static Sala helperEncontrarSala(int id_sala, int *encontrada) {
    Sala temp;
    *encontrada = 0;
    FILE* f = fopen(ARQUIVO_SALAS, "rb");
    if (f == NULL) return temp;
    while (fread(&temp, sizeof(Sala), 1, f) == 1) {
        if (temp.id_sala == id_sala) {
            *encontrada = 1;
            break;
        }
    }
    fclose(f);
    return temp;
}

// Calcula o total de vendas e receita para UMA sessão
static void helperCalcularVendasSessao(int id_sessao, int *total_vendido, float *receita_total) {
    Venda temp;
    *total_vendido = 0;
    *receita_total = 0.0f;
    
    FILE* f = fopen(ARQUIVO_VENDAS, "rb");
    if (f == NULL) return; // Nenhuma venda ainda

    while (fread(&temp, sizeof(Venda), 1, f) == 1) {
        if (temp.id_sessao == id_sessao) {
            *total_vendido += temp.quantidade;
            *receita_total += temp.valor_total_pago;
        }
    }
    fclose(f);
}


// --- Funções dos Relatórios ---
void relatorioVendasPorSessao() {
    Sessao sessao_temp;
    int encontrou_sessoes = 0;

    printf("\n--- Relatorio de Vendas por Sessao ---\n");
    
    FILE* f_sessoes = fopen(ARQUIVO_SESSOES, "rb");
    if (f_sessoes == NULL) {
        printf("Nenhuma sessao cadastrada.\n");
        return;
    }

    while (fread(&sessao_temp, sizeof(Sessao), 1, f_sessoes) == 1) {
        if (sessao_temp.status == 0) continue; // Pula sessoes canceladas
        
        encontrou_sessoes = 1;
        
        // Não precisamos mais do 'encontrado_f' ou 'Filme filme ='
        int encontrado_s = 0;
        
        // 1. Buscar dados complementares
        Sala sala = helperEncontrarSala(sessao_temp.id_sala, &encontrado_s);


        // 2. Calcular vendas
        int ingressos_vendidos = 0;
        float receita_total = 0.0f;
        helperCalcularVendasSessao(sessao_temp.id_sessao, &ingressos_vendidos, &receita_total);

        // 3. Calcular taxa de ocupação
        float taxa_ocupacao = 0.0f;
        if (encontrado_s && sala.capacidade > 0) {
            taxa_ocupacao = ((float)ingressos_vendidos / (float)sala.capacidade) * 100.0f;
        }
        
        // 4. Imprimir relatório da sessão
        printf("----------------------------------------\n");
        
   
        // Agora imprimimos o 'nome_filme' direto da 'sessao_temp'
        printf("Sessao ID: %d (Filme: %s)\n", 
               sessao_temp.id_sessao, 
               sessao_temp.nome_filme); // Não precisamos mais do 'encontrado_f'
    
        
        printf("  Data: %02d/%02d/%d | Hora: %02d:%02d\n",
               sessao_temp.data_sessao.dia, sessao_temp.data_sessao.mes, sessao_temp.data_sessao.ano,
               sessao_temp.hora_sessao.hora, sessao_temp.hora_sessao.minuto);
        
        printf("  Sala: %d (Capacidade: %d)\n", 
               encontrado_s ? sala.numero_sala : -1,
               encontrado_s ? sala.capacidade : 0);
        
        printf("  Ingressos Vendidos: %d\n", ingressos_vendidos);
        printf("  Receita Total: R$ %.2f\n", receita_total);
        printf("  Taxa de Ocupacao: %.2f%%\n", taxa_ocupacao);
    }
    
    if (!encontrou_sessoes) {
        printf("Nenhuma sessao ativa encontrada.\n");
    }
    printf("----------------------------------------\n");
    fclose(f_sessoes);
}


// Gerar relatório de ocupação por sala.
void relatorioOcupacaoPorSala() {
    Sala sala_temp;
    int encontrou_salas = 0;

    printf("\n--- Relatorio de Ocupacao por Sala ---\n");

    FILE* f_salas = fopen(ARQUIVO_SALAS, "rb");
    if (f_salas == NULL) {
        printf("Nenhuma sala cadastrada.\n");
        return;
    }

    while (fread(&sala_temp, sizeof(Sala), 1, f_salas) == 1) {
        if (sala_temp.status == 0) continue; 

        encontrou_salas = 1;
        int total_ingressos_vendidos_sala = 0;
        int total_capacidade_sessoes_sala = 0;
        Sessao sessao_temp;

        FILE* f_sessoes = fopen(ARQUIVO_SESSOES, "rb");
        if (f_sessoes != NULL) {
            while (fread(&sessao_temp, sizeof(Sessao), 1, f_sessoes) == 1) {
                if (sessao_temp.id_sala == sala_temp.id_sala && sessao_temp.status == 1) {
                    
                    int ingressos_sessao = 0;
                    float receita_ignorar = 0; 
                    helperCalcularVendasSessao(sessao_temp.id_sessao, &ingressos_sessao, &receita_ignorar);
                    
                    total_ingressos_vendidos_sala += ingressos_sessao;
                    total_capacidade_sessoes_sala += sala_temp.capacidade;
                }
            }
            fclose(f_sessoes);
        }

        float taxa_media_ocupacao = 0.0f;
        if (total_capacidade_sessoes_sala > 0) {
            taxa_media_ocupacao = ((float)total_ingressos_vendidos_sala / (float)total_capacidade_sessoes_sala) * 100.0f;
        }

        printf("----------------------------------------\n");
        printf("Sala: %d (ID: %d)\n", sala_temp.numero_sala, sala_temp.id_sala);
        printf("  Capacidade por sessao: %d assentos\n", sala_temp.capacidade);
        printf("  Total de Ingressos Vendidos (em todas sessoes): %d\n", total_ingressos_vendidos_sala);
        printf("  Capacidade Total Ofertada (em todas sessoes): %d\n", total_capacidade_sessoes_sala);
        printf("  Taxa Media de Ocupacao: %.2f%%\n", taxa_media_ocupacao);
    }

    if (!encontrou_salas) {
        printf("Nenhuma sala ativa encontrada.\n");
    }
    printf("----------------------------------------\n");
    fclose(f_salas);
}


// Limpa o buffer
void limparBufferTecladoRelatorios() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função de Menu (Chamada pelo main.c) ---
void menuRelatorios() {
    int opcao;

    do {
        printf("\n--- Menu de Relatorios ---\n");
        printf("1. Relatorio de Vendas por Sessao\n");
        printf("2. Relatorio de Ocupacao por Sala\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("-----------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoRelatorios();
            opcao = -1; 
            continue;
        }
        limparBufferTecladoRelatorios();

        switch (opcao) {
            case 1:
                relatorioVendasPorSessao();
                break;
            case 2:
                relatorioOcupacaoPorSala();
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