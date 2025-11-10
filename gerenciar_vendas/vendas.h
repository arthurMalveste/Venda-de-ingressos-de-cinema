#ifndef VENDAS_H
#define VENDAS_H

// --- Estrutura de Dados ---

typedef struct {
    int id_venda;           // ID único da transação
    int id_sessao;          // A sessão para a qual esta venda foi feita
    int quantidade;         // Quantos ingressos foram comprados
    float valor_total_pago; // O valor total (preco_sessao * quantidade)
} Venda;


// --- Protótipos das Funções Públicas ---


void menuRealizarVenda();

#endif