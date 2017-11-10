#include "lifeGrid.h"
#include <regex>
#include <string>

using namespace std;

/* regex pattern to match input in the form "(x, y)" */
const regex INPUT_PATTERN("\\((-?[0-9]+),\\s*(-?[0-9]+)\\)");

LifeGrid::LifeGrid(istream& input) {
	string line;
	getline(input, line);
	while (!input.eof() && line != "") {
		smatch matches;
		if (regex_match(line, matches, INPUT_PATTERN)) {
			cout << matches[1] << " : " << matches[2];
		} else {
			// make this pretty
			throw "Invalid input: " + line;
		}
		getline(input, line);
	}
}