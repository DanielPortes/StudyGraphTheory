// src.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include <cassert>

using namespace std;


/*
 * parametros esperados :
 *
 * •./execGrupoX <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
 */

void inicializaParametros(int argc, char* argv[])
{
	if (argc < 5)
	{
		cerr << "[ERROR] Parametros faltantes!\n";
		assert(false);
	}
	string arquivo_entrada = argv[1];
	string arquivo_saida = argv[2];
	string Opc_Direc = argv[3];
	string Opc_Peso_Aresta = argv[4];
	string Opc_Peso_Nos = argv[5];
}

int main(int argc, char* argv[])
{
	inicializaParametros(argc, argv);

	cin.get();

	return 0;
}
