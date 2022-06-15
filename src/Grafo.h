#pragma once

#include <vector>

using namespace std;

class Aresta
{
private:
    int id;
    int peso;
    int grauInp; // TODO: ATUALIZAR OS GRAUS
    int grauOut;
    Aresta *proxAresta;

    friend class Grafo;

public:
    Aresta() : id(-1), peso(-1), grauInp(0), grauOut(0), proxAresta(nullptr)
    {}

    ~Aresta()
    {
        delete proxAresta;
    }
};

class Vertice
{
private:
    int idNaMemoria;
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

    Vertice *vertices; // primeiro vertices da lista

    const char *path_arquivo_entrada;
    const char *path_arquivo_saida;
    const char *Opc_Direc;
    const char *Opc_Peso_Aresta;
    const char *Opc_Peso_Nos;

    Vertice *criaNovoVertice(int idNoArquivo, int idNaMemoria, int peso);

    void inicializaParametros(int argc, char *argv[]);

    void criaNovaAresta(Vertice *&vertice, int idNoArquivo, int &idNaMemoria, int peso);

public:

    Grafo(int argc, char **argv);

    ~Grafo();

    void leitura(const char *path);

    void adicionaNo(int &idNoArquivo, int &idNaMemoria, int arestaID, int peso);

    const char *getPathArquivoEntrada() const;

    const char *getPathArquivoSaida() const;

    const char *getOpcDirec() const;

    const char *getOpcPesoAresta() const;

    const char *getOpcPesoNos() const;

    void imprimeFechoTransitivoDireto(int idNoArquivo);

    void imprimeFechoTransitivoIndireto(int idNoArquivo);

    void profundidade(int k, vector<pair<Vertice *, bool>> *visitados);

    bool profundidade(const int id, int i, vector<tuple<Vertice *, bool, bool>> *visitados);

    void arvoreGeradoraMinima(vector<Vertice> &vertices);


};

