/**
 * @file testes.cpp
 * @brief Testes automaticos da RedBlackTree (sem framework).
 *
 * Cada verificacao imprime [OK] ou [FALHOU]. No fim aparece o placar.
 * O teste de estresse faz 20.000 operacoes aleatorias comparando com std::set
 * e confere, depois de cada uma, as 5 propriedades e os limites de rotacoes.
 */
#include "RedBlackTree.hpp"

static int total = 0, passou = 0;

static void verificar(bool cond, const string &descricao) {
    total++;
    if (cond) passou++;
    cout << (cond ? "  [OK]     " : "  [FALHOU] ") << setw(2) << total << ". " << descricao << "\n";
}

static vi faixa(int a, int b) {
    vi v;
    for (int i = a; i <= b; i++) v.pb(i);
    return v;
}

int main() {
    cout << "== Basico ==\n";
    {
        RedBlackTree t;
        verificar(t.vazia() && t.altura() == -1 && t.valida(), "arvore vazia e valida e tem altura -1");
        t.inserir(10);
        verificar(t.preOrdem() == vector<string>{"10N"}, "primeira chave vira raiz NEGRA");
        verificar(!t.inserir(10), "chave repetida e recusada");
        t.inserir(5);
        t.inserir(15);
        verificar(t.emLargura() == vector<string>({"10N", "5R", "15R"}), "novos nos entram RUBROS");
        verificar(t.buscar(5) && !t.buscar(7), "buscar encontra 5 e nao encontra 7");
    }

    cout << "== Insercao (insert-fixup) ==\n";
    {
        RedBlackTree t;
        t.inserir(10); t.inserir(20); t.inserir(30);
        verificar(t.emLargura() == vector<string>({"20N", "10R", "30R"}) && t.rotacoes == 1,
                  "10 20 30: caso 3 (linha) -> 1 rotacao");
    }
    {
        RedBlackTree t;
        t.inserir(10); t.inserir(30); t.inserir(20);
        verificar(t.emLargura() == vector<string>({"20N", "10R", "30R"}) && t.rotacoes == 2,
                  "10 30 20: caso 2 + caso 3 (triangulo) -> 2 rotacoes");
    }
    {
        RedBlackTree t;
        for (int k : {20, 10, 30, 5}) t.inserir(k);
        verificar(t.emLargura() == vector<string>({"20N", "10N", "30N", "5R"}) && t.rotacoes == 0,
                  "20 10 30 5: caso 1 so recolore (nenhuma rotacao)");
    }
    {
        RedBlackTree t;
        for (int k : {11, 2, 14, 1, 7, 15, 5, 8, 4}) t.inserir(k);
        verificar(t.preOrdem() == vector<string>({"7N", "2R", "1N", "5N", "4R", "11R", "8N", "14N", "15R"}),
                  "exemplo do Cormen (fig. 13.4): inserir 4 deixa 7 na raiz");
        verificar(t.casosInsercao[1] >= 1 && t.casosInsercao[2] == 1 && t.casosInsercao[3] == 1,
                  "inserir 4 passa pelos casos 1, 2 e 3");
        verificar(t.niveis234() == vector<string>({"[2 7 11]", "[1] [4 5] [8] [14 15]"}),
                  "visao 2-3-4: raiz [2 7 11] e folhas no mesmo nivel");
    }

    cout << "== Remocao (delete-fixup) ==\n";
    {
        RedBlackTree t;
        for (int k : {11, 2, 14, 1, 7, 15, 5, 8, 4}) t.inserir(k);
        t.remover(1);
        verificar(t.valida() && t.casosRemocao[3] == 1 && t.casosRemocao[4] == 1,
                  "remover 1: casos 3 e 4");
        t.remover(8);
        t.remover(15);
        verificar(t.valida() && t.casosRemocao[2] == 1, "remover 15: caso 2 (o negro extra sobe)");
        t.remover(14);
        t.remover(11);
        verificar(t.valida() && t.casosRemocao[1] == 1, "remover 11: caso 1 (irmao rubro)");
        verificar(!t.remover(99), "remover chave inexistente devolve false");
        verificar(t.emOrdem() == vi({2, 4, 5, 7}), "em-ordem continua crescente");
    }
    {
        RedBlackTree t;
        for (int k : {50, 30, 70}) t.inserir(k);
        t.remover(50);                               // raiz com 2 filhos
        verificar(t.valida() && t.emOrdem() == vi({30, 70}), "remover raiz com dois filhos (usa o sucessor)");
        t.remover(30); t.remover(70);
        verificar(t.vazia() && t.tamanho() == 0 && t.valida(), "esvaziar a arvore");
    }

    cout << "== Altura ==\n";
    {
        RedBlackTree t;
        for (int k = 1; k <= 1023; k++) t.inserir(k);
        verificar(t.valida(), "1023 chaves crescentes: 5 propriedades OK");
        verificar(t.altura() <= 2 * log2(1024.0), "altura <= 2*log2(n+1)");
        verificar(t.emOrdem() == faixa(1, 1023), "em-ordem devolve 1..1023");
        int bh = t.alturaNegra();
        verificar((1 << bh) - 1 <= 1023 + 1, "n >= 2^bh - 1 (lema da altura-negra)");
    }

    cout << "== Estresse: 20.000 operacoes aleatorias x std::set ==\n";
    {
        mt19937 rng(2026);
        RedBlackTree t;
        set<int> ref;
        bool iguais = true, validas = true, insOk = true, remOk = true;
        int maxIns = 0, maxRem = 0;
        for (int i = 0; i < 20000; i++) {
            int k = rng() % 2000;
            if (rng() % 2) {
                bool a = t.inserir(k), b = ref.insert(k).second;
                if (a != b) iguais = false;
                maxIns = max(maxIns, t.rotacoesUltimaOp);
                if (t.rotacoesUltimaOp > 2) insOk = false;
            } else {
                bool a = t.remover(k), b = ref.erase(k) > 0;
                if (a != b) iguais = false;
                maxRem = max(maxRem, t.rotacoesUltimaOp);
                if (t.rotacoesUltimaOp > 3) remOk = false;
            }
            if (i % 50 == 0 && !t.valida()) validas = false;
        }
        verificar(iguais && t.emOrdem() == vi(ref.begin(), ref.end()), "mesmo conteudo que std::set");
        verificar(validas && t.valida(), "5 propriedades valem durante todo o teste");
        verificar(insOk, "insercao: no maximo 2 rotacoes (maior vista = " + to_string(maxIns) + ")");
        verificar(remOk, "remocao: no maximo 3 rotacoes (maior vista = " + to_string(maxRem) + ")");
        verificar(t.altura() <= 2 * log2(t.tamanho() + 1.0), "altura final <= 2*log2(n+1)");
    }

    cout << "\n" << passou << "/" << total << " verificacoes passaram\n";
    return passou == total ? 0 : 1;
}
