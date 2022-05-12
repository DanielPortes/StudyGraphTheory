#include <fstream>
#include <cassert>
#include <memory>
#include <sstream>
#include <iostream>

#include "Grafo.h"

using namespace std;

Grafo::Grafo(int argc, char **argv)
{
    this->inicializaParametros( argc, argv);
}

Grafo::~Grafo()
{
// TODO: deleter
}

void Grafo::inicializaParametros(int argc, char **argv)
{
    if (argc != 6) {
        cerr << "[ERROR] Parametros faltantes!\n";
        assert(false);
    }
    path_arquivo_entrada =   argv[1];
    path_arquivo_saida =     argv[2];
    Opc_Direc =              argv[3];
    Opc_Peso_Aresta =        argv[4];
    Opc_Peso_Nos =           argv[5];
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
		cerr << "ERRO: arquivo nao pode ser aberto \n\t lerArquivo";
		assert(false);
	}
	// criando um buffer para o arquivo
	auto bufferSize = tamanhoArquivo(arquivoEntrada);
	unique_ptr<char[]> buffer(new char[bufferSize]);
	arquivoEntrada.read(buffer.get(), bufferSize);
	arquivoEntrada.close();
	stringstream fileIn(buffer.get());

	string linha;
	getline(fileIn, linha);
	this->nVertices = atoi(linha.c_str());
	int id, aresta, peso;
	while (!fileIn.eof())
	{
		getline(fileIn, linha, ' ');
		id = atoi(linha.c_str());

		getline(fileIn, linha, ' ');
		aresta = atoi(linha.c_str());

		getline(fileIn, linha, ' ');
		peso = atoi(linha.c_str());

		this->adicionaNo(id, aresta, peso);
	}
}


// TODO: duvida, essa parada de aresta, como ligar
void Grafo::adicionaNo(int id, int aresta, int peso)
{
    // TODO: fazer a logica do direcionado
    if (!this->no)
    {
        this->no = new No;
        this->no->id = id;
        this->no->peso = peso;
//        this->no->proxAresta = nullptr;
    } else
    {
        auto i = this->no->proxNo;
        for (; i->proxNo != nullptr && i->id != id; i = i->proxNo)
        {
        }
        if (i->id == id)
        {
            i->grauOut++;
            i->proxAresta = new Aresta;
            i->proxAresta->id = 0;
            i->proxAresta->peso = peso;
        } else
        {
            i->proxNo = new No;
            i->id = id;
            i->peso = peso;
//        this->no->proxAresta = nullptr;
        }
    }
    if (Opc_Direc)
    {
        auto i = this->no->proxNo;
        for (; i->proxNo != nullptr && i->id != aresta; i = i->proxNo)
        {
        }
        if (i->id == aresta)
        {
            i->grauOut++;
            i->proxAresta = new Aresta;
            i->proxAresta->id = 0;
            i->proxAresta->peso = peso;
        } else
        {
            i->proxNo = new No;
            i->id = id;
            i->peso = peso;
//        this->no->proxAresta = nullptr;
        }
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
