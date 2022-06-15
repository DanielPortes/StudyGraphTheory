#include <fstream>
#include <cassert>
#include <memory>
#include <sstream>
#include <iostream>
#include <vector>

#include "Grafo.h"

using namespace std;

Grafo::Grafo(int argc, char **argv) : vertices(nullptr), nVerticesMemoria(0), nVerticesArquivo(0)
{
    this->inicializaParametros(argc, argv);
}

Grafo::~Grafo()
{
    delete this->vertices;
}

void Grafo::inicializaParametros(int argc, char **argv)
{
    if (argc != 6)
    {
        cerr << "[ERROR] Parametros faltantes!\n";
        assert(false);
    }
    path_arquivo_entrada = argv[1];
    path_arquivo_saida = argv[2];
    Opc_Direc = argv[3];
    Opc_Peso_Aresta = argv[4];
    Opc_Peso_Nos = argv[5];
}

streampos inline tamanhoArquivo(fstream &arq)
{
    arq.seekg(0, arq.end);
    streampos tam = arq.tellg();
    arq.seekg(0);
    return tam;
}

void Grafo::leitura(const char *path)
{
    fstream arquivoEntrada(path, ios::in);
    if (!arquivoEntrada.is_open())
    {
        cerr << "\n\t[ERRO] arquivo nao pode ser aberto lerArquivo";
        assert(false);
    }

    auto bufferSize = tamanhoArquivo(arquivoEntrada);
    unique_ptr<char[]> buffer(new char[bufferSize]);
    arquivoEntrada.read(buffer.get(), bufferSize);
    arquivoEntrada.close();
    stringstream fileIn(buffer.get());

    string linha;
    getline(fileIn, linha);

    this->nVerticesArquivo = atoi(linha.c_str());
    int idNaMemoria = 0, idNoArquivo = 0, aresta = 0, peso = 0;
    while (!fileIn.eof())
    {
        // TODO: professor flw na possibilidade de o id do vertices nao ser um numero, daria certo?
        getline(fileIn, linha, ' ');
        idNoArquivo = atoi(linha.c_str());

        getline(fileIn, linha, ' ');
        aresta = atoi(linha.c_str());

        if (getOpcPesoAresta())
        {
            getline(fileIn, linha);
            peso = atoi(linha.c_str());
        }
        // TODO: aonde ficaria os peso dos vertices, seria trabalho 2?

        this->adicionaNo(idNoArquivo, idNaMemoria, aresta, peso);
        ++idNaMemoria;
    }
    this->nVerticesMemoria = idNaMemoria;
}

Vertice *Grafo::criaNovoVertice(int idNoArquivo, int idNaMemoria, int peso)
{
    auto vertice = new Vertice();
    vertice->idNoArquivo = idNoArquivo;
    vertice->idNaMemoria = idNoArquivo;
    vertice->peso = peso;
    return vertice;
}

void Grafo::criaNovaAresta(Vertice *&vertice, int idNoArquivo, int &idNaMemoria, int peso)
{
    auto j = this->vertices;
    for (; j->proxVertice != nullptr && j->idNoArquivo != idNoArquivo; j = j->proxVertice)
    {}

    if (j->idNoArquivo != idNoArquivo)
    {
        j->proxVertice = criaNovoVertice(idNoArquivo, idNaMemoria, 0);
    }

    auto *novaAresta = new Aresta();
    novaAresta->id = idNoArquivo;
    novaAresta->peso = peso;
    if (!vertice->proxAresta)
    {
        vertice->proxAresta = novaAresta;
        return;
    }

    // adiciona aresta ao fim da lista encad
    auto i = vertice->proxAresta;
    for (; i->proxAresta != nullptr; i = i->proxAresta)
    {}
    i->proxAresta = novaAresta;
}

void Grafo::adicionaNo(int &idNoArquivo, int &idNaMemoria, int arestaID, int peso)
{
    if (!this->vertices)
    {
        this->vertices = criaNovoVertice(idNoArquivo, idNaMemoria, 0);
        criaNovaAresta(this->vertices, arestaID, ++idNaMemoria, peso);
        this->vertices->grauOut++;
    } else
    {
        auto i = this->vertices;
        for (; i->proxVertice != nullptr && i->idNoArquivo != idNoArquivo; i = i->proxVertice)
        {}

        if (i->idNoArquivo != idNoArquivo) // Vertice nao presente na lista
        {
            i->proxVertice = criaNovoVertice(idNoArquivo, idNaMemoria, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i, arestaID, ++idNaMemoria, peso);
        i->grauOut++;
    }
    if (!atoi(Opc_Direc))
    {
        auto i = this->vertices;
        for (; i->proxVertice != nullptr && i->idNoArquivo != arestaID; i = i->proxVertice)
        {}

        if (i->idNoArquivo == arestaID) // Vertice ja presente na lista
        {
            i->grauOut++;
        } else
        {
            i->proxVertice = criaNovoVertice(arestaID, idNaMemoria, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i, idNoArquivo, ++idNaMemoria, peso);
    } // Nossa, esse codigo ficou lindo
}

const char *Grafo::getPathArquivoEntrada() const
{
    return path_arquivo_entrada;
}

const char *Grafo::getPathArquivoSaida() const
{
    return path_arquivo_saida;
}

const char *Grafo::getOpcDirec() const
{
    return Opc_Direc;
}

const char *Grafo::getOpcPesoAresta() const
{
    return Opc_Peso_Aresta;
}

const char *Grafo::getOpcPesoNos() const
{
    return Opc_Peso_Nos;
}
