#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    srand(time(0));
    cout << stoi(argv[1]);
    ofstream cout("test.txt");
    cout << stoi(argv[1]) << '\n';;
    for (int i = 0; i < stoi(argv[1]); i++) {
	int rnd = rand() % 10, k = rand() % 1000, c;
	if (rnd < 5) {
		c = 1;
	} else if (rnd < 8) {
		c = 2;
	} else {
		c = 3;
	}
	cout << c << ' ' << k << '\n';
    }
}
