#include <iostream>
#include <ctime>
#include <string>



using namespace std;


unsigned int TheHash(int ulaz, unsigned int max){
    return ulaz+100;
}


template <typename T1, typename T2>
struct Cvor {
    T1 first;
    T2 second;
    Cvor<T1,T2>* desni=nullptr;
    Cvor<T1,T2>* lijevi=nullptr;
    Cvor<T1,T2>*parent=nullptr;

};


template <typename T1, typename T2>
struct par {
    T1 first=T1();
    T2 second=T2();
};


template <typename T1, typename T2>
class Mapa
{
public:
    Mapa<T1,T2>() {}
    virtual ~Mapa() {}
    virtual int brojElemenata()const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const T1 &kljuc)=0;
    virtual T2 operator[] (T1 n)const=0;
    virtual T2 &operator[] (T1 n)=0;

};


template <typename T1, typename T2>
class NizMapa : public Mapa<T1,T2>
{
    par<T1,T2>*mapa=nullptr;
    int broj_elemenata=0;
    int kapacitet=500;

public:

    NizMapa(): broj_elemenata(0)
    {
        mapa=new par<T1,T2>[kapacitet];
    }

    ~NizMapa()
    {
        delete[] mapa;
    }

    NizMapa(const NizMapa &m);
    NizMapa(NizMapa &&m);
    int brojElemenata()const
    {
        return broj_elemenata;
    }

    void obrisi();
    void obrisi(const T1 &kljuc);
    T2 operator[](T1 n)const;
    T2 &operator[](T1 n);
    NizMapa& operator=(const NizMapa &m);
    NizMapa& operator=(NizMapa &&m);


};


template <typename T1, typename T2>
NizMapa<T1,T2>::NizMapa(const NizMapa<T1,T2>&m)
{
    kapacitet=m.kapacitet;
    mapa=new par<T1,T2>[kapacitet];
    for(int i=0; i<m.brojElemenata(); i++) {
        mapa[i]=m.mapa[i];
        broj_elemenata++;
    }
}


template <typename T1, typename T2>
NizMapa<T1,T2>::NizMapa(NizMapa<T1,T2> &&m)
{
    if(this==&m) return;
    mapa=m.mapa;
    broj_elemenata=m.brojElemenata();
    kapacitet=m.kapacitet;

}



template <typename T1, typename T2>
void NizMapa<T1,T2>::obrisi()
{
    delete[] mapa;
    kapacitet=100;
    mapa=new par<T1,T2>[kapacitet];
    broj_elemenata=0;
}


template <typename T1, typename T2>
void NizMapa<T1,T2>::obrisi(const T1 &kljuc)
{
    for(int i=0; i<broj_elemenata; i++) {
        if(mapa[i].first==kljuc) {
            for(int j=i; j<broj_elemenata-1; j++) mapa[j]=mapa[j+1];
            broj_elemenata--;
        }
    }

}


template <typename T1, typename T2>
T2 NizMapa<T1,T2>::operator[] (T1 n) const
{
    for(int i=0; i<broj_elemenata; i++) {
        if(mapa[i].first==n) {
            return mapa[i].second;
        }
    }
    return T2();
}



template <typename T1, typename T2>
NizMapa<T1,T2> &NizMapa<T1,T2>::operator=(const NizMapa &m)
{
    if(this==&m) return *this;
    delete[] mapa;
    kapacitet=m.kapacitet;
    mapa=new par<T1,T2>[kapacitet];

    for(int i=0; i<m.brojElemenata(); i++) {
        mapa[i]=m.mapa[i];
    }
    broj_elemenata=m.broj_elemenata;
    return *this;

}


template <typename T1, typename T2>
NizMapa<T1,T2> &NizMapa<T1,T2>::operator=(NizMapa &&m)
{
    if(this==&m) return *this;
    delete[] mapa;
    mapa=m.mapa;
    broj_elemenata=m.broj_elemenata;
    kapacitet=m.kapacitet;
    return *this;
}



template <typename T1, typename T2>
T2 &NizMapa<T1,T2>::operator[] (T1 n)
{
    for(int i=0; i<broj_elemenata; i++) {
        if(mapa[i].first==n) {
            return mapa[i].second;
        }
    }

    if(broj_elemenata==kapacitet) {
        kapacitet*=2;
        par<T1,T2> *novi=new par<T1,T2>[kapacitet];
        for(int j=0; j<broj_elemenata; j++) {
            novi[j]=mapa[j];
        }
        delete[] mapa;
        mapa=novi;
    }

    broj_elemenata++;
    par<T1,T2> temp;
    temp.first=n;
    temp.second=T2();
    mapa[broj_elemenata-1]=temp;
    return mapa[broj_elemenata-1].second;

}



template <typename T1, typename T2>
class BinStabloMapa: public Mapa<T1,T2>
{
    Cvor<T1,T2>*korijen;
    int broj_elemenata;
public:
    BinStabloMapa(): korijen(nullptr), broj_elemenata(0) {}
    ~BinStabloMapa()
    {
        this->obrisi();
    }
    BinStabloMapa(const BinStabloMapa &b);
    BinStabloMapa(BinStabloMapa &&b);
    int brojElemenata()const
    {
        return broj_elemenata;
    }
    void obrisi();
    void obrisi(const  T1 &k);
    T2 &operator [](T1 k);
    T2 operator [](T1 k) const;
    BinStabloMapa &operator = (const BinStabloMapa &b);
    BinStabloMapa &operator=(BinStabloMapa &&b);
    void ppkopiraj(Cvor<T1,T2>* c1, Cvor<T1,T2>* &c2, Cvor<T1,T2>* pret=nullptr);

};

template <typename T1, typename T2>
BinStabloMapa<T1,T2>::BinStabloMapa(const BinStabloMapa &b)
{
    broj_elemenata=b.broj_elemenata;
    ppkopiraj(b.korijen, korijen);
}


template <typename T1, typename T2>
BinStabloMapa<T1,T2>::BinStabloMapa(BinStabloMapa &&b)
{
    broj_elemenata=b.broj_elemenata;
    korijen=b.korijen;
}

template <typename T1, typename T2>
void BinStabloMapa<T1,T2>::obrisi(const T1 &k)
{
    Cvor<T1,T2> *p=korijen, *parent=nullptr;
    while(p!=nullptr && k!=p->first) {
        parent=p;
        if(k<p->first) p=p->lijevi;
        else p=p->desni;
    }
    if(p==nullptr) return;
    Cvor<T1,T2>*m;
    if(p->lijevi==nullptr) m=p->desni;
    else {
        if(p->desni==nullptr) m=p->lijevi;
        else {
            Cvor<T1,T2> *pm=p, *temp;
            m=p->lijevi;
            temp=m->desni;
            while(temp!=nullptr) {
                pm=m;
                m=temp;
                temp=m->desni;

            }
            if(pm!=p) {
                pm->desni=m->lijevi;
                m->lijevi=p->lijevi;
            }
            m->desni=p->desni;

        }
    }
    if(parent==nullptr) korijen=m;
    else {
        if(p==parent->lijevi) parent->lijevi=m;
        else parent->desni=m;
    }
    delete p;
    broj_elemenata--;


}

template <typename T1, typename T2>
void BinStabloMapa<T1,T2>::obrisi()
{
    while(korijen!=nullptr) obrisi(korijen->first);
}

template <typename T1, typename T2>
T2 BinStabloMapa<T1,T2>::operator [](T1 k)const
{
    Cvor<T1,T2> *temp=korijen;
    while(temp!=nullptr && k!=temp->first) {
        if(k<temp->first) temp=temp->lijevi;
        else temp=temp->desni;
    }
    if(temp==nullptr) return T2();
    return temp->second;

}

template <typename T1, typename T2>
T2 &BinStabloMapa<T1,T2>::operator [](T1 k)
{
    Cvor<T1,T2> *temp=korijen;
    while(temp!=nullptr && k!=temp->first) {
        if(k<temp->first) temp=temp->lijevi;
        else temp=temp->desni;
    }

    if(temp==nullptr) {
        temp=new Cvor<T1,T2>();
        broj_elemenata++;
        temp->first=k;
        temp->second=T2();
        Cvor<T1,T2>*x=korijen, *y=nullptr;
        while(x!=nullptr) {
            y=x;
            if(temp->first < x->first) x=x->lijevi;
            else x=x->desni;
        }

        if(y==nullptr) korijen=temp;
        else {
            if(temp->first < y->first) y->lijevi=temp;
            else y->desni=temp;
            temp->parent=y;
        }

    }
    return temp->second;

}

template <typename T1, typename T2>
BinStabloMapa<T1,T2> &BinStabloMapa<T1,T2>::operator=(BinStabloMapa &&b)
{
    if(this==&b) return *this;
    this->obrisi();
    korijen=b.korijen;
    broj_elemenata=b.broj_elemenata;
    return *this;
}

template <typename T1, typename T2>
BinStabloMapa<T1,T2> &BinStabloMapa<T1,T2>::operator=(const BinStabloMapa &b)
{
    if(this==&b) return *this;
    this->obrisi();
    ppkopiraj(b.korijen, korijen);
    broj_elemenata=b.broj_elemenata;
    return *this;
}

template <typename T1, typename T2>
void BinStabloMapa<T1,T2>:: ppkopiraj(Cvor<T1,T2>* c1, Cvor<T1,T2>* &c2, Cvor<T1,T2>* pret)
{
    if(c1!=nullptr) {
        c2=new Cvor<T1,T2>();
        c2->first=c1->first;
        c2->second=c1->second;
        c2->parent=pret;
        ppkopiraj(c1->lijevi, c2->lijevi, c2);
        ppkopiraj(c1->desni, c2->desni, c2);
    }
}


template<typename T1, typename T2>
class HashMapa : public Mapa<T1,T2>
{
    int kapacitet=100;
    int broj_elemenata;
    par<T1,T2>* hmapa;
    unsigned int(*hash)(T1, unsigned int)=0;
public:
    HashMapa(): broj_elemenata(0)
    {
        hmapa=new par<T1, T2>[kapacitet];
    }
    HashMapa(const HashMapa &h);
    HashMapa(HashMapa &&h);
    ~HashMapa()
    {
        delete[] hmapa;
    }
    int brojElemenata() const
    {
        return broj_elemenata;
    }
    void obrisi();
    void obrisi(const T1 &kljuc);
    void definisiHashFunkciju(unsigned int (*fun)(T1, unsigned int))
    {
        hash=fun;
    }
    HashMapa &operator =(const HashMapa &h);
    HashMapa &operator =(HashMapa &&h);
    T2 &operator [](T1 k);
    T2 operator [](T1 k) const;
};
template <typename T1, typename T2>
HashMapa<T1, T2>::HashMapa(const HashMapa &h)
{
    kapacitet=h.kapacitet;
    broj_elemenata=h.broj_elemenata;
    hash=h.hash;
    hmapa=new par<T1, T2> [kapacitet];
    for(int i(0); i<kapacitet; i++) {
        hmapa[i]=h.hmapa[i];
    }
}
template <typename T1, typename T2>
HashMapa<T1, T2>:: HashMapa(HashMapa &&h)
{
    kapacitet=h.kapacitet;
    broj_elemenata=h.broj_elemenata;
    hmapa=h.hmapa;
    hash=h.hash;
}

template<typename T1, typename T2>
void HashMapa<T1, T2>::obrisi()
{
    delete[] hmapa;
    kapacitet=100;
    broj_elemenata=0;
    hmapa=new par<T1, T2>[kapacitet];
}
template<typename T1, typename T2>
void HashMapa<T1, T2>::obrisi(const T1 &k)
{
    for(int i(0); i<kapacitet; i++) {
        if(hmapa[i].first==k) {
            for(int j(i); j<kapacitet-1; j++) {
                hmapa[j]=hmapa[j+1];
            }
            broj_elemenata--;
        }
    }
}
template<typename T1, typename T2>
HashMapa<T1, T2> &HashMapa<T1,T2>::operator =(const HashMapa &h)
{
    if(this==&h) return *this;
    delete[] hmapa;
    kapacitet=h.kapacitet;
    broj_elemenata=h.broj_elemenata;
    hash=h.hash;
    hmapa=new par<T1, T2>[kapacitet];
    for (int i(0); i<kapacitet; i++) {
        hmapa[i]=h.hmapa[i];

    }
    return *this;
}
template<typename T1, typename T2>
HashMapa<T1, T2> &HashMapa<T1, T2>:: operator=(HashMapa &&h)
{
    if(this==&h) return *this;
    delete[] hmapa;
    kapacitet=h.kapacitet;
    broj_elemenata=h.broj_elemenata;
    hash=h.hash;
    hmapa=h.hmapa;
    return *this;
}

template<typename T1, typename T2>
T2 &HashMapa<T1,T2>::operator [] (T1 k)
{
    if(hash==0) throw ("Greska");
    for(int i(0); i<kapacitet; i++) {
        if(hmapa[i].first==k) return hmapa[i].second;
    }
    int indeks=hash(k,kapacitet);
    while(indeks<kapacitet && hmapa[indeks].first!=T1()) indeks++;
    if(indeks>=kapacitet) {
        par<T1,T2>* temp=new par<T1,T2>[kapacitet*2];
        for(int i(0); i<kapacitet; i++) {
            temp[i]=hmapa[i];
        }
        delete[] hmapa;
        kapacitet*=2;
        hmapa=temp;
    }
    broj_elemenata++;
    hmapa[indeks].first=k;
    hmapa[indeks].second=T2();
    return hmapa[indeks].second;
}


template<typename T1, typename T2>
T2 HashMapa<T1,T2>::operator [] (T1 k) const
{

    if(hash==0) throw ("Greska");
    for(int i(0); i<kapacitet; i++) {
        if(hmapa[i].first==k) return hmapa[i].second;
    }
    return T2();

}




void test(){
    NizMapa<int, int> n;
    BinStabloMapa<int, int> b;
    HashMapa<int,int>h;
    h.definisiHashFunkciju(TheHash);
    clock_t t1=clock();
    for(int i(0); i<1000; i++){
        n[i]=rand();
    }
    
    clock_t t2=clock();
    std::cout<<"vrijeme generisanja 1000 cifri i dodavanje u nizMapu je: "<<(t2-t1)/(CLOCKS_PER_SEC/1000)<<std::endl;
    t1=clock();
    for(int i(0); i<1000; i++){
        b[i]=rand();
    }
    
    t2=clock();
    std::cout<<"vrijeme generisanja 1000 cifri i dodavanje u BinStabloMapu je: "<<(t2-t1)/(CLOCKS_PER_SEC/1000)<<std::endl;
    t1=clock();
    for(int i(0); i<1000; i++){
        h[i]=rand();
    }
    
    t2=clock();
    std::cout<<"vrijeme generisanja 1000 cifri i dodavanje u HashMapu je: "<<(t2-t1)/(CLOCKS_PER_SEC/1000)<<std::endl;

    
}






int main()
{

   

    std::cout << "Pripremna Zadaca Za Vjezbu 9, Zadatak 1";
    return 0;
}

