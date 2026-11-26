/**
 * @file desenho.hpp
 * @brief Desenha uma arvore binaria "em pe", com / e \ nos ramos.
 *
 * Exemplo de saida (N = negro, R = rubro):
 *
 *              7N
 *          /        \
 *        2R          11R
 *      /    \       /    \
 *     1N     5N    8N     14N
 *           /               \
 *          4R                15R
 *
 * Na Rubro-Negra as folhas nao sao nullptr, e sim o sentinela NIL.
 * Por isso a funcao recebe o ponteiro `nulo`: todo filho igual a `nulo`
 * e tratado como "nao existe" e nao aparece no desenho.
 */
#ifndef DESENHO_HPP
#define DESENHO_HPP

#include "comum.hpp"

template <typename No, typename Rotulo>
void desenharArvore(No *raiz, No *nulo, Rotulo rotulo) {
    if (raiz == nulo) {
        cout << "    (arvore vazia)\n";
        return;
    }

    // 1) Percurso em-ordem: cada no ganha uma coluna, da esquerda para a direita.
    map<No *, int> coluna;      // coluna do centro de cada no
    map<No *, int> nivel;       // andar do no (raiz = 0)
    int proxima = 0, maiorNivel = 0;

    function<void(No *, int)> posicionar = [&](No *no, int d) {
        if (no == nulo) return;
        posicionar(no->esq, d + 1);
        int largura = rotulo(no).size();
        coluna[no] = proxima + largura / 2;
        nivel[no] = d;
        proxima += largura + 2;
        maiorNivel = max(maiorNivel, d);
        posicionar(no->dir, d + 1);
    };
    posicionar(raiz, 0);

    // 2) Uma linha para os nos de cada andar e, entre elas, uma linha para os ramos.
    vector<string> linhas(2 * maiorNivel + 1, string(proxima + 1, ' '));
    for (auto &par : coluna) {
        No *no = par.first;
        int x = par.second, d = nivel[no];
        string texto = rotulo(no);
        linhas[2 * d].replace(x - texto.size() / 2, texto.size(), texto);

        if (no->esq != nulo) linhas[2 * d + 1][(x + coluna[no->esq]) / 2] = '/';
        if (no->dir != nulo) linhas[2 * d + 1][(x + coluna[no->dir] + 1) / 2] = '\\';
    }

    // 3) Imprime tirando os espacos que sobram no fim de cada linha.
    for (string &linha : linhas) {
        linha.erase(linha.find_last_not_of(' ') + 1);
        cout << "    " << linha << "\n";
    }
}

#endif // DESENHO_HPP
