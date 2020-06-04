/*
 * driveroneCRC.c
 *
 *  Created on: Sep 8 2012
 *      Author: qsp30
 */

#include "crc8_2hcbmc.c"

int main(void)
{

	int K = 8;

	int output = GetCRC8 (0, nondet_char());

	int bv[K];
	for(int i = 0; i < K; i++)
		bv[i] = (output >> i) & 1;

	assert (bv[0] == 0);
	return 0;
}
