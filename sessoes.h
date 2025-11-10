/*
 * Arquivo: sessoes.h
 * Descrição: Header (cardápio) para o módulo de gerenciamento de sessões.
 * Define as estruturas de dados e os protótipos das funções públicas.
 */

// 1. "Guardas de Inclusão"
// Evitam que este arquivo seja incluído mais de uma vez por engano,
// o que causaria erros de "redefinição".
#ifndef SESSOES_H
#define SESSOES_H

// --- Estruturas de Dados ---

// Struct auxiliar para a Data
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// Struct auxiliar para o Horário
typedef struct {
    int hora;
    int minuto;
} Horario;

// Struct principal para a Sessão
typedef struct {
    int id_sessao;      // ID único (ex: 1, 2, 3...)
    int id_filme;       // ID do filme (do módulo de filmes)
    int id_sala;        // ID da sala (do módulo de salas)
    Data data_sessao;   // Struct de Data aninhada
    Horario hora_sessao; // Struct de Horário aninhada
    float preco;
    int status;         // 1 = Ativa, 0 = Cancelada
} Sessao;

/*
 * Agendamento de uma nova sessão.
 * Pede os dados ao usuário e salva no arquivo.
 */
void agendarSessao();

/*
 * Exibe todas as sessões ativas (status == 1)
 * cadastradas no arquivo.
 */
void exibirSessoesDisponiveis();

/*
 * Cancela uma sessão.
 * Pede um ID ao usuário, busca no arquivo e muda seu status para 0.
 */
void cancelarSessao();
/*
 * Exibe o menu de gerenciamento de sessões
 * e permite ao usuário escolher as ações (agendar, listar, cancelar).
 */
void menuGerenciarSessoes();


#endif // Fim da guarda de inclusão

