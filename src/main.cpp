// src.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include <cassert>
#include "parametros.h"
#include "leitura.h"

using namespace std;



/*
 * parametros esperados :
 *
 * •./execGrupoX <path_arquivo_entrada> <path_arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
 */

void inicializaParametros(int argc, char *argv[]) {
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

int main(int argc, char *argv[]) {
   
    // obviamente o path abaixo so serve no meu pc e deve ser removida
    lerArquivo("C:/dev/grafos/cmake-build-debug/teste.txt");

    cin.get();
    return 0;
}
