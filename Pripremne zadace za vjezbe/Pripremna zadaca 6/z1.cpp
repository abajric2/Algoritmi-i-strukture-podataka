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
        for(int i = 0; i<trenutna_velicina; i++) pomocni[i] = mapa[i];
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

// Testiranje klase NizMapa

void test1() {
    NizMapa<int, string> m;
    m[1] = "prvi"; m[5] = "drugi"; m[1526] = "treci"; 
    m[5] = "drugidrugi";
    if(m.brojElemenata()==3 && m[5]=="drugidrugi") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m.obrisi(5);
    if(m.brojElemenata()==2 && m[5]=="" && m.brojElemenata()==3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m.obrisi();
    for(int i = 0; i<10000; i++) m[i]="element";
    if(m.brojElemenata() == 10000 && m[256] == "element") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    NizMapa<int, string> m2(m);
    m[999] = "el";
    if(m2.brojElemenata() == 10000 && m[999] == "el" && m2[999] == "element") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    NizMapa<int, string> m3;
    m3[100] = "stoti";
    m2 = m3;
    if(m2.brojElemenata() == 1 && m2[100] == "stoti") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m.obrisi(9999);
    if(m.brojElemenata() == 9999 && m[9999] == "") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    const NizMapa<int, string> cm;
    if(cm[100]=="" && cm.brojElemenata() == 0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void test2() {
    NizMapa<double, vector<string>> m;
    m[-125.585] = {"a", "b", "c"};
    m[0.0005] = {"A", "B", "C", "D"};
    vector<string> v;
    for(int i = 0; i < 10000; i++) v.push_back("element");
    m[100000.858] = v;
    if(m.brojElemenata() == 3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    bool greska = false;
    for(int i = 0; i < m[100000.858].size(); i++) if(m[100000.858][i] != "element") greska = true;
    if(greska) cout<<"NIJE OK"<<endl;
    else cout<<"OK"<<endl;
    NizMapa<double, vector<string>> m2(m);
    m.obrisi(0.0005);
    if(m2.brojElemenata() == 3 && m2[0.0005][3] == "D") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    const NizMapa<double, vector<string>> cm;
    if(cm[0.01].size() == 0 && cm.brojElemenata() == 0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m = cm;
    if(m[0.01].size() == 0 && m.brojElemenata() == 1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void test3() {
    NizMapa<vector<int>, vector<vector<int>>> m;
    m[{1,2,3}] = {{5,6},{7,8,9}};
    m[{0,0,0,0,0,0,0}] = {{1}, {2, 3}, {4, 5, 6}};
    if(m.brojElemenata() == 2 && m[{0,0,0,0,0,0,0}][1][1] == 3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl; 
    vector<vector<int>> v(10000);
    for(int i = 0; i < 10000; i++) v[i].resize(10000);
    for(int i = 0; i < 10000; i++)
        for(int j = 0; j < 10000; j++) v[i][j] = i+j;
    m[{555}] = v;
    if(m[{555}][9999].size() == 10000 && m[{555}][9999][9999] == 9999+9999) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m.obrisi({555});
    NizMapa<vector<int>, vector<vector<int>>> m2(m);
    m.obrisi();
    if(m2.brojElemenata() == 2 && m2[{1,2,3}][0][0] == 5) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    const NizMapa<vector<int>, vector<vector<int>>> cm(m2);
    if(m2[{1,2,3}][0][0] == 5) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m = cm;
    if(m.brojElemenata() == 2 && m[{1,2,3}][0][0] == 5) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void test4() {
    NizMapa<vector<vector<string>>, vector<double>> m;
    vector<vector<string>> v(100);
    for(int i = 0; i < 100; i++) v[i].resize(100);
    for(int i = 0; i < 100; i++)
        for(int j = 0; j < 100; j++) v[i][j] = "";
    m[v] = {0.1, 0.2, 0.3};
    if(m[v][1] == 0.2 && m.brojElemenata() == 1) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m[{{"a", "b", "c"}, {"A", "B", "C"}}] = {0,0,0,0,0,0};
    if(m[{{"a", "b", "c"}, {"A", "B", "C"}}].size() == 6 && m[{{"a"}, {"b"}, {"c"}}].size() == 0) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    const NizMapa<vector<vector<string>>, vector<double>> cm(m);
    if(cm[{{"abb"}}].size() == 0 && cm.brojElemenata() == 3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    m = m; // test na destruktivnu samododjelu
    if (m.brojElemenata() == 3 && m[{{"a", "b", "c"}, {"A", "B", "C"}}].size() == 6) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
void test5() {
    NizMapa<string, string> m;
    m["a"] = "A"; m["b"] = "B"; m["c"] ="C";
    m["c"] += "CC";
    if(m.brojElemenata() == 3 && m["c"]=="CCC") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    const NizMapa<string, string> cm(m);
    if(cm["d"] == "" && cm.brojElemenata() == 3) cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
    NizMapa<string, string> m3;
    for(string s = "a"; s.size()<100; s += "a") m3[s] = s;
    m = m3;
    m3.obrisi(); m.obrisi("a");
    if(m["aaaaa"] == "aaaaa" && m["a"] == "" && m3["aaaaa"] == "") cout<<"OK"<<endl;
    else cout<<"NIJE OK"<<endl;
}
int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    return 0;
}