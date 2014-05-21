//============================================================================
// Name        : SQIFC
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow Analysis for C
//============================================================================

#include "SQIF.h"

using namespace std;

int main(int argc, const char* argv[]) {

	if (argc < 2) {
		cout << "Please enter config file" << endl;
		return -1;
	}

	config conf(argv[1]);
	sqif s(&conf);
	s.symbolicQIF();
	return 0;
}
