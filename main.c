#include <stdio.h>
#include <stdlib.h>

// Incluindo os headers de cada módulo
#include "gerenciar_filme/filmes.h"
#include "gerenciar_sala/salas.h"
#include "gerenciar_sessao/sessoes.h"
#include "gerenciar_vendas/vendas.h"
#include "gerenciar_relatorio/relatorios.h"

// Definir os nomes dos arquivos aqui facilita a manutenção
#define ARQUIVO_FILMES "filmes.dat"

// Função para limpar o buffer do teclado
void limparBufferTecladoMain() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int opcao;

    do {
        printf("\n\n--- MENU PRINCIPAL - VIEWCINEMAS ---\n");
        printf("1. Gerenciar Filmes\n");
        printf("2. Gerenciar Salas\n");
        printf("3. Gerenciar Sessoes\n");
        printf("4. Realizar Venda de Ingresso\n");
        printf("5. Gerar Relatorios\n");
        printf("0. Sair do Sistema\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite um numero.\n");
            limparBufferTecladoMain();
            opcao = -1; // Reseta a opção para continuar no loop
            continue;   // Pula o resto do loop
        }
        limparBufferTecladoMain(); // Limpa o "Enter"

        switch (opcao) {
            case 1:
                menuGerenciarFilmes(ARQUIVO_FILMES); // Chama o menu de Filmes
                break;
            case 2:
                menuGerenciarSalas(); // Chama o menu de Salas
                break;
            case 3:
                menuGerenciarSessoes(); // Chama o menu de Sessões
                break;
            case 4:
                menuRealizarVenda(); // Chama o menu de Vendas
                break;
            case 5:
                menuRelatorios(); // Chama o menu de relatório
                break;
            case 0:
                printf("\nSalvando dados e encerrando o sistema...\n");
                printf("Ate logo!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0); // O loop continua enquanto a opção não for 0

    return 0;
}