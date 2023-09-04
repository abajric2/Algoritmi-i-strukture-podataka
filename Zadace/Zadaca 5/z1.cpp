#include <iostream>
#include <utility>
#include <vector>
#include <queue>

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
class CvorHMapa {
    public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    CvorHMapa *sljedeci;
    CvorHMapa(TipKljuca kljuc = TipKljuca(), TipVrijednosti vrijednost = TipVrijednosti(), CvorHMapa *sljedeci = nullptr) 
    : kljuc(kljuc), vrijednost(vrijednost), sljedeci(sljedeci) {}
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
    TipVrijednosti err = TipVrijednosti();
    CvorHMapa<TipKljuca, TipVrijednosti> **mapa;
    int trenutna_velicina, kapacitet;
    unsigned int (*hashfun)(TipKljuca ulaz, unsigned int opseg_izlaza);
    public:
    HashMapaLan(unsigned int hfun (TipKljuca ulaz, unsigned int opseg_izlaza) = nullptr, int vel = 100) {
      trenutna_velicina = 0;
        kapacitet = vel;
        hashfun = hfun;
    } 
    HashMapaLan(const HashMapaLan &m);
    HashMapaLan &operator = (const HashMapaLan &m);
    int brojElemenata() const override {
        return trenutna_velicina;
    }
    void obrisi() override {
        if(trenutna_velicina == 0) return;
        for(int i = 0; i < kapacitet; i++) {
        if(mapa[i]) {
            while(mapa[i]) {
                CvorHMapa<TipKljuca, TipVrijednosti> *temp = mapa[i];
                mapa[i] = mapa[i]->sljedeci;
                delete temp;
            }
            mapa[i] = nullptr;
        }
        }
        delete [] mapa;
        trenutna_velicina = 0;
    }
    ~HashMapaLan() {
        this->obrisi();
    }
    TipVrijednosti &operator[] (const TipKljuca &kljuc) override {
        if(!hashfun) throw "Hash funkcija nije definisana";
        unsigned int indeks = hashfun(kljuc, kapacitet);
        if(trenutna_velicina == 0) {
            mapa = new CvorHMapa<TipKljuca, TipVrijednosti>*[kapacitet]{};
            mapa[indeks] = new CvorHMapa<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
            trenutna_velicina ++;
            for(int i = 0; i < kapacitet; i++) {
                if(i != indeks) mapa[i] = nullptr;
            }
            return mapa[indeks]->vrijednost;
        }
        else {
            if(mapa[indeks]) {
                CvorHMapa<TipKljuca, TipVrijednosti> *pok = mapa[indeks];
                while(pok) {
                    if(pok->kljuc == kljuc) return pok->vrijednost;
                    pok = pok->sljedeci;
                }
            }
            if(!mapa[indeks]) {
                mapa[indeks] = new CvorHMapa<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
                trenutna_velicina ++;
                return mapa[indeks]->vrijednost;
            }
            else {
                if(kljuc < mapa[indeks]->kljuc) {
                    CvorHMapa<TipKljuca, TipVrijednosti> *temp = mapa[indeks];
                    mapa[indeks] = new CvorHMapa<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
                    mapa[indeks]->sljedeci = temp;
                    trenutna_velicina ++;
                    return mapa[indeks]->vrijednost;
                }
                CvorHMapa<TipKljuca, TipVrijednosti> *temp1 = mapa[indeks];
                CvorHMapa<TipKljuca, TipVrijednosti> *temp2 = mapa[indeks];
                while(temp2->sljedeci && temp2->kljuc < kljuc) {
                    temp1 = temp2;
                    temp2 = temp2->sljedeci;
                }
                if(!temp2->sljedeci) {
                    temp2->sljedeci = new CvorHMapa<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
                    trenutna_velicina ++;
                    return temp2->sljedeci->vrijednost; 
                }
                else {
                    CvorHMapa<TipKljuca, TipVrijednosti> *temp3 = new CvorHMapa<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
                    temp1->sljedeci = temp3;
                    temp3->sljedeci = mapa[indeks];
                    trenutna_velicina ++;
                    return temp3->vrijednost;
                }
            }
        }
    }
    TipVrijednosti operator[] (const TipKljuca &kljuc) const override {
        if(!hashfun) throw "Hash funkcija nije definisana";
        unsigned int indeks = hashfun(kljuc, kapacitet);
        if(mapa[indeks]) {
            CvorHMapa<TipKljuca, TipVrijednosti> *temp = mapa[indeks];
            while(temp) {
                if(temp->kljuc == kljuc) return temp->vrijednost;
                temp = temp->sljedeci;
            }
        }
        return err;
    }
    void obrisi(const TipKljuca &kljuc) override {
        if(!hashfun) throw "Hash funkcija nije definisana";
        bool postoji = false;
        unsigned int indeks = hashfun(kljuc, kapacitet);
        if(mapa[indeks]) {
            if(mapa[indeks]->kljuc == kljuc) {
                postoji = true;
                trenutna_velicina --;
                mapa[indeks]->vrijednost = TipVrijednosti();
                return;
            }
            CvorHMapa<TipKljuca, TipVrijednosti> *p = mapa[indeks];
            while(p) {
                if(p->sljedeci) {
                    CvorHMapa<TipKljuca, TipVrijednosti> *temp = p->sljedeci;
                    if(temp->kljuc == kljuc) {
                        postoji = true;
                        temp->vrijednost = TipVrijednosti();
                        break;
                    }
                }
                p = p->sljedeci;
            } 
            trenutna_velicina --;
        }
        if(!postoji) throw "Element ne postoji";
    }
    void definisiHashFunkciju(unsigned int hfun (TipKljuca ulaz, unsigned int opseg_izlaza)) {
        hashfun = hfun;
    }
};
template <typename TipKljuca, typename TipVrijednosti>
HashMapaLan<TipKljuca, TipVrijednosti>::HashMapaLan(const HashMapaLan &m) {
    trenutna_velicina = m.trenutna_velicina;
    kapacitet = m.kapacitet;
    mapa = new CvorHMapa<TipKljuca, TipVrijednosti>*[m.kapacitet]{};
    hashfun = m.hashfun;
    for(int i = 0; i<m.kapacitet; i++) {
        if(m.mapa[i]) {
            mapa[i] = new CvorHMapa<TipKljuca, TipVrijednosti>(m.mapa[i]->kljuc, m.mapa[i]->vrijednost);
            CvorHMapa<TipKljuca, TipVrijednosti> *j = mapa[i], *k = m.mapa[i];
            while(k->sljedeci) {
                j->sljedeci = new CvorHMapa<TipKljuca, TipVrijednosti>();
                k = k->sljedeci;
                j->sljedeci->kljuc = k->kljuc;
                j->sljedeci->vrijednost = j->vrijednost;
            }
        }
    }
}
template <typename TipKljuca, typename TipVrijednosti>
HashMapaLan<TipKljuca, TipVrijednosti> &HashMapaLan<TipKljuca, TipVrijednosti>::operator = (const HashMapaLan &m) {
    if(&m == this) return *this;
    for(int i = 0; i < kapacitet; i++) {
        if(mapa[i]) {
            while(mapa[i]) {
                CvorHMapa<TipKljuca, TipVrijednosti> *temp = mapa[i];
                mapa[i] = mapa[i]->sljedeci;
                delete temp;
            }
            mapa[i] = nullptr;
        }
    }
    delete [] mapa;
    mapa = new CvorHMapa<TipKljuca, TipVrijednosti>*[kapacitet]{};
    kapacitet = m.kapacitet;
    trenutna_velicina = m.trenutna_velicina;
    hashfun = m.hashfun;
    for(int i = 0; i<m.kapacitet; i++) {
        if(m.mapa[i]) {
            mapa[i] = new CvorHMapa<TipKljuca, TipVrijednosti>(m.mapa[i]->kljuc, m.mapa[i]->vrijednost);
            CvorHMapa<TipKljuca, TipVrijednosti> *j = mapa[i], *k = m.mapa[i];
            while(k->sljedeci) {
                j->sljedeci = new CvorHMapa<TipKljuca, TipVrijednosti>();
                k = k->sljedeci;
                j->sljedeci->kljuc = k->kljuc;
                j->sljedeci->vrijednost = j->vrijednost;
            }
        }
    }
    return *this;
}

template <typename TipOznake>
class UsmjereniGraf;
template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class Cvor {
    int broj_cvora;
    std::vector<Cvor<TipOznake>> *susjedi;
    UsmjereniGraf<TipOznake> *graf;
    TipOznake oznaka;
    public:
    Cvor() : broj_cvora(0), graf(nullptr), oznaka(TipOznake()), susjedi(nullptr){}
    Cvor (int broj_cvora, UsmjereniGraf<TipOznake> *ug) : broj_cvora(broj_cvora), graf(ug),
    susjedi(nullptr), oznaka(TipOznake()){}///////
   // ~Cvor() {}
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake oznaka) {this->oznaka = oznaka;}
    int dajRedniBroj() const {return broj_cvora;}
};

template <typename TipOznake>
class Grana {
    int polazni_cvor, dolazni_cvor;
    float tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *graf;
    public:
    Grana() : polazni_cvor(0), dolazni_cvor(0), tezina(0), oznaka(TipOznake()),
    graf(nullptr){}
    Grana(int polazni_cvor, int dolazni_cvor, UsmjereniGraf<TipOznake> *ug, float tezina_grane)
    : polazni_cvor(polazni_cvor), dolazni_cvor(dolazni_cvor), graf(ug), tezina(tezina_grane) {}
    ~Grana() {}
    float dajTezinu() const {return tezina;}
    void postaviTezinu(float tezina_grane) {
        graf->dajGranu(polazni_cvor, dolazni_cvor).tezina = tezina_grane;
        tezina = tezina_grane;//////////////////
     //   graf->postaviTezinuGrane(polazni_cvor, dolazni_cvor, tezina);
        //dajGranu(polazni_cvor, dolazni_cvor).tezina = tezina_grane;/////////
    }
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake oznaka) {
        graf->dajGranu(polazni_cvor, dolazni_cvor).oznaka = oznaka;
        this->oznaka = oznaka;/////////
        //graf->postaviOznakuGrane(polazni_cvor, dolazni_cvor, oznaka);
      //  graf->dajGranu(polazni_cvor, dolazni_cvor).oznaka = oznaka;
    }
    bool operator==(Grana<TipOznake> grana) {
        return (grana.polazni_cvor == polazni_cvor && grana.dolazni_cvor == dolazni_cvor);
    }
    Cvor<TipOznake> dajPolazniCvor() const {
        return graf->dajCvor(polazni_cvor);
    }
    Cvor<TipOznake> dajDolazniCvor() const {
        return graf->dajCvor(dolazni_cvor);
    }
};

template <typename TipOznake>
class UsmjereniGraf {
    public:
    UsmjereniGraf() {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int broj_cvorova) = 0;
    virtual void dodajGranu(int polazni_cvor, int dolazni_cvor, float tezina_grane = 0) = 0;
    virtual void obrisiGranu(int polazni_cvor, int dolazni_cvor) = 0;
    virtual void postaviTezinuGrane(int polazni_cvor, int dolazni_cvor, float tezina_grane = 0) = 0;
    virtual float dajTezinuGrane(int polazni_cvor, int dolazni_cvor) = 0;
    virtual bool postojiGrana(int polazni_cvor, int dolazni_cvor) const = 0;
    virtual void postaviOznakuCvora(int broj_cvora, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int broj_cvora) const = 0;
    virtual void postaviOznakuGrane(int polazni_cvor, int dolazni_cvor, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazni_cvor, int dolazni_cvor) = 0;
    virtual Grana<TipOznake> &dajGranu(int polazni_cvor, int dolazni_cvor) = 0;
    virtual Cvor<TipOznake> &dajCvor(int broj_cvora) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake>
class GranaIterator {
    int polazni_cvor, dolazni_cvor;
    UsmjereniGraf<TipOznake> *graf;
    public:
    GranaIterator(int polazni_cvor, int dolazni_cvor, UsmjereniGraf<TipOznake> *ug) :
    polazni_cvor(polazni_cvor), dolazni_cvor(dolazni_cvor), graf(ug) {}
    Grana<TipOznake> operator*() {
        return graf->dajGranu(polazni_cvor, dolazni_cvor);
    }
    bool operator==(const GranaIterator &iter) const {
        return polazni_cvor == iter.polazni_cvor && dolazni_cvor == iter.dolazni_cvor && graf == iter.graf;
    }
    bool operator!=(const GranaIterator &iter) const {
        return polazni_cvor != iter.polazni_cvor || dolazni_cvor != iter.dolazni_cvor || graf != iter.graf;
    }
    GranaIterator& operator++() {
        do {
            if(dolazni_cvor + 1 >= graf->dajBrojCvorova()) {
                dolazni_cvor = 0;
                polazni_cvor ++;
            }
            else dolazni_cvor ++;
        } while(polazni_cvor < graf->dajBrojCvorova() && graf->postojiGrana(polazni_cvor, dolazni_cvor) == false);
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator temp(*this);
        ++(*this);
        return temp;
    }
};

template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
    vector<vector<int>> l;
    vector<Cvor<TipOznake>> cvorovi;
    vector<Grana<TipOznake>> grane;
    public:
    ListaGraf(int broj_cvorova) {
        if(broj_cvorova <= 0) throw "Neispravan broj cvorova";
        for(int i = 0; i < broj_cvorova; i++) {
            cvorovi.push_back(Cvor<TipOznake>(i, this));
            l.push_back(vector<int>());
        }
    }
    ~ListaGraf() {}
    ListaGraf (const ListaGraf &lg);
    ListaGraf &operator =(const ListaGraf &lg);
    int dajBrojCvorova() const override {return cvorovi.size();}
    void postaviBrojCvorova(int broj_cvorova) override{
        if(broj_cvorova < l.size()) throw "Neispravan broj cvorova";
      //  int temp = l.size();
        for(int i = l.size(); i < broj_cvorova; i++) {
            cvorovi.push_back(Cvor<TipOznake>(i, this));
            l.push_back(vector<int>());
        }
    }
    void dodajGranu(int polazni_cvor, int dolazni_cvor, float tezina_grane = 0) override{
        if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
            throw "Pogresan opseg";
        for(int i = 0; i < l[polazni_cvor].size(); i++) {
            if(l[polazni_cvor][i] == dolazni_cvor) throw "Grana vec postoji";
        }
        l[polazni_cvor].push_back(dolazni_cvor);
        grane.push_back(Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, tezina_grane));
    }
    void obrisiGranu(int polazni_cvor, int dolazni_cvor) override{
        for(int i = 0; i < l[polazni_cvor].size(); i++) {
            if(l[polazni_cvor][i] == dolazni_cvor) {
                l[polazni_cvor].erase(l[polazni_cvor].begin() + i);
                break;
            }
        }
        for(int i = 0; i < grane.size(); i++) {
            grane[i] = Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0);
            grane.erase(grane.begin() + i);
            return;
        }
    }
    void postaviTezinuGrane(int polazni_cvor, int dolazni_cvor, float tezina_grane = 0) override{
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, tezina_grane)) {
                grane[i].postaviTezinu(tezina_grane);
                return;
            }
        }
        throw "Grana nije nadjena";
    }
    float dajTezinuGrane(int polazni_cvor, int dolazni_cvor) override{
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) return grane[i].dajTezinu();
        }
        throw "Grana nije nadjena";
    }
    bool postojiGrana(int polazni_cvor, int dolazni_cvor) const override{
        bool postoji = false;
        for(int i = 0; i < l[polazni_cvor].size(); i++) {
            if(l[polazni_cvor][i] == dolazni_cvor) {
                postoji = true;
                break;
            }
        }
        return postoji;
    }
    void postaviOznakuCvora(int broj_cvora, TipOznake oznaka) override{
        if(broj_cvora < 0 || broj_cvora >= cvorovi.size()) throw "Cvor ne postoji";
        cvorovi[broj_cvora].postaviOznaku(oznaka);
    }
    TipOznake dajOznakuCvora(int broj_cvora) const override{
        if(broj_cvora < 0 || broj_cvora >= cvorovi.size()) throw "Cvor ne postoji";
        return cvorovi[broj_cvora].dajOznaku();
    }
    void postaviOznakuGrane(int polazni_cvor, int dolazni_cvor, TipOznake oznaka) override{
        bool nadjena = false;
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) {
                grane[i].postaviOznaku(oznaka);
                nadjena = true;
                break;
            }
        }
        if(!nadjena) throw "Grana nije nadjena";
    }
    TipOznake dajOznakuGrane(int polazni_cvor, int dolazni_cvor) override{
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) {
                return grane[i].dajOznaku();
            }
        }
        throw "Grana nije nadjena";
    }
    Grana<TipOznake> &dajGranu(int polazni_cvor, int dolazni_cvor) override{
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) {
                return grane[i];
            }
        }
        throw "Grana nije nadjena";
    }
    Cvor<TipOznake> &dajCvor(int broj_cvora) override{
        return cvorovi[broj_cvora];
    }
    GranaIterator<TipOznake> dajGranePocetak() override{
        return ++GranaIterator<TipOznake>(0, -1, this);
    }
    GranaIterator<TipOznake> dajGraneKraj() override{
        return GranaIterator<TipOznake>(dajBrojCvorova(), 0, this);
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *g, vector<Cvor<TipOznake>> &cvorovi, Cvor<TipOznake> polazni_cvor) {
    cvorovi.push_back(polazni_cvor);
    queue<Cvor<TipOznake>> q;
    q.push(polazni_cvor);
    while(q.empty() == false && cvorovi.size() != g->dajBrojCvorova()) {
        Cvor<TipOznake> c = q.front();
        q.pop();
        vector<Cvor<TipOznake>> izlaz;
        for(int i = 0; i < g->dajBrojCvorova(); i++) {
            if(g->postojiGrana(c.dajRedniBroj(), i) == true) {
                q.push(g->dajCvor(i));
                izlaz.push_back(g->dajCvor(i));
            }
        }
        bool oznacen = false;
        for(int i = 0; i < izlaz.size(); i++) {
            oznacen = false;
            for(int j = 0; j < cvorovi.size(); j++) {
                if(cvorovi[j].dajRedniBroj() == izlaz[i].dajRedniBroj()) oznacen = true;
            }
            if(oznacen == false) cvorovi.push_back(izlaz[i]);
        }
    }
}
template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *g, vector<Cvor<TipOznake>> &cvorovi, Cvor<TipOznake> polazni_cvor) {
    bool oznacen = false;
    for(int i = 0; i < cvorovi.size(); i++) {
        if(polazni_cvor.dajRedniBroj() == cvorovi[i].dajRedniBroj()) {
            oznacen = true;
            break;
        }
    }
    if(oznacen == false) cvorovi.push_back(polazni_cvor);
    for(int i = 0; i < g->dajBrojCvorova(); i++) {
        if(g->postojiGrana(polazni_cvor.dajRedniBroj(), i) && oznacen == false) dfs(g, cvorovi, g->dajCvor(i));
    }
}
int main() {
    
    return 0;
}