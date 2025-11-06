/*
 * Arquivo: sessoes.h
 * Descrição: Header (card�pio) para o m�dulo de gerenciamento de sess�es.
 * Define as estruturas de dados e os prot�tipos das fun��es p�blicas.
 */

// 1. "Guardas de Inclus�o"
// Evitam que este arquivo seja inclu�do mais de uma vez por engano,
// o que causaria erros de "redefini��o".
#ifndef SESSOES_H
#define SESSOES_H

// --- Estruturas de Dados ---

// Struct auxiliar para a Data
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// Struct auxiliar para o Hor�rio
typedef struct {
    int hora;
    int minuto;
} Horario;

// Struct principal para a Sess�o
typedef struct {
    int id_sessao;      // ID �nico (ex: 1, 2, 3...)
    int id_filme;       // ID do filme (do m�dulo de filmes)
    int id_sala;        // ID da sala (do m�dulo de salas)
    Data data_sessao;   // Struct de Data aninhada
    Horario hora_sessao; // Struct de Hor�rio aninhada
    float preco;
    int status;         // 1 = Ativa, 0 = Cancelada
} Sessao;


// --- Prot�tipos das Fun��es P�blicas ---
// Estas s�o as tr�s fun��es que prometemos fornecer.

/*
 * Agendamento de uma nova sess�o.
 * Pede os dados ao usu�rio e salva no arquivo.
 */
void agendarSessao();

/*
 * Exibe todas as sess�es ativas (status == 1)
 * cadastradas no arquivo.
 */
void exibirSessoesDisponiveis();

/*
 * Cancela uma sess�o.
 * Pede um ID ao usu�rio, busca no arquivo e muda seu status para 0.
 */
void cancelarSessao();
/*
 * Exibe o menu de gerenciamento de sess�es
 * e permite ao usu�rio escolher as a��es (agendar, listar, cancelar).
 */
void menuGerenciarSessoes();


#endif // Fim da guarda de inclus�o
