/**
 * @file RedBlackTree.cpp
 * @brief Implementacao da Arvore Rubro-Negra (algoritmos do Cormen, cap. 13).
 */
#include "RedBlackTree.hpp"
#include "desenho.hpp"

// ============================================================ construcao
RedBlackTree::RedBlackTree() : tamanho_(0) {
    NIL = new NoRB(0, NEGRO, nullptr);   // o sentinela nasce NEGRO (propriedade 3)
    NIL->esq = NIL->dir = NIL->pai = NIL;
    raiz_ = NIL;                         // arvore vazia: a raiz e o proprio NIL
}

RedBlackTree::~RedBlackTree() {
    liberar(raiz_);
    delete NIL;
}

void RedBlackTree::liberar(NoRB *no) {
    if (no == NIL) return;
    liberar(no->esq);
    liberar(no->dir);
    delete no;
}

void RedBlackTree::log(const string &msg) const {
    if (rastrear) cout << "    -> " << msg << "\n";
}

string RedBlackTree::rotulo(NoRB *no) const {
    return to_string(no->chave) + (no->cor == RUBRO ? "R" : "N");
}

// ============================================================ busca
NoRB *RedBlackTree::buscarNo(int chave) const {
    NoRB *x = raiz_;
    while (x != NIL && x->chave != chave)
        x = (chave < x->chave) ? x->esq : x->dir;
    return x;                            // NIL quando nao encontra
}

bool RedBlackTree::buscar(int chave) const { return buscarNo(chave) != NIL; }

NoRB *RedBlackTree::minimo(NoRB *x) const {
    while (x->esq != NIL) x = x->esq;
    return x;
}

// ============================================================ rotacoes
/*
 * Rotacao a esquerda em x (y = filho direito de x sobe):
 *
 *        x                    y
 *       / \                  / \
 *      a   y      ==>       x   c
 *         / \              / \
 *        b   c            a   b
 *
 * So 3 ligacoes mudam (x->dir, y->esq e o pai de x), por isso custa O(1).
 * A ordem em-ordem a x b y c continua a mesma.
 */
void RedBlackTree::rotacaoEsquerda(NoRB *x) {
    NoRB *y = x->dir;
    x->dir = y->esq;                     // b passa a ser filho direito de x
    if (y->esq != NIL) y->esq->pai = x;

    y->pai = x->pai;                     // y ocupa o lugar de x no pai
    if (x->pai == NIL) raiz_ = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;

    y->esq = x;                          // x desce para a esquerda de y
    x->pai = y;
    rotacoes++;
    rotacoesUltimaOp++;
}

/* Espelho da anterior: o filho esquerdo y de x sobe. */
void RedBlackTree::rotacaoDireita(NoRB *x) {
    NoRB *y = x->esq;
    x->esq = y->dir;
    if (y->dir != NIL) y->dir->pai = x;

    y->pai = x->pai;
    if (x->pai == NIL) raiz_ = y;
    else if (x == x->pai->dir) x->pai->dir = y;
    else x->pai->esq = y;

    y->dir = x;
    x->pai = y;
    rotacoes++;
    rotacoesUltimaOp++;
}

// ============================================================ insercao
bool RedBlackTree::inserir(int chave) {
    rotacoesUltimaOp = 0;

    // 1) Insercao normal de BST: desce ate achar o lugar (y guarda o pai).
    NoRB *y = NIL, *x = raiz_;
    while (x != NIL) {
        y = x;
        if (chave == x->chave) return false;
        x = (chave < x->chave) ? x->esq : x->dir;
    }

    // 2) O novo no entra RUBRO: assim a altura-negra (prop. 5) nao muda.
    //    So pode quebrar a prop. 2 (se for a raiz) ou a 4 (se o pai for rubro).
    NoRB *z = new NoRB(chave, RUBRO, NIL);
    z->pai = y;
    if (y == NIL) raiz_ = z;
    else if (chave < y->chave) y->esq = z;
    else y->dir = z;
    tamanho_++;

    // 3) Conserta as cores/rotaciona subindo pela arvore.
    inserirFixup(z);
    return true;
}

/*
 * Enquanto o pai de z for RUBRO ha dois rubros seguidos. Quem decide o
 * conserto e o TIO (irmao do pai):
 *   Caso 1 - tio RUBRO: recolore pai e tio para negro, avo para rubro e
 *            continua do avo (o problema pode ter subido). Sem rotacao.
 *   Caso 2 - tio NEGRO e z forma "triangulo" (joelho) com pai e avo:
 *            rotaciona no pai para virar "linha" e cai no caso 3.
 *   Caso 3 - tio NEGRO e z em "linha": pai vira negro, avo vira rubro e
 *            rotaciona no avo. Terminou.
 * Caso 1 so recolore; caso 2 + caso 3 = no maximo 2 rotacoes por insercao.
 */
void RedBlackTree::inserirFixup(NoRB *z) {
    while (z->pai->cor == RUBRO) {
        NoRB *pai = z->pai, *avo = pai->pai;

        if (pai == avo->esq) {                       // pai e filho ESQUERDO
            NoRB *tio = avo->dir;
            if (tio->cor == RUBRO) {                 // ---- Caso 1
                log("Caso 1 em " + to_string(z->chave) + ": tio " + to_string(tio->chave) +
                    " rubro -> recolore pai, tio e avo " + to_string(avo->chave));
                casosInsercao[1]++;
                pai->cor = NEGRO;
                tio->cor = NEGRO;
                avo->cor = RUBRO;
                z = avo;                             // o avo pode criar novo conflito
            } else {
                if (z == pai->dir) {                 // ---- Caso 2 (triangulo)
                    log("Caso 2 em " + to_string(z->chave) + ": tio negro, triangulo -> rotacao a esquerda em " +
                        to_string(pai->chave));
                    casosInsercao[2]++;
                    z = pai;
                    rotacaoEsquerda(z);
                    pai = z->pai;
                }
                log("Caso 3 em " + to_string(z->chave) + ": tio negro, linha -> recolore e rotacao a direita em " +
                    to_string(avo->chave));
                casosInsercao[3]++;                  // ---- Caso 3 (linha)
                pai->cor = NEGRO;
                avo->cor = RUBRO;
                rotacaoDireita(avo);
            }
        } else {                                     // espelho: pai e filho DIREITO
            NoRB *tio = avo->esq;
            if (tio->cor == RUBRO) {                 // ---- Caso 1
                log("Caso 1 em " + to_string(z->chave) + ": tio " + to_string(tio->chave) +
                    " rubro -> recolore pai, tio e avo " + to_string(avo->chave));
                casosInsercao[1]++;
                pai->cor = NEGRO;
                tio->cor = NEGRO;
                avo->cor = RUBRO;
                z = avo;
            } else {
                if (z == pai->esq) {                 // ---- Caso 2 (triangulo)
                    log("Caso 2 em " + to_string(z->chave) + ": tio negro, triangulo -> rotacao a direita em " +
                        to_string(pai->chave));
                    casosInsercao[2]++;
                    z = pai;
                    rotacaoDireita(z);
                    pai = z->pai;
                }
                log("Caso 3 em " + to_string(z->chave) + ": tio negro, linha -> recolore e rotacao a esquerda em " +
                    to_string(avo->chave));
                casosInsercao[3]++;                  // ---- Caso 3 (linha)
                pai->cor = NEGRO;
                avo->cor = RUBRO;
                rotacaoEsquerda(avo);
            }
        }
    }
    if (raiz_->cor == RUBRO) log("Raiz rubra -> pinta de negro (propriedade 2)");
    raiz_->cor = NEGRO;                              // propriedade 2
}

// ============================================================ remocao
/* Coloca a subarvore v no lugar da subarvore u (u sai da arvore). */
void RedBlackTree::transplante(NoRB *u, NoRB *v) {
    if (u->pai == NIL) raiz_ = v;
    else if (u == u->pai->esq) u->pai->esq = v;
    else u->pai->dir = v;
    v->pai = u->pai;      // vale ate quando v e NIL: o fixup precisa desse pai
}

bool RedBlackTree::remover(int chave) {
    rotacoesUltimaOp = 0;
    NoRB *z = buscarNo(chave);
    if (z == NIL) return false;

    NoRB *y = z;                   // no que sai fisicamente da arvore
    Cor corOriginal = y->cor;      // se ele era NEGRO, um caminho perdeu um negro
    NoRB *x;                       // no que sobe para o lugar de y

    if (z->esq == NIL) {           // 0 ou 1 filho (so o direito)
        x = z->dir;
        transplante(z, z->dir);
    } else if (z->dir == NIL) {    // so o filho esquerdo
        x = z->esq;
        transplante(z, z->esq);
    } else {                       // 2 filhos: o sucessor y assume o lugar de z
        y = minimo(z->dir);
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) {
            x->pai = y;            // importante quando x e o NIL
        } else {
            transplante(y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplante(z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;           // y herda a cor de z: so a posicao antiga de y perde cor
    }
    delete z;
    tamanho_--;

    if (corOriginal == NEGRO) {
        string nome = (x == NIL) ? string("NIL") : to_string(x->chave);
        if (x->cor == NEGRO) log("Saiu um no NEGRO -> " + nome + " fica duplo-negro");
        else log("Saiu um no NEGRO -> " + nome + " e rubro e herda o negro (rubro-e-negro)");
        removerFixup(x);
    }
    return true;
}

/*
 * x carrega um "negro extra" (duplo-negro). Quem decide e o IRMAO w:
 *   Caso 1 - w RUBRO: troca cores de w e do pai e rotaciona no pai.
 *            Agora o irmao e negro -> cai em 2, 3 ou 4.
 *   Caso 2 - w NEGRO com dois filhos NEGROS: w vira rubro e o negro extra
 *            sobe para o pai (se o pai era rubro, ele vira negro e acabou).
 *   Caso 3 - w NEGRO, filho "de perto" RUBRO e "de longe" NEGRO:
 *            troca cores de w e do filho de perto, rotaciona em w -> caso 4.
 *   Caso 4 - w NEGRO com filho "de longe" RUBRO: w recebe a cor do pai,
 *            pai e filho de longe viram negros, rotaciona no pai. Terminou.
 * Caminho com mais rotacoes: 1 -> 3 -> 4 = 3 rotacoes. Caso 2 so sobe na arvore.
 */
void RedBlackTree::removerFixup(NoRB *x) {
    while (x != raiz_ && x->cor == NEGRO) {
        if (x == x->pai->esq) {                          // x e filho ESQUERDO
            NoRB *w = x->pai->dir;
            if (w->cor == RUBRO) {                       // ---- Caso 1
                log("Caso 1: irmao " + to_string(w->chave) + " rubro -> recolore e rotacao a esquerda em " +
                    to_string(x->pai->chave));
                casosRemocao[1]++;
                w->cor = NEGRO;
                x->pai->cor = RUBRO;
                rotacaoEsquerda(x->pai);
                w = x->pai->dir;
            }
            if (w->esq->cor == NEGRO && w->dir->cor == NEGRO) {   // ---- Caso 2
                log("Caso 2: irmao " + to_string(w->chave) + " negro com filhos negros -> irmao vira rubro, sobe para " +
                    to_string(x->pai->chave));
                casosRemocao[2]++;
                w->cor = RUBRO;
                x = x->pai;
            } else {
                if (w->dir->cor == NEGRO) {              // ---- Caso 3
                    log("Caso 3: sobrinho de perto " + to_string(w->esq->chave) +
                        " rubro -> recolore e rotacao a direita em " + to_string(w->chave));
                    casosRemocao[3]++;
                    w->esq->cor = NEGRO;
                    w->cor = RUBRO;
                    rotacaoDireita(w);
                    w = x->pai->dir;
                }
                log("Caso 4: sobrinho de longe " + to_string(w->dir->chave) +
                    " rubro -> recolore e rotacao a esquerda em " + to_string(x->pai->chave));
                casosRemocao[4]++;                       // ---- Caso 4
                w->cor = x->pai->cor;
                x->pai->cor = NEGRO;
                w->dir->cor = NEGRO;
                rotacaoEsquerda(x->pai);
                x = raiz_;                               // encerra o laco
            }
        } else {                                         // espelho: x e filho DIREITO
            NoRB *w = x->pai->esq;
            if (w->cor == RUBRO) {                       // ---- Caso 1
                log("Caso 1: irmao " + to_string(w->chave) + " rubro -> recolore e rotacao a direita em " +
                    to_string(x->pai->chave));
                casosRemocao[1]++;
                w->cor = NEGRO;
                x->pai->cor = RUBRO;
                rotacaoDireita(x->pai);
                w = x->pai->esq;
            }
            if (w->dir->cor == NEGRO && w->esq->cor == NEGRO) {   // ---- Caso 2
                log("Caso 2: irmao " + to_string(w->chave) + " negro com filhos negros -> irmao vira rubro, sobe para " +
                    to_string(x->pai->chave));
                casosRemocao[2]++;
                w->cor = RUBRO;
                x = x->pai;
            } else {
                if (w->esq->cor == NEGRO) {              // ---- Caso 3
                    log("Caso 3: sobrinho de perto " + to_string(w->dir->chave) +
                        " rubro -> recolore e rotacao a esquerda em " + to_string(w->chave));
                    casosRemocao[3]++;
                    w->dir->cor = NEGRO;
                    w->cor = RUBRO;
                    rotacaoEsquerda(w);
                    w = x->pai->esq;
                }
                log("Caso 4: sobrinho de longe " + to_string(w->esq->chave) +
                    " rubro -> recolore e rotacao a direita em " + to_string(x->pai->chave));
                casosRemocao[4]++;                       // ---- Caso 4
                w->cor = x->pai->cor;
                x->pai->cor = NEGRO;
                w->esq->cor = NEGRO;
                rotacaoDireita(x->pai);
                x = raiz_;
            }
        }
    }
    if (x != NIL && x->cor == RUBRO) log("No " + to_string(x->chave) + " rubro-e-negro -> pinta de negro");
    x->cor = NEGRO;   // rubro-e-negro vira negro simples; a raiz fica negra
}

// ============================================================ percursos
vi RedBlackTree::emOrdem() const {
    vi saida;
    function<void(NoRB *)> rec = [&](NoRB *no) {
        if (no == NIL) return;
        rec(no->esq);
        saida.pb(no->chave);
        rec(no->dir);
    };
    rec(raiz_);
    return saida;
}

vector<string> RedBlackTree::preOrdem() const {
    vector<string> saida;
    function<void(NoRB *)> rec = [&](NoRB *no) {
        if (no == NIL) return;
        saida.pb(rotulo(no));
        rec(no->esq);
        rec(no->dir);
    };
    rec(raiz_);
    return saida;
}

vector<string> RedBlackTree::emLargura() const {
    vector<string> saida;
    if (raiz_ == NIL) return saida;
    queue<NoRB *> fila;
    fila.push(raiz_);
    while (!fila.empty()) {
        NoRB *no = fila.front();
        fila.pop();
        saida.pb(rotulo(no));
        if (no->esq != NIL) fila.push(no->esq);
        if (no->dir != NIL) fila.push(no->dir);
    }
    return saida;
}

// ============================================================ alturas
int RedBlackTree::alturaRec(NoRB *no) const {
    if (no == NIL) return -1;
    return 1 + max(alturaRec(no->esq), alturaRec(no->dir));
}

int RedBlackTree::altura() const { return alturaRec(raiz_); }

int RedBlackTree::alturaNegra() const {
    // Pela propriedade 5 qualquer caminho serve: descemos sempre pela esquerda.
    if (raiz_ == NIL) return 0;
    int bh = 0;
    NoRB *x = raiz_->esq;
    while (true) {
        if (x->cor == NEGRO) bh++;       // conta o NIL tambem
        if (x == NIL) break;
        x = x->esq;
    }
    return bh;
}

// ============================================================ validacao
/* Devolve a altura-negra da subarvore (contando o NIL) ou -1 se algo quebrou. */
int RedBlackTree::conferir(NoRB *no, long long lo, long long hi, string &erro) const {
    if (no == NIL) return 1;
    if (no->chave <= lo || no->chave >= hi) { erro = "ordem de BST quebrada em " + to_string(no->chave); return -1; }
    if (no->cor == RUBRO && (no->esq->cor == RUBRO || no->dir->cor == RUBRO)) {
        erro = "dois rubros seguidos em " + to_string(no->chave); return -1;
    }
    if (no->esq != NIL && no->esq->pai != no) { erro = "pai errado abaixo de " + to_string(no->chave); return -1; }
    if (no->dir != NIL && no->dir->pai != no) { erro = "pai errado abaixo de " + to_string(no->chave); return -1; }
    int e = conferir(no->esq, lo, no->chave, erro);
    if (e < 0) return -1;
    int d = conferir(no->dir, no->chave, hi, erro);
    if (d < 0) return -1;
    if (e != d) { erro = "altura-negra diferente em " + to_string(no->chave); return -1; }
    return e + (no->cor == NEGRO ? 1 : 0);
}

bool RedBlackTree::valida(string *motivo) const {
    string erro;
    if (NIL->cor != NEGRO) erro = "NIL nao e negro";
    else if (raiz_->cor != NEGRO) erro = "raiz nao e negra";
    else if (raiz_ != NIL && raiz_->pai != NIL) erro = "pai da raiz nao e NIL";
    else conferir(raiz_, LLONG_MIN, LLONG_MAX, erro);
    if (motivo) *motivo = erro;
    return erro.empty();
}

// ============================================================ visao 2-3-4
/*
 * Equivalencia com a Arvore 2-3-4: um no NEGRO + seus filhos RUBROS formam um
 * no-B com 1, 2 ou 3 chaves. Todas as folhas do no-B ficam no mesmo nivel
 * justamente porque todo caminho tem a mesma quantidade de negros (prop. 5).
 */
vector<string> RedBlackTree::niveis234() const {
    vector<string> linhas;
    vector<NoRB *> nivel;                       // nos negros que comecam cada no-B
    if (raiz_ != NIL) nivel.pb(raiz_);
    while (!nivel.empty()) {
        string linha;
        vector<NoRB *> proximo;
        for (NoRB *b : nivel) {
            vector<NoRB *> grupo;               // chaves do no-B em ordem
            if (b->esq->cor == RUBRO) grupo.pb(b->esq);
            grupo.pb(b);
            if (b->dir->cor == RUBRO) grupo.pb(b->dir);

            linha += "[";
            for (size_t i = 0; i < grupo.size(); i++)
                linha += (i ? " " : "") + to_string(grupo[i]->chave);
            linha += "] ";

            for (NoRB *g : grupo) {             // filhos do no-B = negros logo abaixo
                if (g == b) continue;
                if (g->esq != NIL) proximo.pb(g->esq);
                if (g->dir != NIL) proximo.pb(g->dir);
            }
            if (b->esq != NIL && b->esq->cor == NEGRO) proximo.pb(b->esq);
            if (b->dir != NIL && b->dir->cor == NEGRO) proximo.pb(b->dir);
        }
        sort(proximo.begin(), proximo.end(), [](NoRB *a, NoRB *b) { return a->chave < b->chave; });
        linha.pop_back();
        linhas.pb(linha);
        nivel = proximo;
    }
    return linhas;
}

// ============================================================ desenho
void RedBlackTree::imprimir() const {
    desenharArvore(raiz_, NIL, [this](NoRB *no) { return rotulo(no); });
}
