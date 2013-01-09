#include "OAEP.h"
#include <iostream>
#include <string.h>
#include "BigInt.h"
#include "Key.h"
#include <ctime>
using namespace std;

BigInt OAEP::k_to_m(const BigInt& cnt) {
    //cnt have k bits
    BigInt res(cnt), tmp(cnt);
    while (res.GetBitLength() < M) {
        tmp = (tmp << 1);
        res = (res ^ tmp);
    }
    return res;
}
BigInt OAEP::m_to_k(const BigInt& cnt) {
    //the digits of cnt must more than k
    BigInt filter(1), res(0), tmp(cnt);
    for (int i = 0; i < K/16; i++)
        filter = (filter << 16);
    filter = (filter << (K % 16));
    filter = filter - 1;
    res = filter;
    filter = (filter >> 1);
    while (tmp.GetBitLength() >= K) {
        res = (filter & tmp) ^ res;
        tmp = (tmp >> 1);
    }
    return res;
}

OAEP::OAEP(int k, int m) {
    this->K = k;
    this->M = m;
}
void OAEP::changeMode(int k, int m) {
    this->K = k;
    this->M = m;
}
BigInt OAEP::oaep_encode(const BigInt& cnt) {
    BigInt r, tmp, P1, P2;
    srand((unsigned) time(NULL));
    r.Random(K);
    cout << "the encode random number is " << r;
    tmp = k_to_m(r);
    P1 = cnt ^ tmp;

    tmp = m_to_k(P1);
    P2 = tmp ^ r;

    for (int i = 0; i < K/16; i++)
        P1 = (P1 << 16);
    P1 = (P1 << (K % 16));
    P1 = P1 | P2;
    cout << "encoded msg: " << P1;
    return P1;
}
BigInt OAEP::oaep_decode(const BigInt& cnt) {
    BigInt r, tmp, P1(cnt), P2, res;
    BigInt filter(1);
    for (int i = 0; i < K/16; i++)
        filter = (filter << 16);
    filter = (filter << (K % 16));
    filter = filter - 1;
    P2 = (cnt & filter);
    
    for (int i = 0; i < K/16; i++)
        P1 = (P1 >> 16);
    P1 = (P1 >> (K % 16));

    tmp = m_to_k(P1);
    r = (tmp ^ P2);
    cout << "the decode random number is " << r;

    tmp = k_to_m(r);
    res = (tmp ^ P1);
    cout << "decoded msg: " << res;
    return res;
}
