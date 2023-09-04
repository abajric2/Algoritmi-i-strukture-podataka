#include <iostream>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
    public:
    Mapa() {}
    virtual TipVrijednosti &operator[] (const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti operator[] (const TipKljuca &kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual ~Mapa() {}
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevi, *desni, *roditelj;
        Cvor (const Cvor &c) {
            kljuc = c.kljuc; vrijednost = c.vrijednost;
            lijevi = desni = roditelj = nullptr;
        }
        Cvor(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti()) {
            kljuc = k; vrijednost = v;
            lijevi = desni = roditelj = nullptr;
        }
    };
    Cvor *korijen;
    int broj_elemenata;
    Cvor *trazi(Cvor *korijen, TipKljuca kljuc) const {
        if(korijen == nullptr || kljuc == korijen->kljuc) return korijen;
        if(kljuc < korijen->kljuc) return trazi(korijen->lijevi, kljuc);
        else return trazi(korijen->desni, kljuc);
    }
    void umetni (Cvor *&korijen, Cvor *&novi, Cvor *roditelj) {
        if(korijen == nullptr) {
            korijen = novi;
            korijen->roditelj = roditelj;
        }
        else {
            if(novi->kljuc < korijen->kljuc) umetni(korijen->lijevi, novi, korijen);
            else umetni(korijen->desni, novi, korijen);
        }
    }
    void posjeti(Cvor *cvor) {
        cvor->lijevi = nullptr; 
        cvor->desni = nullptr;
        if(cvor == korijen) korijen = nullptr;
        else {
            if(cvor->roditelj->lijevi == cvor) cvor->roditelj->lijevi = nullptr;
            else cvor->roditelj->desni = nullptr;
        }
        if(cvor != korijen) cvor->roditelj = nullptr;
        cvor->kljuc = TipKljuca(); cvor->vrijednost = TipVrijednosti();
        delete cvor;
    }    
    void obrisi_rekurzivna(Cvor *cvor) {
        if (cvor) {
        obrisi_rekurzivna(cvor->lijevi);
        obrisi_rekurzivna(cvor->desni);
        delete cvor;
        }
    } 
    void preorder (Cvor *izvor) {
        if(izvor != nullptr) {
            Cvor *za_umetnuti = new Cvor(*izvor);
            umetni(korijen, za_umetnuti, nullptr);
            preorder(izvor->lijevi);
            preorder(izvor->desni);
        }
    }
    public:
    BinStabloMapa() {
        korijen = nullptr;
        broj_elemenata = 0;
    }
    
    TipVrijednosti &operator[] (const TipKljuca &kljuc) override {
        Cvor *element = trazi(korijen, kljuc);
        if(element != nullptr) return element->vrijednost;
        Cvor *za_umetnuti = new Cvor(kljuc);
        umetni(korijen, za_umetnuti, nullptr);
        broj_elemenata ++;
        return za_umetnuti->vrijednost;
    }

    TipVrijednosti operator[] (const TipKljuca &kljuc) const override {
        Cvor *element = trazi(korijen, kljuc);
        if(element != nullptr) return element->vrijednost;
        return TipVrijednosti();
    }

    int brojElemenata() const override {
        return broj_elemenata;
    }
    void obrisi(const TipKljuca &kljuc) override {
        Cvor *pomocni = korijen;
        Cvor *roditelj = nullptr;
        while (pomocni != nullptr && kljuc != pomocni->kljuc) {
            roditelj = pomocni;
            if(kljuc < pomocni->kljuc) pomocni = pomocni->lijevi;
            else pomocni = pomocni->desni;
        }
        if (pomocni == nullptr) return;
        Cvor *m, *pm, *temp;
        if(pomocni->lijevi == nullptr) m = pomocni->desni;
        else {
            if(pomocni->desni == nullptr) m = pomocni->lijevi;
            else {
                pm = pomocni;
                m = pomocni->lijevi;
                temp = m->desni;
                while (temp != nullptr) {
                    pm = m;
                    m = temp;
                    temp = m->desni;
                }
                if(pm != pomocni) {
                    pm->desni = m->lijevi;
                    m->lijevi = pomocni->lijevi;
                }
                m->desni = pomocni->desni;
            }
        }
        if (roditelj == nullptr) korijen = m;
        else {
            if(pomocni == roditelj->lijevi) roditelj->lijevi = m;
            else roditelj->desni = m;
        }
        delete pomocni;
        broj_elemenata --;
    }
    void obrisi() override {
        obrisi_rekurzivna(korijen);
        korijen = nullptr;
        broj_elemenata = 0;
    }
    ~BinStabloMapa() {
        obrisi_rekurzivna(korijen);
        broj_elemenata = 0;
        korijen = nullptr;
    }
    BinStabloMapa(const BinStabloMapa &m);
    BinStabloMapa &operator = (const BinStabloMapa &m);
};
template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(const BinStabloMapa &m) {
    korijen = nullptr;
    preorder(m.korijen);
    broj_elemenata = m.broj_elemenata;
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti> &BinStabloMapa<TipKljuca, TipVrijednosti>::operator = (const BinStabloMapa &m) {
    if(this == &m) return *this;
    obrisi();
    preorder(m.korijen);
    broj_elemenata = m.broj_elemenata;
    return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    TipVrijednosti err = TipVrijednosti();
    std::pair<TipKljuca, TipVrijednosti> *mapa;
    int trenutna_velicina, kapacitet;
    void alocirajMapu(int vel) {
        mapa = new std::pair<TipKljuca, TipVrijednosti>[vel]{};
        kapacitet = vel; trenutna_velicina = 0;
    }
    void prosiriMapu() {
        std::pair<TipKljuca, TipVrijednosti> *pomocni = new std::pair<TipKljuca, TipVrijednosti>[kapacitet*2]{};
        for(int i = 0; i<trenutna_velicina; i++) pomocni[i] = mapa[i];
        delete[] mapa;
        mapa = pomocni; kapacitet *= 2; 
    }
    public:
    NizMapa(int vel = 100) {
        alocirajMapu(vel);
    } 
    NizMapa(const NizMapa &m);
    NizMapa &operator = (const NizMapa &m);
    int brojElemenata() const override {
        return trenutna_velicina;
    }
    void obrisi() override {
        delete [] mapa;
        alocirajMapu(100);
    }
    ~NizMapa() {
        delete [] mapa;
        kapacitet = 0; trenutna_velicina = 0;
        mapa = nullptr;
    }
    TipVrijednosti &operator[] (const TipKljuca &kljuc) override {
        for(int i = 0; i < trenutna_velicina; i++) {
            if(mapa[i].first == kljuc) return mapa[i].second;
        }
        if(trenutna_velicina>=kapacitet) prosiriMapu();
        mapa[trenutna_velicina].first = kljuc;
        mapa[trenutna_velicina].second = TipVrijednosti();
        trenutna_velicina++;
        return mapa[trenutna_velicina-1].second;
    }
    TipVrijednosti operator[] (const TipKljuca &kljuc) const override {
        for(int i = 0; i < trenutna_velicina; i++) {
            if(mapa[i].first == kljuc) return mapa[i].second;
        }
        return err;
    }
    void obrisi(const TipKljuca &kljuc) override {
        for(int i = 0; i < trenutna_velicina; i++) {
            if(mapa[i].first == kljuc) {
                for(int j = i+1; j<trenutna_velicina; j++) mapa[j - 1] = mapa[j];
                trenutna_velicina--;
            }
        }
    }

};
template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa &m) {
    alocirajMapu(m.kapacitet);
    for(int i = 0; i<m.trenutna_velicina; i++) mapa[i]=m.mapa[i];
    trenutna_velicina = m.trenutna_velicina;
}
template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &NizMapa<TipKljuca, TipVrijednosti>::operator = (const NizMapa &m) {
    if(&m == this) return *this;
    delete [] mapa;
    trenutna_velicina = 0; kapacitet = 0;
    alocirajMapu(m.kapacitet);
    for(int i = 0; i<m.trenutna_velicina; i++) mapa[i]=m.mapa[i];
    trenutna_velicina = m.trenutna_velicina;
    return *this;
}


int main() {
    NizMapa<int,int> nm;
    BinStabloMapa<int,int> sm;
    int niz[50000];
    int vel = 50000;
    for(int i = 0; i < vel; i++) niz[i] = rand(); 
    clock_t vrijeme1 = clock();
    for(int i = 0; i < vel; i++) nm[niz[i]] = i;
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja za niz mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm[niz[i]] = i;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja za stablo mapu: " << ukvrijeme << " ms." << endl;
    /*
    Primijetimo da je izmjereno vrijeme umetanja velikog broja novih slucajno generisanih elementa u mapu
    implementiranu pomocu binarnog stabla znatno krace nego vrijeme umetanja u
    mapu implementiranu pomocu niza. To je iz razloga sto umetanje u mapu implementiranu
    pomocu niza ima vremensku kompeksnost O(n), dok je vremenska kompleksnost za
    umetanje elementa u mapu implementiranu pomocu binarnog stabla O(h), gdje je h visina stabla.
    U najgorem slucaju, odnosno ako bismo stalno dodavali veci/stalno dodavali manji element, ta 
    kompleksnost bi bila O(n). Ipak, u prosjecnom slucaju, a mozemo reci da se ovdje radi o takvom
    jer dodajemo random brojeve, kompleknost bi bila O(logn).
    */
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) nm[niz[i]]; // Samo pristupamo elementu, ne radimo nista sa njim
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za niz mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm[niz[i]]; // Samo pristupamo elementu, ne radimo nista sa njim 
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za stablo mapu: " << ukvrijeme << " ms." << endl;
    /*
    Ponovo imamo situaciju slicnu kao kod umetanja, odnosno u slucaju mape impelmentirane pomocu
    binarnog stabla vrijeme pristupa postojecem elementu je znatno krace nego kod mape implementirane
    pomocu niza. Objasnjenje je slicno kao i u prethodnom primjeru, odnosno pristup postojecem elementu
    kod mape implementirane pomocu niza je kompleksnosti O(n), dok je kod mape implementirane
    pomocu binarnog stabla kompleksnost u prosjecnom slucaju O(logn), tako da je, naravno, u slucaju 
    implementiranja mape preko binarnog stabla, ova funkcija vremenski efikasnija nego u slucaju implementiranja
    mape pomocu niza. 
    */
    return 0;
}