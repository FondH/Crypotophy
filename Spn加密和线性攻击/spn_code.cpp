#include<iostream>
#include<cstring>
#include<vector>
#include <ctime>

using namespace std;

const int K = 0b00111010100101001101011000111111;

int SBox[16] = { 0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7 };
int S_Box[16] = { 0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5 };
int PBox[16] = { 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16 };

int count2 = 0;


struct Dpair {
    int plain = 0;
    int encry = 0;

};


string toBinary(int n,int l) {
    string result;
    int i = 0;
    while (i < l) {
        result.insert(result.begin(), '0' + (n & 1));
        ++i;
        if (i % 4 == 0 && i != l)
            result.insert(result.begin(), ' ');
        n >>= 1;
    }

    return result.empty() ? "0" : result;
}
string toBinary(int n) {
   return toBinary(n, 16);
}
int get_ki(int k, int i) {
    int rs = ((k << (i * 4) & 0xFFFF0000 ) >> 16 );
   // cout << "K_" << i << ": " << toBinary(rs) << endl;
    return rs;
}
int stoii(char * chr,int l) {
    int rs = 0;
    for (int i = 0;i < l;i++) 
        rs = 2 * rs + chr[i] - '0';
    return rs;

}
int encrypto(int x) {

    int w = x;
    // cout << toBinary(x)<<endl;
    for (int n = 0;n < 3;n++) {
        int kn = get_ki(K, n);
        int u = w ^ kn;
        //cout << "u_" << n << ": " << toBinary(u) << endl;

        // S
        int v = 0;
        for (int m = 0;m < 16;m += 4) {
            int vm = u >> m & 0xF;
            v |= (SBox[vm] << m);

        }
        //cout <<"v_"<<n<<": " << toBinary(v)<<endl;

        //P
        w = 0;
        for (int m = 15;m >= 0;m--) {
            int v_bit = v & 0x1;
            v = v >> 1;
            w |= v_bit << (16 - PBox[m]);
        }
        // cout << "w_" << n << ": " << toBinary(w) << endl;
    }
    int kn = get_ki(K, 3);
    int u = w ^ kn;
    //  cout << "u_3: " << toBinary(u) << endl;

    // S
    int v = 0;
    for (int m = 0;m < 16;m += 4) {

        int vm = u >> m & 0xF;
        v |= (SBox[vm] << m);
    }
    

    kn = get_ki(K, 4);
    int y = 0;
    y = v ^ kn;
    
    //cout << toBinary(y);
    return y;
}

int main() {
    cout << toBinary(K,32)<<endl;
    int Maxsize = 8000;

    int k2 =6;
    int k4 = 15;

    int si_ls[10] = {6250,6500,6750,7000,7250,7500,7750,8000,8250,8500};
    for (int idx = 0;idx < 10;idx++) {
        Maxsize = si_ls[idx];
        vector<Dpair> ps(Maxsize);
        cout << "穷举" << Maxsize << "对：" << endl;

         clock_t start = clock();
          double a = 0;
        for (int epoch = 0;epoch < 2000;epoch++) {
          

         srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0;i < Maxsize;i++) {
            ps[i].plain = rand() & 0xFFFF;
            ps[i].encry = encrypto(ps[i].plain);
        }


        int count[16][16] = { 0 };

        for (int p_i = 0;p_i < ps.size();p_i++) {
            Dpair p = ps[p_i];

            for (int i = 0;i < 16;i++) {
                for (int j = 0;j < 16;j++) {

                    int v2 = i ^ ((p.encry & 0x0F00) >> 8);
                    int v4 = j ^ (p.encry & 0x000F);
                    //cout << toBinary(p.encry, 16) << " " << toBinary(v2,4) << endl;
                    int u2 = S_Box[v2];
                    int u4 = S_Box[v4];
                    int z = ((p.plain >> 11) ^ (p.plain >> 9) ^ (p.plain >> 8) ^ (u2 >> 2) ^ (u2) ^ (u4 >> 2) ^ u4) & 0x1;
                    if (!z)

                        count[i][j] += 1;
                }

            }
        }
        int _i = -1, _j = -1;
        int _count = -1;

        for (int i = 0;i < 16;i++) {
            for (int j = 0;j < 16;j++) {
                count[i][j] = abs(count[i][j] - Maxsize / 2);
                if (count[i][j] > _count)
                {
                    _i = i;
                    _j = j;
                    _count = count[i][j];
                }
            }
        }
        //cout << "K<2>:" << _i<<" --> " << toBinary(_i, 4) << "\n" << "K<4>:" << _j <<" --> " << toBinary(_j, 4) << " " << "\nOffest：" << _count << endl;
   


        /*int count1[16][16] = { 0 };
        int count2[16][16] = { 0 };
        for (Dpair p : ps) {
            int  y = p.encry;
            int  x = p.plain;
            for (int i = 0;i < 16;i++) {
                for (int j = 0;j < 16;j++) {
                    int u1 = S_Box[(y & 0x000F) ^ i];
                    int u2 = S_Box[(y & 0x00F0) >> 4 ^ _i];
                    int u3 = S_Box[(y & 0x00F00) >> 8 ^ j];
                    int u4 = S_Box[(y & 0xF000) >> 12 ^ _j];

                    // k<1>
                    int z = ((x >> 15) ^ (x >> 14) ^ (x >> 12) ^ (u1 >> 3) ^ (u2 >> 3) ^ (u3 >> 3) ^ (u4 >> 3)) & 0x1;
                    if (!z)
                        count1[i][j]++;


                    z = ((x >> 7) ^ (x >> 6) ^ (x >> 4) ^ (u1 >> 1) ^ (u2 >> 1) ^ (u3 >> 1) ^ (u4 >> 1)) & 0x1;
                    if (!z)
                        count2[i][j]++;


                }
            }
        }

        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 16; j++)
            {
                abs(count1[i][j]);
                abs(count2[i][j]);
                count[i][j] = count1[i][j] + count2[i][j];
            }
        //  count[i][j] = abs(count1[i][j]) + abs(count2[i][j]);


        _count = 0;
        int _k1 = 0, _k3 = 0;
        for (int i = 0;i < 16;i++) {
            for (int j = 0;j < 16;j++) {
                count[i][j] = abs(count[i][j] - Maxsize);
                if (count[i][j] > _count)
                {
                    _k1 = i;
                    _k3 = j;
                    _count = count[i][j];
                }
            }
        }
       cout << "K<1>:" << _i << " " << "K<3>:" << _j << " " << "共出现" << _count << endl;
        cout << toBinary(_k1, 4) << " " << toBinary(_i, 4) << " " << toBinary(_k3, 4) << " " << toBinary(_j, 4) << endl;*/

        if (_i == k2 && _j == k4)
            a+=1;
    }

    clock_t end = clock();
    double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
    //cout <<"Time : " << elapsed_secs << "s" << endl;
    cout << "Acc: " << a / 2000 << "   Time : " << elapsed_secs << "s" << endl;
    }
        return 0;
}