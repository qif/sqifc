/*
 * driverN.c
 *
 *  Created on: 13 Oct 2012
 *      Author: qsp30
 */

#include "sco.c"

#define buffsize 16

int main(void)
{
	int optlen = buffsize;
	char out[buffsize];
	struct sock s = {BT_CONNECTED};
	struct socket sk = {0,&s,0};

	memset(out,0,buffsize);
	
	sco_sock_getsockopt_old(&sk,SCO_CONNINFO,out,&optlen);

	int K = 8;
	int bv[K];
	for(int i = 0; i < K; i++)
		bv[i] = (out[5] >> i) & 1;
	assert (bv[0] == 0);
	return 0;
}



