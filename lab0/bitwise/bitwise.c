#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
    return binstr;
}

void print_usage(char *name) {
    printf("Usage: %s <action> <byte> <bit no>\n", name);
    printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
        "\t\t <byte> 8-bit integer in hexadecimal\n"
        "\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[])
{
    char a;    // action: 'h', 'l', 'r', 's', 't'
    unsigned long n; // value to convert must be smaller than 256
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
        if(n > (1 << 8) - 1) { // ( n < 0 || )
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
    binary[8] = 0;
    // Print to stdout the binary representation of n
    printf("%s\n", byte2bin(n, binary));

    // Do what the user asked and print the result

    // h - testar se o valor de um determinado bit está a 1 (high)
    // l - testar se o valor de um determinado bit está a 0 (low)
    // r - colocar a 0 o valor de um determinado bit (reset)
    // s - colocar a 1 o valor de um determinado bit (set)
    // t - inverter o valor de um determinado bit (toggle)

    switch (a) {
    case 'h':
        printf("Is bit %d High? ", bit);
        printf("%s\n", (n & BIT(bit)) ? "Yes" : "No");
        break;
    case 'l':
        printf("Is bit %d Low? ", bit);
        printf("%s\n", (n & BIT(bit)) ? "No" : "Yes");
        break;
    case 'r':
        printf("Reset bit %d.\n", bit);
        n &= ~BIT(bit);
        printf("%s\n", byte2bin(n, binary));
        break;
    case 's':
        printf("Set bit %d.\n", bit);
        n |= BIT(bit);
        printf("%s\n", byte2bin(n, binary));
        break;
    case 't':
        printf("Toggled bit %d.\n", bit);
        n ^= BIT(bit);
        printf("%s\n", byte2bin(n, binary));
        break;
    default:
        break;
    }
    
    return 0;
}