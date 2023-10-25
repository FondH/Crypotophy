#include<iostream>
#include <iomanip>

using namespace std;
const int _x = 0b100011011;
const int Rcon[11] = { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };
const int S_table[16][16] =
{
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
const int MixArray[4][4] =
{
0x02, 0x03, 0x01, 0x01,
0x01, 0x02, 0x03, 0x01,
0x01, 0x01, 0x02, 0x03,
0x03, 0x01, 0x01, 0x02
};

//输出debug
void toHex(int * n) {

    for (int i = 0;i < 16;i++) 

        cout<< setw(2) << setfill('0') << hex<< uppercase << (int)n[i] << " ";
    
    cout << endl;
}

string toBinary(int n, int l) {
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

//实现Fn域内乘法
int multi(int x_0, int x_1) {
    int a = x_0;
    int b = int(x_1);
    int rs = 0;
    int i = 0;
    //按位移动，处理x0 x1乘法
    while (b) {
        if (b & 0x1)
            rs ^= (a << i);
        b = b >> 1;
        i++;
        
    }

    //mod 不可约_x
   // cout << toBinary(rs, 16) << endl;

    while ((rs >> 8) & 0xff) {
        int rs_ = rs >> 8;
        int j = 0;
        //cout << toBinary(rs_, 8);
        while (rs_) {
            if (rs_ & 0x1) {
                rs ^= (_x << j);
                //cout << toBinary(rs_, 8) << endl;
                //cout << toBinary(rs, 16) << endl;
            }
            rs_ >>= 1;
            j++;
        }

    }
    // rs ^= 0b11011;
   // cout << toBinary(rs, 8);
    return rs;
}

//按照字节S盒置换
int byte_substitution(int x) {
    return S_table[x>>4][x & 0xf];
}
//将16个byte进行S盒置换
void  S_sub(int *plain) {
    for (int i = 0;i < 16;i++)
        plain[i] = byte_substitution(plain[i]);
}

//密钥扩展
void Add_key(int * K, int ep) {
    int tpk = K[12];
    for (int i = 13;i < 16;i++)
        K[i - 1] = K[i];
    K[15] = tpk;

    K[0] = byte_substitution(K[12]) ^ K[0] ^ Rcon[ep];
    for (int i = 1;i < 12;i++)
    {
        if (i < 4)
            K[i] = byte_substitution(K[i + 12]) ^ K[i];
        else 
            K[i] = K[i - 4] ^ K[i];
    }
    for (int i = 15;i > 12;i--)
        K[i] = K[i - 1] ^ K[i-4];
    K[12] = tpk ^ K[8];

}

string toBinary(int n, char l) {
    int ll = l;
    return toBinary(n, ll);
}

//
void ShiftRows(int* p) {
    for (int i = 1;i < 4;i++)
        for (int j = i;j < 4;j++) {
            int tp = p[j];
            p[j] = p[j + 4];
            p[j + 4] = p[j + 8];
            p[j + 8] = p[j + 12];
            p[j + 12] = tp;
        }
}
void  MixColumn(int* p) {

    int tp[16];
    int row = 0;
    int col = 0;
    for (int i = 0;i < 16;i++) {
        row = i & 0b11;
        col = i >> 2;
        tp[i] = multi(p[(col<<2)], MixArray[row][0]) ^ multi(p[(col << 2) + 1], MixArray[row][1])
            ^ multi(p[(col << 2) + 2], MixArray[row][2]) ^ multi(p[(col << 2) + 3], MixArray[row][3]);
    }
    for (int i = 0;i < 16;i++)
        p[i] = tp[i];

}

void xor_key(int * K,int * p, int ep) {
    Add_key(K, ep);
    for (int i = 0;i < 16;i++)
        p[i] ^= K[i];

    //toHex(K);
}
//测试，输出每一轮的K和加密后的值
void test(int *K, int *p) {
    //cout<<toBinary(a, 8);
// 000102030405060708090a0b0c0d0e0f
// 00112233445566778899aabbccddeeff 

/*  cout << toBinary(0x9, 8) << endl;
cout << toBinary(0x60, 8)<<endl;
cout << toBinary(0xe1, 8) << endl;
cout << toBinary(0x04, 8) << endl;
int rs = multi(0x9, 2);
int rs1 = multi(0x60, 3);
int rs2 = multi(0xe1, 1);
int rs3 = multi(0x04, 1);
cout << toBinary(rs, 8) << endl;
cout << toBinary(rs1, 8) << endl;
cout << toBinary(rs2, 8) << endl;
cout << toBinary(rs3, 8) << endl;
int s = rs ^ rs1 ^ rs2 ^ rs3;
cout << hex << s << endl;*/

    cout << "输入P: ";
    toHex(p);

    // xor_key(K, p, 0);
    for (int i = 0;i < 16;i++)
        p[i] ^= K[i];
    cout << "Epoch " << 0 << ": " << endl << "K : ";
    toHex(K);
    cout << "P : ";
    toHex(p);

    for (int n = 1;n < 10;n++) {
        cout << "Epoch " << n << ": " << endl;

        S_sub(p);
        cout << "S_sub: ";
        toHex(p);

        ShiftRows(p);
        cout << "ShiftRows: ";
        toHex(p);

        MixColumn(p);

        cout << "MixColumn: ";
        toHex(p);

        xor_key(K, p, n);
        cout << "K : ";
        toHex(K);

        cout << "P : ";
        toHex(p);
    }
    S_sub(p);
    ShiftRows(p);
    xor_key(K, p, 10);
    toHex(p);
}



int main() {


    int K[16] = {0};
    int p[16] = { 0 };

    char plain[64];
    char key[64];




   //输入string，存入K和p
    cin >> key;
    cin >> plain;
    for (int i = 0;i < 32;i++) {
        if (plain[i] >= 'a' && plain[i] <= 'z') 
            plain[i] = int(plain[i] - 'a'+10);
         
        else
            plain[i] = int(plain[i] - '0');

 
        if (key[i] >= 'a' && plain[i] <= 'z')
            key[i] = int(key[i] - 'a' + 10);
        else
            key[i] = int(key[i] - '0');

    }

    for (int i = 0;i < 16;i ++) {

        p[i] += plain[i * 2] * 16;
        p[i] += plain[i*2+1] ;
        K[i] += key[i*2] * 16;
        K[i] += key[i*2+1] ;
    }

  //  test(K, p);

    for (int i = 0;i < 16;i++)
        p[i] ^= K[i];

    for (int n = 1;n < 10;n++) {
        S_sub(p);
        ShiftRows(p);   
        MixColumn(p);
        xor_key(K, p, n);
    }

    S_sub(p);
    ShiftRows(p);
    xor_key(K, p, 10);
    toHex(p);
}