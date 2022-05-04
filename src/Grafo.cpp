#include "Grafo.h"

using namespace std;


Grafo::Grafo(int n)
{
	this->n = n;
	this->matriz = new int*[n];
	for (int i = 0; i < n; ++i)
	{
		matriz[i] = new int[n];
	}
}

Grafo::~Grafo()
{
	for (int i = 0; i < this->n; ++i)
	{
		delete[] this->matriz[i];
	}

	delete[] this->matriz;
}
