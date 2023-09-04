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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevi, *desni, *roditelj;
        int balans; //novi atribut za odredjivanje balansa cvora
        Cvor (const Cvor &c) {
            kljuc = c.kljuc; vrijednost = c.vrijednost;
            lijevi = desni = roditelj = nullptr;
            balans = c.balans; //korigujemo balans
        }
        Cvor(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti()) {
            kljuc = k; vrijednost = v;
            lijevi = desni = roditelj = nullptr;
            balans = 0; //Balans novokreiranog cvora je 0
        }
    };
    Cvor *korijen;
    int broj_elemenata;
    Cvor *trazi(Cvor *korijen, TipKljuca kljuc) const {
        if(korijen == nullptr || kljuc == korijen->kljuc) return korijen;
        if(kljuc < korijen->kljuc) return trazi(korijen->lijevi, kljuc);
        else return trazi(korijen->desni, kljuc);
    }
    /*U nastavku je dodano nekoliko novih metoda koje sluze za azuriranje balansa, vrsenje
    odgovarajucih rotacija i pronalazak visine stabla*/
    int visina (Cvor *c) {
        if(c == nullptr) return -1;
        return c->balans;
    }
    Cvor *jednostrukaDesnaRotacija(Cvor *c) {
        Cvor *temp = c->lijevi;
        c->lijevi = temp->desni;
        temp->desni = c;
        if(visina(c->lijevi) > visina(c->desni)) c->balans = visina(c->lijevi) + 1;
        else c->balans = visina(c->desni) + 1;
        if(visina(temp->lijevi) > c->balans) temp->balans = visina(temp->lijevi) + 1;
        else temp->balans = c->balans + 1;
        return temp;
    }
    Cvor *jednostrukaLijevaRotacija(Cvor *c) {
        Cvor *temp = c->desni;
        c->desni = temp->lijevi;
        temp->lijevi = c;
        if(visina(c->lijevi) > visina(c->desni)) c->balans = visina(c->lijevi) + 1;
        else c->balans = visina(c->desni) + 1;
        if(visina(temp->desni) > c->balans) temp->balans = visina(temp->desni) + 1;
        else temp->balans = c->balans + 1;
        return temp;
    }
    Cvor *lijevoDesnoRotacija(Cvor *c) {
        c->lijevi = jednostrukaLijevaRotacija(c->lijevi);
        c = jednostrukaDesnaRotacija(c);
        return c;
    }
    Cvor *desnoLijevoRotacija(Cvor *c) {
        c->desni = jednostrukaDesnaRotacija(c->desni);
        c = jednostrukaLijevaRotacija(c);
        return c;
    }
    Cvor *azurirajBalans(Cvor *c, TipKljuca kljuc) {
        if(abs(visina(c->lijevi) - visina(c->desni)) != 2) return c;
        if(visina(c->lijevi) > visina(c->desni)) {
            if(kljuc < c->lijevi->kljuc) c = jednostrukaDesnaRotacija(c);
            else c = lijevoDesnoRotacija(c);
        }
        else {
            if(kljuc < c->desni->kljuc) c = desnoLijevoRotacija(c);
            else c = jednostrukaLijevaRotacija(c);
        }
        return c;
    }
    void umetni (Cvor *&korijen, Cvor *&novi, Cvor *roditelj) { 
        if(korijen == nullptr) {
            korijen = novi;
            korijen->roditelj = roditelj; 
        }
        else {
            if(novi->kljuc < korijen->kljuc) umetni(korijen->lijevi, novi, korijen);
            else umetni(korijen->desni, novi, korijen);
            korijen = azurirajBalans(korijen, novi->kljuc); //popravljamo balanse i vrsimo rotacije ako treba
        }
        korijen->balans = max(visina(korijen->desni), visina(korijen->lijevi)) + 1; //korekcija balansa
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
    AVLStabloMapa() {
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
    ~AVLStabloMapa() {
        obrisi_rekurzivna(korijen);
        broj_elemenata = 0;
        korijen = nullptr;
    }
    AVLStabloMapa(const AVLStabloMapa &m);
    AVLStabloMapa &operator = (const AVLStabloMapa &m);
};
template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::AVLStabloMapa(const AVLStabloMapa &m) {
    korijen = nullptr;
    preorder(m.korijen);
    broj_elemenata = m.broj_elemenata;
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti> &AVLStabloMapa<TipKljuca, TipVrijednosti>::operator = (const AVLStabloMapa &m) {
    if(this == &m) return *this;
    obrisi();
    preorder(m.korijen);
    broj_elemenata = m.broj_elemenata;
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
int main() {
    AVLStabloMapa<int,int> am;
    BinStabloMapa<int,int> sm;
    int niz[500000];
    int vel = 500000;
    for(int i = 0; i < vel; i++) niz[i] = rand(); 
    clock_t vrijeme1 = clock();
    for(int i = 0; i < vel; i++) am[niz[i]] = i;
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja novog elementa za AVLstablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm[niz[i]] = i;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja novog elementa za BinStablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) am[niz[i]];
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za AVL stablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm[niz[i]];
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za BinStablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) am.obrisi(niz[i]);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za AVL stablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm.obrisi(niz[i]);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za BinStablo mapu: " << ukvrijeme << " ms." << endl;
    /*
    Posmatrajmo prvo prosjecan slucaj, odnosno kada u mape dodajemo veliki broj slucajno generisanih elemenata. Primijetimo da
    operacija dodavanja novog elementa ima nesto duze vrijeme izvrsavanja za avl stablo mapu, iz razloga sto se trosi izvjesno 
    vrijeme na provjeru i korekciju balansa. Sto se tice pristupa postojecem elementu i brisanju elemenata, vrijeme izvrsavanja je
    nesto krace za avl stablo mapu, ali vidimo da razlika nije toliko velika. Naime, kako kod AVL stablo mape vrsimo korekciju balansa,
    ne moze se desiti da razlika u visinama podstabala bude primjetna, dok je kod binstablo mape to moguce iako se radi o prosjecnom slucaju, pa
    to moze povecati vrijeme izvrsavanja.
    Kompleknost sve tri navedene operacije je O(h), gdje je h visina stabla, a kako dodajemo slucajno generisane elemente, mozemo reci da je prosjecan
    slucaj u pitanju, pa ce ove kompkeksnosti biti O(logn). Sada cemo smanjiti broj elemenata i provjeriti slucaj kada stalno dodajemo element veci od prethodnog.
    */
    vel = 10000;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) am[i] = i;;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja novog elementa kada stalno dodajemo veci element od prethodnog za AVL stablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm[i] = i;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme umetanja novog elementa kada stalno dodajemo veci element od prethodnog za BinStablo mapu: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) am[i];
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za AVL stablo mapu, gdje je svaki novi element koji je bio ubacen bio veci od prethodnog: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = 0; i < vel; i++) sm[i];
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme pristupa postojecem elementu za BinStablo mapu, gdje je svaki novi element koji je bio ubacen bio veci od prethodnog: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = vel; i >= 0; i--) am.obrisi(i);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za AVL stablo mapu, gdje je svaki novi element koji je bio ubacen bio veci od prethodnog: " << ukvrijeme << " ms." << endl;
    vrijeme1 = clock();
    for(int i = vel; i >= 0; i--) sm.obrisi(i);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme brisanja elemenata za BinStablo mapu, gdje je svaki novi element koji je bio ubacen bio veci od prethodnog: " << ukvrijeme << " ms." << endl;
    /*
    Primijetimo da u ovom slucaju AVL stablo mapa ima znatno bolje performanse nego bin stablo mapa. Upravo u ovom slucaju se i ogleda
    prednost AVL stablo mape. Naime, kako je vec receno, vremenske kompleksnosti ovih operacija ovise o visini stabla, tj. one su O(h).
    Kod AVL stabla vodimo racuna o tome da odrzimo balans cvororva takav da nikad nemamo primjetnu razliku u visinama lijevog i desnog
    podstabla. S druge strane, kod bin stablo mape ne vodimo racuna o tome, tako da ukoliko stalno dodajemo veci ili stalno manji element
    od prethodnog, oni ce se uvijek dodavati u isto podstablo, tako da ce visina stabla biti jednaka broju umetnutih elemenata, pa ce kompleksnost
    razmatranih operacija biti O(n). U slucaju AVL stablo mape, kompleksnosti su idalje O(logn).
    */
    return 0;
}