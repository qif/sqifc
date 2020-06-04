/*
 * driverN1.c
 *
 *  Created on: 12 Oct 2012
 *      Author: qsp30
 */

#include "osf_sys1.c"

/* libc library */
int memcmp(char *s1, char *s2, unsigned int n) {
  int i;
  for(i=0;i<n;i++) {
     if(s1[i] != s2[i]) return -1;
  }
  return 0;
}

void memcpy(char *dst, char *src, unsigned int n) {
  int i;
   for(i=0;i<n;i++) {
      dst[i] = src[i];
   }
}

void memset(char *dst, char c, unsigned int n) {
  int i;
  for(i=0;i<n;i++) {
       dst[i] = c;
  }
}
/* ------ */

int nondet_int(); 

int main(void)
{
	int flag,k,len,namelen,buffsize,N;
	len = 33;
	buffsize = 34;
	char out[buffsize];
	
	memset(out,0,buffsize);

	namelen = nondet_int();
	osf_getdomainname(out,namelen);
	
	int K = 8;
	int bv[K];
	for(int i = 0; i < K; i++)
		bv[i] = (out[len] >> i) & 1;
	assert (bv[0] == 0);

}

