// src.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include <cassert>
#include "Grafo.h"

using namespace std;



/*
 * parametros esperados :
 *
 * •./execGrupoX <path_arquivo_entrada> <path_arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
 */


int main(int argc, char *argv[]) {

    Grafo grafo(argc,argv);
    grafo.leitura(grafo.getPathArquivoEntrada());
    grafo.imprimeFechoTransitivoDireto(1);
//    grafo.imprimeFechoTransitivoIndireto(5);
//    cin.get();
    return 0;
}
