#include <iostream>
#include <vector>

using namespace std;

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
        tezina = tezina_grane;//////////////////
     //   graf->postaviTezinuGrane(polazni_cvor, dolazni_cvor, tezina);
        //dajGranu(polazni_cvor, dolazni_cvor).tezina = tezina_grane;/////////
    }
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake oznaka) {
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
    virtual const Grana<TipOznake> dajGranu(int polazni_cvor, int dolazni_cvor) const = 0;
    virtual Cvor<TipOznake> &dajCvor(int broj_cvora) = 0;
    virtual const Cvor<TipOznake> dajCvor(int broj_cvora) const = 0;
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
class MatricaGraf : public UsmjereniGraf<TipOznake> {
    vector<Cvor<TipOznake>> cvorovi;
    vector<Grana<TipOznake>> grane;
    vector<vector<bool>> matrica_susjedstva;
    public:
    MatricaGraf(int broj_cvorova);
    ~MatricaGraf() {}
    int dajBrojCvorova() const override {return cvorovi.size();}
    void postaviBrojCvorova(int broj_cvorova) override;
    void dodajGranu(int polazni_cvor, int dolazni_cvor, float tezina_grane = 0) override;
    void obrisiGranu(int polazni_cvor, int dolazni_cvor) override;
    void postaviTezinuGrane(int polazni_cvor, int dolazni_cvor, float tezina_grane = 0) override;
    float dajTezinuGrane(int polazni_cvor, int dolazni_cvor) override;
    bool postojiGrana(int polazni_cvor, int dolazni_cvor) const override {
        if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0) 
            return false;
        return matrica_susjedstva[polazni_cvor][dolazni_cvor];
    }
    void postaviOznakuCvora(int broj_cvora, TipOznake oznaka) override {
        if(broj_cvora < 0 || broj_cvora >= cvorovi.size()) throw "Cvor ne postoji";
        cvorovi[broj_cvora].postaviOznaku(oznaka);
    }
    TipOznake dajOznakuCvora(int broj_cvora) const override {
        if(broj_cvora < 0 || broj_cvora >= cvorovi.size()) throw "Cvor ne postoji";
        return cvorovi[broj_cvora].dajOznaku();
    }
    void postaviOznakuGrane(int polazni_cvor, int dolazni_cvor, TipOznake oznaka) override;
    TipOznake dajOznakuGrane(int polazni_cvor, int dolazni_cvor) override;
    Grana<TipOznake> &dajGranu(int polazni_cvor, int dolazni_cvor) override {
        for(int i = 0; i<grane.size(); i++){
        if(grane[i] == Grana<TipOznake>(polazni_cvor,dolazni_cvor, this,0))
            return grane[i];
        }
        throw std::domain_error("Grana nije nadjena");
        /*if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
            throw "Pogresan opseg";
        return Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0);*/
    }
    const Grana<TipOznake> dajGranu(int polazni_cvor, int dolazni_cvor) const override {
        if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
            throw "Pogresan opseg";
        return Grana<TipOznake>(polazni_cvor, dolazni_cvor, (const_cast<MatricaGraf*>(this)), 0);
    }
    Cvor<TipOznake> &dajCvor(int broj_cvora) override {
        if(broj_cvora < 0 || broj_cvora >= cvorovi.size()) throw "Cvor ne postoji";
        return cvorovi[broj_cvora];
    }
    const Cvor<TipOznake> dajCvor(int broj_cvora) const override {
        if(broj_cvora < 0 || broj_cvora >= cvorovi.size()) throw "Cvor ne postoji";
        return Cvor<TipOznake>(broj_cvora, (const_cast<MatricaGraf*>(this)));
    }
    GranaIterator<TipOznake> dajGranePocetak() override {
        return ++GranaIterator<TipOznake>(0, -1, this);
    }
    GranaIterator<TipOznake> dajGraneKraj() override {
        return GranaIterator<TipOznake>(dajBrojCvorova(), 0, this);
    }
};
template <typename TipOznake>
MatricaGraf<TipOznake>::MatricaGraf(int broj_cvorova) {
        if(broj_cvorova < 1) throw "Neipsravan broj cvorova";
        matrica_susjedstva.resize(broj_cvorova);
        for(int i = 0; i < broj_cvorova; i++) matrica_susjedstva.resize(broj_cvorova);
        for(int i = 0; i < broj_cvorova; i++) matrica_susjedstva[i].resize(broj_cvorova);
        for(int i = 0; i < broj_cvorova; i++) {
            for(int j = 0; j < broj_cvorova; j++) {
                matrica_susjedstva[i][j] = false;
            }
            cvorovi.push_back(Cvor<TipOznake>(i, this));
        }
    }
template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviBrojCvorova(int broj_cvorova) {
        if(broj_cvorova < matrica_susjedstva.size()) throw "Ne mozete postaviti manji broj cvorova od trenutnog";
        int stari_br_cvorova = matrica_susjedstva.size();
        for(int i = matrica_susjedstva.size(); i < broj_cvorova; i++) cvorovi.push_back(Cvor<TipOznake>(i, this));
        if(matrica_susjedstva.size() < broj_cvorova) matrica_susjedstva.resize(broj_cvorova);
        for(int i = 0; i < broj_cvorova; i++) matrica_susjedstva[i].resize(broj_cvorova);
        for(int i = 0; i < broj_cvorova; i++) {
            for(int j = 0; j < broj_cvorova; j++) {
                if(i >= stari_br_cvorova || j >= stari_br_cvorova) matrica_susjedstva[i][j] = false;
            }
        }
    }
template <typename TipOznake>
void MatricaGraf<TipOznake>::dodajGranu(int polazni_cvor, int dolazni_cvor, float tezina_grane) {
    if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
        throw "Pogresan opseg";
    if(matrica_susjedstva[polazni_cvor][dolazni_cvor]) throw "Grana vec postoji";
    matrica_susjedstva[polazni_cvor][dolazni_cvor] = true;
    grane.push_back(Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, tezina_grane));
}
template <typename TipOznake>
void MatricaGraf<TipOznake>::obrisiGranu(int polazni_cvor, int dolazni_cvor) {
    if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
        throw "Pogresan opseg";/////////////////
    matrica_susjedstva[polazni_cvor][dolazni_cvor] = false;
    for(int i = 0; i < grane.size(); i++) {
        if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) {
            grane.erase(grane.begin() + i);
            return;
        }
    }
}
template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviTezinuGrane(int polazni_cvor, int dolazni_cvor, float tezina_grane) {
    if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
        throw "Pogresan opseg";
    if(matrica_susjedstva[polazni_cvor][dolazni_cvor]) {
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, tezina_grane)) {
                grane[i].postaviTezinu(tezina_grane);
                return;
            }
        }
    }
    throw "Grana nije nadjena";
}
template <typename TipOznake>
float MatricaGraf<TipOznake>::dajTezinuGrane(int polazni_cvor, int dolazni_cvor) {
    if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
        throw "Pogresan opseg";
    if(matrica_susjedstva[polazni_cvor][dolazni_cvor]) {
        for(int i = 0; i < grane.size(); i++) {/////////////
            if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) return grane[i].dajTezinu();
        }
    }
    throw "Grana nije nadjena";
}
template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuGrane(int polazni_cvor, int dolazni_cvor, TipOznake oznaka) {
    if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
        throw "Pogresan opseg";
    for(int i = 0; i < grane.size(); i++){
        if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) grane[i].postaviOznaku(oznaka);
    }
}
template <typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuGrane(int polazni_cvor, int dolazni_cvor) {
    if(polazni_cvor >= cvorovi.size() || polazni_cvor < 0 || dolazni_cvor >= cvorovi.size() || dolazni_cvor < 0)
        throw "Pogresan opseg";
    for(int i = 0; i < grane.size(); i++) {
        if(grane[i] == Grana<TipOznake>(polazni_cvor, dolazni_cvor, this, 0)) return grane[i].dajOznaku();
    }
    throw "Grana nije nadjena";///////////
}
int main() {
        try {
        UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for (GranaIterator<bool> iter = g->dajGranePocetak();
        iter != g->dajGraneKraj(); ++iter)
        cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
        << (*iter).dajDolazniCvor().dajRedniBroj() << " "
        << (*iter).dajTezinu() << endl;
        delete g;
        } catch (const char izuzetak[]) {
        cout << izuzetak << endl;
        }
        return 0;
}