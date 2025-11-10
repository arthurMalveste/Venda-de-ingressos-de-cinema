
#ifndef SALAS_H
#define SALAS_H

// --- Estrutura de Dados ---

typedef struct {
    int id_sala;        
    int numero_sala;    
    int capacidade;     
    int status;       
} Sala;


//Coleta os dados do usuário e salva uma nova sala no arquivo.
void cadastrarSala();

// Lê o arquivo e exibe todas as salas ativas (status == 1).
 
void listarSalas();

// Exibe o menu de gerenciamento de salase permite ao usuário escolher as ações.

void menuGerenciarSalas();


#endif 