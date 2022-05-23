#pragma once

class Aresta
{
private:
    int id;
    int peso;
    int grauInp;
    int grauOut;
    Aresta *proxAresta;

    friend class Grafo;
};

class No
{
private:
    int id;
    int peso;
    int grauInp;
    int grauOut;
    No *proxNo;
    Aresta *proxAresta; //  Nao foi algo que o professor detalhou, possivel duvida

    friend class Grafo;
};


class Grafo
{
private:
    int nVertices;
    No *no; // primeiro no da lista
public:
    Grafo(int argc, char **argv);

    ~Grafo();

    void leitura(const char *path);

    void adicionaNo(int, int, int);

private:
    void inicializaParametros(int argc, char *argv[]);

    const char *path_arquivo_entrada;
public:
    const char *getPathArquivoEntrada() const;

    const char *getPathArquivoSaida() const;

private:
    const char *path_arquivo_saida;
    const char *Opc_Direc;
    const char *Opc_Peso_Aresta;
    const char *Opc_Peso_Nos;
};

