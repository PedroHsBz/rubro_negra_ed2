/**
 * @file demo_rb.cpp
 * @brief Demonstracao passo a passo da Arvore Rubro-Negra (usada na videoaula).
 *
 * Partes:
 *   1. Insercao chave a chave, mostrando os casos do insert-fixup.
 *   2. A insercao do 4, que passa pelos casos 1, 2 e 3 de uma vez (Cormen, fig. 13.4),
 *      e a mesma arvore vista como Arvore 2-3-4.
 *   3. Remocoes cobrindo os quatro casos do delete-fixup.
 *   4. Chaves em ordem crescente: BST degenera, Rubro-Negra continua baixa.
 *
 * Rotulos: 7N = no 7 NEGRO, 2R = no 2 RUBRO. As folhas NIL nao aparecem no desenho.
 */
#include "RedBlackTree.hpp"

static void titulo(const string &t) {
    cout << "\n==================================================================\n"
         << t << "\n==================================================================\n";
}

static void conferir(const RedBlackTree &t) {
    string motivo;
    bool ok = t.valida(&motivo);
    cout << "    altura = " << t.altura() << " | altura-negra = " << t.alturaNegra()
         << " | 5 propriedades: " << (ok ? "OK" : "QUEBRADAS (" + motivo + ")") << "\n";
}

int main() {
    RedBlackTree t;
    t.rastrear = true;

    titulo("PARTE 1 - Insercao passo a passo: 11 2 14 1 7 15 5 8");
    for (int k : {11, 2, 14, 1, 7, 15, 5, 8}) {
        cout << "\n>> inserir(" << k << ")  [entra RUBRO]\n";
        t.inserir(k);
        t.imprimir();
    }
    conferir(t);

    titulo("PARTE 2 - inserir(4): casos 1, 2 e 3 em sequencia");
    cout << "Antes:\n";
    t.imprimir();
    cout << "\n>> inserir(4)\n";
    t.inserir(4);
    cout << "Depois (" << t.rotacoesUltimaOp << " rotacoes):\n";
    t.imprimir();
    conferir(t);
    cout << "\nA mesma arvore como Arvore 2-3-4 (negro + filhos rubros = um no-B):\n";
    for (const string &linha : t.niveis234()) cout << "    " << linha << "\n";

    titulo("PARTE 3 - Remocao e os quatro casos do duplo-negro");
    for (int k : {1, 8, 15, 14, 11}) {
        cout << "\n>> remover(" << k << ")\nAntes:\n";
        t.imprimir();
        t.remover(k);
        cout << "Depois (" << t.rotacoesUltimaOp << " rotacoes):\n";
        t.imprimir();
        conferir(t);
    }

    titulo("PARTE 4 - 1023 chaves em ordem crescente");
    RedBlackTree grande;
    for (int k = 1; k <= 1023; k++) grande.inserir(k);
    cout << "    BST comum ............ altura 1022 (vira uma lista)\n";
    cout << "    AVL .................. altura 9\n";
    cout << "    Rubro-Negra .......... altura " << grande.altura()
         << " (limite 2*log2(n+1) = 20)\n";
    cout << "    Rotacoes no total .... " << grande.rotacoes << "\n";
    conferir(grande);

    cout << "\nResumo dos casos usados na arvore pequena:\n"
         << "    insercao: caso1=" << t.casosInsercao[1] << " caso2=" << t.casosInsercao[2]
         << " caso3=" << t.casosInsercao[3] << "\n"
         << "    remocao : caso1=" << t.casosRemocao[1] << " caso2=" << t.casosRemocao[2]
         << " caso3=" << t.casosRemocao[3] << " caso4=" << t.casosRemocao[4] << "\n";
    return 0;
}
