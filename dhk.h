#ifndef DHK_H
#define DHK_H

using namespace std;

class Dhk{
private:
	int p, g, x_a, x_b, y_a, y_b, k1, k2;
	int opt;			// option (1 = manual, 2 = random)
	map<int, int> pr;	// will hold all the primitive roots of p

	bool isPrime(int p);	// checks if p is prime
	string toBinary(int n);	// decimal to binary converter
	int sq_and_mul(int a, string b, int n);		// Square and multiply algorithm to calculate
												// a^b mod n

	/* Functions used in random version */
	int get_p_rand();		// get random  p
	int get_g_rand();		// get random g
	int get_sec_key();		// get random XA and XB
	/* End of random version functions */

	int get_key(int a, int b, int p);	// Compute YA, YB, KA/K, KB/K


	void manual_run();
	void random_run();
public:
	Dhk(int _opt){
		opt = _opt;
		p = 0;
		g = 0;
		x_a = 0;
		x_b = 0;
		y_a = 0;
		y_b = 0;
		k1 = 0;
		k2 = 0;
	}

	int get_p(){
		return p;
	}
	int get_g(){
		return g;
	}
	int get_xa(){
		return x_a;
	}
	int get_xb(){
		return x_b;
	}
	int get_k(){
		return k1;
	}
	void run();
};

int Dhk::get_sec_key(){
	return rand() % (p - 1) + 1;
}

string Dhk::toBinary(int n){
	string b = "00000000";

	map<int, int> bit;
	bit[0] = 128; bit[1] = 64;
	bit[2] = 32; bit[3] = 16;
	bit[4] = 8; bit[5] = 4;
	bit[6] = 2; bit[7] = 1;

	for(int i = 0; i < 8; i++){
		if(n >= bit[i]){
			n = n - bit[i];
			b[i] = '1';
		}
	}
	return b;
}

int Dhk::sq_and_mul(int a, string b_bin, int n){
	int c = 0, d = 1;

	for(int i = 7; i >=0; i--){
		c = 2 * c;
		d = (d * d) % n;
		if(b_bin[8 - i - 1] == '1'){
			c++;
			d = (d * a) % n;
		}
	}
	return d;
}

int Dhk::get_key(int a, int b, int p){
	string b_bin;
	int val;
	b_bin = toBinary(b);
	val = sq_and_mul(a, b_bin, p);

	return val;
}

bool Dhk::isPrime(int p){
	for(int i = 2; i <= p/2; i++){
		if(p % i == 0)
			return false;
	}
	return true;
}

int Dhk::get_p_rand(){
	map<int, int> primes;	// holds all prime numbers upto 100
	int count = 1;
	for(int i = 3; i < 100; i = i + 2){
		if(isPrime(i)){
			primes[count] = i;
			count++;
		}
	}
	int k = rand() % primes.size() + 1;

	return primes[k];
}

int Dhk::get_g_rand(){
	map<int, int> myMap;	// holds results of a^b mod n
	int count = 0;
	string b_bin;
	for(int a = 2; a < p; a++){
		myMap.clear();
		for(int b = 1; b < p; b++){
			b_bin = toBinary(b);
			myMap[sq_and_mul(a, b_bin, p)] = b;
		}
		if(myMap.size() == p - 1){	// if size != p - 1, not a primitive root
			count++;
			pr[count] = a;
		}
	}
	int k = rand() % pr.size() + 1;
	return pr[k];
}

void Dhk::manual_run(){
	do{
		cout << "Enter value of p (prime number upto 100): ";
		cin >> p;
		if(!isPrime(p))
			cout << "Not a prime number. Try again!" << endl;
		else
			break;
	}while(true);

	get_g_rand();	// Computes the map<> pr
	do{
		cout << "\nPlease enter g(primitive root) from below: " << endl;
		map<int, int>::iterator it;
		for(it = pr.begin(); it != pr.end(); it++){
			cout << it->second << " ";
		}
		cout << "\n\ng: ";
		cin >> g;
		bool found = false;
		for(it = pr.begin(); it != pr.end(); it++){
			if(it->second == g)
				found = true;
		}
		if(!found)
			cout << "Invalid!" << endl;
		else
			break;
	}while(true);

	do{
		cout << "Enter secret key XA (1 < XA < P): ";
		cin >> x_a;
		if(x_a <= 1 || x_a >= p)
			cout << "Invalid!" << endl;
		else
			break;
	}while(true);

	do{
		cout << "Enter secret key XB (1 < XB < P): ";
		cin >> x_b;
		if(x_b <= 1 || x_b >= p)
			cout << "Invalid!" << endl;
		else
			break;
	}while(true);
}

void Dhk::random_run(){
	p = get_p_rand();
	g = get_g_rand();
	x_a = get_sec_key();
	x_b = get_sec_key();
	y_a = get_key(g, x_a, p);
	y_b = get_key(g, x_b, p);
	k1 = get_key(y_a, x_b, p);
	k2 = get_key(y_b, x_a, p);
}

void Dhk::run(){
	if(opt == 1)
		manual_run();
	else
		random_run();

	/* Compute public keys and shared secret key */
	y_a = get_key(g, x_a, p);
	y_b = get_key(g, x_b, p);
	k1 = get_key(y_a, x_b, p);
	k2 = get_key(y_b, x_a, p);
}

#endif
