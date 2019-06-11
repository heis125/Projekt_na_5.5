/*********************************************************************************************************************************************************************/
/*                                                        Przeszukiwanie algorytmem DFS ORAZ A*                                                                      */
/*                                                                                                                                                                   */
/*********************************************************************************************************************************************************************/


#include "pch.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <set>
#include <vector>
using namespace std;

#define ROZMIAR 5				//definicja rozmiaru planszy
#define WIERZCHOLKI 25			//określenie ilosci wierzcholków 
#define INF 999999999			// wartosc nieskonczona 



// tablica inteksów pól na planszy 
int tab[ROZMIAR][ROZMIAR] =
{
	0, 1, 2, 3, 4,
	5 ,6, 7, 8, 9,
	10,11,12,13,14,
	15,16,17,18,19,
	20,21,22,23,24
};



int* f_wartosc;				// wskaźnik przechowujacy wartość funkcji f(n) = h(n)+g(n)  dla każdego wezła. Staramy sie aby f(n) było jak najmniejsze
int* g_wartosc;				// wskaźnik przechowujacy  wartość funkcji g(n) przechowującej ilość sprawdzonych już wierzchołków  
int* h_wartosc;				// wskaźnik przechowujacy  wartość funkcji heurystycznej h(n) której wzór został określony w instrukcji do projektu 
int* poprzednik;			// wskaźnik na tablicę poprzedników przechowujacą numer wierzchołka poprzedniego elementy

// zmienne do wygenerowania i sprawdzenia wszsytkich mozliwych ruchów skoczka z danego pola 
int dx[8] = { 1, 1, 2, 2, -1, -1, -2, -2 };
int dy[8] = { 2, -2, 1, -1, 2, -2, 1, -1 };

//Tworzenie grafu przechowującego identyfikatry sąsiadów dla określonego wierzchołka 
vector<int> graf[WIERZCHOLKI];



// struktura listy 
struct lista
{
public:
	lista *next; // wskaźnik na kolejny element listy
	int K;		// numer wierzchołka 
};

// implementacja stosu wraz z funkcjami 
class stos
{
private:
	lista *S;
public:
	// konstruktor
	stos()
	{
		S = NULL;
	}
	// destruktor
	~stos()
	{
		while (S)  pop();
	}
	// sprawdzenie czy pusty 
	bool empty()
	{
		return !S;
	}
	// zwaracanie watrości z góry stosu 
	int top()
	{
		return S->K;
	}
	// dodawanie elementu na stos
	void push(int to)
	{
		lista *tmp = new lista;
		tmp->K = to;
		tmp->next = S;
		S = tmp;
	}
	// usuwanie górnego elementu ze stosu 
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





// funkcja wyświetlająca graf
void wyswietlGraf()
{
	cout << "GRAF: " << endl;
	for (int i = 0; i < WIERZCHOLKI; i++)
	{

		cout << i << ": ";
		for (int k = 0; k < graf[i].size(); k++)
		{
			cout << graf[i][k] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

// tworzenie grafu
void zbudujGraf()
{
	// określenie współrzędnych króla na planszy
	int x_krola, y_krola, x_wieży, y_wieży;
	x_krola = 1;
	y_krola = 2;
	x_wieży = 0;
	y_wieży = 0;
	// dla każdego pola sprawdzamy możliwe ruchy 
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			for (int k = 0; k < 8; k++) // sprawdzenie wszystkich 8 ruchów 
			{
				int nowy_x = i + dy[k];
				int nowy_y = j + dx[k];
				if (nowy_x >= 0 && nowy_x <= 4 && nowy_y >= 0 && nowy_y <= 4)// musi stać na planszy 
				{
					if ((nowy_y != y_wieży && nowy_x != x_wieży) || (nowy_y == y_wieży && nowy_x == x_wieży))  // nie może stać w tej samej kolumnie ani wierszu co wieża ale może ją zbić
					{
						graf[tab[i][j]].push_back(tab[nowy_x][nowy_y]); // jeśli wszsytkie warunki spełnione dodajemy do grafu
					}
				}
			}
		}
	}

	//stworzenie tablic dla funkcji okrteślonych w instrukcji oraz tablicy poprzedników 
	f_wartosc = new int[WIERZCHOLKI];
	g_wartosc = new int[WIERZCHOLKI];
	h_wartosc = new int[WIERZCHOLKI];
	poprzednik = new int[WIERZCHOLKI];

	//określenie ich wartości funkcji dla każdego wierzchołka oraz przydzielenie współrzędnych do tablicy poprzedników 
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			h_wartosc[tab[i][j]] = abs(x_krola - i) + abs(y_krola - j);
			g_wartosc[tab[i][j]] = INF;
			f_wartosc[tab[i][j]] = INF;
			poprzednik[tab[i][j]] = tab[i][j];
		}
	}
}

// usuwanie wszystkich dynamicznych zmiennych
void usunGraf()

{

	delete[] f_wartosc;
	delete[] g_wartosc;
	delete[] h_wartosc;
	delete[] poprzednik;
}

// funkcja wybierajaca pare której wartość funkcji f jest minimalna
int wybierzNajmniejsza(set<int> & nieodwiedzone)
{
	int najmniejsza;
	int najmniejsza_wartosc = INF * 2;
	for (auto iter = nieodwiedzone.begin(); iter != nieodwiedzone.end(); iter++)
	{
		int wierzcholek = *iter;
		if (f_wartosc[wierzcholek] < najmniejsza_wartosc)
		{
			najmniejsza_wartosc = f_wartosc[wierzcholek];
			najmniejsza = wierzcholek;
		}
	}
	return najmniejsza;
}


// funkcja odbudowujaca ścieżkę przejscia do danego wierzchołka
void odbudujSciezke(int obecny)
{
	vector<int> sciezka;
	while (poprzednik[obecny] != obecny)
	{
		sciezka.push_back(obecny);
		obecny = poprzednik[obecny];
	}
	sciezka.push_back(obecny);
	// jeśli algorytm A* znalazł ścieżkę wyświetlamy ją
	cout << "Sciezka A*: " << endl;
	for (int i = sciezka.size() - 1; i >= 0; i--)
	{

		cout << sciezka.size() - i << "  Wierzcholek nr:" << sciezka[i] << " f(n)=" << f_wartosc[sciezka[i]] << " h(n)=" << h_wartosc[sciezka[i]] << " g(n)=" << g_wartosc[sciezka[i]] << endl;

	}
	cout << endl;
	return;
}

// główna funkcja dla algorytmu A*
void AStar(int start, int cel)
{
	// określenie ilości wykonanych kroków w przeszukiwaniu  na 0 bo jeszcze się nie ruszyliśmy 
	int niepewnaWartoscG = 0;

	set<int> przejrzane; // inicjowanie zbioru elementów już przejrzanych
	set<int> nieodwiedzone; // inicjowanie zbioru elementów jeszcze nieodwiedzonych

	// dodajemy poczatkowy do zbioru nieodwiedzonych 
	nieodwiedzone.insert(start);
	g_wartosc[start] = 0;
	f_wartosc[start] = g_wartosc[start] + h_wartosc[start];

	// resetowanie wartości w tablicy poprzedników 
	for (int i = 0; i < WIERZCHOLKI; i++)
		poprzednik[i] = i;
	// dopuki nie odwiedzimy wszystich 
	while (!nieodwiedzone.empty())
	{

		int obecny = wybierzNajmniejsza(nieodwiedzone);

		//jeśli znajdziemy wierzchołek docelowy to koniec
		if (obecny == cel)
		{
			odbudujSciezke(cel);
			return;

		}
		// aktualny wierzchołek dodajemy do przejrzanych i zabieramy z nieodwiedzonych 
		nieodwiedzone.erase(obecny);
		przejrzane.insert(obecny);
		// sprawdzamy wszystkich sąsiadów 
		for (int i = 0; i < (graf[obecny].size()); i++)
		{
			int sprawdzany = graf[obecny][i];
			//sprawdzamy czy jest w liście przejrzanych 
			if (przejrzane.find(sprawdzany) != przejrzane.end())
			{
				continue;
			}
			niepewnaWartoscG = niepewnaWartoscG + 1; // za każdym sprawdzeniem zwiększamy licznik
			bool czyNiepewnaLepsza = false;
			// sprawdzamy czy sprawdzany jest w liscie nieodwiedzonych
			if (nieodwiedzone.find(sprawdzany) == nieodwiedzone.end())
			{
				nieodwiedzone.insert(sprawdzany);
				czyNiepewnaLepsza = true;
			}
			// jeśli warość wywyłań jest mniejsza od danej funkcji g(n) to uznajemy ze niepewna jest lepsza 
			else if (niepewnaWartoscG < g_wartosc[sprawdzany])
			{
				czyNiepewnaLepsza = true;
			}
			//jeśli niepewna jest lepsza nadpisujemy wartości i dodajemy obecny element do tablicy poprzedników 
			if (czyNiepewnaLepsza)
			{
				poprzednik[sprawdzany] = obecny;
				g_wartosc[sprawdzany] = niepewnaWartoscG;
				f_wartosc[sprawdzany] = g_wartosc[sprawdzany] + h_wartosc[sprawdzany];
			}
		}
	}
	cout << "Nie ma sciezki" << endl << endl;
	return;
}



// przeszukiwanie w głąb grafu 
void DFS(int poczatkowy, int koncowy)
{
	stos S, W; // stos sprawdzajacy
	bool * odwiedzone, znaleziono; // tablica odwiedzonych pól i zmienna okeslajaca czy znaleziono szukane pole
	int  v, u; // zmienne  

	odwiedzone = new bool[WIERZCHOLKI]; // stworzenie tablicy elementów odwiedzonych
	for (int i = 0; i < WIERZCHOLKI; i++)   // Tablicę odwiedzonych zerujemy
		odwiedzone[i] = false;

	// tworzymy tablice w której określimy który wierzchołek z którym "został połaczony scieżką" 
	//Poprzednicy = new int[WIERZCHOLKI];
	// określamy pierwszy wierzchołęk do którego poszliśmy czyli wierzcholek startowy 
	poprzednik[poczatkowy] = -1;
	// dodanie pierwszego elementu na stos
	S.push(poczatkowy);
	// pierwszy element jest odwiedzony 
	odwiedzone[poczatkowy] = true;
	//	pierwszy element nie jest szukanym 
	znaleziono = false;

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
		for (int i = 0; i < (graf[v].size()); i++)
		{
			u = graf[v][i];
			if (!odwiedzone[u])
			{
				poprzednik[u] = v;
				S.push(u);
				odwiedzone[u] = true;
			}
		}

	}
	// jeśli nie udało się znaleźć ścieżki 
	if (!znaleziono) cout << "Nie ma sciezki" << endl << endl;

	// jesli udało sie znaleźć ścieżke wypisujemy wynik 
	else
	{
		// dopuki są poprzednicy
		while (v > -1)
		{
			W.push(v); // zapisujemy na stos
			v = poprzednik[v]; // przechodzimy na poprzednika
		}
		// wyświetlenie ścieżki w kolejnosci przechodzenia
		cout << "Sciezka DFS: ";
		while (!W.empty())
		{
			cout << W.top() << "  ";
			W.pop();

		}
		cout << endl << endl << endl;
	}
	// usuwanie tablicy dynamicznej 
	delete[] odwiedzone;
}

/*********************************************************************************************************************************************************************/
/*                                                                     Głowna część programu                                                                         */
/*                                                                                                                                                                   */
/*********************************************************************************************************************************************************************/
int main()
{

	int pozycjaKrola = 7; // określenie pozycji króla
	int pozycjaSkoczka = 22; // określenie pozycji skocznka

	zbudujGraf(); // tworzenie grafu uzywanego przez poniższe algorytmy

	//wyswietlGraf(); // wyświetlanie stworzonego grafu

	// szukanie ścieżki DFS
	DFS(pozycjaSkoczka, pozycjaKrola);

	// szukanie ścieżki A*
	AStar(pozycjaSkoczka, pozycjaKrola);

	// usuwanie dynamicznych struktur
	usunGraf();

	return 0;
}