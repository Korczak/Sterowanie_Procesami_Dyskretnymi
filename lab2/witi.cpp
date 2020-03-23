#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <functional>
#include <map>
#include <ctime>

#define MaskymalnaIloscZadan 100
using namespace std;

clock_t START_TIMER;

clock_t tic()
{
    return START_TIMER = clock();
}

void toc(clock_t start = START_TIMER)
{
    cout
        << "Elapsed time: "
        << (clock() - start) / (double)CLOCKS_PER_SEC << "s"
        << endl;
}


struct Zadanie {
        int czas;//czas trwania
        int waga;//waga
        int termin;//termin zakonczenia
};
Zadanie TabZadan[MaskymalnaIloscZadan]; //deklaracja tablicy zadan
int kolejnosc[MaskymalnaIloscZadan];

struct Rozwiazanie {
        Rozwiazanie(int k, int i, string kZ) {
            koszt = k;
            index = i;
            kolejnoscZadan = kZ;
        }
        int koszt;//czas trwania
        int index;//waga
        string kolejnoscZadan;
};

struct RozwiazaneZadanie {
        RozwiazaneZadanie(int k, int i, string z, string kZ) {
            koszt = k;
            minIndex = i;
            zadania = z;
            kolejnoscZadan = kZ;
        }
        int koszt;
        int minIndex;
        string zadania;
        string kolejnoscZadan;
};
map<string, RozwiazaneZadanie> WykonaneZadania;
int ilosc = 0;

istream& operator>> (istream& wejscie, Zadanie& zad){
    wejscie >> zad.czas >> zad.waga >> zad.termin;
    return wejscie;
}
ostream& operator<< (ostream& wyjscie, const Zadanie& zad){
    cout << zad.czas << " - " << zad.waga << " - " << zad.termin << endl; 
    return wyjscie;
}    

void CzytajZPliku(int& LiczbaZadan, string filename) {
    bool xFirst=1;
    string line;
    cout<<"Podaj nazwe pliku z danymi (wpisz:`1`,`2`,`3`,`4`,`5`,`6`,`7`): ";
    cin>>filename;
    cout<<"\n";
    int index = 0;

    ifstream myfile ("dane."+filename);
    if (myfile.is_open()){
        while (getline (myfile,line) ){
            istringstream dane(line);
            if (xFirst==1){
                dane >> LiczbaZadan;
                //cout << "Liczba zadan: " << LiczbaZadan <<"\n";
                xFirst = 0;
            }else{
                dane >> TabZadan[index]; //wgranie danych do tablicy struktur
                //cout << TabZadan[index]; //wyswietlenie danych
                index++;
            }
        }   
        cout << "\n Odczytano plik z danymi.\n";
        myfile.close();
    }else cout << "Unable to open file"; 
}

void ZapisDoPliku(int cm) {
    string nazwaPliku;
    
    cout<<"Podaj nazwe pliku do zapisu danych: ";
    cin >> nazwaPliku;
    // zapis do pliku
    FILE *fp = fopen(nazwaPliku.c_str(),"w");
    if (fp){
        fprintf(fp,"%d\n", cm);
        fclose(fp);
        cout << "Plik zostal zapisany.\n";
    }else {
        cout << "Nie moge otworzyc pliku " << nazwaPliku << endl;
    }
}

int koszt(int t, Zadanie zadanie) {
    if(t < zadanie.termin)
        return 0;
    return (t - zadanie.termin) * zadanie.waga;
}

bool CzyZadanieRozwiazano(string problem) {
    if(WykonaneZadania.find(problem) == WykonaneZadania.end()) {
        return false;
    }
    return true;
}

RozwiazaneZadanie WezRozwiazanieZadania(string problem) {
    map<string, RozwiazaneZadanie>::iterator it;
    it = WykonaneZadania.find(problem);
    return it->second;
}

Rozwiazanie AlgorytmPD(int t, string aktualneZadania, int iloscPozostalych, int liczbaZadan) {
    if(iloscPozostalych == 0) {
        Rozwiazanie rozw(0, 0, "");
        return rozw;
    }

    if(CzyZadanieRozwiazano(aktualneZadania)) {
        RozwiazaneZadanie rozw = WezRozwiazanieZadania(aktualneZadania);
        Rozwiazanie rozwiazanie(rozw.koszt, 0, rozw.kolejnoscZadan);
        ilosc++;
        return rozwiazanie;
    }
    
    int minimum = INT_MAX;
    string minimalnaKolejnosc = "";
    int minimumIndex = 0;
    for(int i = 0; i < liczbaZadan; i++) {
        if(aktualneZadania[i] == '1') { 
            aktualneZadania[i] = '0';
            Rozwiazanie rozwiazanie = AlgorytmPD(t - TabZadan[i].czas, aktualneZadania, iloscPozostalych - 1, liczbaZadan);
            aktualneZadania[i] = '1';
            int kosztAktualny = rozwiazanie.koszt + koszt(t, TabZadan[i]);
            if(kosztAktualny < minimum) {
                minimum = kosztAktualny;
                minimalnaKolejnosc = rozwiazanie.kolejnoscZadan;
                minimumIndex = i;
            }
        }
    }
    string kolejnoscZadan = minimalnaKolejnosc + " " + to_string(minimumIndex + 1);
    Rozwiazanie rozw(minimum, minimumIndex, kolejnoscZadan);
    string aktualneZadaniaRozwiazanie = aktualneZadania;
    aktualneZadaniaRozwiazanie[minimumIndex] = '0';
    RozwiazaneZadanie rozwiazaneZadanie(minimum, minimumIndex, aktualneZadaniaRozwiazanie, kolejnoscZadan);
    WykonaneZadania.insert(pair<string, RozwiazaneZadanie>(aktualneZadania, rozwiazaneZadanie));
    return rozw;
}


Rozwiazanie AlgorytmZachlanny(int t, string aktualneZadania, int iloscPozostalych, int liczbaZadan) {
    if(iloscPozostalych == 0) {
        Rozwiazanie rozw(0, 0, "");
        return rozw;
    }
    
    int minimum = INT_MAX;
    int indexMinimum = 0;
    for(int i = 0; i < liczbaZadan; i++) {
        if(aktualneZadania[i] == '1') { 
            aktualneZadania[i] = '0';
            Rozwiazanie rozwiazanie = AlgorytmZachlanny(t - TabZadan[i].czas, aktualneZadania, iloscPozostalych - 1, liczbaZadan);
            aktualneZadania[i] = '1';
            int kosztAktualny = rozwiazanie.koszt + koszt(t, TabZadan[i]);
            if(kosztAktualny < minimum) {
                minimum = kosztAktualny;
                indexMinimum = i;
            }
        }
    }
    Rozwiazanie rozw(minimum, indexMinimum, "");
    return rozw;
}


int CalkowityCzasTrwania(int iloscZadan) {
    int t = 0;
    for(int i = 0; i < iloscZadan; i++) {
        t += TabZadan[i].czas;
    }
    return t;
}

Rozwiazanie RozwiazZadanie(int t, int liczbaZadan) {
    string aktualneZadania = "";
    for (int i = 0; i < liczbaZadan; i++) {
        aktualneZadania += "1";
    }
    Rozwiazanie rozw = AlgorytmPD(t, aktualneZadania, liczbaZadan, liczbaZadan);
    return rozw;
}

int main(){
    for(int i = 11; i < 12; i++) {
        int LiczbaZadan = 0;
        CzytajZPliku(LiczbaZadan, to_string(i));
        int t = CalkowityCzasTrwania(LiczbaZadan);
        
        //tic();
        Rozwiazanie rozwiazanie = RozwiazZadanie(t, LiczbaZadan);
        //toc();
        
        cout << "Optymalny koszt: " << rozwiazanie.koszt << endl;
        cout << "Kolejnosc zadan: " << rozwiazanie.kolejnoscZadan << endl;
        cout << "Ilosc: " << ilosc << endl;
    }
    
    return 0;
}