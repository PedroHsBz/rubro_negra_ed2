/**
 * Exercicio 3 - Agenda de Senhas: remocao na Rubro-Negra (delete-fixup)
 * Adaptado de: LeetCode 450 - Delete Node in a BST
 *              beecrowd 1201 - Operacoes em ABP II
 *
 * Processa comandos sobre uma Rubro-Negra com sentinela NIL:
 *   I x  insere x            R x  remove x          B x  busca x
 *   P    pre-ordem com cores  H    altura e altura-negra
 * No fim imprime quantas vezes cada caso do delete-fixup ocorreu e quantas
 * rotacoes as remocoes fizeram.
 *
 * Complexidade: O(log N) por comando.
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

No *NIL;                 // sentinela negro: folhas e pai da raiz
No *raiz;
int casosRemocao[5];     // casos[1..4]
int rotacoesRemocao;
bool removendo = false;  // so conta rotacoes feitas durante remocoes

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
    if (removendo) rotacoesRemocao++;
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
    if (removendo) rotacoesRemocao++;
}

// ------------------------------------------------------------ insercao
void inserirFixup(No *z) {
    while (z->pai->cor == RUBRO) {
        No *pai = z->pai, *avo = pai->pai;
        bool paiEsquerdo = (pai == avo->esq);
        No *tio = paiEsquerdo ? avo->dir : avo->esq;
        if (tio->cor == RUBRO) {                         // caso 1
            pai->cor = tio->cor = NEGRO;
            avo->cor = RUBRO;
            z = avo;
        } else if (paiEsquerdo) {
            if (z == pai->dir) { z = pai; rotacaoEsquerda(z); pai = z->pai; }   // caso 2
            pai->cor = NEGRO; avo->cor = RUBRO; rotacaoDireita(avo);           // caso 3
        } else {
            if (z == pai->esq) { z = pai; rotacaoDireita(z); pai = z->pai; }
            pai->cor = NEGRO; avo->cor = RUBRO; rotacaoEsquerda(avo);
        }
    }
    raiz->cor = NEGRO;
}

bool inserir(int chave) {
    No *y = NIL, *x = raiz;
    while (x != NIL) {
        y = x;
        if (chave == x->chave) return false;
        x = (chave < x->chave) ? x->esq : x->dir;
    }
    No *z = new No{chave, RUBRO, NIL, NIL, y};
    if (y == NIL) raiz = z;
    else if (chave < y->chave) y->esq = z;
    else y->dir = z;
    inserirFixup(z);
    return true;
}

// ------------------------------------------------------------ remocao
No *buscar(int chave) {
    No *x = raiz;
    while (x != NIL && x->chave != chave) x = (chave < x->chave) ? x->esq : x->dir;
    return x;
}

No *minimo(No *x) {
    while (x->esq != NIL) x = x->esq;
    return x;
}

// Poe a subarvore v no lugar de u. Funciona mesmo com v == NIL
// (o NIL guarda temporariamente o pai, e o fixup usa essa informacao).
void transplante(No *u, No *v) {
    if (u->pai == NIL) raiz = v;
    else if (u == u->pai->esq) u->pai->esq = v;
    else u->pai->dir = v;
    v->pai = u->pai;
}

// x tem um "negro extra" (duplo-negro). Decide pela cor do irmao w e dos sobrinhos.
void removerFixup(No *x) {
    while (x != raiz && x->cor == NEGRO) {
        bool esquerdo = (x == x->pai->esq);
        No *w = esquerdo ? x->pai->dir : x->pai->esq;

        if (w->cor == RUBRO) {                           // Caso 1: irmao rubro
            casosRemocao[1]++;
            w->cor = NEGRO;
            x->pai->cor = RUBRO;
            if (esquerdo) rotacaoEsquerda(x->pai); else rotacaoDireita(x->pai);
            w = esquerdo ? x->pai->dir : x->pai->esq;    // novo irmao (negro)
        }
        No *perto = esquerdo ? w->esq : w->dir;          // sobrinho do lado de x
        No *longe = esquerdo ? w->dir : w->esq;          // sobrinho do lado oposto

        if (perto->cor == NEGRO && longe->cor == NEGRO) { // Caso 2: sobe o negro extra
            casosRemocao[2]++;
            w->cor = RUBRO;
            x = x->pai;
        } else {
            if (longe->cor == NEGRO) {                   // Caso 3: vira o caso 4
                casosRemocao[3]++;
                perto->cor = NEGRO;
                w->cor = RUBRO;
                if (esquerdo) rotacaoDireita(w); else rotacaoEsquerda(w);
                w = esquerdo ? x->pai->dir : x->pai->esq;
                longe = esquerdo ? w->dir : w->esq;
            }
            casosRemocao[4]++;                           // Caso 4: resolve
            w->cor = x->pai->cor;
            x->pai->cor = NEGRO;
            longe->cor = NEGRO;
            if (esquerdo) rotacaoEsquerda(x->pai); else rotacaoDireita(x->pai);
            x = raiz;
        }
    }
    x->cor = NEGRO;
}

bool remover(int chave) {
    No *z = buscar(chave);
    if (z == NIL) return false;
    removendo = true;

    No *y = z, *x;
    Cor corOriginal = y->cor;
    if (z->esq == NIL) {                     // nenhum filho ou so o direito
        x = z->dir;
        transplante(z, z->dir);
    } else if (z->dir == NIL) {              // so o esquerdo
        x = z->esq;
        transplante(z, z->esq);
    } else {                                 // dois filhos: sucessor ocupa o lugar
        y = minimo(z->dir);
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) x->pai = y;
        else {
            transplante(y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplante(z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    delete z;
    if (corOriginal == NEGRO) removerFixup(x);   // so sumir um NEGRO quebra a prop. 5
    removendo = false;
    return true;
}

// ------------------------------------------------------------ consultas
void preOrdem(No *no, vector<string> &saida) {
    if (no == NIL) return;
    saida.pb(to_string(no->chave) + (no->cor == RUBRO ? "R" : "N"));
    preOrdem(no->esq, saida);
    preOrdem(no->dir, saida);
}

int altura(No *no) { return no == NIL ? -1 : 1 + max(altura(no->esq), altura(no->dir)); }

int alturaNegra() {
    if (raiz == NIL) return 0;
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
    raiz = NIL;

    int Q;
    cin >> Q;
    while (Q--) {
        string cmd;
        cin >> cmd;
        if (cmd == "I") {
            int x; cin >> x;
            if (!inserir(x)) cout << x << " ja existe\n";
        } else if (cmd == "R") {
            int x; cin >> x;
            if (!remover(x)) cout << x << " nao existe\n";
        } else if (cmd == "B") {
            int x; cin >> x;
            cout << x << (buscar(x) != NIL ? " existe\n" : " nao existe\n");
        } else if (cmd == "P") {
            vector<string> v;
            preOrdem(raiz, v);
            if (v.empty()) cout << "vazia";
            for (size_t i = 0; i < v.size(); i++) cout << (i ? " " : "") << v[i];
            cout << "\n";
        } else if (cmd == "H") {
            cout << "altura " << altura(raiz) << ", altura-negra " << alturaNegra() << "\n";
        }
    }
    cout << "Casos de remocao: 1=" << casosRemocao[1] << " 2=" << casosRemocao[2]
         << " 3=" << casosRemocao[3] << " 4=" << casosRemocao[4] << "\n";
    cout << "Rotacoes nas remocoes: " << rotacoesRemocao << "\n";
    liberar(raiz);
    delete NIL;
    return 0;
}
