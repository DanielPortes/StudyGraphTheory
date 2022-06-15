#include <fstream>
#include <cassert>
#include <memory>
#include <sstream>
#include <iostream>

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
    this->nVertices = atoi(linha.c_str());
    int id = 0, aresta = 0, peso = 0;
    while (!fileIn.eof())
    {
        getline(fileIn, linha, ' ');
        id = atoi(linha.c_str());

        getline(fileIn, linha, ' ');
        aresta = atoi(linha.c_str());

        if (getOpcPesoAresta())
        {
            getline(fileIn, linha);
            peso = atoi(linha.c_str());
        }
        // TODO: aonde ficaria os peso dos vertices, seria trabalho 2?

        this->adicionaNo(id, aresta, peso);
    }

}

Vertice *Grafo::criaNovoVertice(int id, int peso)
{
    auto vertice = new Vertice();
    vertice->id = id;
    vertice->peso = peso;
    return vertice;
}

void Grafo::criaNovaAresta(Aresta *&aresta, int id, int peso)
{
    Aresta* novaAresta = new Aresta();
    novaAresta->id = id;
    novaAresta->peso = peso;
    if (!aresta)
    {
        aresta = novaAresta;
        return;
    }

    auto i = aresta;
    for (; i->proxAresta != nullptr; i = i->proxAresta)
    {}
    // adiciona aresta ao fim da lista encad
    i->proxAresta = novaAresta;
}

void Grafo::adicionaNo(int id, int arestaID, int peso)
{
    if (!this->vertice)
    {
        this->vertice = criaNovoVertice(id, 0);
        criaNovaAresta(this->vertice->proxAresta, arestaID, peso);
        this->vertice->grauOut++;
    } else
    {
        auto i = this->vertice;
        for (; i->proxVertice != nullptr && i->id != id; i = i->proxVertice)
        {}

        if (i->id != id) // Vertice nao presente na lista
        {
            i->proxVertice = criaNovoVertice(id, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i->proxAresta, arestaID, peso);
        i->grauOut++;
    }
    if (atoi(Opc_Direc))
    {
        auto i = this->vertice;
        for (; i->proxVertice != nullptr && i->id != arestaID; i = i->proxVertice)
        {}

        if (i->id == arestaID) // Vertice ja presente na lista
        {
            i->grauOut++;
        } else
        {
            i->proxVertice = criaNovoVertice(arestaID, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i->proxAresta, id, peso);
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
