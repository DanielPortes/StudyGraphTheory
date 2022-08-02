/*
 * TRABALHO DESENVOLVIDO POR:
 * - DANIEL FAGUNDES - 100%
 *
 *
 * */


#include <iostream>
#include <sstream>
#include "Grafo.h"
#include "Timer.h"

using namespace std;

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
    cout << escolhas::caminhoMinimoDijkstra << " - Caminho minimo entro dois pairPontos por Dijkstra" << endl;
    cout << escolhas::caminhoMinimoFloyd << " - Caminho minimo entro dois pairPontos por Floyd" << endl;
    cout << escolhas::agmEmSubgrafoPorPrim << " - Arvore Geradora Minima sobre o subgrafo vertice-induzido por X usando o algoritmo de Prim;" << endl;
    cout << escolhas::agmEmSubgrafoPorKruskal << " - Arvore Geradora Minima sobre o subgrafo vertice-induzido por X usando o algoritmo de Kruskal" << endl;
    cout << escolhas::agmComArestasDeRetorno << " - Arvore dada pela ordem de caminhamento em profundidade a partir de no dado parametro destaque as arestas de retorno" << endl;
    cout << escolhas::sair << " - Sair" << endl;
}

auto separaNumerosdaString(const string& str, char delimitador) -> vector<int>
{
    stringstream ss(str);
    string item;
    vector<int> numbers;
    while (getline(ss, item, delimitador))
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
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                Timer timer("\nTempo de Fecho Transitivo Direto");
                grafo->fechoTransitivoDireto(vertice);
                break;
            }
            case escolhas::fechoTransitivoIndireto:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                Timer timer("\nTempo de Fecho Transitivo Inireto");
                grafo->fechoTransitivoIndireto(vertice);
                break;
            }
            case escolhas::coeficienteAglomeracaoLocal:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                Timer timer("\nTempo de Coeficiente de Aglomeracao Local");
                grafo->coeficienteAglomeracaoLocal(vertice);
                break;
            }
            case escolhas::coeficienteAglomeracaoMedio:
            {
                Timer timer("\nTempo de Coeficiente de Aglomeracao Medio");
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
                Timer timer("\nTempo de Caminho Minimo Dijkstra");
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
                Timer timer("\nTempo de Caminho Minimo Floyd");
                grafo->caminhoMinimoFloyd(verticeOrigem, verticeDestino);
                break;
            }
            case escolhas::agmEmSubgrafoPorPrim:
            {
                cout << "Digite o conjunto de vertices separados por virgula (-1 = todos): ";
                string vertices;
                cin >> vertices;
                vector<int> verticesSeparados = separaNumerosdaString(vertices, ',');
                Timer timer("\nTempo de AGM em Subgrafo por Prim");
                grafo->retornaAgmEmSubgrafoPorPrim(verticesSeparados);
                break;
            }
            case escolhas::agmEmSubgrafoPorKruskal:
            {
                cout << "Digite o conjunto de vertices separados por virgula (-1 = todos): ";
                string vertices;
                cin >> vertices;
                vector<int> verticesSeparados = separaNumerosdaString(vertices, ',');
                Timer timer("\nTempo de AGM em Subgrafo por Kruskal");
                grafo->retornaAgmEmSubgrafoPorKruskal(verticesSeparados);
                break;
            }
            case escolhas::agmComArestasDeRetorno:
            {
                cout << "Digite o vertice: ";
                int vertice;
                cin >> vertice;
                Timer timer("\nTempo de AGM com Arestas de Retorno");
                grafo->imprimeAgmComArestasDeRetorno(vertice);
                break;
            }
            case escolhas::sair:
            {
                return;
            }
            default:
            {
                cout << "Opcao invalida" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    }
}

/*
 * parametros esperados :
 *
 * •./execGrupoX <path_arquivo_entrada> <path_arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
 */
int main(int argc, char *argv[])
{
    Grafo grafo(argc, argv);
    {
        Timer timer("Tempo de leitura: ");
        grafo.leitura(grafo.getPathArquivoEntrada());
    }
    menu(&grafo);

    return 0;
}
