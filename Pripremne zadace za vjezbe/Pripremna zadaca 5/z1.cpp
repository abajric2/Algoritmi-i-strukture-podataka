#include <iostream>

using namespace std;

int fib2_0(int n, int prosli=1, int pretprosli=0) {
    static int i=0, granica=n;
    if(i==granica-1) return n;
    int rezultat=prosli+pretprosli;
    pretprosli=prosli;
    prosli=rezultat;
    i++;
    return fib2_0(rezultat, prosli, pretprosli);
}
int nzd(int x, int y) {
    if(y==0) return x;
    return nzd(y, x%y);
}
int main() {

    return 0;
}