#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int  main(int argc, char const *argv[])
{
	FILE *in = fopen(argv[1], "r");
	int b;
	fseek(in, 0, SEEK_END);
	int pos = ftell(in);
	uint32_t k;
	fseek(in, 18, SEEK_SET);
	fread(&b, sizeof(int),1,in);
	printf("Dimensiune: %d\n", b);
	uint32_t x;
	unsigned char a;
	fseek(in, 54, SEEK_SET);
	fread(&x, sizeof(uint32_t),1,in);
	printf("Nr culori: %d\n", x);
	fread(&k, sizeof(uint32_t),1,in);
	printf("Nr noduri: %d\n", k);
	for (int i = 0; i < k; i++)
	{
		printf("\n\n");
	printf("Indice: %d\n", i);
	printf("BGRR: ");
	fread(&a, sizeof(unsigned char),1,in);
	printf("%d ", a);
	fread(&a, sizeof(unsigned char),1,in);
	printf("%d ", a);
	fread(&a, sizeof(unsigned char),1,in);
	printf("%d ", a);
	fread(&a, sizeof(unsigned char),1,in);
	printf("%d\n", a);
	fread(&x, sizeof(uint32_t),1,in);
	printf("Aria: %d\n", x);
	fread(&x, sizeof(int32_t),1,in);
	printf("Indici vecini: %d ", x);
	fread(&x, sizeof(int32_t),1,in);
	printf("%d ", x);
	fread(&x, sizeof(int32_t),1,in);
	printf("%d ", x);
	fread(&x, sizeof(int32_t),1,in);
	printf("%d\n", x);
	if (ftell(in) == pos)
		break;

}
	return 0;
}