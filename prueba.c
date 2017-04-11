#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>	// Need this for SSE2 compiler intrinsics

int main(int argc, char* argv[])
{
   char *a;
   __m128i v1, v2;
   int i;

   if (posix_memalign((void*) &a, 16, 32) != 0) {
      perror("posix_memalign dio error");
      exit(1);
   }

   a[0] = 4; a[1] = 4; a[2]  = 2; a[3]  = 1; a[4]  = 1; a[5]  = 4; a[6]  = 9; a[7]  = 9;
   a[8] = 7; a[9] = 4; a[10] = 2; a[11] = 2; a[12] = 2; a[13] = 5; a[14] = 5; a[15] = 6;

   v1 = _mm_load_si128((__m128i*)a);
   v2 = _mm_slli_si128 (v1, 1);
   //v2 = _mm_set1_epi8(2);
   //v2 = _mm_cmpeq_epi8(v1, v2);
   _mm_store_si128((__m128i*)(a+16), v2);

   for (i = 0; i < 16; i++)
      printf("%3d ", a[i]);
   printf("\n");

   for (i = 16; i < 32; i++)
      printf("%3hhx ", a[i]);
   printf("\n");

   return 0;
}
