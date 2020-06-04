typedef unsigned char size_t;

size_t nondet_uchar(); 

size_t func(){
	// N is the number of voters
	size_t N = 6;

	// the output is the number of votes for one of the two candidates
	size_t output = 0;

	// these bits represent the preferences of each voter
	size_t vote[N];

	// these bits represent the votes received by the voting machine
	size_t decl[N];

	// this is just a counter
	size_t i = 0;

	// generate the random numbers
	for (i = 0; i < N; i++) {
		vote[i] = nondet_uchar() % 2;
	}

	// voting
	i = 0;
	while (i<N) {
		if (vote[i] == 0)
		  	decl[i] = 0;
	  	else
	    		decl[i] = 1;
	  	
	  	i = i + 1;
	}

	// count the number of votes for one of the candidates
	for (i = 0; i < N; i++) {
		output = output + decl[i];
	}

	return output;
}
