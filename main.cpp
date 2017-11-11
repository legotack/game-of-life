#include <iostream>
#include "lifeGrid.h"
#include <limits>

using namespace std;

int main() {
	LifeGrid grid(cin, 5);

	while (true) {
		cout << grid << endl;
		grid.tick();
	}

	return 0;
}