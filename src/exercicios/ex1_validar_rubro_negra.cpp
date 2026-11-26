/**
 * Exercicio 1 - Isto e uma Arvore Rubro-Negra?
 * Adaptado de: PAT (Advanced Level) 1135 - Is It A Red-Black Tree
 *
 * Recebe o percurso em pre-ordem de uma BST (chave negativa = no RUBRO,
 * positiva = no NEGRO) e diz se a arvore respeita as propriedades da
 * Rubro-Negra. Quando nao respeita, informa a primeira regra quebrada.
 *
 * Complexidade: O(N * h) para montar a BST e O(N) para validar.
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

struct No {
    int chave;
    bool rubro;
    No *esq, *dir;
    No(int c, bool r) : chave(c), rubro(r), esq(nullptr), dir(nullptr) {}
};

// Insercao comum de BST: a pre-ordem de uma BST reconstroi a mesma arvore.
No *inserir(No *no, int chave, bool rubro) {
    if (no == nullptr) return new No(chave, rubro);
    if (chave < no->chave) no->esq = inserir(no->esq, chave, rubro);
    else no->dir = inserir(no->dir, chave, rubro);
    return no;
}

void liberar(No *no) {
    if (no == nullptr) return;
    liberar(no->esq);
    liberar(no->dir);
    delete no;
}

string erro;   // primeira regra quebrada (vazio = tudo certo)

/*
 * Devolve a altura-negra da subarvore contando a folha nula (NIL), que e
 * NEGRA (propriedade 3). Ao mesmo tempo confere:
 *   - propriedade 4: no rubro nao pode ter filho rubro;
 *   - propriedade 5: os dois lados precisam ter a mesma altura-negra.
 * Percorre em pos-ordem: primeiro os filhos, depois o proprio no.
 */
int alturaNegra(No *no) {
    if (no == nullptr) return 1;                         // NIL conta como 1 negro
    if (no->rubro && ((no->esq && no->esq->rubro) || (no->dir && no->dir->rubro)) && erro.empty())
        erro = "rubros consecutivos abaixo de " + to_string(no->chave);
    int e = alturaNegra(no->esq);
    int d = alturaNegra(no->dir);
    if (e != d && erro.empty())
        erro = "altura-negra diferente em " + to_string(no->chave) +
               " (" + to_string(e) + " x " + to_string(d) + ")";
    return max(e, d) + (no->rubro ? 0 : 1);
}

int main() {
    IOFAST();
    int K;
    cin >> K;
    while (K--) {
        int N;
        cin >> N;
        No *raiz = nullptr;
        for (int i = 0; i < N; i++) {
            int v;
            cin >> v;
            raiz = inserir(raiz, abs(v), v < 0);         // sinal so indica a cor
        }

        erro.clear();
        if (raiz->rubro) erro = "raiz rubra";            // propriedade 2
        else alturaNegra(raiz);                          // propriedades 4 e 5

        if (erro.empty()) cout << "Sim\n";
        else cout << "Nao: " << erro << "\n";
        liberar(raiz);
    }
    return 0;
}
