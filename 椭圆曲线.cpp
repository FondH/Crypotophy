#include<iostream> 
using namespace std;
typedef long long ll;

ll Get_Inverse(ll a, ll b) {
	// 扩展欧几里得求逆
	// 最后a'= s, b'= t
	if (a < 0) a += b;
	ll r0 = a, r1 = b;
	//si = si−2 − qi−1si−1, ti = ti−2 − qi−1ti−1, 其中 qi = ri−1/r
	ll s_2 = 1, s_1 = 0, t_2 = 0, t_1 = 1;
	ll s = 0, q, t = 0, r;
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
	if (s < 0) s += b;
	//if (t < 0) t += a;
	return s;
}
ll multi(ll a, ll b, ll mod) {
	a %= mod;
	b %= mod;
	a = a < 0 ? a + mod : a;
	b = b < 0 ? b + mod : b;
	ll res = 0;

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
ll multi(ll a, ll mod) {
	return multi(a, a, mod);

}
ll pow(ll a, ll d, ll p) {
	// return a^n mod p 
	ll res = 1;
	while (d > 0) {
		if (d & 1)
			res = multi(res, a, p);

		d = d >> 1;
		a = multi(a, p);
	}
	return res;

}
ll qpow(ll a, ll n, ll p) {
	// return a^n mod p 
	ll res = 1;
	for (ll i = 0;i < n;i++) {
		res *= a;
		res %= p;
	}
	return res;
}
bool Is_singular(ll p, ll a, ll b) {
	/*  4*a^3 + 27*b^2 != 0  */
	if ((4 * pow(a, 3, p) + 27 * pow(b, 2, p)) % p)
		return 1;
	else return 0;
}


class Ep {
	ll a, b, p;
	//y^3 = x^2 + ax + b  
	//无穷远点： (p , p)

public:
	Ep(ll o, ll q, ll w) :p(o), a(q), b(w) {}
	friend ll pow(ll a, ll b, ll c);
	bool Is_singular() {
		/*  4*a^3 + 27*b^2 != 0  */
		if ((4 * pow(a, 3, p) + 27 * pow(b, 2, p)) % p) {
			//printf("E_%d(%d,%d) is singular\n", p, a, b);
			return 1;
		}
		else {
			//printf("E_%d(%d,%d) is not singular\n", p, a, b);
			return 0;
		}
	}
	bool Is_on(ll x, ll y) {
		ll l = pow(y, 2, p);
		ll r = pow(x, 3, p) + a * x + b;
		if ((l - r) % p) {
			//printf("P(%d, %d) is not on E\n", x, y);
			return 0;
		}
		else {
			//printf("P(%d, %d) is on E\n", x, y);
			return 1;
		}
	}


	void _add(ll x1, ll y1, ll x2, ll y2, bool pr, ll* res) {
		/*
		return:	  x3 = res[0], y3 = res[1]

		*/
		
		if (x1 == p) {
			res[0] = x2;res[1] = y2;
			//if (pr)
				//printf("O + (%d, %d) = (%d, %d)\n", x2, y2, x2, y2);
			return ;
		}
		if (x2 == p) {
			res[0] = x1;res[1] = y1;
			//if (pr)
				//printf("(%d, %d) + O = (%d, %d)\n", x1, y1, x1, y1);
			return ;
		}
		//ll k = (y2 - y1) * Get_Inverse(x2 - x1, p) % p;

	
		ll k;
		if (x1 == x2) {
			if (y1 == 0 && y2 == 0)
			{
				res[0] = res[1] = p;
				return;
			}
			if (y1 == y2)
				
				k = multi((3 * pow(x1, 2, p) + a), Get_Inverse((2 * y1)%p, p),p) ;
			else
			{
				//if (pr)
					//printf("(%d, %d) + (%d, %d) = O\n", x1, y1, x2, y2);
				res[0] = res[1] = p;
				return;
			}
		}
		else
			k = multi((y2 - y1), Get_Inverse(x2 - x1, p), p);
		//res[0] = (k * k - x1 - x2) % p < 0 ? (k * k - x1 - x2) % p + p : (k * k - x1 - x2) % p;
		//res[1] = (k * (x1 - res[0]) - y1) % p < 0 ? (k * (x1 - res[0]) - y1) % p + p : (k * (x1 - res[0]) - y1) % p;
		res[0] = (multi(k, p) - x1 - x2) ;
		res[1] = (multi(k, (x1 - res[0]), p) - y1) ;

		while (res[0] < 0)
			res[0] += p;

		while (res[1] < 0 )
			res[1] += p;

		/*if (pr)
			printf("(%d, %d) + (%d, %d) = (%d, %d)\n", x1, y1, x2, y2, res[0], res[1]);*/
		return ;

	}
	void _add_double(ll x, ll y, ll* res) {
		_add(x, y, x, y, 0, res);
	}

	ll* Calc_MultiAdd(ll x, ll y, ll m) {
		if (!(m%p)) {
			cout << "-1 -1";
			return nullptr;

		}
		ll bin[65] = { 0 }, i = 0;
		ll* res = new ll[2];
		res[0] = this->p;res[1] = this->p;
		ll  _m = m;
		// 到此， i记录了二进制串长度，bin[1:i]记录二进制值；
		while (_m) {
			bin[++i] = _m % 2;
			_m /= 2;
		}

		//进行倍加-和；
		for (;i > 0;i--) {
			_add_double(res[0], res[1], res);
			if (bin[i]) 
				_add(x, y, res[0], res[1], 0,res);
			
		}

		if (res[0] == p)
			//printf("%d(%d, %d) = O\n", m, x, y);
			cout << "-1 -1";
		else
			//printf("%dP(%d, %d) = (%d, %d)\n", m, x, y, res[0], res[1]);
			cout << res[0] << " " << res[1];
		return res;

	}
	
};

int main() {

	ll p = 7;
	ll a = 5;
	ll b = 4;
	ll x, y, k;

	cin >> a;
	cin >> b;
	cin >> p;
	cin >> x;
	cin >> y;
	cin>>k;
	Ep	t(p, a, b);

	if (t.Is_singular()) 
		t.Calc_MultiAdd(x, y, k);


}