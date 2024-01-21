
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
	int tura = 2;
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
	int zycia;
	int przedmioty;
};
int main() {
	setlocale(LC_CTYPE, "Polish");
	int wybor;
	cout << "Wprowadź '1' aby rozpocząć grę" << endl;
	cout << "Wprowadź '2' aby zakończyć grę" << endl;
	while (true) {
		if (cin >> wybor) {
			if (wybor == 1) {
				cout << "Wybrano rozpoczecie gry\n";
				break; // Zakończenie pętli, program idzie dalej
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
	Gra ruletka;
	ruletka.zaladuj_bron();
	for (int i = 0; i < ruletka.ilosc_naboi[2]; i++) {
		if (ruletka.naboje[i] == 1) cout << "prawdziwy" << endl;
		else cout << "falszywy" << endl;
	}
	ruletka.rozladuj_bron();
}