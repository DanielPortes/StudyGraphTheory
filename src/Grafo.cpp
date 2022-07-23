#include <fstream>
#include <cassert>
#include <memory>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>
#include <stack>

#include "Grafo.h"

using namespace std;

Grafo::Grafo(int argc, char **argv) : vertices(nullptr), nVerticesMemoria(0), nVerticesArquivo(0), nArestasArquivo(0)
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
//    for (auto v = this->vertices; v != nullptr; v = v->proxVertice)
//    {
//        cout << "Vertice " << v->idNoArquivo << ": ";
//        for (auto a = v->proxAresta; a != nullptr; a = a->proxAresta)
//        {
//            cout << a->id << " ";
//        }
//        cout << "\n";
//    }
//    cout << "\n";
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
    int verticeIdArquivo = 0, arestaId = 0, peso = 0;
    while (!fileIn.eof())
    {
        // TODO: professor flw na possibilidade de o id do vertices nao ser um numero, daria certo?
        getline(fileIn, linha, ' ');
        verticeIdArquivo = atoi(linha.c_str());

        getline(fileIn, linha, ' ');
        arestaId = atoi(linha.c_str());

        if (getOpcPesoAresta())
        {
            getline(fileIn, linha);
            peso = atoi(linha.c_str());
        }
        // TODO: aonde ficaria os peso dos vertices, seria trabalho 2?

        this->adicionaNo(verticeIdArquivo, arestaId, peso);
    }
    this->imprimirGrafo();
}

Vertice *Grafo::criaNovoVertice(int idNoArquivo, int peso)
{
    auto vertice = new Vertice();
    vertice->idNoArquivo = idNoArquivo;
    vertice->idNaMemoria = this->nVerticesMemoria;
    ++this->nVerticesMemoria;
    vertice->peso = peso;
    return vertice;
}

void Grafo::criaNovaAresta(Vertice *&vertice, int idPonta, int pesoAresta)
{
    auto j = this->vertices;
    for (; j->proxVertice != nullptr && j->idNoArquivo != idPonta; j = j->proxVertice)
    {}

    if (j->idNoArquivo != idPonta)
    {
        j->proxVertice = criaNovoVertice(idPonta, 0);
    }
    auto *novaAresta = new Aresta();
    novaAresta->id = idPonta;
    novaAresta->idNaMemoria = this->nArestasArquivo;
    ++this->nArestasArquivo;
    novaAresta->peso = pesoAresta;
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

void Grafo::adicionaNo(int &idNoArquivo, int arestaID, int peso)
{
    if (!this->vertices)
    {
        this->vertices = criaNovoVertice(idNoArquivo, 0);
        criaNovaAresta(this->vertices, arestaID, peso);
        this->vertices->grauOut++;
    }
    else
    {
        auto i = this->vertices;
        for (; i->proxVertice != nullptr && i->idNoArquivo != idNoArquivo; i = i->proxVertice)
        {}

        if (i->idNoArquivo != idNoArquivo) // Vertice nao presente na lista
        {
            i->proxVertice = criaNovoVertice(idNoArquivo, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i, arestaID, peso);
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
        }
        else
        {
            i->proxVertice = criaNovoVertice(arestaID, 0);
            i = i->proxVertice;
        }
        criaNovaAresta(i, idNoArquivo, peso);
    } // Nossa, esse codigo ficou lindo
}

// TODO: e se o vertices estiver desconectado de todos
void Grafo::imprimeFechoTransitivoDireto(int idNoArquivo)
{
    if (idNoArquivo < 0 || idNoArquivo > this->nVerticesArquivo)
    {
        cerr << "\n[ERROR] Grafo::imprimeFechoTransitivoDireto(int idNoArquivo), \t idNoArquivo invalido";
        assert(false);
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
        if (no.second && no.first->idNoArquivo != idNoArquivo)
        { // TODO: solucao preguicosa para n imprimirGrafo a si mesmo
            cout << no.first->idNoArquivo << ", ";
        }
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
        assert(false);
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
        {
            cout << get<0>(no)->idNoArquivo << ", ";
        }
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

Grafo *Grafo::getSubGrafo(const vector<int> &conjVeticeInduzido, bool direcionado)
{
    cout
            << "Gerando subgrafo... \n[WARNING] Atencao aos vertices passados por parametro no conjunto vertice induzido\n";
    vector<const char *> argv{"subgrafo",
                              const_cast<char *>(this->getPathArquivoEntrada()),
                              const_cast<char *>(this->getPathArquivoSaida()),
                              const_cast<char *>(direcionado ? "1" : "0"),
                              const_cast<char *>(getOpcPesoAresta()),
                              const_cast<char *>(getOpcPesoNos())};
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
                    subgrafo->adicionaNo(i->idNoArquivo, j->id, j->peso);
                    ++idContador;
                }
            }
        }
    }
    subgrafo->nVerticesArquivo = conjVeticeInduzido.size();
    subgrafo->nVerticesMemoria = idContador;
    return subgrafo; // lembrar de deletar o grafo retornado
}


// TODO: verificar se o grafo eh conexo e tem peso nas arestas?
void Grafo::retornaAgmEmSubgrafoPorPrim(vector<int> &conjVeticeInduzido)
{
    if (conjVeticeInduzido.size() == 1)
    {
        conjVeticeInduzido.pop_back();
        for (int i = 0; i < this->nVerticesArquivo; ++i)
        {
            conjVeticeInduzido.push_back(i);
        }
    }
    Grafo *subgrafo = getSubGrafo(conjVeticeInduzido, false);
    priority_queue<pair<int, Vertice *>, vector<pair<int, Vertice *>>, greater<>> lista; // peso e vertice
    vector<int> key(this->nVerticesArquivo, (numeric_limits<int>::max() / 2));
    vector<bool> finished(this->nVerticesArquivo, false);
    vector<Vertice *> parent(this->nVerticesArquivo, nullptr);
    auto i = subgrafo->vertices;
    key[i->idNoArquivo] = 0;
    lista.push(make_pair(0, i));
    while (!lista.empty())
    {
        auto u = lista.top().second;
        lista.pop();
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
            conjArestasInduzidas.push_back(make_pair(parent[i]->idNoArquivo, i));
        }
    }
    auto grafoAresta = getSubGraphEdgeInduced(conjArestasInduzidas, false);

    grafoAresta->writeDotFile(false);
    delete grafoAresta;

    int somaPesos = 0;
    for (auto valor: key)
    {
        if (valor != (numeric_limits<int>::max() / 2))
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
    Grafo *subgrafo = getSubGrafo(conjVeticeInduzido, false);
    DisjointSet ds(this->nVerticesArquivo);
    vector<pair<Vertice *, Aresta *>> arestas; // com essa representacao eu tenho informacao sobre cada aresta do grafo (u,v), e tenho acesso ao peso da aresta por v->peso
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

    vector<pair<int, int>> conjArestasInduzidas;
    int somaPesos = 0;
    for (auto &aresta: arestas)
    {
        if (ds.find(aresta.first->idNoArquivo) != ds.find(aresta.second->id))
        {
            ds.unionSet(aresta.first->idNoArquivo, aresta.second->id);
            somaPesos += aresta.second->peso;
            cout << "(" << aresta.first->idNoArquivo << "," << aresta.second->id << ") ";
            conjArestasInduzidas.push_back(make_pair(aresta.first->idNoArquivo, aresta.second->id));
        }
    }
    auto grafoAresta = getSubGraphEdgeInduced(conjArestasInduzidas, false);
    grafoAresta->writeDotFile(false);
    delete grafoAresta;
    cout << "\nSoma dos pesos: " << somaPesos << "\n";
    cout << "\n";
    delete subgrafo;
}


void Grafo::imprimeAGMcomArestasDeRetorno(int id)
{
    Vertice *u = encontrar(id);
    if (u == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        return;
    }
    vector<bool> visited(this->nVerticesArquivo, false);
    stack<pair<int, int>> backEdges;
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
                        arestas.push_back({u->idNoArquivo, v->id});
                    }
                    stack.push(encontrar(v->id));
                }
                if ((visited[v->id] && visited[u->idNoArquivo]) &&
                    v->id != lastest[u->idNoArquivo]->idNoArquivo) // AVISO: por causa desse if, o grafo PRECISA comecar pelo vertice 0. TODO: simplificar esse if
                {
                    backEdges.push({u->idNoArquivo, v->id});
                }
            }
        }
    }
    auto grafoAresta = getSubGraphEdgeInduced(arestas, false);
    grafoAresta->writeDotFile(false, backEdges);
    delete grafoAresta;
    cout << "\nArestas de retorno: \n";
    while (!backEdges.empty())
    {
        cout << "(" << backEdges.top().first << "," << backEdges.top().second << ") ";
        backEdges.pop();
    }
}

double Grafo::coeficienteAglomeracaoLocal(int id)
{
    Vertice *u = encontrar(id);
    if (u == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        assert(false);
    }
    vector<int> neighborsU;
    for (auto v = u->proxAresta; v != nullptr; v = v->proxAresta)
    {
        neighborsU.push_back(v->id);
    }
    int links = 0;
    for (int i = 0; i < neighborsU.size(); i++)
    {
        auto v = encontrar(neighborsU[i]);
        for (auto w = v->proxAresta; w != nullptr; w = w->proxAresta)
        {
            if (find(neighborsU.begin(), neighborsU.end(), w->id) != neighborsU.end())
            {
                links++;
            }
        }
    }
    if (neighborsU.size() <= 1)
    {
        cout << "Clustering coefficient: 0\n";
        return 0.0;
    }
    cout << "Clustering coefficient: " << (double) links / (neighborsU.size() * (neighborsU.size() - 1)) << "\n";
    return (double) links / (neighborsU.size() * (neighborsU.size() - 1));
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
    cout << "Average clustering coefficient: " << soma / (double) n << "\n";
}

void Grafo::averageClusteringCoefficient()
{
    /*
     * INCORRETO
     * */
    int links = 0;
    for (auto i = vertices; i != nullptr; i = i->proxVertice)
    {
        for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
        {
            if (i->idNoArquivo != j->id)
            {
                links++;
            }
        }
    }
    cout << "Average clustering coefficient: " << (double) links / (nVerticesArquivo * (nVerticesArquivo - 1)) << "\n";
}

vector<Vertice *> Grafo::breadthFirstSearch(int id)
{
    Vertice *x = encontrar(id);
    if (x == nullptr)
    {
        cout << "Vertice nao encontrado\n";
        assert(false);
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
//        cout << u->idNoArquivo << "->";
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
            fecho = breadthFirstSearch(v->idNoArquivo);
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

Grafo *Grafo::getSubGraphEdgeInduced(vector<pair<int, int>> &conjArestasInduzidas, bool direcionado)
{
    vector<const char *> argv{"subgrafo",
                              const_cast<char *>(this->getPathArquivoEntrada()),
                              const_cast<char *>(this->getPathArquivoSaida()),
                              const_cast<char *>(direcionado ? "1" : "0"),
                              const_cast<char *>(getOpcPesoAresta()),
                              const_cast<char *>(getOpcPesoNos())};
    auto sub = new Grafo(argv.size(), const_cast<char **>(argv.data()));
    int i = 0;
    for (auto aresta: conjArestasInduzidas)
    {
        sub->adicionaNo(aresta.first, aresta.second, 0);
        i++;
    }
    sub->nVerticesArquivo = i;
    sub->nVerticesMemoria = i;
    sub->nArestasArquivo = conjArestasInduzidas.size() *
                           2; // contando ida e volta, visto que estava fazendo isso desde o inicio do projeto
    return sub;
}

void Grafo::writeDotFile(bool direcionado, stack<pair<int, int>> conjArestasRetorno)
{
    ofstream file;
    file.open(this->getPathArquivoSaida());
    file << "strict graph G {\n";
    string seta = direcionado ? "->" : "--";
    for (auto i = this->vertices; i != nullptr; i = i->proxVertice)
    {
        for (auto j = i->proxAresta; j != nullptr; j = j->proxAresta)
        {
            if (i->idNoArquivo != j->id)
            {
                file << i->idNoArquivo << seta << j->id << ";\n";
            }
        }
    }
    if (!conjArestasRetorno.empty())
    {
        while (!conjArestasRetorno.empty())
        {
            auto aresta = conjArestasRetorno.top();
            conjArestasRetorno.pop();
            file << aresta.first << seta << aresta.second << "[style=dotted];\n";
        }
    }
    file << "}\n";
    file.close();
}

