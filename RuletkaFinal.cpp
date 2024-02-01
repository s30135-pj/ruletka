#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>

using namespace std;

// Deklaracja wstępna struktury Gracz
struct Gracz;

struct Gra {
  const int zycia[4] = { 0, 1, 2, 3 };
  const int naboje[4] = { 0, 4, 12, 16 };
  const double prawdziweNaboje[4] = { 0, 0.25, 0.33, 0.375 };

  int runda = 1;
  int indeksNaboju = 0;
  int* bron = nullptr;
  int* przedmioty = nullptr;

  void zaladujBron() {
    double calkowiteNaboje = naboje[runda];
    double prawdziweRatio = prawdziweNaboje[runda];
    indeksNaboju = 0;
    if (bron != nullptr) rozladujBron();
    bron = new int[(int)calkowiteNaboje];

    int iloscPrawdziwychNaboi = ceil(calkowiteNaboje * prawdziweRatio);
    int iloscFalszywychNaboi = calkowiteNaboje - iloscPrawdziwychNaboi;

    cout << "Prawdziwe Naboje: " << iloscPrawdziwychNaboi << endl;
    cout << "Falszywe Naboje: " << iloscFalszywychNaboi << endl;

    srand((unsigned)time(0));
    for (int i = 0; i < calkowiteNaboje; i++) {
      int rzut = rand() % 100;
      if ((rzut < prawdziweRatio * 100 && iloscPrawdziwychNaboi > 0) || iloscFalszywychNaboi == 0) {
        bron[i] = 1;
        iloscPrawdziwychNaboi--;
      }
      else {
        bron[i] = 0;
        iloscFalszywychNaboi--;
      }
    }
  }

  void rozladujBron() {
    delete[] bron;
    bron = nullptr;
  }

  void przygotujPrzedmioty() {
    delete[] przedmioty;
    przedmioty = new int[runda]; // Przygotowanie przedmiotów Kula szczęścia
    for (int i = 0; i < runda; ++i) {
      przedmioty[i] = i + 1; // Ustawienie liczby przedmiotów Kula szczęścia
    }
  }

  void uzyjPrzedmiotu(Gracz* gracz); // Deklaracja wstępna funkcji uzyjPrzedmiotu

  ~Gra() {
    delete[] bron;
    delete[] przedmioty;
  }
};

struct Gracz {
  string nazwa;
  int zycia;
  int przedmioty;

  void strzel(Gracz* cel, int naboj) {
    if (cel != this) {
      cout << nazwa << " strzela do " << cel->nazwa << endl;
    }
    else {
      cout << nazwa << " strzela do siebie" << endl;
    }

    if (naboj) {
      cel->zycia--;
      cout << "Bron wystrzelila, " << cel->nazwa << " pozostale zycia: " << cel->zycia << endl;
    }
    else {
      cout << "Pusty naboj, strzal nieudany" << endl;
    }
  }
};

int turaKomputera(Gracz& komputer, Gracz& czlowiek, Gra& ruletka, int runda) {
  if (komputer.zycia > 0 && ruletka.indeksNaboju <= ruletka.naboje[runda]) {
    int losowyWybor = rand() % 2 + 1;
    if (losowyWybor == 1) {
      komputer.strzel(&czlowiek, ruletka.bron[ruletka.indeksNaboju]);
      ruletka.indeksNaboju++;
      if (czlowiek.zycia <= 0) {
        cout << "Gracz " << czlowiek.nazwa << " przegrywa runde " << runda << ". Koniec gry.\n";
        return 0; // Zakończenie gry
      }
      return 1; // Kontynuacja gry, tura gracza
    }
    else {
      komputer.strzel(&komputer, ruletka.bron[ruletka.indeksNaboju]);
      ruletka.indeksNaboju++;
      if (komputer.zycia <= 0) {
        cout << "Gracz " << czlowiek.nazwa << " wygrywa runde " << runda << ".\n";
        return 2; // Koniec rundy, nie gry
      }
      // W przypadku pustego strzału do siebie, komputer kontynuuje turę
      if (ruletka.bron[ruletka.indeksNaboju] == 0)
        return turaKomputera(komputer, czlowiek, ruletka, runda);
    }
  }
  return 1; // Gra toczy się dalej
}


void Gra::uzyjPrzedmiotu(Gracz* gracz) {
  if (przedmioty[runda - 1] > 0) { // Sprawdzenie, czy gracz ma przedmiot Kula szczęścia w bieżącej turze
    cout << "Kula szczescia! ";
    if (bron[indeksNaboju] == 1) {
      cout << "Aktualny naboj to prawdziwy!" << endl;
    }
    else {
      cout << "Aktualny naboj to fałszywy!" << endl;
    }
    przedmioty[runda - 1]--; // Zmniejszenie liczby przedmiotów Kula szczęścia po użyciu
  }
  else {
    cout << "Nie masz juz Kuli szczescia w tej turze." << endl;
  }
}

void graj() {
  Gra ruletka;
  Gracz komputer = { "Komputer" };
  Gracz czlowiek = { "Czlowiek" };

  for (int runda = 1; runda <= 3; ++runda) {
    komputer.zycia = ruletka.zycia[runda];
    czlowiek.zycia = ruletka.zycia[runda];
    czlowiek.przedmioty = runda; // Zwiększenie liczby przedmiotów zgodnie z rundą
    ruletka.runda = runda;
    cout << "Runda " << ruletka.runda << endl;
    ruletka.zaladujBron();
    ruletka.przygotujPrzedmioty();// Przygotowanie przedmiotów w każdej turze

    int wybor;

    for (int i = 0; i < ruletka.naboje[runda]; i++) {
      cout << "Wybierz akcje:\n";
      cout << "1. Strzel do komputera\n";
      cout << "2. Strzel do siebie\n";
      cout << "3. Uzyj Kuli szczescia\n";

      if (cin >> wybor) {
        if (wybor == 1) {
          czlowiek.strzel(&komputer, ruletka.bron[ruletka.indeksNaboju]);
          ruletka.indeksNaboju++;
          if (komputer.zycia <= 0) {
            cout << czlowiek.nazwa << " wygrywa runde " << runda << "!\n";
            break;
          }
          else {
            int wynikTury = turaKomputera(komputer, czlowiek, ruletka, runda);
            if (wynikTury == 0) {
              return; // Koniec gry
            }
            else if (wynikTury == 2) {
              break; // Koniec rundy, przejście do następnej
            }
          }
        }
        else if (wybor == 2) {
          czlowiek.strzel(&czlowiek, ruletka.bron[ruletka.indeksNaboju]);
          ruletka.indeksNaboju++;
          if (ruletka.bron[ruletka.indeksNaboju] == 1) {
            cout << czlowiek.nazwa << " strzelil prawdziwym nabojem do siebie. Pozostale zycia: "
              << czlowiek.zycia << "\n";
            if (czlowiek.zycia <= 0) {
              cout << czlowiek.nazwa << " przegrywa runde " << runda << ". Koniec gry.\n";
              return;
            }
            else {
              int wynikTury = turaKomputera(komputer, czlowiek, ruletka, runda);
              if (wynikTury == 0) {
                return; // Koniec gry
              }
              else if (wynikTury == 2) {
                break; // Koniec rundy, przejście do następnej
              }
            }
          }
          else {
            cout << czlowiek.nazwa
              << " strzelil falszywym nabojem do siebie. Mozna podjac decyzje ponownie.\n";
            continue;
          }
        }
        else if (wybor == 3) {
          ruletka.uzyjPrzedmiotu(&czlowiek); // Użycie przedmiotu Kula szczęścia
          i--; // Runda pozostaje ta sama
          continue;
        }
        else {
          cout << "Wprowadz poprawne dane\n";
          i--;
        }

        if (komputer.zycia <= 0) {
          cout << czlowiek.nazwa << " wygrywa runde " << runda << "!\n";
          break;
        }

      }
      else {
        cout << "Wprowadz poprawne dane\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        i--;
      }
    }

    ruletka.rozladujBron();
  }

  cout << "Koniec gry. " << czlowiek.nazwa << " wygrywa!\n";
}

int main() {
  setlocale(LC_CTYPE, "Polish");
  int wybor;
  while (true) {
    cout << "Wprowadz '1' aby rozpoczac gre" << endl;
    cout << "Wprowadz '2' aby zakonczyc gre" << endl;
    if (cin >> wybor) {
      if (wybor == 1) {
        cout << "Wybrano rozpoczecie gry\n";
        graj();
      }
      else if (wybor == 2) {
        cout << "Wybrano zakonczenie gry\n";
        return 0;
      }
      else {
        cout << "Wprowadz poprawne dane\n";
      }
    }
    else {
      cout << "Wprowadz poprawne dane\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
}