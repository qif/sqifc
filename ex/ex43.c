int func(){
	unsigned int S, O;
	S = (S & 0x55555555) + ((S >> 1) & 0x55555555);
	S = (S & 0x33333333) + ((S >> 2) & 0x33333333);
	S = (S & 0x0f0f0f0f) + ((S >> 4) & 0x0f0f0f0f);
	S = (S & 0x00ff00ff) + ((S >> 8) & 0x00ff00ff);
	O = (S + (S>>16)) & 0xffff;
	return O;
}


