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
    //cout<<"pozvan";
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

template <typename Tip>
class Stek {
    JednostrukaLista<Tip> l;
    public:
    Stek() {}
    void brisi();
    void stavi (const Tip &el);
    Tip skini();
    Tip &vrh();
    int brojElemenata() const;
};

template <typename Tip>
void Stek<Tip>::brisi() {
    l.pocetak();
    int br_el=l.brojElemenata();
    for(int i=0; i<br_el; i++) {
        l.obrisi(); 
    }
}
template <typename Tip>
void Stek<Tip>::stavi(const Tip &el) {
    l.dodajIspred(el); l.prethodni();
}
template <typename Tip>
Tip Stek<Tip>::skini() {
    l.pocetak();
    Tip za_vratiti=l.trenutni();
    l.obrisi();
    return za_vratiti;
}
template <typename Tip>
Tip &Stek<Tip>::vrh() {
    l.pocetak();
    return l.trenutni();
}
template <typename Tip>
int Stek<Tip>::brojElemenata() const {
    return l.brojElemenata();
}

//Testiranje klase Stek

void testirajBrisi() {
    Stek<int> s;
    s.stavi(1); s.stavi(2); s.stavi(3); 
    s.brisi();
    if(s.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.stavi(5); s.stavi(6);
    s.brisi();
    if(s.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajStavi() {
    Stek<int> s;
    s.stavi(1);
    if(s.brojElemenata()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.stavi(2); s.stavi(3);
    if(s.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajSkini() {
    Stek<int> s;
    try {
        s.skini(); cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    s.stavi(1); 
    if(s.skini()==1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(s.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.stavi(4); s.stavi(2); s.stavi(3);
    if(s.skini()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(s.brojElemenata()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajVrh() {
    Stek<int> s;
    try {
        s.vrh(); cout<<"NIJE OK"<<endl;
    }
    catch(...) {
        cout<<"OK"<<endl;
    }
    s.stavi(1); s.stavi(2); s.stavi(3);
    if(s.vrh()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.vrh()=5;
    if(s.vrh()==5) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.vrh()++;
    if(s.vrh()==6) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajBrojElemenata() {
    Stek<int> s;
    if(s.brojElemenata()==0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.stavi(1); s.stavi(2); s.stavi(3);
    if(s.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajKopirajuciKonstruktor() {
    Stek<int> s;
    s.stavi(1); s.stavi(2); s.stavi(3);
    Stek<int> s2(s);
    if(s2.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(s2.vrh()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s.skini();
    if(s2.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s2.stavi(4);
    if(s.brojElemenata()==2 && s2.brojElemenata()==4) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(s.vrh()==2) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    if(s2.vrh()==4) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void testirajKopirajuciOperatorDodjele() {
    Stek<int> s;
    s.stavi(1); s.stavi(2); s.stavi(3);
    Stek<int> s2;
    s2.stavi(4); s2.stavi(5); s2.stavi(6); s2.stavi(7);
    s2=s;
    if(s2.brojElemenata()==3 && s2.vrh()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s2.vrh()=10;
    if(s2.vrh()==10 && s.vrh()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    s2.stavi(11);
    s2=s2; //Testiranje na destruktivnu samododjelu
    if(s2.brojElemenata()==4) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
int main() {
    //Testiranje klase Stek
    testirajBrisi();
    testirajStavi();
    testirajSkini();
    testirajVrh();
    testirajBrojElemenata();
    testirajKopirajuciKonstruktor();
    testirajKopirajuciOperatorDodjele();
    return 0;
}