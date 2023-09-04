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
    else if(tekuci->sljedeci==k) {
        k->sljedeci=new Cvor{el, nullptr};
        dduzina++;
        k=k->sljedeci;
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
template <typename Tip>
class Red {
    JednostrukaLista<Tip> r;
    public:
    void brisi();
    void stavi(const Tip &el);
    Tip skini();
    Tip &celo();
    int brojElemenata() const;
    Tip test() {return r.trenutni();}
};
template <typename Tip>
void Red<Tip>::brisi() {
    r.pocetak();
    int granica=brojElemenata();
    for(int i=0; i<granica; i++) r.obrisi();
}
template <typename Tip>
void Red<Tip>::stavi(const Tip &el) {
    r.dodajIza(el);
    r.sljedeci();
}
template <typename Tip>
Tip Red<Tip>::skini() {
    if(brojElemenata()==0) throw "Red je prazan";
    r.pocetak();
    int el=r.trenutni();
    r.obrisi();
    return el;
}
template <typename Tip>
Tip &Red<Tip>::celo() {
    if(brojElemenata()==0) throw "Red je prazan";
    r.pocetak();
    return r.trenutni();
}
template <typename Tip>
int Red<Tip>::brojElemenata() const {
    return r.brojElemenata();
}


//Testiranje klase Red

void testirajBrisi() {
    Red<int> r;
    r.stavi(1); r.stavi(2); r.stavi(3);
    r.brisi();
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajStavi() {
    Red<int> r;
    r.stavi(1);
    if(r.celo()==1 && r.brojElemenata()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.brisi();
    r.stavi(5); r.stavi(6); r.stavi(7);
    if(r.celo()==5 && r.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajSkini() {
    Red<int> r;
    try {
        r.skini();
        cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    r.stavi(1); r.stavi(2); r.stavi(3);
    if(r.skini()==1 && r.brojElemenata()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(r.celo()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.skini(); r.skini();
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajCelo() {
    Red<int> r;
    try {
        r.celo();
        cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    r.stavi(1); r.stavi(2); r.stavi(3);
    if(r.celo()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.celo()=5;
    if(r.celo()==5) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajBrojElemenata() {
    Red<int> r;
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.stavi(1); r.stavi(2); r.stavi(3);
    if(r.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r.brisi();
    if(r.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajKopirajuciKonstruktor() {
    Red<int> r1, r2(r1);
    if(r2.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r1.stavi(1); r1.stavi(2); r2.stavi(3);
    if(r1.brojElemenata()==2 && r2.brojElemenata()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    Red<int> r3(r1);
    r1.stavi(4);
    if(r3.brojElemenata()==2 && r3.celo()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r1.brisi();
    if(r3.brojElemenata()==2 && r3.celo()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajKopirajuciOperatorDodjele() {
    Red<int> r1, r2;
    r1.stavi(1); r1.stavi(2); r1.stavi(3);
    r2.stavi(4); r2.stavi(5); r2.stavi(6); r2.stavi(7);
    r1=r2;
    r2.stavi(8); r2.celo()=10;
    if(r1.brojElemenata()==4 && r2.brojElemenata()==5 && r1.celo()==4 && r2.celo()==10) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    r2.brisi();
    if(r1.brojElemenata()==4 && r2.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
int main() {
    testirajBrisi();
    testirajStavi();
    testirajSkini();
    testirajCelo();
    testirajBrojElemenata();
    testirajKopirajuciKonstruktor();
    testirajKopirajuciOperatorDodjele();
    return 0;
}