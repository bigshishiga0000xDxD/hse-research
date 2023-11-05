#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <map>
#include <array>

#include "matrix.hpp"


constexpr int intpow(int b, int e) {
    int result = 1;
    for (int i = 0; i < e; ++i) {
        result *= b;
    }
    return result;
}

std::mt19937 rng(0);

#define n SHAPE_N
#define m SHAPE_M
#define q FIELD_Q

const int nodes = intpow(q, n * m);           // q^(nm) -- число вершин в графе
const int clique_size = intpow(q, n * m - m); // q^(nm-m) -- размер клики, которую мы ищем
std::bitset<nodes> graph[nodes];              // матрица смежности графа


int main() {
    std::cerr << "enter number of iterations:" << std::endl;
    int IT;
    std::cin >> IT;

    std::cerr << "print found cliques? (Y/N)" << std::endl;
    char in;
    std::cin >> in;
    bool print = (tolower(in) == 'y');

    std::cerr << "print debug info? (Y/N)" << std::endl;
    std::cin >> in;
    bool debug = (tolower(in) == 'y');

    if (debug) {
        std::cout << "running for n = " << n << ", m = " << m << ", q = " << q << std::endl;
    }

    for (int mask1 = 0; mask1 < nodes; ++mask1) {
        for (int mask2 = mask1 + 1; mask2 < nodes; ++mask2) {
            // перебираем пары различных матриц

            Matrix<n, m, q> A(mask1), B(mask2);
            A += B;

            if (A.is_singular()) {
                graph[mask1][mask2] = graph[mask2][mask1] = 1;
                // если сумма вырождена, ставим 1 в матрице смежности
            }
        }
    }

    if (debug) {
        std::cout << "graph is built in " << 1.0 * clock() / CLOCKS_PER_SEC << " secs" << std::endl;
    }

    std::vector<int> clique;              // список вершин, которые мы уже добавили в клику
    std::vector<int> cand;                // список кандидатов на добавление
    std::bitset<nodes> clique_mask;       // на i-той позиции стоит 1, если вершина i есть в clique и 0 иначе
    std::bitset<nodes> cand_mask;         // на i-той позиции стоит 1, если вершина i есть в cand и 0 иначе
    std::map<std::vector<int>, int> dist; // множество распределений клик по рангам
    int max_clique = 0;                   // размер максимальной клики, которую нашла программа

    
    for (int it = 0; it <= IT; ++it) {
        clique.clear();
        clique_mask.reset();
        // изначально клика пустая и в clique_mask стоят нули

        while (true) {
            // на каждой итерации цикла добавляется одна вершина в клику

            cand_mask.set();
            cand_mask ^= clique_mask;
            // в начале
            // считаем, что кандидатом может быть любая вершина,
            // кроме уже добавленных

            for (int u : clique) {
                cand_mask &= graph[u];
                // исключаем тех кандидатов, которые не являются
                // соседями вершины u
            }

            // выписываем позиции всех единичек и cand_mask в cand
            cand.clear();
            for (int u = 0; u < nodes; ++u) {
                if (cand_mask[u]) {
                    cand.push_back(u); 
                }
            }

            // если оказалось, что кандидатов нет, то заканчиваем
            if (cand.empty()) {
                break;
            }

            // выбираем случайного кандидата и добавляем в клику
            int choice = cand[rng() % cand.size()];
            clique.push_back(choice);
            clique_mask[choice] = 1;
        }

        if (clique.size() > max_clique) {
            // мы нашли клику больше, чем все до этого
            if (debug) {
                std::cout << "found clique of size " << clique.size() << " on iteration: " << it << std::endl;
            }
            max_clique = clique.size();
        }
        
        if (clique.size() == clique_size) {
            // с помощью этого куска кода я проверяю, что существуют
            // максимальные клики, не изоморфные стандартной

            // считаем распределение клики по рангам
            std::vector<int> ranks(n + 1);
            for (int u : clique) {
                Matrix<n, m, q> A(u);
                ++ranks[A.matrix_rank()];
            }

            if (!dist.count(ranks) && debug) {
                std::cout << "found new clique type on iteration: " << it << std::endl;

                for (int i = 0; i <= n; ++i) {
                    std::cout << "rank " << i << ": " << ranks[i] << std::endl;
                }
                std::cout << std::endl;
            }
            ++dist[ranks];

            if (print) {
                for (int mask : clique) {
                    std::cout << mask << " ";
                }
                std::cout << std::endl;
            }
        }

        if (it % 1000 == 0 && debug) {
            // std::cout << "\33[2K\r";
            // std::cout << "iteration: " << it;
            // std::fflush(stdout);
        }
    }


    if (debug) {
        std::cout << "_______________________________" << std::endl << std::endl;
        std::cout << "ran " << IT << " iterations" << std::endl;
        std::cout << "maximum clique is of size " << max_clique << std::endl;
        std::cout << "hypothesis is " << clique_size << std::endl;

        for (auto &[ranks, cnt] : dist) {
            std::cout << "ranks (";
            for (int x : ranks) {
                std::cout << x << ", ";
            }
            std::cout << "): " << cnt << std::endl;
        }

        std::cout << std::endl;
        std::cout << "_______________________________" << std::endl << std::endl;
    }
    return 0;
}