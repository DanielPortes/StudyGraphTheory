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

// TODO: e se o vertices estiver desconectado de todos
void Grafo::imprimeFechoTransitivoDireto(int idNoArquivo)
{
    if (idNoArquivo < 0 || idNoArquivo > this->nVerticesArquivo)
    {
        cerr << "\n[ERROR] Grafo::imprimeFechoTransitivoDireto(int idNoArquivo), \t idNoArquivo invalido";
    }
    auto visitados = new vector<pair<Vertice *, bool>>;
    visitados->resize(this->nVerticesArquivo);
    auto i = this->vertices;

    for (int k = 0; i != nullptr; i = i->proxVertice, ++k)
    {
        (*visitados)[k] = make_pair(i,
                                    false); // TODO: por segurando troquei [K] pelo id, testar e possivelmente voltar para K
    }
    profundidade(idNoArquivo, visitados);

    cout << "\nFecho transitivo direto do vertices ( " << idNoArquivo << " ) = { ";
    for (auto no: *visitados)
    {
        if (no.second && no.first->idNoArquivo != idNoArquivo) // TODO: solucao preguicosa para n imprimir a si mesmo
            cout << no.first->idNoArquivo << ", ";
    }
    cout << "\b\b }\n";
    delete visitados;
}

void Grafo::profundidade(const int k, vector<pair<Vertice *, bool>> *visitados)
{
    (*visitados)[k].second = true;

    auto i = (*visitados)[k].first;
    for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
    {
        if (!(*visitados)[j->id].second)
        {
            profundidade(j->id, visitados);
        }
    }
}

void Grafo::imprimeFechoTransitivoIndireto(int idNoArquivo)
{
    if (idNoArquivo < 0 || idNoArquivo > this->nVerticesArquivo)
    {
        cerr << "\n[ERROR] Grafo::imprimeFechoTransitivoDireto(int idNoArquivo), \t idNoArquivo invalido";
    }
    // pointeiro vertice, condicao de visita, condicao de acesso ao vertice
    auto visitados = new vector<tuple<Vertice *, bool, bool>>;
    visitados->resize((this->nVerticesArquivo));
    auto i = this->vertices;

    // TODO: criar um vetor de nVerticesArquivo e acessar com idNaMemoria, BUG-PRONE
    for (int k = 0; i != nullptr; i = i->proxVertice, ++k)
    {
        (*visitados)[k] = make_tuple(i,
                                     false,
                                     false); // TODO: por segurando troquei [K] pelo id, testar e possivelmente voltar para K
    }
    int aux = 0;
    for (auto &vertice: *visitados)
    {
        if (!get<1>(vertice))
        {
            profundidade(idNoArquivo, aux, visitados);
        }
        ++aux;
    }

    cout << "\nFecho transitivo indireto do vertices ( " << idNoArquivo << " ) = { ";

    for (auto no: *visitados)
    {
        if (get<2>(no))
            cout << get<0>(no)->idNoArquivo << ", ";
    }
    cout << "\b\b }\n";
    delete visitados;
}

bool Grafo::profundidade(const int id, int i, vector<tuple<Vertice *, bool, bool>> *visitados)
{
    get<1>((*visitados)[i]) = true;

    Vertice *v = get<0>((*visitados)[i]);

    for (auto j = v->proxAresta; j != nullptr; j = j->proxAresta)
    {
        if (j->id == id)
        {
            get<2>((*visitados)[i]) = true;
            return true;
        }

        get<2>((*visitados)[i]) = profundidade(id, j->id, visitados);
    }
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

