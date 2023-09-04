#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

vector<int> sastaviRedove(queue<int> q0, queue<int> q1, queue<int> q2, queue<int> q3, queue<int> q4, 
queue<int> q5, queue<int> q6, queue<int> q7, queue<int> q8, queue<int> q9) {
    vector<int> za_vratiti;
    while(!q0.empty()) {
        za_vratiti.push_back(q0.front());
        q0.pop(); 
    }   
    while(!q1.empty()) {
        za_vratiti.push_back(q1.front());
        q1.pop(); 
    } 
    while(!q2.empty()) {
        za_vratiti.push_back(q2.front());
        q2.pop(); 
    } 
    while(!q3.empty()) {
        za_vratiti.push_back(q3.front());
        q3.pop(); 
    } 
    while(!q4.empty()) {
        za_vratiti.push_back(q4.front());
        q4.pop(); 
    } 
    while(!q5.empty()) {
        za_vratiti.push_back(q5.front());
        q5.pop(); 
    } 
    while(!q6.empty()) {
        za_vratiti.push_back(q6.front());
        q6.pop(); 
    } 
    while(!q7.empty()) {
        za_vratiti.push_back(q7.front());
        q7.pop(); 
    } 
    while(!q8.empty()) {
        za_vratiti.push_back(q8.front());
        q8.pop(); 
    } 
    while(!q9.empty()) {
        za_vratiti.push_back(q9.front());
        q9.pop(); 
    } 
    return za_vratiti;
}
bool daLiSuSviSoritrani(vector<int> pomocni, int temp1, int temp2) {
    bool sortirani = true;
    for(int i = 0; i < pomocni.size(); i++) {
        if((pomocni[i]%temp1)/temp2 != 0) sortirani = false;
    }
    return sortirani;
}
void radixSort(vector<int> &a) {
    vector<int> pomocni(a);
    int temp1 = 10, temp2 = 1;
    while(true) {
        queue<int> q0, q1, q2, q3, q4, q5, q6, q7, q8, q9;
        for(int i = 0; i < pomocni.size(); i++) {
            int cifra = pomocni[i]%temp1;
            cifra /= temp2;
            switch(cifra) {
                case 0:
                q0.push(pomocni[i]);
                break;
                case 1:
                q1.push(pomocni[i]);
                break;
                case 2:
                q2.push(pomocni[i]);
                break;
                case 3:
                q3.push(pomocni[i]);
                break;
                case 4:
                q4.push(pomocni[i]);
                break;
                case 5:
                q5.push(pomocni[i]);
                break;
                case 6:
                q6.push(pomocni[i]);
                break;
                case 7:
                q7.push(pomocni[i]);
                break;
                case 8:
                q8.push(pomocni[i]);
                break;
                case 9:
                q9.push(pomocni[i]);
                break;
            }
        }
        pomocni = sastaviRedove(q0, q1, q2, q3, q4, q5, q6, q7, q8, q9);
        temp1 *= 10;
        temp2 *= 10;
        if (daLiSuSviSoritrani(pomocni, temp1, temp2)) break;
    }
    a = pomocni;
}
bool jeLiList(vector<int> a, int i, int velicina) {
    return i >= velicina/2 && i < velicina;
}
int lijevoDijete(int i) {
    return 2 * i + 1;
}
int desnoDijete(int i) {
    return 2 * i + 2;
}
void popraviDolje(vector<int> &a, int i, int velicina) {
    while(!jeLiList(a, i, velicina)) {
        int veci = lijevoDijete(i);
        int dd = desnoDijete(i);
        if(dd < velicina && a[dd] > a[veci]) veci = dd;
        if(a[i] > a[veci]) return;
        int temp = a[i];
        a[i] = a[veci];
        a[veci] = temp;
        i = veci;
    }
}
void stvoriGomilu(vector<int> &a) {
    for(int i = (a.size()/2) - 1; i >= 0; i--) {
        popraviDolje(a, i, a.size());
    }
}
int roditelj(int i) {
    return (i - 1) / 2;
}
void popraviGore(vector<int> &a, int i) {
    while(i != 0 && a[i] > a[roditelj(i)]) {
        int temp = a[i];
        a[i] = a[roditelj(i)];
        a[roditelj(i)] = temp;
        i = roditelj(i);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    if(velicina >= a.size()) a.push_back(umetnuti);
    else a[velicina] = umetnuti;
    velicina ++;
    popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {
    if(a.size() == 0) throw "Gomila je prazna!";
    int temp = a[0];
    a[0] = a[velicina - 1];
    a[velicina - 1] = temp;
    velicina --;
    if(velicina != 0) popraviDolje(a,0, velicina);    
    return a[velicina];
}

void gomilaSort(vector<int> &a) {
    int velicina = a.size();
    stvoriGomilu(a);
    for(int i = a.size() - 1; i >=1; i--) {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        velicina --;
        popraviDolje(a, 0, velicina);
    }
}

int main() {

    return 0;
}