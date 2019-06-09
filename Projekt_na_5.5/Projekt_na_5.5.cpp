// Projekt_na_5.5.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;



struct lista
{
public:
	lista *next;
	int K;
	int H ,G, F; // H - wartość funkcji heurestycznej ; G - wartosc okreslonej ilosci kroków ; F = H+G;
};

class stos
{
private:
	lista *S;
public:
	stos()
	{
		S = NULL;
	}
	~stos()
	{
		while(S)  pop();
	}
	bool empty()
	{
		return !S;
	}
	int top()
	{
		return S->K;
	}
	void push(int to)
	{
		lista *tmp = new lista;
		tmp->K = to;
		tmp->next = S;
		S = tmp;
	}
	void pop()
	{
		if (S)
		{
			lista * tmp = S;
			S = S->next;
			delete tmp;
		}
	}
};



int wierzcholki = 16; // ilosc wierzchołków w grafie

lista ** A;// lista sąsiedztwa

// tablica inteksów pól na planszy 
int tab[4][4] =
{
	0,1,2,3,
	4,5,6,7,
	8,9,10,11,
	12,13,14,15

};

// dodawanie krawedzi do grafu
void dodaj_krawedz(int v1, int v2, lista *p)
{
	p = new lista;    // Tworzymy nowy element
	p->K = v2;          // Numerujemy go jako v2
	p->next = A[v1]; // Dodajemy go na początek listy A[v1]
	A[v1] = p;
	p = new lista;    // Tworzymy nowy element
	p->K = v1;          // Numerujemy go jako v1
	p->next = A[v2];    // Dodajemy go na początek listy A[v2]
	A[v2] = p;
}

// tworzy graf z połączeniem wszsytkich możliwych ruchów skoczka z każdego możliwego pola.
void ruch_skoczka(lista ** A, lista *p)
{
	int x, y; // zmienen okreslajace współżedne analizowanego ruchu

	// fory przechodzace przez wszystkie pola i określajace jakie ruchy z kazdego pola moze wykonać skoczek
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			x = i + 2;
			y = j + 1;
			if (x< 4 && y < 4)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);
				
			}
			x = i + 2;
			y = j - 1;
			if (x < 4 && y > 0)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
			x = i - 2;
			y = j + 1;
			if (x > 0 && y < 4)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
			x = i - 2;
			y = j - 1;
			if (x > 0 && y > 0)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
			x = i + 1;
			y = j + 2;
			if (x < 4 && y < 4)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
			x = i - 1;
			y = j + 2;
			if (x > 0 && y < 4)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
			x = i + 1;
			y = j - 2;
			if (x < 4 && y > 0)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
			x = i - 1;
			y = j - 2;
			if (x > 0 && y > 0)
			{
				dodaj_krawedz(tab[i][j], tab[x][y], p);

			}
		}
	}
	
}

void tworzenie_grafu()
{
	lista * p;
	p = new lista;

	A = new lista *[wierzcholki]; // tworzenie listy sasiedztwa
	for (int i = 0; i < wierzcholki; i++)
		A[i] = NULL; // wypełniamy ją pustymi listami 

	//wczesniejsze dodanie krawedzi zanim wymysliłem jak dodac je lepiej 
	/*dodaj_krawedz(0, 6, p);
	dodaj_krawedz(0, 9, p);
	dodaj_krawedz(3, 10, p);
	dodaj_krawedz(3, 5, p);
	dodaj_krawedz(15, 9, p);
	dodaj_krawedz(15, 6, p);
	dodaj_krawedz(12, 5, p);
	dodaj_krawedz(12, 10, p);
	dodaj_krawedz(5, 14, p);
	dodaj_krawedz(5, 11, p);
	dodaj_krawedz(6, 8, p);
	dodaj_krawedz(6, 13, p);
	dodaj_krawedz(9, 7, p);
	dodaj_krawedz(9, 2, p);
	dodaj_krawedz(10, 1, p);
	dodaj_krawedz(10, 4, p);
	dodaj_krawedz(4, 2, p);
	dodaj_krawedz(4,13, p);
	dodaj_krawedz(1, 8, p);
	dodaj_krawedz(1, 7, p);
	dodaj_krawedz(11, 2, p);
	dodaj_krawedz(11, 13, p);
	dodaj_krawedz(14, 8, p);
	dodaj_krawedz(14, 7, p);*/

	// wygenerowanie grafu 
	ruch_skoczka(A, p);
}

// przeszukiwanie w głąb grafu 
void DFS_Droga(int poczatkowy, int koncowy)
{
	stos S; // stos sprawdzajacy
	bool * odwiedzone, znaleziono; // tablica odwiedzonych pól i zmienna okeslajaca czy znaleziono szukane pole
	int *P, v, u; // zmienne 
	lista * pv; // lista pomocnicza wykożystana przy przeszukiwaniu sąsiadów 

	odwiedzone = new bool[wierzcholki]; // stworzenie tablicy elementów odwiedzonych
	for (int i = 0; i < wierzcholki; i++)   // Tablicę odwiedzonych zerujemy
		odwiedzone[i] = false;

	// tworzymy tablice w której określimy który wierzchołek z którym "został połaczony scieżką" 
	P = new int[wierzcholki];
	// określamy pierwszy wierzchołęk do którego poszliśmy czyli wierzcholek startowy 
	P[poczatkowy] = -1;
	// dodanie pierwszego elementu na stos
	S.push(poczatkowy); 
	// pierwszy element jest odwiedzony 
	odwiedzone[poczatkowy] = true;
	//	pierwszy element nie jest szukanym 
	znaleziono = false;
	// tablica do zamienienia kolejnosci ścieżki metoda do poprawienie powinna dzialać lepiej 
	int tab[50];
	// zmienna do określenia ilosci elementów ścieżce 
	int a = 0;
	 // dopuki stos nie jest pusty 
	while (!S.empty())
	{
		v = S.top();
		S.pop();
		// sprawdzamy czy znaleźliśmy szukany wierzchołek 
		if (v == koncowy)
		{
			znaleziono = true;
			break;
		}
		// sprawdzenie wszsytkich sasiadów elementu i dodanie ich na stos
		for (pv = A[v]; pv; pv = pv->next)
		{
			u = pv->K;
			if (!odwiedzone[u])
			{
				P[u] = v;
				S.push(u);
				odwiedzone[u] = true;
			}
		}
	}
	// jeśli nie udało się znaleźć ścieżki 
	if (!znaleziono) cout << "brak ścieżki";

	// jesli udało sie znaleźć ścieżke wypisujemy wynik 
	else
	{
		// dopuki nie bedzie elementu 
		while (v > -1)
		{
			tab[a] = v;
			//cout << v << " ";
			v = P[v];
			// sprawdzanie ile jest elementów scieżki 
			a++;
		}
		// wyświetlenie ścieżki w kolejnosci przechodzenia
		for (int i = a-1; i >= 0; i--)
		{
			cout << tab[i] << " ";
		}
		cout << endl;
	}
}

int main()
{
	// tworzenie grafu 
	tworzenie_grafu();
	// szukanie ścieżki 
	DFS_Droga(13, 1);

	return 0;
}
