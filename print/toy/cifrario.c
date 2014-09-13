
#include <string.h>

#include <inttypes.h>

#define rot(x,n) ((x << (n)) | (x >> (32-(n))))

void crypt (  unsigned int __attribute__((key))   *_key,
                    const unsigned int __attribute__((plain))  *_input,
                    unsigned int *output )
{
    const unsigned int *key = _key;
    const unsigned int *input = _input;
    unsigned int subkeys[4];
    unsigned int tmp;
    subkeys[0]=key[1];
    subkeys[1]=key[0];
    subkeys[2]=key[0] ^ key[1];
    subkeys[3]=key[1] << 10 ^ 0xFCEF;
    tmp=*input;
    #define iter(n)  tmp=tmp^subkeys[n]; tmp=rot(tmp,2+n)
	iter(0);
	iter(1);
	iter(2);
	iter(3);
    *output=tmp;
}

#include <stdio.h>
int main(){
unsigned int out;
unsigned int correct = 0;
unsigned int plain = 0x5678ABCD;
unsigned int key[2] = {0x12345678,0x9ABCDEF0};

crypt(&key,&plain,&out);
if(memcmp(&out,&correct,sizeof(correct))==0)  printf("OK\n"); else printf("ERROR\n");
  printf("%x",out);  
}
