//============================================================================
// Name        : SQIFC
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow Analysis for C
//============================================================================

#include "SQIF.h"

void sqif::createAssertion(const char* pc) {

	ofstream tmp;
	tmp.open(pConf->getTmpPath());

	char notpc[512];
	sprintf(notpc, "!(%s)", pc);

	src.replace(found, asst.size(), notpc); // 10 is the size of "bv[0] == 0"
	asst = notpc;
	tmp << src;
	tmp.close();

}

bool sqif::isSAT(const char* pc) {
	call++;
	createAssertion(pc);

	FILE *in;
	char buff[512];

	char command[256];
	sprintf(command, "%s %s", pConf->getCBMCpath(), pConf->getTmpPath());

	if (!(in = popen(command, "r"))) {
		// return false;
	}

	while (fgets(buff, sizeof(buff), in) != NULL) {
		// cout << buff;
	}

	pclose(in);

	char * pch = NULL;
	pch = strstr(buff, "SUCCESSFUL");
	if (pch != NULL) {
		// cout << "UNSAT\n";
		return false;
	} else {
		// cout << "SAT\n";
		return true;
	}
}

void sqif::symCount(const char* pc, int i) {
	if(N >= policy){
		cout << "The program is insecure" << endl;
		return;
	}
	char pc1[512];
	sprintf(pc1, "%s && bv[%d]", pc, i);
	if (isSAT(pc1)) {
		if (i == K) {
			//TODO: check for duplicate instances
			//cout << pc1 << endl;
			N++;
		} else
			symCount(pc1, i + 1);
	}

	char pc2[512];
	sprintf(pc2, "%s && !bv[%d]", pc, i);
	if (isSAT(pc2)) {
		if (i == K) {
			//TODO: check for duplicate instances
			//cout << pc2 << endl;
			N++;
		} else
			symCount(pc2, i + 1);
	}
}

int sqif::check(int i, bool t) {
	char pi[10];
	if (t) {
		sprintf(pi, "bv[%d]", i);
		switch (p[i]) {
		case UNCHECKED:
			if (isSAT(pi))
				p[i] = SAT;
			else
				p[i] = UNSAT;
			break;
		case INVALID:
			if (isSAT(pi))
				p[i] = NONFIXED;
			else
				p[i] = UNSAT;
			break;
		default: // No need to check true if it is already SAT/VALID/UNSAT/NONFIXED
			return p[i];
		}
	} else {
		sprintf(pi, "!bv[%d]", i);
		switch (p[i]) {
		case UNCHECKED:
			if (isSAT(pi))
				p[i] = INVALID;
			else
				p[i] = VALID;
			break;
		case SAT:
			if (isSAT(pi))
				p[i] = NONFIXED;
			else
				p[i] = VALID;
			break;
		default: // No need to check false if it is already INVALID/VALID/UNSAT/NONFIXED
			return p[i];
		}
	}
	return p[i];
}

void sqif::doBCP() {
	char pi[10];
	for (int i = 0; i <= pConf->getSize(); i++) {
		sprintf(pi, "bv[%d]", i);
		if (!isSAT(pi)) {
			p[i] = UNSAT;
			continue;
		}
		sprintf(pi, "!bv[%d]", i);
		if (!isSAT(pi)) {
			p[i] = VALID;
			continue;
		}
		p[i] = NONFIXED;
	}
}

void sqif::symCountBCPnew(const char* pc, int i) {
	if(N >= policy){
		// cout << "The program is insecure" << endl;
		return;
	}
	if (p[i] == VALID || p[i] == UNSAT) {
		if (i == K)
			N++;
		else
			symCountBCPnew(pc, i + 1);
		return;
	}

	char pc1[512];
	sprintf(pc1, "%s && bv[%d]", pc, i);
	if (isSAT(pc1)) {
		if (i == K) {
			//TODO: check for duplicate instances
			//cout << pc1 << endl;
			N++;
		} else
			symCountBCPnew(pc1, i + 1);
	}

	char pc2[512];
	sprintf(pc2, "%s && !bv[%d]", pc, i);
	if (isSAT(pc2)) {
		if (i == K) {
			//TODO: check for duplicate instances
			//cout << pc2 << endl;
			N++;
		} else
			symCountBCPnew(pc2, i + 1);
	}

}

void sqif::symCountBCP(const char* pc, int i) {
	// first part
	if(N >= policy){
		cout << "The program is insecure" << endl;
		return;
	}
	check(i, true);
	if (p[i] == VALID) {
		if (i == K)
			N++;
		else
			symCountBCP(pc, i + 1); // skip i
	}
	if (p[i] == SAT || p[i] == NONFIXED) {
		char pc1[512];
		sprintf(pc1, "%s && bv[%d]", pc, i);
		if (isSAT(pc1)) {
			if (i == K)
				N++;
			else
				symCountBCP(pc1, i + 1);
		}
	}
	// second part
	check(i, false);
	if (p[i] == UNSAT) {
		if (i == K)
			N++;
		else
			symCountBCP(pc, i + 1); // skip i
	}
	if (p[i] == INVALID || p[i] == NONFIXED) {
		char pc2[512];
		sprintf(pc2, "%s && !bv[%d]", pc, i);
		if (isSAT(pc2)) {
			if (i == K)
				N++;
			else
				symCountBCP(pc2, i + 1);
		}
	}

}

void sqif::symbolicQIF() {
	// symCount("1", 0);

	// symCountBCP("1", 0);

	doBCP();
	symCountBCPnew("1", 0);

	cout << "N = " << N << endl;
	cout << "Number of calls to cbmc: " << call - 1 << endl;
	if (N >= policy)
		cout << "The program is insecure regarding to this policy" << endl;
	else
		cout << "Maximum leakage is " << log2(N) << " bits" << endl;
}

sqif::sqif(config*conf) {
	pConf = conf;
	call = 0;
	K = pConf->getSize();
	for (int i = 0; i <= K; i++)
		p[i] = UNCHECKED;
	N = 0;
	policy = pConf->getPolicy();

	cout << "Policy is : " << policy << endl;

	asst = "bv[0] == 0";
	// read src file
	ifstream driver;
	driver.open(pConf->getDriverPath());
	if (!driver.is_open()) {
		cout << "Unable to open driver file" << endl << endl;
		return;
	}
	driver.seekg(0, std::ios::end);
	src.reserve(driver.tellg());
	driver.seekg(0, std::ios::beg);
	src.assign((std::istreambuf_iterator<char>(driver)),
			std::istreambuf_iterator<char>());
	found = src.find(asst);
	driver.close();
}

sqif::~sqif() {
}
