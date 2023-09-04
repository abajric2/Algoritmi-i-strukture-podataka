#include <iostream>

using namespace std;

template <typename Tip>
class Iterator;

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
    friend class Iterator<Tip>;
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
    friend class Iterator<Tip>;
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
class Iterator {
    const Lista<Tip> *l;
    typename DvostrukaLista<Tip>::Cvor *trenutniDL;
    public:
    Iterator(const DvostrukaLista<Tip> &l1) {
       l=&l1; trenutniDL=l1.tekuci;
    }   
    Tip trenutni() {
        if(l->brojElemenata()==0) throw "Lista je prazna";
        return trenutniDL->sljedeci->element;
    }
    void pocetak() {
        if(l->brojElemenata()==0) throw "Lista je prazna";
        trenutniDL=(dynamic_cast<const DvostrukaLista<Tip> *>(l))->p;
    }
    void sljedeci() {
        if(l->brojElemenata()==0) throw "Lista je prazna";
        trenutniDL=trenutniDL->sljedeci;
    }
    int brojElemenata() {
        return (dynamic_cast<const DvostrukaLista<Tip> *>(l))->brojElemenata();
    }
};
template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n) {
    Iterator<Tip> l(dynamic_cast<const DvostrukaLista<Tip> &>(n));
    l.pocetak();
    Tip max=l.trenutni();
    l.sljedeci();
    for(int i=1; i<l.brojElemenata(); i++) {
        if(l.trenutni()>max) max=l.trenutni();
        l.sljedeci();
    }
    return max;
}

//Testiranje klase DvostrukaLista

void testirajBrojElemenata() {
    DvostrukaLista<int> l;
    if(l.brojElemenata()==0) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.dodajIspred(1); l.dodajIspred(2); l.dodajIspred(3);
    l.dodajIza(4); l.dodajIza(5);
    if (l.brojElemenata()==5) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    for(int i=0; i<5; i++) l.obrisi();
    if(l.brojElemenata()==0) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajTrenutni() {
    DvostrukaLista<int> l;
    try {
        l.trenutni();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIza(1); l.dodajIza(2); l.dodajIza(3);
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.dodajIspred(4);
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.sljedeci();
    if(l.trenutni()==3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajPrethodni() {
    DvostrukaLista<int> l;
    try {
        l.prethodni();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIspred(1);
    if(!l.prethodni()) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.dodajIspred(2); l.dodajIza(3);
    l.prethodni();
    if(l.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajSljedeci() {
    DvostrukaLista<int> l;
    try {
        l.sljedeci();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIza(1); l.dodajIza(2); l.dodajIza(3);
    l.sljedeci();
    if(l.trenutni()==3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.sljedeci();
    if(!l.sljedeci()) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajPocetak() {
    DvostrukaLista<int> l;
    try {
        l.pocetak();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIspred(1); l.dodajIspred(2); l.dodajIspred(3);
    l.pocetak();
    if(l.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajKraj() {
    DvostrukaLista<int> l;
    try {
        l.kraj();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIza(1); l.dodajIza(2); l.dodajIza(3);
    l.kraj();
    if(l.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajObrisi() {
    DvostrukaLista<int> l;
    try {
        l.obrisi();
        std::cout<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIza(1); l.dodajIza(2);
    l.obrisi();
    if(l.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.dodajIza(3);
    l.sljedeci();
    l.obrisi();
    if(l.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajDodajIspred() {
    DvostrukaLista<int> l;
    l.dodajIspred(1);
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.dodajIspred(2); l.dodajIspred(3);
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.obrisi();
    l.dodajIspred(4);
    l.prethodni();
    if(l.trenutni()==4) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    for(int i=0; i<5000; i++) l.dodajIspred(i);
    l.prethodni();
    if(l.trenutni()==4999) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajDodajIza() {
    DvostrukaLista<int> l;
    l.dodajIza(1);
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.dodajIza(2); l.dodajIza(3);
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.sljedeci();
    l.dodajIza(4);
    l.sljedeci();
    if(l.trenutni()==4) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l.kraj();
    l.dodajIza(5);
    l.sljedeci();
    if(l.trenutni()==5) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    for(int i=0; i<5000; i++) l.dodajIza(i);
    l.sljedeci();
    if(l.trenutni()==4999) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajOperatorUglasteZagrade() {
    DvostrukaLista<int> l;
    try {
        std::cout<<l[0]<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    l.dodajIza(1); l.dodajIza(2); l.dodajIza(3);
    if(l[0]==1 && l[1]==3 && l[2]==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l[1]=5;
    if(l[1]==5) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    try {
        std::cout<<l[-125]<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
    try {
        std::cout<<l[3]<<"NIJE OK"<<std::endl;
    }
    catch(...) {
        std::cout<<"OK"<<std::endl;
    }
}
void testirajKopirajuciKonstruktor() {
    DvostrukaLista<int> l1;
    l1.dodajIza(1); l1.dodajIza(2); l1.dodajIza(3);
    DvostrukaLista<int> l2(l1);
    if(l2.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l2.sljedeci();
    if(l2.trenutni()==3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l2.sljedeci();
    if(l2.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    if(!l2.sljedeci()) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l1.obrisi();
    if(l2.brojElemenata()==3 && l1.brojElemenata()==2 && l1[0]==3 && l2[0]==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}
void testirajKopirajuciOperatorDodjele() {
    DvostrukaLista<int> l1;
    l1.dodajIza(1); l1.dodajIza(2); l1.dodajIza(3);
    DvostrukaLista<int> l2;
    l2.dodajIza(6); l2.dodajIza(7);
    l2=l1;
    if(l2.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l2.sljedeci();
    if(l2.trenutni()==3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l2.sljedeci();
    if(l2.trenutni()==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    if(!l2.sljedeci()) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l1.obrisi();
    if(l2.brojElemenata()==3 && l1.brojElemenata()==2 && l1[0]==3 && l2[0]==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
    l1=l1; //Test na testruktivnu samododjelu
    l1.pocetak();
    if(l1.trenutni()==3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NIJE OK"<<std::endl;
}

//Testiranje funkcije dajMaksimum

void testirajDajMaksimum() {
    DvostrukaLista<int> l;
    try {
        dajMaksimum(l);
        cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    l.dodajIza(11); l.dodajIspred(126); l.dodajIspred(555555); l.dodajIza(1); l.dodajIza(-156); 
    l.dodajIspred(-55555); l.dodajIza(25); l.dodajIspred(0);
    if(dajMaksimum(l)==555555) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
int main() {
    testirajBrojElemenata();
    testirajTrenutni();
    testirajPrethodni();
    testirajSljedeci();
    testirajPocetak();
    testirajKraj();
    testirajObrisi();
    testirajDodajIspred();
    testirajDodajIza();
    testirajOperatorUglasteZagrade();
    testirajKopirajuciKonstruktor();
    testirajKopirajuciOperatorDodjele();
    testirajDajMaksimum();
    return 0;
}