#include <iostream>
#define Epoch 5
using namespace std;

typedef long long lp;

//直接乘法会越界，思想就是对y不断除以2，是奇数拆成偶数+1形式...
lp multi(lp a, lp b, lp mod) {
    a %= mod;
    b %= mod;
    lp res = 0;

    while (b > 0) {
        // 如果 b 的当前位是 1，则将 a 加到结果中
        if (b & 1) {
            res = (res + a) % mod;
        }

        // 将 a 翻倍并保持在模 mod 的范围内
        a = (a * 2) % mod;

        // 将 b 右移一位（相当于除以 2）
        b >>= 1;
    }

    return res;
}
lp multi(lp a, lp mod) {
    return multi(a, a, mod);

}

lp qPow(lp a, lp d, lp p) {
    //d 是奇数
    lp res = 1;
    while (d > 0) {
        if (d & 1)
            res = multi(res, a, p);

        d = d >> 1;
        a = multi(a, p);
    }
    return res;

}

bool miller(int m, lp d, lp n){

    lp  a = 2 + rand() % (n - 2);
    d = qPow(a, d, n);

    if ((d % n == 1) || (d%n ==n-1)) {
        //cout << "Yes";
        return 1;
    }


    for (int i = 0;i < m;i++) {
        d = multi(d, n);
        if (d % (n) == 1) {
            //cout << "No";
            return 0;
        }
        if (d % (n) == n - 1) {
            //cout << "Yes";
            return 1;
        }
    }
    return 0;
}

int main() {
    srand(time(NULL));

    lp n ;
    cin >> n;
    if (n ==1) {
        cout << "No";
        return 0;
    }
    if (n == 3 || n==2)
    {
        cout << "Yes";
        return 0;
    }


    lp d = n - 1;
    lp m = 0;
    while (!(d % 2)) {
        d /= 2;
        m++;
    }
    for (int k = 0;k < Epoch;k++)
        if (!miller(m, d, n)) {
            cout << "No";
            return 0;
        }
            
    cout << "Yes";
    return 0;
}