//
// Created by Daniel on 12/05/2022.
//
#include <fstream>
#include <cassert>
#include <memory>
#include <sstream>
#include <iostream>
#include <strstream>


using namespace std;

streampos inline tamanhoArquivo(fstream &arq)
{
    arq.seekg(0, arq.end);
    streampos tam = arq.tellg();
    arq.seekg(0);
    return tam;
}

void lerArquivo(const string& path_arquivo)
{
    fstream arquivoEntrada(path_arquivo, ios::in);
    if (!arquivoEntrada.is_open())
    {
        cerr << "ERRO: arquivo nao pode ser aberto \n\t lerArquivo";
        assert(false);
    }
    // criando um buffer para o arquivo
    auto bufferSize = tamanhoArquivo(arquivoEntrada);
    unique_ptr<char[]> buffer(new char[bufferSize]);
    arquivoEntrada.read(buffer.get(), static_cast<int>(bufferSize));
    arquivoEntrada.close();
    stringstream fileIn(buffer.get());

    // variavel auxiliar
    string linha;
    while (!fileIn.eof())
    {
        getline(fileIn, linha);    
        cout << linha << "\n";
    }
}
