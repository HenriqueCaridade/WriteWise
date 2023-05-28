#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define BIT(n) (1<<(n))

void byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    for(int i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }
    int k=7;
    for(int i=0;i<8;i++){
        binstr[k-i]=binary[i]+'0';
    }
    binstr[8]='\0';
    /*// printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);*/  
}

void print_usage(char *name) {
    printf("Usage: %s <action> <byte> <bit no>\n", name);
    printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
        "\t\t <byte> 8-bit integer in hexadecimal\n"
        "\t\t <bit no> of bit to operate on\n");
}

int is_bit_um(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < num_bits_int );
  int mascara_num_bit = (1 << num_bit);
  return ( valor & mascara_num_bit );
}
int set_bit(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < num_bits_int );
  int mascara_num_bit = (1 << num_bit);
  return ( valor | mascara_num_bit );
}
int reset_bit(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < num_bits_int );
  int mascara_num_bit = ~(1 << num_bit);
  return ( valor & mascara_num_bit );
}
int toggle_bit(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < num_bits_int );
  int mascara_num_bit = (1 << num_bit);
  return ( valor ^ mascara_num_bit );
}
int main(int argc, char *argv[]){

    char a;    // action: 'h', 'l', 'r', 's', 't'
    int n; // value to convert must be smaller than 256
    int bit; // bit to operate on: must be between 0 an 7
    
    { // Validate command line arguments 
        if (argc != 4) {
            print_usage(argv[0]);
            return 1;
        }
        
        // In C a string is essentially a null terminated character array
        a = argv[1][0];
        if( a != 'h' && a != 'l' && a != 'r' && a != 's' && a != 't' ) {
            print_usage(argv[0]);
            printf("\t<action> must be one of 'h'|'l'|'r'|'s'|'t'\n");
            return 1;
        }
    
        // We are expecting the argument to be an hexadecimal string
        n = strtoul(argv[2],NULL,16);
        if( n < 0 || n > (1 << 8) - 1) {
            print_usage(argv[0]);
            printf("\t<byte> must be non-negative and smaller than 256\n");
            return 1;
        }
        
        bit = atoi(argv[3]);
        if( bit < 0 || bit > 7 ) {
            print_usage(argv[0]);
            printf("\t<bit no> must be between 0 and 7\n");
            return 1;
        }    
    } // End of command line arguments validation
    
    char binary[9]; // array for binary representation of n,
    //  remember that in C strings are terminated with a 0
    byte2bin(n,binary);
    printf("%s\n", binary);
    switch (a)
    {
        case 'h':{
            if (is_bit_um(n,bit)!=0){
                printf("The bit is one!\n");
            }
            else{
                printf("The bit is not one :(\n");
            }
            break;
        }
        case 'l':{
            if (is_bit_um(n,bit)==0){
                printf("The bit is zero!\n");
            }
            else{
                printf("The bit is not zero :(\n");
            }
            break;
        }
        case 'r':{
            int aux=reset_bit(n,bit);
            byte2bin(aux,binary);
            printf("Result after bit reset: %s\n",binary);
            break;
        }  
        case 's':{
            int aux=set_bit(n,bit);
            byte2bin(aux,binary);
            printf("Result after bit set: %s\n",binary);
            break;
        } 
        case 't':{
            int aux=toggle_bit(n,bit);
            byte2bin(aux,binary);
            printf("Result after bit toggle: %s\n",binary);
            break;
        }   
    }
    return 0;
}