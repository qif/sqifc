char nondet_char(void);

unsigned char GetCRC8( unsigned char check , unsigned char ch)
{
   int i ;

   for ( i = 0 ; i < 8 ; i++ ) {
      if ( check & 0x80 ) {
         check<<=1;
         if ( ch & 0x80 ) {
            check = check | 0x01;
         } else {
            check =check & 0xfe;
         }
         check = check ^ 0x85;
      } else {
        check <<=1;
        if ( ch & 0x80 ) {
            check = check | 0x01;
        } else {
            check = check & 0xfe;
        }
      }
      // ch<<=1;
      ch = ch * 2;
   }
   return ( check >> 3 ); // to make manageable we shift 5 bits right so the 8bits check becomes a 3 bits var
							// so leakage is below 3 bits 8 distinctions
}

