//============================================================================
// Name        : SQIFC
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow Analysis for C
//============================================================================

typedef unsigned int size_t;

size_t nondet_uchar(); 

size_t func(){

	// N is the number of cryptographers at the table
	size_t N = 6;

	// this bit represents the output
	size_t output = 0;

	// these bits represent the coin tosses
	// public array [N] of int1 coin;
	size_t coin[N];

	// these are the observable coins
	size_t obscoin[2];

	// this is just a counter
	size_t i = 0;

	// these bits represent the bits declared by the three cryptographers
	size_t decl[N];

	// the secret has N+1 possible values:
	// 0 if the Master paid
	// 1 if Cryptographer A paid
	// 2 if Cryptographer B paid
	// 3 if Cryptographer C paid
	// ... and so on
	size_t h = nondet_uchar() % (N+1);

	// counter
	size_t c = 0;

	// tossing the coins
	/*
	for (c in coin) do
  		random c = randombit(0.7);
	od
	*/
	// incorrect model of probability distribution ???
	for (c = 0; c < N; c++){
		coin[c] = nondet_uchar() % 2;
	}

	// if the attacker is one of the cryptographers, he can observe two of the coins.
	// To encode an external attacker comment the next two lines.
	// obscoin[0] = coin[0];
	// obscoin[1] = coin[1];

	// producing the declarations according to the secret value
	while (i<N){
  		decl[i] = coin[i]^coin[(i+1)%N];
  		if (h==i+1){
    			decl[i] = !decl[i];
  		}
		i = i+1;
	}

	//producing the output bit and terminating
	for (c = 0; c < N; c++)
		// output = output ^ decl[c];
		output = output + decl[c];
	return output;
}

/*
int main(void)
{
	int output = nondet_uchar() % 2;
	assert (output == 0 || output == 1);	
}
//*/
