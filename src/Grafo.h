#pragma once

#include <vector>
#include <string_view>
#include <stack>

class Aresta
{
private:
    int id; // TODO: seria vantagem ter um pointeiro para o vertice que aresta aponta?
    int peso;
    Aresta *proxAresta;

    friend class Grafo;

public:
    Aresta() : id(-1), peso(1), proxAresta(nullptr)
    {}

    ~Aresta()
    {
        delete proxAresta;
    }

};

class Vertice
{
private:
    int idNoArquivo; // poderia trocar para string, assim funcionaria para grafos com nomes de vertices
    int peso;
    int grauOut;
    int grauIn;
    Aresta *proxAresta;
    Vertice *proxVertice;

    friend class Grafo;

public:
    Vertice()
            : idNoArquivo(-1), peso(-1), grauOut(0), grauIn(0), proxVertice(nullptr),
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
    using pairPontos = std::pair<int, int>; // lembre-se que uma aresta eh representada por dois pontos (origem e destino)
    int nVerticesArquivo;
    int nVerticesMemoria; // contador de vertices criados de fato, possivel auxilio em debug, nada mais
    int nArestasArquivo;
    Vertice *vertices; // primeiro vertices da lista
    // Vertice *ultimo; // ultimo vertice da lista
    // TODO: possibilidade de criar uma tabela hash para armazenar os vertices, para acelerar a busca por um vertice especifico

    std::string path_arquivo_entrada;
    std::string path_arquivo_saida;
    std::string Opc_Direc;
    std::string Opc_Peso_Aresta;
    std::string Opc_Peso_Nos;

    void adicionaNo(pairPontos, int peso);

    Vertice *criaNovoVertice(int idNoArquivo, int peso);

    void criaNovaAresta(Vertice *&vertice, int idPonta, int pesoAresta);

    void addVerticesDesconexos();
public:

    Grafo(size_t argc, char **argv);

    ~Grafo();

    Grafo(const Grafo &) = delete;

    Grafo &operator=(const Grafo &) = delete;

    void leitura(std::string_view path);

    void imprimirGrafo();

    [[nodiscard]] std::string getPathArquivoEntrada() const;

    [[nodiscard]] std::string getPathArquivoSaida() const;

    [[nodiscard]] bool getOpcDirec() const;

    [[nodiscard]] bool getOpcPesoAresta() const;

    [[nodiscard]] bool getOpcPesoNos() const;

    void caminhoMinimoDijkstra(int src, int destino);

    void caminhoMinimoFloyd(int src, int destino);

    Grafo *retornaSubgrafoVerticeInduzido(const std::vector<int> &conjVeticeInduzido, bool direcionado);

    void retornaAgmEmSubgrafoPorPrim(std::vector<int> &conjVeticeInduzido);

    void retornaAgmEmSubgrafoPorKruskal(std::vector<int> &conjVeticeInduzido);

    Vertice *encontrar(int id);

    void imprimeAgmComArestasDeRetorno(int id);

    double coeficienteAglomeracaoLocal(int id);

    void coeficienteAglomeracaoMedio();

    std::vector<Vertice *> buscaEmLargura(int id);

    void fechoTransitivoDireto(int id);

    void fechoTransitivoIndireto(int id);

    Grafo *retornaSubgrafoArestaInduzido(std::vector<std::pair<int, int>> &conjArestasInduzidas, bool direcionado);

    void escreveArquivoDot(bool direcionado, std::stack<std::tuple<int, int, int>> *conjArestasRetorno = nullptr,
                           const std::string_view *pathSaida = nullptr);
};
