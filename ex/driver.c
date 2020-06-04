/*
 * driver.c
 *
 *  Created on: Sep 8 2012
 *      Author: qsp30
 */

//#include "ex41.c"
//#include "grade.c"
//#include "dining.c"
#include "vote.c"


int main(void)
{
	int K = 32;
	
	int output = func();
	
	int bv[K];
	for(int i = 0; i < K; i++)
		bv[i] = (output >> i) & 1;

	assert (bv[0] == 0);
	return 0;
}
