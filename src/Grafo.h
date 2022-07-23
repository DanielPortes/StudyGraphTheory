#pragma once

#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

class DisjointSet
{
private:
    vector<int> *rank;
    vector<int> *parent;
public:
    DisjointSet(int n)
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
                swap(a, b);
            }
            (*parent)[b] = a;
            if ((*rank)[a] == (*rank)[b])
            {
                (*rank)[a]++;
            }
        }
    }
};

class Aresta
{
private:
    int id; // TODO: seria vantagem ter um pointeiro para o vertice que aresta aponta?
    int idNaMemoria;
    int peso;
    int grauInp; // TODO: ATUALIZAR OS GRAUS
    int grauOut;
    Aresta *proxAresta;

    friend class Grafo;

public:
    Aresta() : id(-1), idNaMemoria(-1), peso(1), grauInp(0), grauOut(0), proxAresta(nullptr)
    {}

    ~Aresta()
    {
        delete proxAresta;
    }

};


class Vertice
{
private:
    int idNaMemoria; // TODO: acho que não precisa disso, verificar no futuro
    int idNoArquivo;
    int peso; // TODO: COMO USAR O PESO NOS VERTICES, TRABALHO 2?
    int grauInp; // TODO: ATUALIZAR OS GRAUS
    int grauOut;
    Aresta *proxAresta;
    Vertice *proxVertice;

    friend class Grafo;

public:
    Vertice()
            : idNaMemoria(-1), idNoArquivo(-1), peso(-1), grauInp(0), grauOut(0), proxVertice(nullptr),
              proxAresta(nullptr)
    {}

    ~Vertice()
    {
        delete proxAresta;
        delete proxVertice;
    }
};

class Grafo
{
private:
    int nVerticesArquivo;
    int nVerticesMemoria;
    int nArestasArquivo;
    Vertice *vertices; // primeiro vertices da lista
    // TODO: criar uma tabela hash para armazenar os vertices, para acelerar a busca por um vertice especifico
    const char *path_arquivo_entrada;
    const char *path_arquivo_saida;
    const char *Opc_Direc;
    const char *Opc_Peso_Aresta;
    const char *Opc_Peso_Nos;

    void adicionaNo(int &idNoArquivo, int arestaID, int peso);

    Vertice *criaNovoVertice(int idNoArquivo, int peso);

    void criaNovaAresta(Vertice *&vertice, int idPonta, int pesoAresta);

public:

    Grafo(int argc, char **argv);

    ~Grafo();

    void leitura(const char *path);

    void imprimirGrafo();

    const char *getPathArquivoEntrada() const;

    const char *getPathArquivoSaida() const;

    const char *getOpcDirec() const;

    const char *getOpcPesoAresta() const;

    const char *getOpcPesoNos() const;

    void imprimeFechoTransitivoDireto(int idNoArquivo);

    void imprimeFechoTransitivoIndireto(int idNoArquivo);

    void profundidade(int k, vector<pair<Vertice *, bool>> *visitados);

    bool profundidade(int id, int i, vector<tuple<Vertice *, bool, bool>> *visitados);

    void caminhoMinimoDijkstra(int src, int destino);

    void caminhoMinimoFloyd(int src, int destino);

    Grafo *getSubGrafo(const vector<int> &conjVeticeInduzido, bool direcionado);

    void retornaAgmEmSubgrafoPorPrim(vector<int> &conjVeticeInduzido);

    void retornaAgmEmSubgrafoPorKruskal(vector<int> &conjVeticeInduzido);

    Vertice *encontrar(int id);

    void imprimeAGMcomArestasDeRetorno(int id);

    double coeficienteAglomeracaoLocal(int id);

    void averageClusteringCoefficient();

    void coeficienteAglomeracaoMedio();

    vector<Vertice*> breadthFirstSearch(int id);

    void fechoTransitivoDireto(int id);

    void fechoTransitivoIndireto(int id);

    Grafo *getSubGraphEdgeInduced(vector<pair<int, int>> &conjArestasInduzidas, bool direcionado);

    void writeDotFile(bool direcionado, stack<pair<int, int>> conjArestasRetorno = {});
};
