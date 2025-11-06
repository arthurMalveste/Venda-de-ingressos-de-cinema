#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filmes.h"
#define BLOCO 10


//Struct dos filmes (Suas informações)
typedef struct{
    int ID;
    char titulo[50];
    char genero[50];
    char sinopse[700];
    int duracao;
    int classificacao;
    int cartaz;
    float valor_ingresso;
} Filme;


//função para adicionar os filmes
void adicionarFilmes(const char *arquivo, Filme filme_novo){


    //abrindo o arquivo para poder adicionar o filme
    FILE *file = fopen(arquivo, "ab");
    if (file == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    //escrevendo um novo filme
    fwrite(&filme_novo, sizeof(Filme), 1, arquivo);
    fclose(file);
    printf("Novo filme cadastrado.\n");
};

//atualizar os filmes
void atualizarInfoFilmes(const char *arquivo, const char *titulo, Filme atualizado){

};


//listar filme
void listarFilmes(const char *arquivo){};


//buscar um único filme
void buscarFilmes(const char *arquivo){
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
                printf("Filme: %s; \nGenêro: %s; \nSinopse: %s; \nDuração: %dmin; \nClassificação indicativa: %d; \n ");
            }
        }
    }


};


//remover filme
void removerFilmes(const char *arquivo, const char *titulo){};