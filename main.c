#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

// Incluindo os headers de cada módulo
#include "gerenciar_filme/filmes.h"
#include "gerenciar_sala/salas.h"
#include "gerenciar_sessao/sessoes.h"
#include "gerenciar_vendas/vendas.h"
#include "gerenciar_relatorio/relatorios.h"

// --- Constantes Globais ---
#define ARQUIVO_FILMES "filmes.dat"
#define SENHA_ADMIN "abc123" // <<< SENHA DO ADMINISTRADOR

// --- Protótipos das funções neste arquivo ---
void limparBufferTecladoMain();
void tentarLoginAdmin();
void menuAdministrador();
void menuCliente();


// --- Função de Limpeza ---
void limparBufferTecladoMain() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// FUNÇÃO PRINCIPAL (SELETOR DE PERFIL)
int main() {
    int opcao;

    printf("Sistema de Cinema carregado.\n");

    do {
        printf("\n\n--- BEM-VINDO AO VIEWCINEMAS ---\n");
        printf("Selecione seu tipo de acesso:\n");
        printf("1. Sou Administrador\n");
        printf("2. Sou Cliente\n");
        printf("0. Sair do Sistema\n");
        printf("--------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite um numero.\n");
            limparBufferTecladoMain();
            opcao = -1; 
            continue;
        }
        limparBufferTecladoMain();

        switch (opcao) {
            case 1:
                tentarLoginAdmin(); // <<< MUDANÇA: Chama a tela de login
                break;
            case 2:
                menuCliente(); 
                break;
            case 0:
                printf("\nSalvando dados e encerrando o sistema...\n");
                printf("Ate logo!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0); 

    return 0;
}


// NOVA FUNÇÃO DE LOGIN DO ADMIN

void tentarLoginAdmin() {
    char senha_digitada[50];

    printf("\n--- Login Administrador ---\n");
    printf("Digite a senha de acesso: ");
    
    // Lê a senha
    scanf(" %49[^\n]", senha_digitada);
    limparBufferTecladoMain();

    // Compara a senha digitada com a senha definida
    if (strcmp(senha_digitada, SENHA_ADMIN) == 0) {
        printf("\nLogin bem-sucedido! Acessando painel...\n");
        menuAdministrador(); // Sucesso: Chama o menu admin
    } else {
        printf("\n>>> SENHA INCORRETA. Acesso negado. <<<\n");
        // Falha: Volta para o menu principal
    }
}


// MENU DO ADMINISTRADOR
void menuAdministrador() {
    int opcao;
    
    do {
        printf("\n\n--- PAINEL DO ADMINISTRADOR ---\n");
        printf("1. Gerenciar Filmes\n");
        printf("2. Gerenciar Salas\n");
        printf("3. Gerenciar Sessoes\n");
        printf("4. Realizar Venda (Teste)\n"); 
        printf("5. Gerar Relatorios\n");
        printf("0. Voltar (Fazer Logout)\n"); // Volta para a tela de seleção de perfil
        printf("-------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoMain();
            opcao = -1;
            continue;
        }
        limparBufferTecladoMain();

        switch (opcao) {
            case 1:
                menuGerenciarFilmes(ARQUIVO_FILMES);
                break;
            case 2:
                menuGerenciarSalas();
                break;
            case 3:
                menuGerenciarSessoes();
                break;
            case 4:
                menuRealizarVenda(); 
                break;
            case 5:
                menuRelatorios();
                break;
            case 0:
                printf("\nFazendo logout do painel de administrador...\n");
                break; // Sai deste 'do-while' e volta para o 'main'
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0); // Loop do menu Admin
}


// MENU DO CLIENTE

void menuCliente() {
    int opcao;

    do {
        printf("\n\n--- AREA DO CLIENTE ---\n");
        printf("O que deseja fazer?\n");
        printf("1. Ver Filmes em Cartaz\n");
        printf("2. Comprar Ingresso\n");
        printf("0. Voltar\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoMain();
            opcao = -1;
            continue;
        }
        limparBufferTecladoMain();

        switch (opcao) {
            case 1:
                printf("\n--- Filmes em Cartaz ---\n");
                listarFilmes(ARQUIVO_FILMES);
                printf("-------------------------\n");
                break;
            case 2:
                menuRealizarVenda();
                break;
            case 0:
                printf("\nObrigado por visitar. Volte sempre!\n");
                break; 
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0); // Loop do menu Cliente
}