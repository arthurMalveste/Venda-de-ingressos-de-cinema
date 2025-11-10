#ifndef FILMES_H
#define FILMES_H
#include <stdio.h>

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


//menu de gerenciamento de filmes
void menuGerenciarFilmes(const char *arquivo);

// Nova função para coletar dados do usuário
Filme coletarDadosFilme();

//adicionar filme
void adicionarFilmes(const char *arquivo, Filme filme_novo);

//atualizar as informações do filme
void atualizarInfoFilmes(const char *arquivo, const char *titulo, Filme atualizado);

//listar filme
void listarFilmes(const char *arquivo);

// remover filme
void removerFilmes(const char *arquivo, const char *titulo);



#endif //FILMES_H