//============================================================================
// Name        : SQIFC
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow Analysis for C
//============================================================================

typedef unsigned char size_t;

size_t nondet_uchar();

int func(){
	// S is the number of the student
	size_t S = 5;
	// G is the number of grade, from 0 to G-1
	size_t G = 5;
	// n is the number of random numbers generated
	size_t n;
	// this is the sum that will be printed
	size_t output;

	// this is an internal counter for the sum
	size_t sum = 0;

	// these are the random numbers; each one is shared between two students
	size_t numbers[S];

	// these are the public announcements of each student
	size_t announcements[S];

	// there are S secret votes, each one with G possible values:
	size_t h[S]; // h[i] can only take value from 0 to G-1

	// these are just counters
	size_t i = 0;
	size_t j = 0;

	// calculating n
	n = ((G-1)*S)+1;
	
	// counter
	size_t c = 0;

	// generate the random numbers
	for (c = 0; c < S; c++) {
		h[c] = nondet_uchar() % G;
	}

	// generate the random numbers
	for (c = 0; c < S; c++) {
		numbers[c] = nondet_uchar() % n;
	}

	// producing the declarations according to the secret value
	while (i<S) {
  		j=0;
  		while (j<G) {
    			if (h[i]==j)
      				announcements[i]=(j+numbers[i]-numbers[(i+1)%S])%n;
    			j=j+1;
  		}
  		i=i+1;
	}

	//computing the sum, producing the output and terminating
	for (c = 0; c < S; c++) {
  		sum += announcements[c];
	}
	output = sum % n;

	return output;
}
