// src.cpp: define o ponto de entrada para o aplicativo.
//

#include <iostream>
#include <cassert>
#include <sstream>
#include "Grafo.h"

using namespace std;


/*
 * parametros esperados :
 *
 * •./execGrupoX <path_arquivo_entrada> <path_arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
 */
enum escolhas {
    imprimirGrafo = 0,
    fechoTransitivoDireto = 1,
    fechoTransitivoIndireto = 2,
    coeficienteAglomeracaoLocal = 3,
    coeficienteAglomeracaoMedio = 4,
    caminhoMinimoDijkstra = 5,
    caminhoMinimoFloyd = 6,
    agmEmSubgrafoPorPrim = 7,
    agmEmSubgrafoPorKruskal = 8,
    agmComArestasDeRetorno = 9,
    sair = 10
};
void info()
{
    cout << "\n";
    cout << escolhas::imprimirGrafo << " - Imprimir grafo" << endl;
    cout << escolhas::fechoTransitivoDireto << " - Imprimir fecho transitivo direto" << endl;
    cout << escolhas::fechoTransitivoIndireto << " - Imprimir fecho transitivo indireto" << endl;
    cout << escolhas::coeficienteAglomeracaoLocal << " - Coeficiente de aglomeracao local" << endl;
    cout << escolhas::coeficienteAglomeracaoMedio << " - Coeficiente de aglomeracao medio" << endl;
    cout << escolhas::caminhoMinimoDijkstra << " - Caminho minimo entro dois pontos por Dijkstra" << endl;
    cout << escolhas::caminhoMinimoFloyd << " - Caminho minimo entro dois pontos por Floyd" << endl;
    cout << escolhas::agmEmSubgrafoPorPrim << " - Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Prim;" << endl;
    cout << escolhas::agmEmSubgrafoPorKruskal << " - Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Kruskal" << endl;
    cout << escolhas::agmComArestasDeRetorno << " - Árvore dada pela ordem de caminhamento em profundidade a partir de nó dado parâmetro destacando as arestas de retorno;" << endl;
    cout << escolhas::sair << " - Sair" << endl;
}

vector<int> separaNumerosdaString(const string& str, char separator)
{
    stringstream ss(str);
    string item;
    vector<int> numbers;
    while (getline(ss, item, separator))
    {
        numbers.push_back(stoi(item));
    }
    return numbers;
}

void menu(Grafo* grafo)
{
    cout << "\n";
    int opcao = -1;
    while (true)
    {
        info();
        cout << "\nEscolha uma opcao: \n";
        cin >> opcao;
        switch (opcao)
        {
            case escolhas::imprimirGrafo:
            {
                grafo->imprimirGrafo();
                break;
            }
            case escolhas::fechoTransitivoDireto:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                grafo->fechoTransitivoDireto(vertice);
                break;
            }
            case escolhas::fechoTransitivoIndireto:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                grafo->fechoTransitivoIndireto(vertice);
                break;
            }
            case escolhas::coeficienteAglomeracaoLocal:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                grafo->coeficienteAglomeracaoLocal(vertice);
                break;
            }
            case escolhas::coeficienteAglomeracaoMedio:
            {
                grafo->coeficienteAglomeracaoMedio();
                break;
            }
            case escolhas::caminhoMinimoDijkstra:
            {
                cout << "Digite o vertice de origem: ";
                int verticeOrigem;
                cin >> verticeOrigem;
                cout << "Digite o vertice de destino: ";
                int verticeDestino;
                cin >> verticeDestino;
                grafo->caminhoMinimoDijkstra(verticeOrigem, verticeDestino);
                break;
            }
            case escolhas::caminhoMinimoFloyd:
            {
                cout << "Digite o vertice de origem: ";
                int verticeOrigem;
                cin >> verticeOrigem;
                cout << "Digite o vertice de destino: ";
                int verticeDestino;
                cin >> verticeDestino;
                grafo->caminhoMinimoFloyd(verticeOrigem, verticeDestino);
                break;
            }
            case escolhas::agmEmSubgrafoPorPrim:
            {
                cout << "Digite o conjunto de vertices separados por virgula (-1 = todos): ";
                string vertices;
                cin >> vertices;
                vector<int> verticesSeparados = separaNumerosdaString(vertices, ',');
                grafo->retornaAgmEmSubgrafoPorPrim(verticesSeparados);
                break;
            }
            case escolhas::agmEmSubgrafoPorKruskal:
            {
                cout << "Digite o conjunto de vertices separados por virgula (-1 = todos): ";
                string vertices;
                cin >> vertices;
                vector<int> verticesSeparados = separaNumerosdaString(vertices, ',');
                grafo->retornaAgmEmSubgrafoPorKruskal(verticesSeparados);
                break;
            }
            case escolhas::agmComArestasDeRetorno:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                grafo->imprimeAGMcomArestasDeRetorno(vertice);
                break;
            }
            case escolhas::sair:
            {
                return;
            }
            default:
            {
                cout << "Opcao invalida" << endl;
                cin.clear(); // TODO: ainda nao resolve se a pessoa colocar uma letra, resultado loop 'infinito'
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Grafo grafo(argc, argv);
    grafo.leitura(grafo.getPathArquivoEntrada());
    menu(&grafo);

    return 0;
}
