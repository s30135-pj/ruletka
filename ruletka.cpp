
#include <iostream>

using namespace std;
// Jakub Olchowy, Szymon Osiecki, Władysław Farat - projekt nr. 5
//Struktura odpowiadajaca za podstawowe funkcje gry, trzymanie danych o grze
struct Gra {
	//dane gry
	const int zycia[4] = { 0, 1, 2 ,3 };
	const double ilosc_naboi[4] = { 0, 4, 6 ,8 };
	const double prawdziwe[4] = { 0, 50, 66, 75 };
	//dane obecnej instancji gry
	int tura = 1;
	int nr_naboju = 1;
	int* naboje = nullptr;
	//funkcja odpowiadajaca za losowe zaladowanie broni
	void zaladuj_bron() {
		//zainicjalizowanie tabeli
		double ile = ilosc_naboi[tura];
		double szansa = prawdziwe[tura];
		if (naboje != nullptr) rozladuj_bron();
		naboje = new int[ile];
		//obliczenie ile powinno byc naboi prawdziwych a ile falszywych
		int prawdziwe = ceil((ile * szansa) / 100);
		int falszywe = ile - prawdziwe;
		cout << "Naboi prawdziwych " << prawdziwe << endl;
		cout << "Naboi falszywych " << falszywe << endl;
		//inicjalizacja randa
		srand((unsigned)time(0));
		//wypelnienie tabeli wartosciami
		for (int i = 0; i < ile; i++) {
			int rzut = rand() % 100;
			if ((rzut < szansa && prawdziwe > 0) || falszywe == 0) {
				naboje[i] = 1;
				prawdziwe--;
			}
			else {
				naboje[i] = 0;
				falszywe--;
			}
		}
	}
	void rozladuj_bron() {
		delete[] naboje;
		naboje = nullptr;
	}
};
//struktura odpowiadajaca za gracza
struct Gracz {
	string nazwa;
	int zycia;
	int przedmioty;
	//funkcja do strzelania
	void strzel(Gracz *cel, int naboj) {
		//sprawdzenie czy cel nie jest graczem i wyswietlenie odpowiedniej wiadomosci
		if (cel != this) {
			cout << nazwa << " strzela do gracza " << cel->nazwa << endl;
		}
		else {
			cout << nazwa << " strzela do siebie" << endl;
		}
		//sprawdzenie czy nabój jest prawdziwy czy nie
		if (naboj) {
			cel->zycia--;
			cout << "Rewolwer wystrzelił, pozostałe życia gracza " << cel->nazwa << " to " << cel->zycia << endl;
		}
		else {
			cout << "Pusty nabój, strzał nieudany" << endl;
		}
	}
};

//funkcja gry
void graj() {
	//utworzenie gracza i gry
	Gra ruletka;
	Gracz komputer = {"Komputer"};
	Gracz czlowiek = {"Gracz"};
	//to powinno być w pętli, z odpowiednimi wiadomościami i logiką zachowania komputera
	komputer.zycia = ruletka.zycia[ruletka.tura];
	czlowiek.zycia = ruletka.zycia[ruletka.tura];
	czlowiek.przedmioty = ruletka.zycia[ruletka.tura];
	ruletka.zaladuj_bron();
	//tutaj zaprezentowanie jak działa funkcja, do strzelania
	for (int i = 0; i < ruletka.ilosc_naboi[ruletka.tura]; i++) {
		if (i % 2) komputer.strzel(&czlowiek, ruletka.naboje[i]);
		else czlowiek.strzel(&komputer, ruletka.naboje[i]);
	}
	ruletka.rozladuj_bron();
}
int main() {
	setlocale(LC_CTYPE, "Polish");
	int wybor;
	while (true) {
		cout << "Wprowadź '1' aby rozpocząć grę" << endl;
		cout << "Wprowadź '2' aby zakończyć grę" << endl;
		if (cin >> wybor) {
			if (wybor == 1) {
				cout << "Wybrano rozpoczecie gry\n";
				graj();
			}
			else if (wybor == 2) {
				cout << "Wybrano zakonczenie gry\n";
				return 0; // Zakończenie programu
			}
			else {
				cout << "Wprowadz poprawne dane\n";
			}
		}
		else {
			cout << "Wprowadz poprawne dane\n";
			cin.clear(); // Wyczyszczenie flagi błędu
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorowanie niepoprawnych danych
		}
	}

}