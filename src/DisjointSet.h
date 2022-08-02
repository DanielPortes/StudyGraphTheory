//
// Created by Daniel on 23/07/2022.
//

#ifndef DEV_DISJOINTSET_H
#define DEV_DISJOINTSET_H
#include <vector>
using std::vector;

class DisjointSet // importante apenas para o algoritmo de Kruskal
{
private:
    vector<int> *rank;
    vector<int> *parent;
public:
    explicit DisjointSet(int n)
    {
        rank = new vector<int>(n);
        parent = new vector<int>(n);
        for (int i = 0; i < n; i++)
        {
            (*rank)[i] = 0;
            (*parent)[i] = i;
        }
    }

    ~DisjointSet()
    {
        delete rank;
        delete parent;
    }

    void makeSet(int v)
    {
        (*parent)[v] = v;
        (*rank)[v] = 0;
    }

    int find(int v) // otimizacao path compression
    {
        if (v == (*parent)[v])
        {
            return v;
        }
        return (*parent)[v] = find((*parent)[v]);
    }

    void unionSet(int a, int b) // otimizacao uniao por rank
    {
        a = find(a);
        b = find(b);
        if (a != b)
        {
            if ((*rank)[a] < (*rank)[b])
            {
                std::swap(a, b);
            }
            (*parent)[b] = a;
            if ((*rank)[a] == (*rank)[b])
            {
                (*rank)[a]++;
            }
        }
    }
};


#endif //DEV_DISJOINTSET_H
