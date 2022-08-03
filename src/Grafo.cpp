/*
 * Desenvolvido por: DANIEL FAGUNDES 100%
 *
 * */


#include <fstream>
#include <memory>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>
#include <string_view>

#include "Grafo.h"
#include "DisjointSet.h"

using namespace std;

Grafo::Grafo(size_t argc, char **argv) : vertices(nullptr), nVerticesMemoria(0), nVerticesArquivo(0), nArestasArquivo(0)
{
    if (argc != 6)
    {
        cerr << "[ERROR] Parametros faltantes!\n";
        exit(10);
    }
    path_arquivo_entrada = argv[1];
    path_arquivo_saida = argv[2];
    Opc_Direc = argv[3];
    Opc_Peso_Aresta = argv[4];
    Opc_Peso_Nos = argv[5];
}

Grafo::~Grafo()
{
    delete this->vertices;
}

void Grafo::imprimirGrafo()
{
    cout << "Imprimindo grafo..." << "\n";
    cout << "Vertices: " << this->nVerticesMemoria << "\n";
    cout << "Arestas: " << this->nArestasArquivo << "\n";
    cout << "Opc_Direc: " << this->Opc_Direc << "\n";
    cout << "Opc_Peso_Aresta: " << this->Opc_Peso_Aresta << "\n";
    cout << "Opc_Peso_Nos: " << this->Opc_Peso_Nos << "\n";
    cout << "Imprimindo vertices..." << "\n";

    for (int i = 0; i < this->nVerticesArquivo; ++i)
    {
        auto v = encontrar(i);
        if (v == nullptr)
        {
            cout << "Vertice " << i << ": nao encontrado!" << "\n";
            continue;
        }
        cout << "Vertice " << v->idNoArquivo << ": ";
        for (auto a = v->proxAresta; a != nullptr; a = a->proxAresta)
        {
            cout << a->id << " ";
        }
        cout << "\n";
    }

#ifndef NDEBUG
    string_view pathSaida = "grafoCompleto.dot";
    escreveArquivoDot(this->getOpcDirec(), nullptr, &pathSaida);
#endif
}

streampos inline tamanhoArquivo(fstream &arq)
{
    arq.seekg(0, std::fstream::end);
    streampos tam = arq.tellg();
    arq.seekg(0);
    return tam;
}

void Grafo::leitura(string_view path)
{
    fstream arquivoEntrada(static_cast<string>(path), ios::in);
    if (!arquivoEntrada.is_open())
    {
        cerr << "\n\t[ERRO] arquivo nao pode ser aberto lerArquivo";
        exit(10);
    }

    auto bufferSize = tamanhoArquivo(arquivoEntrada);
    unique_ptr<char[]> buffer(new char[bufferSize]);
    arquivoEntrada.read(buffer.get(), bufferSize);
    arquivoEntrada.close();
    stringstream fileIn(buffer.get());

    string linha;
    getline(fileIn, linha);
    this->nVerticesArquivo = stoi(linha);

    char delimitador = this->getOpcPesoAresta() ? ' ' : '\n';
    int verticeIdArquivo = 0, arestaId = 0, pesoAresta = 1; //pesoAresta padrao, se ausente = 1
    bool arestasComPesos = getOpcPesoAresta();
    while (!fileIn.eof())
    {
        getline(fileIn, linha, ' ');
        verticeIdArquivo = atoi(linha.c_str());

        getline(fileIn, linha, delimitador);
        arestaId = atoi(linha.c_str());

        if (arestasComPesos)
        {
            getline(fileIn, linha);
            pesoAresta = atoi(linha.c_str());
        }
        if (verticeIdArquivo == 0 && arestaId == 0) // TODO: ficou uma droga isso, refatorar
        {
            break;
        }
        this->adicionaNo({verticeIdArquivo, arestaId}, pesoAresta);
    }
    if (this->nVerticesArquivo != this->nVerticesMemoria)
    {
        addVerticesDesconexos();
        this->nVerticesMemoria--; // contando 1 a mais
    }
    this->imprimirGrafo();
}

void Grafo::addVerticesDesconexos()
{
    auto ultimo = this->vertices;
    for (; ultimo->proxVertice != nullptr; ultimo = ultimo->proxVertice)
    {}
    for (int i = 0; i < this->nVerticesArquivo; ++i)
    {
        if (encontrar(i) == nullptr)
        {
            ultimo->proxVertice = this->criaNovoVertice(i, 0);
            ultimo = ultimo->proxVertice;
        }
    }
}

Vertice *Grafo::criaNovoVertice(int idNoArquivo, int peso)
{
    auto vertice = new Vertice();
    vertice->idNoArquivo = idNoArquivo;
    this->nVerticesMemoria++;
    vertice->peso = peso;
    return vertice;
}

// TODO: refatorar, quebrar o codigo e usar ptr ultimo vertice
void Grafo::criaNovaAresta(Vertice *&vertice, int idPonta, int pesoAresta)
{
    auto i = vertice->proxAresta;
    for (; i && i->proxAresta != nullptr && i->id != idPonta; i = i->proxAresta)
    {}
    if (i != nullptr && i->id == idPonta) // ja existe aresta
    {
        return;
    }
    // primeiro verifica-se vertice destino existe
    auto j = this->vertices;
    for (; j->proxVertice != nullptr && j->idNoArquivo != idPonta; j = j->proxVertice)
    {}
    if (j->idNoArquivo != idPonta)
    {
        j->proxVertice = criaNovoVertice(idPonta, 0); // nao existe, cria-se vertice destino
        j = j->proxVertice;
    }
    j->grauIn++;

    auto *novaAresta = new Aresta();
    novaAresta->id = idPonta;
    novaAresta->peso = pesoAresta;
    ++this->nArestasArquivo;
    ++vertice->grauOut;

    // por fim, sempre adicionar a aresta no final da lista encadeada de arestas
    if (!vertice->proxAresta)
    {
        vertice->proxAresta = novaAresta;
        return;
    }
    i = vertice->proxAresta;
    for (; i->proxAresta != nullptr && i->id != novaAresta->id; i = i->proxAresta)
    {}
    i->proxAresta = novaAresta;
}

void Grafo::adicionaNo(pairPontos pontos, int peso)
{
    if (!this->vertices)
    {
        this->vertices = criaNovoVertice(pontos.first, 0);
        criaNovaAresta(this->vertices, pontos.second, peso);
    }
    else
    {
        auto i = this->vertices;
        for (; i->proxVertice != nullptr && i->idNoArquivo != pontos.first; i = i->proxVertice)
        {}

        if (i->idNoArquivo != pontos.first) // Vertice nao presente na lista
        {
            i->proxVertice = criaNovoVertice(pontos.first, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i, pontos.second, peso);
    }
    if (!getOpcDirec()) // // se nao direcionado, adiciona aresta inversa
    {
        auto i = this->vertices;
        for (; i->proxVertice != nullptr && i->idNoArquivo != pontos.second; i = i->proxVertice)
        {}

        if (i->idNoArquivo != pontos.second) // Vertice ainda nao presente na lista
        {
            i->proxVertice = criaNovoVertice(pontos.second, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i, pontos.first, peso);

    }
}

string Grafo::getPathArquivoEntrada() const
{
    return path_arquivo_entrada;
}

std::string Grafo::getPathArquivoSaida() const
{
    return path_arquivo_saida;
}

bool Grafo::getOpcDirec() const
{
    return Opc_Direc == "1";
}

bool Grafo::getOpcPesoAresta() const
{
    return Opc_Peso_Aresta == "1";
}

bool Grafo::getOpcPesoNos() const
{
    return Opc_Peso_Nos == "1";
}

//TODO: verificar se o grafo possui ciclo negativo
//TODO: verificar se os nos do parametro estao no grafo
void Grafo::caminhoMinimoDijkstra(int src, int destino)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> lista;
    vector<int> dist(this->nVerticesArquivo, numeric_limits<int>::max());
    vector<int> parent(this->nVerticesArquivo, -1);
    lista.push({0, src});
    dist[src] = 0;

    while (!lista.empty())
    {
        auto u = lista.top();
        lista.pop();
        if (u.second == destino)
        {
            break;
        }
        for (auto i = this->vertices; i != nullptr; i = i->proxVertice)
        {
            if (i->idNoArquivo == u.second)
            {
                for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
                {
                    if (dist[j->id] > dist[u.second] + j->peso)
                    {
                        dist[j->id] = dist[u.second] + j->peso;
                        parent[j->id] = u.second;
                        lista.push({dist[j->id], j->id});
                    }
                }
            }
        }
    }

    if (numeric_limits<int>::max() == dist[destino])
    {
        cout << "Nao existe caminho minimo entre os vertices " << src << " e " << destino << "\n";
        return;
    }
    else
    {
        cout << "\nCaminho minimo do vertice " << src << " ate o vertice " << destino << " = " << dist[destino] << "\n";
    }

    cout << "\nCaminho minimo do vertice " << src << " ate o vertice " << destino << ": ";
    int aux = destino;
    while (aux != src)
    {
        cout << aux << " <- ";
        aux = parent[aux];
    }
    cout << aux << "\n";
}

//TODO: usar vetores dinamicamente alocados
//TODO: verificar se o grafo possui ciclo negativo
//TODO: verificar se os nos do parametro estao no grafo
void Grafo::caminhoMinimoFloyd(int src, int destino)
{
    vector<vector<int>> dist(this->nVerticesArquivo,
                             vector<int>(this->nVerticesArquivo, (numeric_limits<int>::max() / 2)));
    vector<vector<int>> parent(this->nVerticesArquivo, vector<int>(this->nVerticesArquivo, -1));
    for (int i = 0; i < this->nVerticesArquivo; i++)
    {
        for (int j = 0; j < this->nVerticesArquivo; j++)
        {
            if (i == j)
            {
                dist[i][j] = 0;
            }
            else
            {
                for (auto k = this->vertices; k != nullptr; k = k->proxVertice)
                {
                    if (k->idNoArquivo == i)
                    {
                        for (auto l = k->proxAresta; l != nullptr; l = l->proxAresta)
                        {
                            if (l->id == j)
                            {
                                dist[i][j] = l->peso;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int k = 0; k < this->nVerticesArquivo; k++)
    {
        for (int i = 0; i < this->nVerticesArquivo; i++)
        {
            for (int j = 0; j < this->nVerticesArquivo; j++)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    parent[i][j] = k;
                }
            }
        }
    }
    if ((numeric_limits<int>::max() / 2) == dist[src][destino])
    {
        cout << "Nao existe caminho minimo entre os vertices " << src << " e " << destino << "\n";
        return;
    }
    else
    {
        cout << "\nCaminho minimo do vertice " << src << " ate o vertice " << destino << " = " << dist[src][destino]
             << "\n";
    }
    cout << "\nCaminho minimo do vertice " << src << " ate o vertice " << destino << ": ";
    int aux = destino;
    while (aux != src && parent[src][aux] != -1)
    {
        cout << aux << " <- ";
        aux = parent[src][aux];
    }
    cout << aux << " <- ";
    cout << src << "\n";
}

Grafo *Grafo::retornaSubgrafoVerticeInduzido(const vector<int> &conjVeticeInduzido, bool direcionado)
{
    cout
            << "Gerando subgrafo... \n[WARNING] Atencao aos vertices passados por parametro no conjunto vertice induzido\n";
    vector<const char *> argv{"subgrafo",
                              this->getPathArquivoEntrada().c_str(),
                              this->getPathArquivoSaida().c_str(),
                              const_cast<char *>(direcionado ? "1" : "0"),
                              const_cast<char *>(getOpcPesoAresta() ? "1" : "0"),
                              const_cast<char *>(getOpcPesoNos() ? "1" : "0")};
    auto subgrafo = new Grafo(argv.size(), const_cast<char **>(argv.data()));
    int idContador = 0;
    for (auto i = this->vertices; i != nullptr; i = i->proxVertice)
    {
        if (find(conjVeticeInduzido.begin(), conjVeticeInduzido.end(), i->idNoArquivo) != conjVeticeInduzido.end())
        {
            for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
            {
                if (find(conjVeticeInduzido.begin(), conjVeticeInduzido.end(), j->id) != conjVeticeInduzido.end())
                {
                    subgrafo->adicionaNo({i->idNoArquivo, j->id}, j->peso);
                }
            }
        }
    }
    subgrafo->nVerticesArquivo = static_cast<int>(conjVeticeInduzido.size());
    return subgrafo; // lembrar de deletar o grafo retornado
}


// TODO: verificar se o grafo eh conexo e tem peso nas arestas?
void Grafo::retornaAgmEmSubgrafoPorPrim(vector<int> &conjVeticeInduzido)
{
    if (conjVeticeInduzido.front() == -1 && conjVeticeInduzido.size() == 1) // seleciona todos os vertices do grafo
    {
        conjVeticeInduzido.pop_back();
        for (int i = 0; i < this->nVerticesArquivo; ++i)
        {
            conjVeticeInduzido.push_back(i);
        }
    }
    Grafo *subgrafo = retornaSubgrafoVerticeInduzido(conjVeticeInduzido,
                                                     false); // obrigatorio false para algoritmo de Prim
    priority_queue<pair<int, Vertice *>, vector<pair<int, Vertice *>>, greater<>> lista; // peso e vertice
    vector<int> key(this->nVerticesArquivo, (numeric_limits<int>::max()));
    vector<bool> finished(this->nVerticesArquivo, false);
    vector<Vertice *> parent(this->nVerticesArquivo, nullptr);
    auto p = subgrafo->vertices;
    key[p->idNoArquivo] = 0;
    lista.push(make_pair(0, p));
    while (!lista.empty())
    {
        auto u = lista.top().second;
        lista.pop();
        if (finished[u->idNoArquivo])
        {
            continue;
        }
        finished[u->idNoArquivo] = true;
        for (auto v = u->proxAresta; v != nullptr; v = v->proxAresta)
        {
            if (v->peso < key[v->id] && !finished[v->id])
            {
                parent[v->id] = u;
                key[v->id] = v->peso;
                lista.push(make_pair(v->peso, subgrafo->encontrar(v->id)));
            }
        }
    }
    cout << "\nArvore de minimo custo: \n";
    vector<pair<int, int>> conjArestasInduzidas;
    for (int i = 0; i < subgrafo->nVerticesArquivo; i++)
    {
        if (parent[i] != nullptr)
        {
            cout << "(" << parent[i]->idNoArquivo << "," << i << ") ";
            conjArestasInduzidas.emplace_back(parent[i]->idNoArquivo, i);
        }
    }
    auto grafoAresta = retornaSubgrafoArestaInduzido(conjArestasInduzidas, false);

    grafoAresta->escreveArquivoDot(false);
    delete grafoAresta;

    int somaPesos = 0;
    for (auto valor: key)
    {
        if (valor != (numeric_limits<int>::max()))
        {
            somaPesos += valor;
        }
    }
//    cout << "\nSoma dos pesos: " << accumulate(key.begin(), key.end(), 0) << "\n";
    cout << "\nSoma dos pesos: " << somaPesos << "\n";
    cout << "\n";
    delete subgrafo;
}

Vertice *Grafo::encontrar(int id)
{
    Vertice *u = vertices;
    while (u != nullptr && u->idNoArquivo != id)
    {
        u = u->proxVertice;
    }
    return u;
}

// TODO: verificar se o grafo eh conexo e tem peso nas arestas?
// TODO: Otimizacao: pensar em uma forma de usar a estrutura de dados set disjunto mais inteligente, para nao precisar criar todos os vertices do grafo original
// TODO: Otimizacao: parar quando obtiver n -1 arestas
void Grafo::retornaAgmEmSubgrafoPorKruskal(vector<int> &conjVeticeInduzido)
{
    if (conjVeticeInduzido.size() == 1)
    {
        conjVeticeInduzido.pop_back();
        for (int i = 0; i < this->nVerticesArquivo; ++i)
        {
            conjVeticeInduzido.push_back(i);
        }
    }
    Grafo *subgrafo = retornaSubgrafoVerticeInduzido(conjVeticeInduzido, false);
    DisjointSet ds(this->nVerticesArquivo);
    vector<pair<Vertice *, Aresta *>> arestas; // com essa representacao eu tenho informacao sobre cada aresta do grafo (u, v), e tenho acesso ao peso da aresta por v->peso
    arestas.reserve(subgrafo->nArestasArquivo);
    for (auto i = subgrafo->vertices; i != nullptr; i = i->proxVertice)
    {
        for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
        {
            arestas.emplace_back(i, j);
        }
    }
    sort(arestas.begin(), arestas.end(), [](const pair<Vertice *, Aresta *> &a, const pair<Vertice *, Aresta *> &b)
    {
        return a.second->peso < b.second->peso;
    });
    int contadorArestas = 0;
    vector<pair<int, int>> conjArestasInduzidas;
    int somaPesos = 0;
    for (auto &aresta: arestas)
    {
        if (ds.find(aresta.first->idNoArquivo) != ds.find(aresta.second->id))
        {
            ds.unionSet(aresta.first->idNoArquivo, aresta.second->id);
            somaPesos += aresta.second->peso;
            cout << "(" << aresta.first->idNoArquivo << "," << aresta.second->id << ") ";
            conjArestasInduzidas.emplace_back(aresta.first->idNoArquivo, aresta.second->id);
//            contadorArestas++;
        }
        if (contadorArestas >= subgrafo->nVerticesArquivo - 1)
        {
            break;
        }
    }
    auto grafoAresta = retornaSubgrafoArestaInduzido(conjArestasInduzidas, false);
    grafoAresta->escreveArquivoDot(false);
    delete grafoAresta;
    cout << "\nSoma dos pesos: " << somaPesos << "\n";
    cout << "\n";
    delete subgrafo;
}


void Grafo::imprimeAgmComArestasDeRetorno(int id)
{
    Vertice *u = encontrar(id);
    if (u == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        return;
    }
    vector<bool> visited(this->nVerticesArquivo, false);
    stack<tuple<int, int, int>> backEdges;
    vector<pair<int, int>> arestas;
    vector<Vertice *> lastest(this->nVerticesArquivo, nullptr);
    stack<Vertice *> stack;
    stack.push(u);
    cout << "Arvore gerada pela busca em profundidade: \n";
    while (!stack.empty())
    {
        u = stack.top();
        stack.pop();
        if (!visited[u->idNoArquivo])
        {
            visited[u->idNoArquivo] = true;
            cout << u->idNoArquivo << "->";
            for (auto v = u->proxAresta; v != nullptr; v = v->proxAresta)
            {
                if (!visited[v->id])
                {
                    if (lastest[v->id] == nullptr)
                    {
                        lastest[v->id] = u;
                        arestas.emplace_back(u->idNoArquivo, v->id);
                    }
                    stack.push(encontrar(v->id));
                }
                if ((visited[v->id] && visited[u->idNoArquivo]) &&
                    v->id !=
                    lastest[u->idNoArquivo]->idNoArquivo) // AVISO: por causa desse if, o grafo PRECISA comecar pelo vertice 0. TODO: simplificar esse if
                {
                    backEdges.push({u->idNoArquivo, v->id, v->peso});
                }
            }
        }
    }
    auto grafoAresta = retornaSubgrafoArestaInduzido(arestas, false);
    grafoAresta->escreveArquivoDot(false, &backEdges);
    delete grafoAresta;
    cout << "\nArestas de retorno: \n";
    while (!backEdges.empty())
    {
        cout << "(" << get<0>(backEdges.top()) << "," << get<1>(backEdges.top()) << ") ";
        backEdges.pop();
    }
}

double Grafo::coeficienteAglomeracaoLocal(int id)
{
    Vertice *u = encontrar(id);
    if (u == nullptr)
    {
        cout << "Vertice nao encontrado\n";
    }
    vector<int> vizinhos;
    for (auto v = u->proxAresta; v != nullptr; v = v->proxAresta)
    {
        vizinhos.push_back(v->id);
    }
    int links = 0;
    for (int i = 0; i < vizinhos.size(); i++)
    {
        auto v = encontrar(vizinhos[i]);
        for (auto w = v->proxAresta; w != nullptr; w = w->proxAresta)
        {
            if (find(vizinhos.begin(), vizinhos.end(), w->id) != vizinhos.end())
            {
                links++;
            }
        }
    }
    if (vizinhos.size() <= 1)
    {
        cout << "Coeficiente de aglomeracao Local: 0\n";
        return 0.0;
    }
    int grauV = static_cast<int>(vizinhos.size());

    /*
     * seguindo essa formula : https://en.wikipedia.org/wiki/Local_clustering_coefficient
     * */
//    double coeficiente = this->getOpcDirec() ? (double) links / (grauV * (grauV - 1)) : (double) links * 2 /
//                                                                                        (grauV * (grauV - 1));

/*
 * seguindo orientacao do professor:
 * */
    double coeficiente = (double) links * 2 / (grauV * (grauV - 1));
    cout << "Coeficiente de aglomeracao Local: " << coeficiente << "\n";
    return coeficiente;
}

void Grafo::coeficienteAglomeracaoMedio()
{
    double soma = 0.0f;
    int n = 0;
    for (auto i = vertices; i != nullptr; i = i->proxVertice)
    {
        soma += coeficienteAglomeracaoLocal(i->idNoArquivo);
        n++;
    }
    cout << "\n\nCoeficiente de aglomeracao Medio: " << soma / (double) n << "\n";
}

vector<Vertice *> Grafo::buscaEmLargura(int id)
{
    Vertice *x = encontrar(id);
    if (x == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        exit(10);
    }
    vector<bool> visited(this->nVerticesArquivo, false);
    queue<Vertice *> q;
    q.push(x);
    visited[x->idNoArquivo] = true;
    vector<Vertice *> result;
    while (!q.empty())
    {
        auto u = q.front();
        q.pop();
        result.push_back(u);
        for (auto v = u->proxAresta; v != nullptr; v = v->proxAresta)
        {
            if (!visited[v->id])
            {
                visited[v->id] = true;
                q.push(encontrar(v->id));
            }
        }
    }
    return result;
}

void Grafo::fechoTransitivoDireto(int id)
{
    auto inicial = encontrar(id);
    if (inicial == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        return;
    }
    vector<bool> visited(this->nVerticesArquivo, false);
    stack<Vertice *> s;
    s.push(inicial);
    visited[s.top()->idNoArquivo] = true;
    cout << "\n Fecho transitivo direto: \n";
    while (!s.empty())
    {
        auto u = s.top();
        s.pop();
        cout << u->idNoArquivo << "->";
        for (auto v = u->proxAresta; v != nullptr; v = v->proxAresta)
        {
            if (!visited[v->id])
            {
                visited[v->id] = true;
                s.push(encontrar(v->id));
            }
        }
    }
    cout << "\n";
}

void Grafo::fechoTransitivoIndireto(int id)
{
    auto inicial = encontrar(id);
    if (inicial == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        return;
    }
    vector<bool> visited(this->nVerticesArquivo, false);
    visited[inicial->idNoArquivo] = true;
    vector<Vertice *> result;
    for (auto v = this->vertices; v != nullptr; v = v->proxVertice)
    {
        vector<Vertice *> fecho;
        if (!visited[v->idNoArquivo])
        {
            fecho = buscaEmLargura(v->idNoArquivo);
        }
        if (find(fecho.begin(), fecho.end(), inicial) != fecho.end())
        {
            result.push_back(v);
        }
        visited[v->idNoArquivo] = true;
    }
    cout << "\n Fecho transitivo indireto: \n";
    for (auto vertice: result)
    {
        cout << vertice->idNoArquivo << "->";
    }
}

Grafo *Grafo::retornaSubgrafoArestaInduzido(vector<pair<int, int>> &conjArestasInduzidas, bool direcionado)
{
    vector<const char *> argv{"subgrafo",
                              this->getPathArquivoEntrada().c_str(),
                              this->getPathArquivoSaida().c_str(),
                              const_cast<char *>(direcionado ? "1" : "0"),
                              const_cast<char *>(getOpcPesoAresta() ? "1" : "0"),
                              const_cast<char *>(getOpcPesoNos() ? "1" : "0")};
    auto sub = new Grafo(argv.size(), const_cast<char **>(argv.data()));
    int i = 0;
    for (auto aresta: conjArestasInduzidas)
    {
        for (auto v = this->vertices; v != nullptr; v = v->proxVertice)
        {
            if (v->idNoArquivo == aresta.first)
            {
                for (auto w = v->proxAresta; w != nullptr; w = w->proxAresta)
                {
                    if (w->id == aresta.second)
                    {
                        sub->adicionaNo({aresta.first, aresta.second}, w->peso);
                        i++;
                    }
                }
            }
        }
    }
    sub->nVerticesArquivo = i;
    sub->nVerticesMemoria = i;
    sub->nArestasArquivo = static_cast<int>(conjArestasInduzidas.size() *
                                            2U); // contando ida e volta, visto estar a fazer isso desde o inicio do projeto
    return sub;
}

void Grafo::escreveArquivoDot(bool direcionado, stack<tuple<int, int, int>> *conjArestasRetorno,
                              const std::string_view *pathSaida)
{
    ofstream file;
    file.open(pathSaida ? pathSaida->data() : this->getPathArquivoSaida().c_str());
    string header = this->getOpcDirec() ? "digraph" : "graph";
    file << "strict " << header << " {\n";
    string seta = direcionado ? " -> " : " -- ";
    for (auto i = this->vertices; i != nullptr; i = i->proxVertice)
    {
        for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
        {
            if (i->idNoArquivo != j->id)
            {
                file << i->idNoArquivo << seta << j->id << " [label=" << j->peso << "];\n";
            }
        }
    }
    if (conjArestasRetorno != nullptr)
    {
        while (!conjArestasRetorno->empty())
        {
            auto aresta = conjArestasRetorno->top();
            file << get<0>(aresta) << seta << get<1>(aresta) << "[label=" << get<2>(aresta) << " style=dotted];\n";
            conjArestasRetorno->pop();
        }
    }
    file << "}\n";
    file.close();
}





