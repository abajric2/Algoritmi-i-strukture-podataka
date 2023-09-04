#include <iostream>
#include <utility>
#include <vector>

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

unsigned int h(int ulaz, unsigned int max) {
    unsigned int suma=23;
    suma = suma*31 + ulaz;
    return suma % max;
}

template<typename TipKljuca>
class Kljuc {
    TipKljuca k;
    bool obrisan, prazno_mjesto;
    public:
    Kljuc(TipKljuca k = TipKljuca(), bool obrisan = false, bool prazno_mjesto = true) 
    : k(k), obrisan(obrisan), prazno_mjesto(prazno_mjesto) {}
    TipKljuca dajKljuc () const {return k;}
    void postaviKljuc(const TipKljuca &k) {this->k = k;}
    bool daLiJeObrisan () const {return obrisan;}
    void postaviObrisan (bool obrisan) {this->obrisan = obrisan;}
    void postaviDaLiJePraznoMjesto(bool prazno_mjesto) {this->prazno_mjesto = prazno_mjesto;}
    bool DaLiJePraznoMjesto() const {return prazno_mjesto;}
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
    TipVrijednosti err = TipVrijednosti();
    std::pair<Kljuc<TipKljuca>, TipVrijednosti> *mapa;
    int trenutna_velicina, kapacitet;
    //TipKljuca prazno_mjesto;
    unsigned int (*hashfun)(TipKljuca ulaz, unsigned int opseg_izlaza);
    void alocirajMapu(int vel) {
        mapa = new std::pair<Kljuc<TipKljuca>, TipVrijednosti>[vel]{};
        kapacitet = vel; trenutna_velicina = 0;
        hashfun = nullptr;
       // prazno_mjesto = pmjesto;
       // for(int i = 0; i < kapacitet; i++) mapa[i].first.postaviDaLiJePraznoMjesto(true);
    }
    void prosiriMapu() {
        kapacitet *= 2;
        std::pair<Kljuc<TipKljuca>, TipVrijednosti> *pomocni = new std::pair<Kljuc<TipKljuca>, TipVrijednosti>[kapacitet]{};
        for(int i = 0; i<kapacitet/2; i++) {
            if(!mapa[i].first.DaLiJePraznoMjesto()) {
                int k = 0, indeks;
                do {
                    int j = hashfun(mapa[i].first.dajKljuc(), kapacitet);
                    indeks = j + k;
                    if(indeks >= kapacitet) indeks %= kapacitet;
                    if(pomocni[indeks].first.DaLiJePraznoMjesto()) {
                        pomocni[indeks].first.postaviKljuc(mapa[i].first.dajKljuc());
                        pomocni[indeks].first.postaviObrisan(mapa[i].first.daLiJeObrisan());
                        pomocni[indeks].first.postaviDaLiJePraznoMjesto(mapa[i].first.DaLiJePraznoMjesto());
                        pomocni[indeks].second = mapa[i].second;
                        break;
                    }
                    else k ++;
                } while(k != kapacitet);
            }
        }
        delete[] mapa;
        mapa = pomocni;
    }
    public:
    HashMapa(unsigned int hfun (TipKljuca ulaz, unsigned int opseg_izlaza) = nullptr, int vel = 100, TipKljuca pmjesto = TipKljuca()) {
        alocirajMapu(vel);
        hashfun = hfun;
        //hashfun = nullptr;
        //prazno_mjesto = pmjesto;
      //  for(int i = 0; i < kapacitet; i++) mapa[i].first = prazno_mjesto;
    } 
    HashMapa(const HashMapa &m);
    HashMapa &operator = (const HashMapa &m);
    int brojElemenata() const override {
        return trenutna_velicina;
    }
    void obrisi() override {
      //  delete [] mapa;
        //alocirajMapu(100);
        for(int i = 0; i < kapacitet; i++) {
            mapa[i].first.postaviKljuc(TipKljuca());
            mapa[i].second = TipVrijednosti();
            mapa[i].first.postaviDaLiJePraznoMjesto(true);
            mapa[i].first.postaviObrisan(true);
        }
        trenutna_velicina = 0;
      //  hashfun = nullptr;
    }
    ~HashMapa() {
        delete [] mapa;
        kapacitet = 0; trenutna_velicina = 0;
        mapa = nullptr;
        hashfun = nullptr;
    }
    TipVrijednosti &operator[] (const TipKljuca &kljuc) override {
        if(!hashfun) throw "Hash funkcija nije definisana";
        int i = 0, j, indeks;
        if(trenutna_velicina >= kapacitet) prosiriMapu();
        do {
            j = hashfun(kljuc, kapacitet);
            indeks = i + j;
            if(indeks >= kapacitet) indeks %= kapacitet;
            if(mapa[indeks].first.DaLiJePraznoMjesto() && 
            !mapa[indeks].first.daLiJeObrisan()) {
                mapa[indeks].first.postaviKljuc(kljuc);
                mapa[indeks].first.postaviObrisan(false);
                mapa[indeks].first.postaviDaLiJePraznoMjesto(false);
                mapa[indeks].second = TipVrijednosti();
                trenutna_velicina ++;
                return mapa[indeks].second;
            }
            else if(!mapa[indeks].first.DaLiJePraznoMjesto() &&
            mapa[indeks].first.dajKljuc() == kljuc) return mapa[indeks].second;
            else i ++;
        } while(i != kapacitet);
        trenutna_velicina ++;
        return mapa[j].second;
        /*int indeks, i = 0;
        if(trenutna_velicina != 0) {
            do {
                indeks = hashfun(kljuc, trenutna_velicina);
                if(mapa[indeks].first.dajKljuc() == kljuc) return mapa[indeks].second;
                else i ++;
            } while (mapa[indeks].first.DaLiJePraznoMjesto() == false && i != trenutna_velicina);
        }
       // if(mapa[indeks].first == kljuc) return mapa[indeks].second;
        if(trenutna_velicina>=kapacitet) prosiriMapu();
        int j;
        i = 0;
        bool nadjeno_mjesto = false;
        do {
            j = hashfun(kljuc, i);
            if(mapa[j].first.DaLiJePraznoMjesto()) {
                mapa[j].first.postaviKljuc(kljuc);
                mapa[j].first.postaviDaLiJePraznoMjesto(false);
                nadjeno_mjesto = true;
                break;
            }
            else i ++;
        } while (j < kapacitet);
        if(nadjeno_mjesto) {
           // mapa[j].first = kljuc;
            mapa[j].second = TipVrijednosti();
            trenutna_velicina++;
            return mapa[j].second;
        }
        //return TipVrijednosti();*/
    }
    TipVrijednosti operator[] (const TipKljuca &kljuc) const override {
        if(!hashfun) throw "Hash funkcija nije definisana";
        int i = 0, j, indeks;
       // if(trenutna_velicina >= kapacitet) prosiriMapu();
        do {
            j = hashfun(kljuc, kapacitet);
            indeks = i + j;
            if(indeks >= kapacitet) indeks %= kapacitet;
            if(!mapa[indeks].first.DaLiJePraznoMjesto() && 
            mapa[indeks].first.dajKljuc() == kljuc) return mapa[indeks].second;
            else i ++;
        } while(i != kapacitet);
       // trenutna_velicina ++;
        //return mapa[j].second;
        /*int indeks, i = 0;
        if(trenutna_velicina != 0) {
            do {
                indeks = hashfun(kljuc, trenutna_velicina);
                if(mapa[indeks].first.dajKljuc()== kljuc) return mapa[indeks].second;
                else i ++;
            } while (mapa[indeks].first.DaLiJePraznoMjesto() == false && i != trenutna_velicina);
        }*/
        return err;
    }
    void obrisi(const TipKljuca &kljuc) override {
       /* for(int i = 0; i < trenutna_velicina; i++) {
            if(mapa[i].first.dajKljuc() == kljuc) {
                mapa[i].first.postaviDaLiJePraznoMjesto(true);
                //for(int j = i+1; j<trenutna_velicina; j++) mapa[j - 1] = mapa[j];
                trenutna_velicina--;
            }
        }*/
        if(!hashfun) throw "Hash funkcija nije definisana";
        int i = 0, j, indeks;
        do {
            j = hashfun(kljuc, kapacitet);
            indeks = j + i;
            if(indeks >= kapacitet) indeks %= kapacitet;
            if(mapa[indeks].first.dajKljuc() == kljuc) {
                mapa[indeks].first.postaviKljuc(TipKljuca());
                mapa[indeks].first.postaviObrisan(true);
                mapa[indeks].first.postaviDaLiJePraznoMjesto(true);
                mapa[indeks].second = TipVrijednosti();
                trenutna_velicina --;
                return;
            }
            else i ++;
        } while (i != kapacitet);
    }
    void definisiHashFunkciju(unsigned int hfun (TipKljuca ulaz, unsigned int opseg_izlaza)) {
        hashfun = hfun;
    }
};
template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(const HashMapa &m) {
    alocirajMapu(m.kapacitet);
    for(int i = 0; i<m.kapacitet; i++) {
        mapa[i].first.postaviKljuc(m.mapa[i].first.dajKljuc());
        mapa[i].first.postaviDaLiJePraznoMjesto(m.mapa[i].first.DaLiJePraznoMjesto());
        mapa[i].first.postaviObrisan(m.mapa[i].first.daLiJeObrisan());
        mapa[i].second = m.mapa[i].second;
    }
    trenutna_velicina = m.trenutna_velicina;
    hashfun = m.hashfun;
}
template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti> &HashMapa<TipKljuca, TipVrijednosti>::operator = (const HashMapa &m) {
    if(&m == this) return *this;
    delete [] mapa;
    trenutna_velicina = 0; kapacitet = 0;
    alocirajMapu(m.kapacitet);
    for(int i = 0; i<m.kapacitet; i++) {
        mapa[i].first.postaviKljuc(m.mapa[i].first.dajKljuc());
        mapa[i].first.postaviDaLiJePraznoMjesto(m.mapa[i].first.DaLiJePraznoMjesto());
        mapa[i].first.postaviObrisan(m.mapa[i].first.daLiJeObrisan());
        mapa[i].second = m.mapa[i].second;
    }
    trenutna_velicina = m.trenutna_velicina;
    hashfun = m.hashfun;
    return *this;
}

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
        for(int i = 0; i<kapacitet; i++) pomocni[i] = mapa[i];
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
    HashMapa<int, int> hm;
    hm.definisiHashFunkciju(h);
    int niz[30000];
    int vel = 30000;
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
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) hm[niz[i]] = i;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja za hash mapu: " << ukvrijeme << " ms." << endl;    
    /*
    Vidimo da najduze traje umetanje u mapu implementiranu preko niza, zatim u hashmapu,
    a najkrace je za mapu implementrianu preko binarnog stabla. Inace, hash mapa nastoji ubacivati
    elemente sa kompleksnoscu O(1), medjutim, razlog ovakvom ponasanju jeste cinjenica da moze biti
    previse kolizija u nasem slucaju. Ubacivanje u mapu implementiranu preko bianrnog stabla
    ima u prosjecnom slucaju vremensku kompleksnost O(logn), a sto se tice mape implementirane preko
    niza, kompleksnost umetanja u nju je O(n). Napomenimo da u slucaju hash mape vrijeme izvrsavanja
    ove operacije ovisi i od izbora hash funkcije.
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
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) hm[niz[i]];
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za hash mapu: " << ukvrijeme << " ms." << endl;    
    /*
    Ponovo imamo situaciju slicnu kao kod umetanja, odnosno u slucaju mape impelmentirane pomocu
    niza vrijeme pristupa postojecem elementu je najsporije. Objasnjenje je slicno kao i u prethodnom primjeru, 
    odnosno pristup postojecem elementu kod mape implementirane pomocu niza je kompleksnosti O(n). Kod mape implementirane
    pomocu binarnog stabla kompleksnost je u prosjecnom slucaju O(logn), tako da je, naravno, u slucaju 
    implementiranja mape preko binarnog stabla, ova funkcija vremenski efikasnija nego u slucaju implementiranja
    mape pomocu niza. Sto se tice pristupa postojecem elementu hash mape, ona se najkrace izvrsavala i razlog tome je
    sto ona ima najbolju vremensku kompleksnost za ovu operaciju, odnosno tezi O(1). Naravno, ona ce ponovo ovisiti
    i od izbora hash funkcije.
    */ 
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) nm.obrisi(niz[i]); // Samo pristupamo elementu, ne radimo nista sa njim
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za niz mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm.obrisi(niz[i]); // Samo pristupamo elementu, ne radimo nista sa njim 
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za stablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) hm.obrisi(niz[i]);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za hash mapu: " << ukvrijeme << " ms." << endl;    
    /*
    Ponovo se najbrze brisu elementi iz hash mape, zatim iz binstablo mape, a najsporije je
    u slucaju niz mape. Dakle, kod hash mape ce opet teziti O(1), a ovisiti i od izbora hash funkcije,
    kod binstablo mape je prosjecno O(logn), a kod niz mape O(n).
    */
    return 0;
}