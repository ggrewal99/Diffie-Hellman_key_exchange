#include<iostream>
#include<map>
#include<ctime>
#include<cmath>

#include "dhk.h"

using namespace std;

int main(){
	cout << "**** DIFFIE-HELLMAN KEY EXCHANGE ****\n" << endl;

	srand(time(0));

	cout << "Please select an option from below:" << endl;
	cout << "1. Manual Input." << endl;
	cout << "2. Random Input." << endl;
	int opt;

	do{
		cout << "[option] ";
		cin >> opt;
		if(opt != 1 && opt != 2)
			cout <<"Invalid input. Please enter option number from above choices." << endl;
		else
			break;
	}while(true);

	Dhk test(opt);
	test.run();

	/* All results excluding YA, YB */
	cout << endl;
	cout << "Results: " << endl;
	cout << "p (prime number): " << test.get_p() << endl;
	cout << "g (primitive root): " << test.get_g() << endl;
	cout << "XA (Secret key): " << test.get_xa() << endl;
	cout << "XB (Secret key): " << test.get_xb() << endl;
	cout << "K (Shared secret key): " << test.get_k() << endl;

	return 0;
}
