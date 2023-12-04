#include<iostream>
using namespace std;
void Get_Inverse(int a, int b) {
	int r0 = a, r1 = b;
	//si = si−2 − qi−1si−1, ti = ti−2 − qi−1ti−1, 其中 qi = ri−1/r
	int s_2 = 1, s_1 = 0, t_2 = 0, t_1 = 1;
	int s, q, t, r;
	while (r0 % r1) {
		q = r0 / r1;
		s = s_2 - q * s_1;
		t = t_2 - q * t_1;
		s_2 = s_1;
		s_1 = s;
		t_2 = t_1;
		t_1 = t;
		r = r0 % r1;
		r0 = r1;
		r1 = r;
	}

	if (s < 0) { s += b; }
	cout << s;



}
int main() {
	int x;cin >> x;
	int y;cin >> y;
	Get_Inverse(x, y);

}