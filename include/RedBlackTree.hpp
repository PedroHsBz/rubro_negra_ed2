/**
 * @file RedBlackTree.hpp
 * @brief Arvore Rubro-Negra (Red-Black Tree) com no sentinela NIL.
 *
 * As cinco propriedades que a arvore mantem depois de toda operacao:
 *   1. Todo no e RUBRO ou NEGRO.
 *   2. A raiz e NEGRA.
 *   3. Toda folha (o sentinela NIL) e NEGRA.
 *   4. Um no RUBRO nunca tem filho RUBRO (nada de dois rubros seguidos).
 *   5. Todo caminho de um no ate as folhas NIL abaixo dele passa pelo
 *      mesmo numero de nos NEGROS (altura-negra).
 *
 * Consequencia: altura <= 2 * log2(n + 1), entao buscar, inserir e remover
 * custam O(log n). A insercao faz no maximo 2 rotacoes e a remocao no maximo 3.
 *
 * Sobre o sentinela NIL: em vez de usar nullptr, todas as folhas (e o pai da
 * raiz) apontam para UM unico no NIL, que e sempre NEGRO. Assim o codigo pode
 * perguntar "x->cor" mesmo quando x e uma folha, sem testar nullptr.
 */
#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

#include "comum.hpp"

enum Cor { RUBRO, NEGRO };

struct NoRB {
    int chave;
    Cor cor;
    NoRB *esq, *dir, *pai;
    NoRB(int c, Cor k, NoRB *nil) : chave(c), cor(k), esq(nil), dir(nil), pai(nil) {}
};

class RedBlackTree {
public:
    RedBlackTree();
    ~RedBlackTree();

    bool inserir(int chave);          // false se a chave ja existe
    bool remover(int chave);          // false se a chave nao existe
    bool buscar(int chave) const;

    vi emOrdem() const;               // chaves em ordem crescente
    vector<string> preOrdem() const;  // "7N", "2R", ...
    vector<string> emLargura() const; // nivel por nivel

    int altura() const;               // em arestas: vazia = -1, so a raiz = 0
    int alturaNegra() const;          // nos negros da raiz ate NIL (sem a raiz, com o NIL)
    int tamanho() const { return tamanho_; }
    bool vazia() const { return raiz_ == NIL; }

    // Confere as 5 propriedades, a ordem de BST e os ponteiros pai.
    // Devolve true se tudo esta certo; senao, escreve o motivo em `motivo`.
    bool valida(string *motivo = nullptr) const;

    void imprimir() const;            // desenho da arvore com / e \.

    // Mesma arvore vista como Arvore B de ordem 4 (2-3-4): cada no NEGRO
    // junta seus filhos RUBROS em um unico no-B. Uma linha por nivel.
    vector<string> niveis234() const;

    // Ferramentas de aula: com rastrear = true, cada caso do fixup e impresso.
    bool rastrear = false;
    int rotacoes = 0;                 // total desde a criacao
    int rotacoesUltimaOp = 0;         // rotacoes da ultima insercao/remocao
    int casosInsercao[4] = {0, 0, 0, 0};  // indices 1..3
    int casosRemocao[5] = {0, 0, 0, 0, 0}; // indices 1..4

private:
    NoRB *NIL;        // sentinela: unico, negro, representa todas as folhas
    NoRB *raiz_;
    int tamanho_;

    NoRB *buscarNo(int chave) const;
    NoRB *minimo(NoRB *x) const;

    void rotacaoEsquerda(NoRB *x);
    void rotacaoDireita(NoRB *x);

    void inserirFixup(NoRB *z);
    void transplante(NoRB *u, NoRB *v);
    void removerFixup(NoRB *x);

    void liberar(NoRB *no);
    int alturaRec(NoRB *no) const;
    int conferir(NoRB *no, long long lo, long long hi, string &erro) const;
    string rotulo(NoRB *no) const;
    void log(const string &msg) const;
};

#endif // REDBLACKTREE_HPP
