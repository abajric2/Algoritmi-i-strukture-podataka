#include <iostream>

using namespace std;

template <typename Tip>
class Lista {
    public:
    Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip trenutni() const = 0;
    virtual Tip &trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred (const Tip &el) = 0;
    virtual void dodajIza (const Tip &el) = 0;
    virtual Tip operator[] (int indeks) const = 0;
    virtual Tip &operator[] (int indeks) = 0;
    virtual ~Lista() {}
};
template <typename Tip>
class DvostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor *sljedeci, *prethodni;
        Cvor (const Tip &element, Cvor *sljedeci, Cvor *prethodni) : element(element), sljedeci(sljedeci), prethodni(prethodni) {}
        Cvor (Cvor *sljedeci=nullptr, Cvor *prethodni=nullptr) : sljedeci(sljedeci), prethodni(prethodni) {}
    };
    int lduzina, dduzina;
    Cvor *p, *k, *tekuci;
    public:
    DvostrukaLista() {
        p=tekuci=new Cvor;
        p->sljedeci=k=new Cvor{nullptr, p};
        lduzina=0; dduzina=0;
    }
    DvostrukaLista (const DvostrukaLista &l);
    DvostrukaLista &operator =(const DvostrukaLista &l);
    ~DvostrukaLista();
    int brojElemenata() const override {
        return lduzina+dduzina;
    }
    Tip trenutni() const override {
        if(brojElemenata()==0) throw "Lista je prazna";
        return tekuci->sljedeci->element;
    }
    Tip &trenutni() override {
        if(brojElemenata()==0) throw "Lista je prazna";
        return tekuci->sljedeci->element;
    }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override {
        if(brojElemenata()==0) throw "Lista je prazna";
        tekuci=p;
        dduzina=brojElemenata();
        lduzina=0;
    }
    void kraj() override;
    void obrisi() override;
    void dodajIspred (const Tip &el) override;
    void dodajIza (const Tip &el) override;
    Tip &operator[] (int indeks) override;
    Tip operator[] (int indeks) const override;
};

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &l1) { 
    p=tekuci=new Cvor;
    Cvor *pomocni=p;
    for(Cvor* i=l1.p; i->sljedeci!=l1.k;) {
        pomocni->sljedeci=new Cvor{i->sljedeci->element, nullptr, pomocni};
        if(i==l1.tekuci) tekuci=pomocni;
        i=i->sljedeci; pomocni=pomocni->sljedeci;
    }
    pomocni->sljedeci=k=new Cvor{nullptr, pomocni};
    pomocni=nullptr;
    lduzina=l1.lduzina; dduzina=l1.dduzina;
}
template <typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator=(const DvostrukaLista<Tip> &l1) { 
    if(&l1==this) return *this;
    for(Cvor *i=p->sljedeci; i!=k;) {
        Cvor* pomocni=i;
        i=pomocni->sljedeci;
        delete pomocni;
    }
    delete p; delete k;
    p=tekuci=new Cvor;
    Cvor *pomocni=p;
    for(Cvor* i=l1.p; i->sljedeci!=l1.k;) {
        pomocni->sljedeci=new Cvor{i->sljedeci->element, nullptr, pomocni};
        if(i==l1.tekuci) tekuci=pomocni;
        i=i->sljedeci; pomocni=pomocni->sljedeci;
    }
    pomocni->sljedeci=k=new Cvor{nullptr, pomocni};
    pomocni=nullptr;
    lduzina=l1.lduzina; dduzina=l1.dduzina;
    return *this;
}
template <typename Tip>
DvostrukaLista<Tip>::~DvostrukaLista() {
    for(Cvor *i=p->sljedeci; i!=k;) {
        Cvor* pomocni=i;
        i=pomocni->sljedeci;
        delete pomocni;
    }
    delete p; delete k;
    p=k=tekuci=nullptr;
    lduzina=dduzina=0;
}

template <typename Tip>
bool DvostrukaLista<Tip>::prethodni() {
    if(brojElemenata()==0) throw "Lista je prazna";
    if(tekuci==p) return false;
    tekuci=tekuci->prethodni;  
    lduzina--; dduzina++;
    return true;
}
template <typename Tip>
bool DvostrukaLista<Tip>::sljedeci() {
    if(brojElemenata()==0) throw "Lista je prazna";
    if(tekuci->sljedeci==k->prethodni) return false;
    tekuci=tekuci->sljedeci;
    dduzina--; lduzina++;
    return true;
}
template <typename Tip>
void DvostrukaLista<Tip>::kraj() {
    if(brojElemenata()==0) throw "Lista je prazna";
    tekuci=k;
    tekuci=tekuci->prethodni->prethodni;
    lduzina=brojElemenata()-1;
    dduzina=1;
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi() {
    if(brojElemenata()==0) throw "Lista je prazna";
    Cvor *pomocni=tekuci->sljedeci;
    tekuci->sljedeci=pomocni->sljedeci;
    pomocni->sljedeci->prethodni=tekuci;
    delete pomocni;
    dduzina--;
    if(tekuci->sljedeci==k && tekuci!=p) tekuci=tekuci->prethodni;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred (const Tip &el) {
    if(brojElemenata()==0) {
        p->sljedeci=new Cvor(el, k, p);
        k->prethodni=p->sljedeci;
        dduzina++;
    }
    else {
        Cvor *pomocni=tekuci->sljedeci;
        tekuci->sljedeci=new Cvor{el, pomocni, tekuci};
        pomocni->prethodni=tekuci->sljedeci;
        pomocni=nullptr;
        lduzina++;
        tekuci=tekuci->sljedeci;
    }
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza (const Tip &el) {
    if(brojElemenata()==0) {
        p->sljedeci=new Cvor{el, k, p};
        k->prethodni=p->sljedeci;
        dduzina++;
    }
    else {
        Cvor *pomocni=tekuci->sljedeci->sljedeci;
        tekuci->sljedeci->sljedeci=new Cvor{el, pomocni, tekuci->sljedeci};
        pomocni->prethodni=tekuci->sljedeci->sljedeci;
        pomocni=nullptr;
        dduzina++;
    }
}

template <typename Tip>
Tip &DvostrukaLista<Tip>::operator[](int indeks) {  
    if(indeks<0 || indeks>=brojElemenata()) throw "Neispravan indeks";
    int p=0, ld=lduzina, dd=dduzina;
    Cvor *pomocni=tekuci;
    int k=0;
    if(indeks<abs(indeks-ld) && indeks<(ld+dd)/2) {
        k=indeks;
        pomocni=this->p;
    }
    else if(abs(indeks-ld)<ld+dd-indeks) k=indeks-ld;
    else {
        k=indeks-ld-dd-1;
        pomocni=this->k;
    }
    if(k<0) {
        for(int i=0; i<abs(k); i++) pomocni=pomocni->prethodni;
    }
    else {
        for(int i=0; i<k; i++) pomocni=pomocni->sljedeci;
    }
    return pomocni->sljedeci->element;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator[](int indeks) const { 
    if(indeks<0 || indeks>=brojElemenata()) throw "Neispravan indeks";
    int p=0, ld=lduzina, dd=dduzina;
    Cvor *pomocni=tekuci;
    int k=0;
    if(indeks<abs(indeks-ld) && indeks<(ld+dd)/2) {
        k=indeks;
        pomocni=this->p;
    }
    else if(abs(indeks-ld)<ld+dd-indeks) k=indeks-ld;
    else {
        k=indeks-ld-dd-1;
        pomocni=this->k;
    }
    if(k<0) {
        for(int i=0; i<abs(k); i++) pomocni=pomocni->prethodni;
    }
    else {
        for(int i=0; i<k; i++) pomocni=pomocni->sljedeci;
    }
    return pomocni->sljedeci->element;
}

template <typename Tip>
class DvostraniRed {
    DvostrukaLista<Tip> r;
    public:
    void brisi();
    void staviNaCelo(const Tip &el);
    Tip skiniSaVrha();
    Tip &celo();
    int brojElemenata() const;
    void staviNaVrh(const Tip &el);
    Tip &vrh();
    Tip skiniSaCela();
};
template <typename Tip>
Tip &DvostraniRed<Tip>::vrh() {
    r.pocetak();
    return r.trenutni();
}
template <typename Tip>
void DvostraniRed<Tip>::staviNaVrh(const Tip &el) {
    if(brojElemenata()!=0) r.pocetak();
    r.dodajIspred(el); r.prethodni();
}
template <typename Tip>
void DvostraniRed<Tip>::brisi() {
    r.pocetak();
    int granica=brojElemenata();
    for(int i=0; i<granica; i++) r.obrisi();
}
template <typename Tip>
void DvostraniRed<Tip>::staviNaCelo(const Tip &el) {
    if(brojElemenata()!=0) r.kraj();
    r.dodajIza(el);
    r.sljedeci();
}
template <typename Tip>
Tip DvostraniRed<Tip>::skiniSaVrha() {
    if(brojElemenata()==0) throw "DvostraniRed je prazan";
    r.pocetak();
    int el=r.trenutni();
    r.obrisi();
    return el;
}
template <typename Tip>
Tip DvostraniRed<Tip>::skiniSaCela() {
    if(brojElemenata()==0) throw "DvostraniRed je prazan";
    r.kraj();
    int el=r.trenutni();
    r.obrisi();
    return el;
}
template <typename Tip>
Tip &DvostraniRed<Tip>::celo() {
    if(brojElemenata()==0) throw "DvostraniRed je prazan";
    r.kraj();
    return r.trenutni();
}
template <typename Tip>
int DvostraniRed<Tip>::brojElemenata() const {
    return r.brojElemenata();
}

//Testiranje klase DvostraniRed

void testirajBrisi() {
    DvostraniRed<int> r;
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.staviNaCelo(1); r.staviNaVrh(2); r.staviNaVrh(3);
    r.brisi();
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    for(int i=0; i<10000; i++) r.staviNaCelo(i);
    r.brisi();
    if(r.brojElemenata()==0) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajStaviNaCelo() {
    DvostraniRed<int> r;
    r.staviNaCelo(1);
    if(r.celo()==1 && r.brojElemenata()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.brisi();
    r.staviNaCelo(5); r.staviNaCelo(6); r.staviNaCelo(7);
    if(r.celo()==7 && r.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.brisi();
    for(int i=0; i<10000; i++) r.staviNaCelo(i);
    if(r.celo()==9999) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajSkiniSaVrha() {
    DvostraniRed<int> r;
    try {
        r.skiniSaVrha();
        cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    r.staviNaCelo(1); r.staviNaVrh(2); r.staviNaCelo(3);
    if(r.skiniSaVrha()==2 && r.brojElemenata()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(r.celo()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.skiniSaVrha(); r.skiniSaVrha();
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajCelo() {
    DvostraniRed<int> r;
    try {
        r.celo();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    r.staviNaCelo(1); r.staviNaCelo(2); r.staviNaCelo(3);
    if(r.celo()==3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    r.skiniSaCela();
    if(r.brojElemenata()==2 && r.celo()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajBrojElemenata() {
    DvostraniRed<int> r;
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.staviNaVrh(1); r.staviNaCelo(2); r.staviNaVrh(3);
    if(r.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.brisi();
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajStaviNaVrh() {
    DvostraniRed<int> r;
    r.staviNaVrh(1);
    if(r.brojElemenata()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.staviNaVrh(2); r.staviNaVrh(3);
    if(r.brojElemenata()==3 && r.vrh()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    for(int i=0; i<10000; i++) r.staviNaVrh(i);
    if(r.vrh()==9999) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajVrh() {
    DvostraniRed<int> r;
    try {
        r.vrh(); cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    r.staviNaVrh(1); r.staviNaVrh(2); r.staviNaVrh(3);
    if(r.vrh()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.vrh()=5;
    if(r.vrh()==5) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.vrh()++;
    if(r.vrh()==6) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajSkiniSaCela() {
    DvostraniRed<int> r;
    try {
        r.skiniSaCela(); cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    r.staviNaCelo(1); r.staviNaCelo(2); r.staviNaCelo(3);
    if(r.skiniSaCela()==3 && r.brojElemenata()==2) std::cout<<"OK"<<std::endl;
}
void testirajKopirajuciKonstruktor() {
    DvostraniRed<int> r1, r2(r1);
    if(r2.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r1.staviNaCelo(1); r1.staviNaCelo(2); r2.staviNaCelo(3);
    if(r1.brojElemenata()==2 && r2.brojElemenata()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    DvostraniRed<int> r3(r1);
    r1.staviNaCelo(4);
    if(r3.brojElemenata()==2 && r3.celo()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r1.brisi();
    if(r3.brojElemenata()==2 && r3.celo()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    for(int i=0; i<10000; i++) r1.staviNaCelo(i);
    DvostraniRed<int> r4(r1);
    if(r4.brojElemenata()==10000 && r4.celo()==9999) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajKopirajuciOperatorDodjele() {
    DvostraniRed<int> r1, r2;
    r1.staviNaVrh(1); r1.staviNaVrh(2); r1.staviNaVrh(3);
    r2.staviNaVrh(4); r2.staviNaVrh(5); r2.staviNaVrh(6); r2.staviNaVrh(7);
    r1=r2;
    r2.staviNaVrh(8); r2.celo()=10;
    if(r1.brojElemenata()==4 && r2.brojElemenata()==5 && r1.celo()==4 && r2.celo()==10) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r2.brisi();
    if(r1.brojElemenata()==4 && r2.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    for(int i=0; i<10000; i++) r2.staviNaVrh(i);
    DvostraniRed<int> r4;
    for(int i=0; i<20000; i++) r4.staviNaCelo(i);
    r2=r4;
    r4.brisi();
    if(r2.brojElemenata()==20000 && r2.vrh()==0) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
int main() {
    testirajBrisi();
    testirajStaviNaCelo();
    testirajSkiniSaVrha();
    testirajCelo();
    testirajBrojElemenata();
    testirajStaviNaVrh();
    testirajVrh();
    testirajSkiniSaCela();
    testirajKopirajuciKonstruktor();
    testirajKopirajuciOperatorDodjele();
    return 0;
}