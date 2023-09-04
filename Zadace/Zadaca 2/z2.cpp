#include <iostream>
#include <vector>

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
int binarnaPretragaVektora(vector<int> v, int trazeni) {
    int dno=0, vrh=int(v.size())-1;
    while(vrh>=dno) {
        int srednji=(dno+vrh)/2;
        if(v[srednji]==trazeni) return srednji;
        else if(v[srednji]>trazeni) vrh=srednji-1;
        else dno=srednji+1;
    }
    return -1;
}
void pretraga(Stek<vector<int> >& s, int trazeni) {
    Stek<vector<int> > pomocni;
    int indeks=-1, broj_preostalih=0;
    while(s.brojElemenata()!=0) {
        std::vector<int> v=s.skini();
        pomocni.stavi(v);
        if(v.size()>0 && v[0]<=trazeni && trazeni<=v[int(v.size())-1]) {
            indeks=binarnaPretragaVektora(v, trazeni);
            if(indeks==-1) break;     
            else {
                broj_preostalih=s.brojElemenata();
                break;
            }
        }
    }
    while(pomocni.brojElemenata()!=0) s.stavi(pomocni.skini());
    if(indeks==-1) cout<<"Nema elementa";
    else cout<<indeks<<" "<<broj_preostalih;
}
void test1() {
    Stek<vector<int>> s;
    cout<<"Ocekivani ispis: Nema elementa\nIspisuje: ";
    pretraga(s, 0);
    cout<<endl<<"-----------------------------------------"<<endl;
}
void test2() {
    Stek<vector<int>> s;
    s.stavi({-100,-85,-27,-1,0,7,9});
    s.stavi({16,25,88,102,125,254});
    s.stavi({255,258,260,270,299,300});
    s.stavi({301,302,303,304,305,306,307});
    s.stavi({1000,2000,3000,4000,5000});
    cout<<"Ocekivani ispis: 6 0\nIspisuje: ";
    pretraga(s, 9);
    cout<<endl<<"-----------------------------------------"<<endl;
}
void test3() {
    Stek<vector<int>> s;
    vector<int> v;
    for(int i=0; i<10000; i++) v.push_back(i);
    s.stavi(v);
    v.resize(0);
    for(int i=10000; i<20000; i++) v.push_back(i);
    s.stavi(v);
    v.resize(0);
    for(int i=20000; i<30000; i++) v.push_back(i);
    s.stavi(v);
    v.resize(0);
    cout<<"Ocekivani ispis: 1 1\nIspisuje: ";
    pretraga(s, 10001);
    cout<<endl<<"-----------------------------------------"<<endl;
}
int main() {
    test1();
    test2();
    test3();
    return 0;
}