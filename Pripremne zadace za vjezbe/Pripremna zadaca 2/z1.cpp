#include <iostream>

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
class NizLista : public Lista<Tip> {
    Tip **l;
    int kapacitet, broj_elemenata, tekuci;
    void prosiriListu();
    public:
    NizLista(int vel=100) {
        l=new Tip*[vel]{};
        kapacitet=vel; broj_elemenata=0;
        tekuci=0;
    }
    int brojElemenata() const override {
        return broj_elemenata;
    }
    Tip trenutni() const override {
        if(broj_elemenata==0) throw "Lista je prazna";
        return *l[tekuci];
    }
    Tip &trenutni() override {
        if(broj_elemenata==0) throw "Lista je prazna";
        return *l[tekuci];
    }
    bool prethodni() override {
        if(broj_elemenata==0) throw "Lista je prazna";
        if(tekuci==0) return false;
        tekuci--;
        return true;
    }
    bool sljedeci() override {
        if(broj_elemenata==0) throw "Lista je prazna";
        if(tekuci==broj_elemenata-1) return false;
        tekuci++;
        return true;
    }
    void pocetak() override {
        if(broj_elemenata==0) throw "Lista je prazna";
        tekuci=0;
    }
    void kraj() override {
        if(broj_elemenata==0) throw "Lista je prazna";
        tekuci=broj_elemenata-1;
    }
    void obrisi() override;
    void dodajIspred (const Tip &el) override;
    void dodajIza (const Tip &el) override;
    Tip operator[] (int indeks) const override {
        if(indeks<0 || indeks>=broj_elemenata) throw "Neispravna vrijednost indeksa";
        return *l[indeks];
    }
    Tip &operator[] (int indeks) override {
        if(indeks<0 || indeks>=broj_elemenata) throw "Neispravna vrijednost indeksa";
        return *l[indeks];
    }
    ~NizLista();
    NizLista (const NizLista &l1);
    NizLista &operator =(const NizLista &l1);
};

template <typename Tip>
void NizLista<Tip>::prosiriListu() {
    kapacitet*=2;
    Tip **pomocni=new Tip*[kapacitet]{};
    for(int i=0; i<broj_elemenata; i++) pomocni[i]=new Tip{*l[i]};
    for(int i=0; i<broj_elemenata; i++) {
        delete l[i]; l[i]=nullptr;
    }
    delete [] l;
    l=pomocni;
    pomocni=nullptr;
}

template <typename Tip>
void NizLista<Tip>::obrisi() {
    if(broj_elemenata==0) throw "Lista je prazna";
    delete l[tekuci];
    if(tekuci==broj_elemenata-1) {
        broj_elemenata--;
        if(tekuci!=0) tekuci--;
        return;
    }
    for(int i=tekuci; i<broj_elemenata-1; i++) l[i]=l[i+1];
    l[broj_elemenata-1]=nullptr;
    broj_elemenata--;
}

template <typename Tip> 
void NizLista<Tip>::dodajIspred(const Tip &el) {
    if(kapacitet==broj_elemenata) prosiriListu();
    if(broj_elemenata==0) {
        l[0]=new Tip{el};
        tekuci=0;
        broj_elemenata++;
    }
    else {
        for(int i=broj_elemenata; i>tekuci; i--) l[i]=l[i-1];
        l[tekuci]=new Tip{el};
        tekuci++;
        broj_elemenata++;
    }
}

template <typename Tip>
void NizLista<Tip>::dodajIza(const Tip &el) {
    if(broj_elemenata==kapacitet) prosiriListu();
    if(broj_elemenata==0) {
        l[0]=new Tip{el};
        tekuci=0;
        broj_elemenata++;
    }
    else {
        for(int i=broj_elemenata; i>tekuci+1; i--) l[i]=l[i-1];
        l[tekuci+1]=new Tip{el};
        broj_elemenata++;
    }
}

template <typename Tip>
NizLista<Tip>::~NizLista() {
    for(int i=0; i<broj_elemenata; i++) {
        delete l[i];
        l[i]=nullptr;
    }
    delete[] l; l=nullptr;
    kapacitet=0; broj_elemenata=0; tekuci=0;
}

template<typename Tip> 
NizLista<Tip>::NizLista (const NizLista &l1) {
    l=new Tip*[l1.kapacitet]{};
    for(int i=0; i<l1.broj_elemenata; i++) l[i]=new Tip{*(l1.l[i])};
    tekuci=l1.tekuci; kapacitet=l1.kapacitet; broj_elemenata=l1.broj_elemenata;
}

template<typename Tip>
NizLista<Tip> &NizLista<Tip>::operator=(const NizLista<Tip> &l1) {
    if(&l1==this) return *this;
    for(int i=0; i<broj_elemenata; i++) {
        delete l[i]; l[i]=nullptr;
    }
    delete[] l;
    l=new Tip*[l1.kapacitet]{};
    for(int i=0; i<l1.broj_elemenata; i++) l[i]=new Tip{*(l1.l[i])};
    tekuci=l1.tekuci; kapacitet=l1.kapacitet; broj_elemenata=l1.broj_elemenata;
    return *this;
}

template <typename Tip>
class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor (const Tip &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci) {}
        Cvor (Cvor *sljedeci=nullptr) : sljedeci(sljedeci) {}
    };
    int lduzina, dduzina;
    Cvor *p, *k, *tekuci;
    public:
    JednostrukaLista() {
        p=k=tekuci=new Cvor;
        lduzina=0; dduzina=0;
    }
    JednostrukaLista (const JednostrukaLista &l);
    JednostrukaLista &operator =(const JednostrukaLista &l);
    ~JednostrukaLista();
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
    bool sljedeci() override {
        if(brojElemenata()==0) throw "Lista je prazna";
        if(tekuci->sljedeci==k) return false;
        tekuci=tekuci->sljedeci;//
        dduzina--; lduzina++;
        return true;
    }
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
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista &l1) {
    p=k=tekuci=new Cvor;
    Cvor *pomocni=p;
    for(Cvor* i=l1.p; i->sljedeci!=nullptr;) {
        pomocni->sljedeci=new Cvor{i->sljedeci->element, nullptr};
        if(i==l1.tekuci) tekuci=pomocni;
        if(i->sljedeci==l1.k) k=pomocni->sljedeci; 
        i=i->sljedeci; pomocni=pomocni->sljedeci;
    }
    pomocni=nullptr;
    lduzina=l1.lduzina; dduzina=l1.dduzina;
}
template <typename Tip>
JednostrukaLista<Tip> &JednostrukaLista<Tip>::operator=(const JednostrukaLista<Tip> &l1) {
    if(&l1==this) return *this;
    for(Cvor *i=p->sljedeci; i!=nullptr;) {
        Cvor* pomocni=i;
        i=pomocni->sljedeci;
        delete pomocni;
    }
    delete p;
    p=tekuci=k=new Cvor;
    Cvor *pomocni=p;
    for(Cvor *i=l1.p; i->sljedeci!=nullptr;) {
        pomocni->sljedeci=new Cvor{i->sljedeci->element, nullptr};
        if(i==l1.tekuci) tekuci=pomocni;
        if(i->sljedeci==l1.k) k=pomocni->sljedeci;
        i=i->sljedeci; pomocni=pomocni->sljedeci;
    }
    pomocni=nullptr;
    lduzina=l1.lduzina; dduzina=l1.dduzina;
    return *this;
}
template <typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista() {
    for(Cvor *i=p->sljedeci; i!=nullptr;) {
        Cvor* pomocni=i;
        i=pomocni->sljedeci;
        delete pomocni;
    }
    delete p;
    p=k=tekuci=nullptr;
    lduzina=dduzina=0;
}

template <typename Tip>
bool JednostrukaLista<Tip>::prethodni() {
    if(brojElemenata()==0) throw "Lista je prazna";
    if(tekuci==p) return false;
    for(Cvor* i=p; i!=k; i=i->sljedeci) {
        if(i->sljedeci==tekuci) {
            tekuci=i; break;
        }
    }
    lduzina--; dduzina++;
    return true;
}

template <typename Tip>
void JednostrukaLista<Tip>::kraj() {
    if(brojElemenata()==0) throw "Lista je prazna";
    for(Cvor* i=p; i!=k; i=i->sljedeci) {
        if(i->sljedeci==k) {
            tekuci=i; break;
        }
    }
    lduzina=brojElemenata()-1;
    dduzina=1;
}

template <typename Tip>
void JednostrukaLista<Tip>::obrisi() {
    if(brojElemenata()==0) throw "Lista je prazna";
    Cvor *pomocni=tekuci->sljedeci;
    tekuci->sljedeci=pomocni->sljedeci;
    delete pomocni;
    dduzina--;
    if(tekuci->sljedeci==nullptr) {
        Cvor* i=p;
        if(i->sljedeci==nullptr) k=p;
        for(; i->sljedeci!=nullptr;) {
            if(i->sljedeci==tekuci) {
                tekuci=i; 
                k=i->sljedeci; 
                break;
            }
            i=i->sljedeci;
        }
    }
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred (const Tip &el) {
    if(brojElemenata()==0) {
        p->sljedeci=new Cvor{el, nullptr};
        dduzina++;
    }
    else {
        Cvor *pomocni=tekuci->sljedeci;
        tekuci->sljedeci=new Cvor{el, pomocni};
        pomocni=nullptr;
        lduzina++;
        tekuci=tekuci->sljedeci;
    }
    if(k->sljedeci!=nullptr) {
        k=tekuci;
        for(int i=0; i<dduzina; i++) k=k->sljedeci;
    }
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza (const Tip &el) {
    if(brojElemenata()==0) {
        p->sljedeci=new Cvor{el, nullptr};
        dduzina++;
    }
    else {
        Cvor *pomocni=tekuci->sljedeci->sljedeci;
        tekuci->sljedeci->sljedeci=new Cvor{el, pomocni};
        pomocni=nullptr;
        dduzina++;
    }
    if(k->sljedeci!=nullptr) {
        k=tekuci;
        for(int i=0; i<dduzina; i++) k=k->sljedeci;
    }
}

template <typename Tip>
Tip &JednostrukaLista<Tip>::operator[](int indeks) {
    if(indeks<0 || indeks>=brojElemenata()) throw "Neispravan indeks";
    if(indeks<lduzina) {
        int i=0;
        for(Cvor* ip=p; ip!=k; ip=ip->sljedeci) {
            if(i==indeks) return ip->sljedeci->element;
            i++;
        }
    }
    else {
        int i=lduzina;
        for(Cvor *ip=tekuci; ip!=k; ip=ip->sljedeci) {
            if(i==indeks) return ip->sljedeci->element;
            i++;
        }
    }
}

template <typename Tip>
Tip JednostrukaLista<Tip>::operator[](int indeks) const {
    if(indeks<0 || indeks>=brojElemenata()) throw "Neispravan indeks";
    if(indeks<lduzina) {
        int i=0;
        for(Cvor* ip=p; ip!=k; ip=ip->sljedeci) {
            if(i==indeks) return ip->sljedeci->element;
            i++;
        }
    }
    else {
        int i=lduzina;
        for(Cvor *ip=tekuci; ip!=k; ip=ip->sljedeci) {
            if(i==indeks) return ip->sljedeci->element;
            i++;
        }
    }
}

//Testiranje klase NizLista

void testirajBrojElemenataNL() {
    NizLista<int> l;
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
void testirajTrenutniNL() {
    NizLista<int> l;
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
void testirajPrethodniNL() {
    NizLista<int> l;
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
void testirajSljedeciNL() {
    NizLista<int> l;
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
void testirajPocetakNL() {
    NizLista<int> l;
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
void testirajKrajNL() {
    NizLista<int> l;
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
void testirajObrisiNL() {
    NizLista<int> l;
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
void testirajDodajIspredNL() {
    NizLista<int> l;
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
void testirajDodajIzaNL() {
    NizLista<int> l;
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
void testirajOperatorUglasteZagradeNL() {
    NizLista<int> l;
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
void testirajKopirajuciKonstruktorNL() {
    NizLista<int> l1;
    l1.dodajIza(1); l1.dodajIza(2); l1.dodajIza(3);
    NizLista<int> l2(l1);
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
void testirajKopirajuciOperatorDodjeleNL() {
    NizLista<int> l1;
    l1.dodajIza(1); l1.dodajIza(2); l1.dodajIza(3);
    NizLista<int> l2;
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

//Testiranje klase JednostrukaLista

void testirajBrojElemenataJL() {
    JednostrukaLista<int> l;
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
void testirajTrenutniJL() {
    JednostrukaLista<int> l;
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
void testirajPrethodniJL() {
    JednostrukaLista<int> l;
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
void testirajSljedeciJL() {
    JednostrukaLista<int> l;
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
void testirajPocetakJL() {
    JednostrukaLista<int> l;
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
void testirajKrajJL() {
    JednostrukaLista<int> l;
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
void testirajObrisiJL() {
    JednostrukaLista<int> l;
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
void testirajDodajIspredJL() {
    JednostrukaLista<int> l;
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
void testirajDodajIzaJL() {
    JednostrukaLista<int> l;
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
void testirajOperatorUglasteZagradeJL() {
    JednostrukaLista<int> l;
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
void testirajKopirajuciKonstruktorJL() {
    JednostrukaLista<int> l1;
    l1.dodajIza(1); l1.dodajIza(2); l1.dodajIza(3);
    JednostrukaLista<int> l2(l1);
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
void testirajKopirajuciOperatorDodjeleJL() {
    JednostrukaLista<int> l1;
    l1.dodajIza(1); l1.dodajIza(2); l1.dodajIza(3);
    JednostrukaLista<int> l2;
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
int main() {
    //Testiranje klase NizLista
    testirajBrojElemenataNL();
    testirajTrenutniNL();
    testirajPrethodniNL();
    testirajSljedeciNL();
    testirajPocetakNL();
    testirajKrajNL();
    testirajObrisiNL();
    testirajDodajIspredNL();
    testirajDodajIzaNL();
    testirajOperatorUglasteZagradeNL();
    testirajKopirajuciKonstruktorNL();
    testirajKopirajuciOperatorDodjeleNL();
    //Testiranje klase JednostrukaLista
    testirajBrojElemenataJL();
    testirajTrenutniJL();
    testirajPrethodniJL();
    testirajSljedeciJL();
    testirajPocetakJL();
    testirajKrajJL();
    testirajObrisiJL();
    testirajDodajIspredJL();
    testirajDodajIzaJL();
    testirajOperatorUglasteZagradeJL();
    testirajKopirajuciKonstruktorJL();
    testirajKopirajuciOperatorDodjeleJL();
    return 0;
}