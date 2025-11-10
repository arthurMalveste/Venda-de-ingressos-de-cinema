#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filmes.h"
#define BLOCO 10



// PARA COLETAR DADOS DO USUÁRIO
Filme coletarDadosFilme(){
    Filme novo_filme;

    printf("\n--- Cadastro de Novo Filme ---\n");

    // NOTA: O 'cartaz' será definido como 1 (em cartaz) automaticamente.
    novo_filme.cartaz = 1;

    // INFORMAR O ID:
    printf("Digite o ID do filme: ");
    scanf("%d", &novo_filme.ID);

    // IMPORTANTE: O espaço antes do "%" em " %[^\n]"
    // serve para limpar o "Enter" (newline) deixado pelo scanf anterior.
    printf("Digite o Titulo: ");
    scanf(" %[^\n]", novo_filme.titulo);

    printf("Digite o Genero: ");
    scanf(" %[^\n]", novo_filme.genero);

    printf("Digite a Sinopse (ate 700 caracteres): ");
    scanf(" %[^\n]", novo_filme.sinopse);

    printf("Digite a Duracao (em minutos): ");
    scanf("%d", &novo_filme.duracao);

    printf("Digite a Classificacao Indicativa: ");
    scanf("%d", &novo_filme.classificacao);

    printf("Digite o Valor do Ingresso (ex: 30.50): ");
    scanf("%f", &novo_filme.valor_ingresso);

    printf("\nFilme '%s' pronto para ser cadastrado!\n", novo_filme.titulo);

    return novo_filme; // Retorna o filme preenchido
}


//função para adicionar os filmes
void adicionarFilmes(const char *arquivo, Filme filme_novo){


    //abrindo o arquivo para poder adicionar o filme
    FILE *file = fopen(arquivo, "ab");
    if (file == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    //escrevendo um novo filme
    fwrite(&filme_novo, sizeof(Filme), 1, file);
    fclose(file);
    printf("Novo filme cadastrado.\n");
};

//atualizar os filmes
void atualizarInfoFilmes(const char *arquivo, const char *titulo, Filme atualizado){
    FILE *file = fopen(arquivo, "rb+");
    if(file == NULL){
        printf("Erro ao abrir o arquivo. \n");
        return;
    }

    Filme filmes[BLOCO];
    size_t numLidos;


    //Leitura do arquivo em blocos, buscando registro para atualização
    while((numLidos = fread(filmes, sizeof(Filme), BLOCO, file)) > 0){
        for (size_t i = 0; i < numLidos; i++){
            if(strcmp(filmes[i].titulo, titulo) == 0 && filmes[i].cartaz){
                filmes[i] = atualizado;


                // Volte o número de structs * o tamanho de cada struct
                fseek(file, -((long)numLidos * sizeof(Filme)), SEEK_CUR); 
                fwrite(filmes, sizeof(Filme), numLidos, file);
                printf("Filme: %s foi atualizado com sucesso!! :) \n", titulo);
                printf("---------------------------------------------- \n");
                printf(
                "Filme: %s;\n"
                "Genero: %s; \n"
                "Sinopse: %s; \n"
                "Duração: %dmin; \n"
                "Classificacao indicativa: %d; \n"
                "Cartaz: %d; \n"
                "Valor do Ingresso para esse filme: %.2f; \n"
                "------------------------------------------------------ \n", 
                filmes[i].titulo, 
                filmes[i].genero, 
                filmes[i].sinopse, 
                filmes[i].duracao, 
                filmes[i].classificacao, 
                filmes[i].cartaz, 
                filmes[i].valor_ingresso);
                fclose(file);
                return;
            }
        }
    }

    printf("O filme %s não foi encontrado em cartaz. \n", titulo);
    fclose(file);

};

//listar filme
void listarFilmes(const char *arquivo){
    FILE *file = fopen(arquivo, "rb");
       if (file == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Filme filmes[BLOCO];
    size_t numLidos;

    while((numLidos = fread(filmes, sizeof(Filme), BLOCO, file)) > 0){
        for(size_t i = 0; i < numLidos; i++){
            if(filmes[i].cartaz){
                printf(
                "Filme: %s;\n"
                "Genero: %s; \n"
                "Sinopse: %s; \n"
                "Duracao: %dmin; \n"
                "Classificacaoo indicativa: %d; \n"
                "Cartaz: %d; \n"
                "Valor do Ingresso para esse filme: %.2f; \n"
                "------------------------------------------------------ \n", 
                filmes[i].titulo, 
                filmes[i].genero, 
                filmes[i].sinopse, 
                filmes[i].duracao, 
                filmes[i].classificacao, 
                filmes[i].cartaz, 
                filmes[i].valor_ingresso);
            }
        }
    }


};

// remover filme
void removerFilmes(const char *arquivo, const char *titulo){
    FILE *file = fopen(arquivo, "rb+");
    
    if(file == NULL){
        printf("Erro ao abrir o arquivo \n");
        return;
    }

    Filme filmes[BLOCO];
    size_t numLidos;

    while((numLidos = fread(filmes, sizeof(Filme), BLOCO, file)) > 0){
        for(size_t i = 0; i < numLidos; i++){
            if(strcmp(filmes[i].titulo, titulo) == 0 && filmes[i].cartaz){
               filmes[i].cartaz = 0;
               
               
             
            fseek(file, -((long)numLidos * sizeof(Filme)), SEEK_CUR); 
            fwrite(filmes, sizeof(Filme), numLidos, file);
            printf("O Filme '%s' foi excluido com sucesso!! :) \n", titulo);
            fclose(file);
             return;
            }
        }
    }

    

};


// Adicione esta função de limpeza (igual às outras)
void limparBufferTecladoFilmes() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Adicione esta função principal do menu de filmes
void menuGerenciarFilmes(const char *arquivo) {
    int opcao;
    
    do {
        printf("\n--- GESTAO DE FILMES ---\n");
        printf("1. Cadastrar Novo Filme\n");
        printf("2. Listar Filmes em Cartaz\n");
        printf("3. Atualizar Filme\n");
        printf("4. Remover Filme\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("--------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBufferTecladoFilmes();
            opcao = -1; 
            continue;
        }
        limparBufferTecladoFilmes(); // Limpa o buffer após ler o número

        switch (opcao) {
            case 1: { // Cadastrar
                Filme novo_filme = coletarDadosFilme();
                adicionarFilmes(arquivo, novo_filme);
                break;
            }
            case 2: { // Listar
                listarFilmes(arquivo);
                break;
            }
            case 3: { // Atualizar
                char titulo_antigo[50];
                printf("Digite o Titulo do filme que deseja atualizar: ");
                scanf(" %[^\n]", titulo_antigo);
                limparBufferTecladoFilmes();
                
                printf("\n--- Digite os novos dados do filme ---\n");
                Filme filme_atualizado = coletarDadosFilme();
                

                atualizarInfoFilmes(arquivo, titulo_antigo, filme_atualizado);
                break;
            }
            case 4: { // Remover
                char titulo_remover[50];
                printf("Digite o Titulo do filme que deseja REMOVER: ");
                scanf(" %[^\n]", titulo_remover);
                limparBufferTecladoFilmes();
                
                removerFilmes(arquivo, titulo_remover);
                break;
            }
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}