#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <functional>

using namespace std;

struct Zadanie {
        int kolejnosc;//kolejnosc zadania
        int r;//czas przygotowania
        int p;//czas na maszynie
        int q;//czas stygniecia
        int poczatkowa;
};
Zadanie TabZadan[100]; //deklaracja tablicy zadan
int kolejnosc[100];

unsigned int max(unsigned int x, unsigned int y){ return (x<=y)?y:x; }
unsigned int min(unsigned int x, unsigned int y){ return (x<=y)?x:y; }
int cmax(unsigned int rozmiar){
    unsigned int i, t=0,u=0;
    for (i=1;i<=rozmiar;i++){
        for(int j = 0; j < rozmiar; j++) {
            if(TabZadan[j].kolejnosc == i) {
                t=max(t,TabZadan[j].r)+TabZadan[j].p;
                u=max(u,t+TabZadan[j].q);
                break;
            }
        }
    }
    return u;
}

istream& operator>> (istream& wejscie, Zadanie& zad){
    zad.kolejnosc = -1;
    wejscie >> zad.r >> zad.p >> zad.q;
    return wejscie;
}
ostream& operator<< (ostream& wyjscie, const Zadanie& zad){
    cout << zad.r << " - " << zad.p << " - " << zad.q << endl; 
    return wyjscie;
}    

struct {
    bool operator()(Zadanie a, Zadanie b) const
    {   
        return a.q > b.q;
    }   
} customSort;


void sortuj(int LiczbaZadan) { 

    std::sort(TabZadan, TabZadan + LiczbaZadan, customSort);
}

void algorytmik(int LiczbaZadan) {
    int iIndexZadan = 0;
    bool czySkonczone = false;
    for(int t = 0; !czySkonczone; t+=1) {
        int minP = 0;
        for(int i = 0; i < LiczbaZadan; i++) {
            if(TabZadan[i].kolejnosc == -1 && TabZadan[i].r <= t) {
                TabZadan[i].kolejnosc = iIndexZadan + 1;
                t += TabZadan[i].p;
                iIndexZadan++;
                break;
            }
        }
        if(iIndexZadan == LiczbaZadan) {
            czySkonczone = true;
        }
    }
}


void algorytmikPodDwojeczke(int LiczbaZadan) {
    int iIndexZadan = 0;
    bool czySkonczone = false;
    for(int t = 0; !czySkonczone; t+=1) {
        int minP = 0;
        for(int i = 0; i < LiczbaZadan; i++) {
            if(TabZadan[i].kolejnosc == -1 && TabZadan[i].r <= t) {
                minP = TabZadan[i].p;
            }
        }
        for(int i = 0; i < LiczbaZadan; i++) {
            if(TabZadan[i].kolejnosc == -1 && TabZadan[i].r <= t + minP) {
                TabZadan[i].kolejnosc = iIndexZadan + 1;
                t += TabZadan[i].p;
                iIndexZadan++;
                break;
            }
        }
        if(iIndexZadan == LiczbaZadan) {
            czySkonczone = true;
        }
    }
}

void CzytajZPliku(int& LiczbaZadan) {
    bool xFirst=1;
    string line, filename;
    cout<<"Podaj nazwe pliku z danymi: ";
    cin>>filename;
    cout<<"\n";
    int index = 0;

    ifstream myfile ("dane."+filename);
    if (myfile.is_open()){
        while (getline (myfile,line) ){
            istringstream dane(line);
            if (xFirst==1){
                dane >> LiczbaZadan;
                cout << "Liczba zadan: " << LiczbaZadan<<"\n";
                xFirst = 0;
            }else{
                dane >> TabZadan[index]; //wgranie danych do tablicy struktur
                TabZadan[index].poczatkowa = index;
                TabZadan[index].kolejnosc = -1;
                cout << TabZadan[index]; //wyswietlenie danych
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

void Wyswietl(int LiczbaZadan, int cm){ 
    // wyswietlenie kolejnosci:
    cout << endl << "Wyliczona kolejnosc: ";
    for (unsigned int i = 0; i < LiczbaZadan + 1; i++){
        //cout << TabZadan[i];
        for(int j = 0 ; j < LiczbaZadan; j++) {
            if(TabZadan[j].kolejnosc == i)
                cout << TabZadan[j].poczatkowa + 1 << " ";
        }
    }
    cout << endl << "Obliczanie Cmax: ";
    cout << cm << endl;
}

int main(){
    for(int i = 0; i < 4; i++) {
        int LiczbaZadan = 0;
        CzytajZPliku(LiczbaZadan);
        // obsluga algorytmu, czyli petle zamieniajace kolejnosc dzialan
    
        sortuj(LiczbaZadan);
        if(i == 1)
            algorytmikPodDwojeczke(LiczbaZadan);    
        else 
            algorytmik(LiczbaZadan);
        int cm = cmax(LiczbaZadan);

        Wyswietl(LiczbaZadan, cm);

        ZapisDoPliku(cm);
    }
    return 0;
}