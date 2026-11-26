/**
 * @file comum.hpp
 * @brief Cabecalho padrao do grupo: includes, macros e typedefs usados em todo o projeto.
 *
 * Todos os arquivos do repositorio incluem este cabecalho para manter um unico
 * padrao de tipos (ll, vi, ii...) e atalhos (pb, mp, F, S...).
 */
#ifndef COMUM_HPP
#define COMUM_HPP

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

#endif // COMUM_HPP
