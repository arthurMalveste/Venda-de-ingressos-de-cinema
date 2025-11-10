#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filmes.h"
#define BLOCO 10



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


                fseek(file, -((long)numLidos), SEEK_CUR);
                fwrite(filmes, sizeof(Filme), numLidos, file);
                printf("Filme: %s foi atualizado com sucesso!! :) \n", titulo);
                printf("---------------------------------------------- \n");
                printf(
                "Filme: %s;\n"
                "Genêro: %s; \n"
                "Sinopse: %s; \n"
                "Duração: %dmin; \n"
                "Classificação indicativa: %d; \n"
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
                "Genêro: %s; \n"
                "Sinopse: %s; \n"
                "Duração: %dmin; \n"
                "Classificação indicativa: %d; \n"
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
               
               
               fseek(file, -((long) sizeof(Filme)), SEEK_CUR);
               fwrite(filmes, sizeof(Filme), numLidos, file);
               printf("O Filme '%s' foi excluído com sucesso!! :) \n", titulo);
               fclose(file);
               return;
            }
        }
    }

};