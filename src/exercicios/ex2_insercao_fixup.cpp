/**
 * Exercicio 2 - Insercao na Rubro-Negra e os casos do insert-fixup
 * Adaptado de: HackerRank - Self Balancing Tree (versao rubro-negra)
 *              LeetCode 102 - Binary Tree Level Order Traversal
 *
 * Insere N chaves em uma Rubro-Negra vazia (com sentinela NIL) e imprime:
 * o percurso em largura com as cores, a altura, a altura-negra, quantas
 * vezes cada caso do insert-fixup ocorreu e o total de rotacoes.
 *
 * Complexidade: O(log N) por insercao -> O(N log N) no total.
 */
#include <bits/stdc++.h>
#include <string>
using namespace std;

#define IOFAST() ios_base::sync_with_stdio(false); cin.tie(nullptr)
#define ulli unsigned long long int
#define uli unsigned long int
#define lli long long int

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;

#define INF 0x3f3f3f3f
#define LINF 0x3f3f3f3f3f3f3f3fLL
#define pb push_back
#define mp make_pair
#define pq priority_queue
#define LSONE(s) ((s) & (-(s)))
#define DEG_to_RAD(x) (((x) * PI) / 180.0)
#define F first
#define S second
#define PI acos(-1)

#include <iostream>
#include <utility>

enum Cor { RUBRO, NEGRO };

struct No {
    int chave;
    Cor cor;
    No *esq, *dir, *pai;
};

No *NIL;            // sentinela: unico no negro que faz papel de todas as folhas
No *raiz;
int casos[4];       // casos[1..3]
int rotacoes;

No *novoNo(int chave) {
    No *z = new No{chave, RUBRO, NIL, NIL, NIL};   // todo no novo entra RUBRO
    return z;
}

void rotacaoEsquerda(No *x) {
    No *y = x->dir;
    x->dir = y->esq;
    if (y->esq != NIL) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL) raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    rotacoes++;
}

void rotacaoDireita(No *x) {
    No *y = x->esq;
    x->esq = y->dir;
    if (y->dir != NIL) y->dir->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL) raiz = y;
    else if (x == x->pai->dir) x->pai->dir = y;
    else x->pai->esq = y;
    y->dir = x;
    x->pai = y;
    rotacoes++;
}

// Enquanto houver dois rubros seguidos (z e o pai), olha a cor do TIO.
void inserirFixup(No *z) {
    while (z->pai->cor == RUBRO) {
        No *pai = z->pai, *avo = pai->pai;
        bool paiEsquerdo = (pai == avo->esq);
        No *tio = paiEsquerdo ? avo->dir : avo->esq;

        if (tio->cor == RUBRO) {                          // Caso 1: so recolore
            casos[1]++;
            pai->cor = NEGRO;
            tio->cor = NEGRO;
            avo->cor = RUBRO;
            z = avo;                                      // o conflito pode subir
            continue;
        }
        if (paiEsquerdo) {
            if (z == pai->dir) {                          // Caso 2: triangulo
                casos[2]++;
                z = pai;
                rotacaoEsquerda(z);
                pai = z->pai;
            }
            casos[3]++;                                   // Caso 3: linha
            pai->cor = NEGRO;
            avo->cor = RUBRO;
            rotacaoDireita(avo);
        } else {                                          // espelho
            if (z == pai->esq) {
                casos[2]++;
                z = pai;
                rotacaoDireita(z);
                pai = z->pai;
            }
            casos[3]++;
            pai->cor = NEGRO;
            avo->cor = RUBRO;
            rotacaoEsquerda(avo);
        }
    }
    raiz->cor = NEGRO;                                    // propriedade 2
}

void inserir(int chave) {
    No *y = NIL, *x = raiz;
    while (x != NIL) {                                    // descida de BST
        y = x;
        if (chave == x->chave) return;                    // repetida: ignora
        x = (chave < x->chave) ? x->esq : x->dir;
    }
    No *z = novoNo(chave);
    z->pai = y;
    if (y == NIL) raiz = z;
    else if (chave < y->chave) y->esq = z;
    else y->dir = z;
    inserirFixup(z);
}

int altura(No *no) { return no == NIL ? -1 : 1 + max(altura(no->esq), altura(no->dir)); }

int alturaNegra() {                                       // conta o NIL, nao conta a raiz
    int bh = 0;
    for (No *x = raiz->esq; ; x = x->esq) {
        if (x->cor == NEGRO) bh++;
        if (x == NIL) break;
    }
    return bh;
}

void liberar(No *no) {
    if (no == NIL) return;
    liberar(no->esq);
    liberar(no->dir);
    delete no;
}

int main() {
    IOFAST();
    NIL = new No{0, NEGRO, nullptr, nullptr, nullptr};
    NIL->esq = NIL->dir = NIL->pai = NIL;

    int T;
    cin >> T;
    for (int caso = 1; caso <= T; caso++) {
        raiz = NIL;
        rotacoes = 0;
        casos[1] = casos[2] = casos[3] = 0;

        int N;
        cin >> N;
        for (int i = 0; i < N; i++) {
            int k;
            cin >> k;
            inserir(k);
        }

        cout << "Caso " << caso << ":\nNivel:";
        queue<No *> fila;                                 // percurso em largura
        fila.push(raiz);
        while (!fila.empty()) {
            No *no = fila.front();
            fila.pop();
            cout << " " << no->chave << (no->cor == RUBRO ? "R" : "N");
            if (no->esq != NIL) fila.push(no->esq);
            if (no->dir != NIL) fila.push(no->dir);
        }
        cout << "\nAltura: " << altura(raiz)
             << "\nAltura-negra: " << alturaNegra()
             << "\nCasos: 1=" << casos[1] << " 2=" << casos[2] << " 3=" << casos[3]
             << "\nRotacoes: " << rotacoes << "\n\n";
        liberar(raiz);
    }
    delete NIL;
    return 0;
}
