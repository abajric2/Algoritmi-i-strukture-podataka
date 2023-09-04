#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    for(int i = 0; i < vel - 1; i++) {
        Tip min = niz[i];
        int pmin = i;
        for(int j = i+1; j < vel; j++) {
            if(niz[j] < min) {
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}

template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for(int i = vel - 1; i >= 1; i--) {
        for(int j = 1; j <= i; j++) {
            if(niz[j - 1] > niz[j]) {
                Tip temp = niz[j - 1];
                niz[j - 1] = niz[j];
                niz[j] = temp;
            }
        }
    }
}
template <typename Tip>
void merge(Tip *A, int l, int p, int q, int u) {
    int i = 0, j = q - l, k = l;
    Tip *B = new Tip[u-l+1];
    for(int m = 0; m <= u - l; m++) B[m] = A[l + m];
    while(i <= p - l && j <= u - l) {
        if (B[i] < B[j]) {
            A[k] = B[i];
            i++;
        }
        else {
            A[k] = B[j];
            j++;
        }
        k++;
    }
    while (i <= p - l) {
        A[k] = B[i];
        k++; i++;
    }
    while (j <= u - l) {
        A[k] = B[j];
        k++; j++;
    }
    delete [] B;
}

template <typename Tip>
void merge_sort(Tip* niz, int vel, int poc = 0) {
    if(vel - 1 > poc) {
        int p = (poc + vel - 2) / 2;
        int q = p + 1;
        merge_sort(niz, p + 1, poc);
        merge_sort(niz, vel, q);
        merge(niz, poc, p, q, vel - 1);
    }
}

template <typename Tip>
int particija (Tip *niz, int prvi, int zadnji) {
    Tip pivot = niz[prvi];
    int p = prvi + 1;
    while(p <= zadnji && niz[p] < pivot) p++;
    for(int i = p + 1; i <=zadnji; i++) {
        if(niz[i] < pivot) {
            Tip temp = niz[i];
            niz[i] = niz[p];
            niz[p] = temp;
            p++;
        }
    }
    Tip temp = niz[prvi];
    niz[prvi] = niz[p - 1];
    niz[p - 1] = temp;
    return p - 1;
}

template <typename Tip>
void quick_sort(Tip* niz, int vel, int poc = 0) {
    if(poc < vel - 1) {
        int j = particija (niz, poc, vel - 1);
        quick_sort(niz, j, poc);
        quick_sort(niz, vel, j + 1);
    }
}


void ucitaj(string filename, int*& niz, int &vel) {
    std::ifstream datoteka (filename, std::ios::in);
    vel = 0;
    int broj;
    while(datoteka >> broj) vel ++;
    if(datoteka.eof()) {
        datoteka.clear();
        datoteka.close();
    }
    niz = new int[vel];
    datoteka.open(filename, std::ios::in);
    for(int i = 0; i < vel; i++) datoteka >> niz[i];
    if(datoteka.eof()) {
        datoteka.clear();
        datoteka.close();
    }
}

void generisi(string filename, int vel) {
    std::ofstream datoteka (filename, std::ios::out);
    for(int i = 0; i < vel; i++) {
	    int random = rand();
        datoteka << random << " ";
    }
    datoteka.close();
}

void zapisi_u_datoteku (string filename, int *niz, int vel) {
    std::ofstream datoteka (filename, std::ios::out);
    for(int i = 0; i < vel; i++) datoteka << niz[i] << " ";
}
void sortiraj(string filename) {
    int *niz, vel;
    ucitaj(filename, niz, vel);
    cout << "Unesite koji algoritam sortiranja zelite koristiti (1 - bubble sort, 2 - selection sort, 3 - quick sort, 4 - merge sort)" << endl;
    int algoritam;
    cin >> algoritam;
    switch(algoritam) {
        case 1:
        {
            clock_t vrijeme1 = clock();
            bubble_sort(niz, vel);
            clock_t vrijeme2 = clock();
            int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
            break;
        }
        case 2:
        {
            clock_t vrijeme1 = clock();
            selection_sort(niz, vel);
            clock_t vrijeme2 = clock();
            int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
            break;
        }
        case 3:
        {
            clock_t vrijeme1 = clock();
            quick_sort(niz, vel);
            clock_t vrijeme2 = clock();
            int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
            break;
        }
        case 4:
        {
            clock_t vrijeme1 = clock();
            merge_sort(niz, vel);
            clock_t vrijeme2 = clock();
            int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
            break;
        }
        default:
        cout << "Pogresan unos" << endl;
    }
    bool sortiran = true;
    for(int i = 1; i < vel; i++) 
        if (niz[i] < niz[i - 1]) {
            sortiran = false; 
            break;
        }
    if (sortiran) cout << "Niz je sortiran" << endl;
    else cout << "Niz nije sortiran" << endl;
    zapisi_u_datoteku("IZLAZ.TXT", niz, vel);
}

int main() {
    generisi("RANDOMNIZ.TXT", 10000);
    sortiraj("RANDOMNIZ.TXT");
    return 0;
}