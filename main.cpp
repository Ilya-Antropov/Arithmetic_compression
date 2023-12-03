#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

struct Range {
    char s;
    int numb;
    int lb;
    int rb;
};

struct Sort {
    bool operator() (Range l, Range r) {
        return l.numb > r.numb;
    }
};

double encode(ifstream& f, ofstream& g){
    int count=0;
    map <char, int> m;
    map <char, int> ::iterator ii;
    list<Range> L;
    while(!f.eof()) {
        char c=f.get();
        m[c]++;
        count++;
    }
    for(ii=m.begin(); ii!=m.end(); ii++) {
        Range p;
        p.s=ii->first;
        p.numb=ii->second;
        L.push_back(p);
    }
    L.sort(Sort());
    L.begin()->rb=L.begin()->numb;
    L.begin()->lb=0;
    list<Range>::iterator it=L.begin(), i2;
    i2=it;
    it++;
    for(; it!=L.end(); it++) {
        it->lb=i2->rb;
        it->rb=it->lb+it->numb;
        i2++;
    }
    int ik=0;
    for (ii = m.begin(); ii != m.end(); ii++) {

        if (ii->second != 0) ik += 40;
    }
    g.write((char*)(&ik), sizeof(ik));
    for (int k=0; k<256; k++) {
        if (m[char(k)]>0) {
            char c=char(k);
            g.write((char*)(&c), sizeof(c));
            g.write((char*)(&m[char(k)]), sizeof(m[char(k)]));
        }
    }
    f.clear();
    f.seekg(0);
    int l=0, h=65535, i=0, delitel=L.back().rb;
    int F_q=(h+1)/4, Half=F_q*2, T_q=F_q*3, bits_to_follow=0;
    char tx=0;
    count=0;
    while(!f.eof()) {
        char c=f.get(); i++;
        for(it=L.begin(); it!=L.end(); it++) {
            if(c==it->s) break;
        }
        if(c!=it->s) {
            cout<<"Error!"<<endl;
            break;
        }
        int l2=l;
        l=l+it->lb*(h-l+1)/delitel;
        h=l2+it->rb*(h-l2+1)/delitel-1;
        for(;;) {
            if(h<Half) {
                count++;
                if(count==8) {
                    count = 0;
                    g << tx;
                    tx = 0;
                }
                for(; bits_to_follow>0; bits_to_follow--) {
                    tx=tx | (1<<(7-count));
                    count++;
                    if(count==8) {
                        count = 0;
                        g << tx;
                        tx = 0;
                    }
                }
            }
            else if(l>=Half) {
                tx=tx | (1<<(7-count));
                count++;
                if(count==8) {
                    count = 0;
                    g << tx;
                    tx = 0;
                }
                for(; bits_to_follow>0; bits_to_follow--) {
                    count++;
                    if(count==8) {
                        count = 0;
                        g << tx;
                        tx = 0;
                    }
                }
                l-=Half;
                h-=Half;
            }
            else if((l>=F_q) && (h<T_q)) {
                bits_to_follow++;
                l-=F_q;
                h-=F_q;
            }
            else break;
            l+=l;
            h+=h+1;
        }
    }
    g<<tx;
    f.clear();
    f.seekg(0, std::ios::end);
    g.seekp(0, std::ios::end);
    double sizeF = f.tellg();
    double sizeG = g.tellp();
    f.close();
    g.close();
    return sizeG / sizeF;
}

int main() {
    ifstream f("text.txt", ios::out | ios::binary);
    ofstream g("code.txt", ios::out | ios::binary);
    double coef = encode(f, g);
    std::cout << coef << std::endl;
}
