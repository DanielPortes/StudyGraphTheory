#pragma once

class No
{
private:
    int id;
    int peso;
    int grauInp;
    int grauOut;
    No *proxNo;
//    Aresta *proxAresta;   Nao foi algo que o professor detalhou, possivel duvida
};

class Aresta
{
private:
    int id;
    int peso;
    int grauInp;
    int grauOut;
    Aresta *proxAresta;
};


class Grafo
{
private:

public:
	Grafo();
	~Grafo();

};

