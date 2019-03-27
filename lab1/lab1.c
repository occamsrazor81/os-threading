#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

int cnt = 45279;
int m = 0;
uint64_t maxBroj = 0xffffffffffffffffULL;



uint64_t generate(uint64_t rnd)
{
  uint64_t A = 8654098236518264769ULL;
  uint64_t B = 18116652324302926351ULL;

  uint64_t rnd1;
  rnd1 = (rnd * A) % B | 1;
  //printf ("%" PRIu64 "\n", rnd); //pravi odabir za ux


  return rnd1;
}




float generate01()
{


  float x = (float)rand()/(float)(RAND_MAX);

  return x;

}

double dsecnd (void)
{
    return (double)( clock( ) ) / CLOCKS_PER_SEC;
}




int prost(uint64_t rnd)
{
  if(cnt < 100000) cnt++;
  else cnt = 37581;
  for(int i = 2; i < cnt; ++i)
  if(rnd % i == 0) return 0;

  return 1;
}


/////////
int testBitova(uint64_t rnd)
{
  int prvajedinica = 0;
  for (int i = 63; i >= 2; i--)
  {
      int k1 = rnd >> i;
      int k2 = rnd >> i-1;
      int k3 = rnd >> i-2;

      if(k1 & 1 == 1) prvajedinica = 1;

      if(prvajedinica == 1)
      if ( ((k1 & 1 == 1) && (k2 & 1 == 1) && (k3 & 1 == 1)) ||
       ((k1 & 1 == 0) && (k2 & 1 == 0) && (k3 & 1 == 0)))
        return 0;

  }

  return 1;

}




void ispis(double start, uint64_t ms[])
{
  printf("\nU ispisu:\n");
  double ukupno = dsecnd();
  double elapsed = ukupno - start;
  printf("%g ",elapsed);

  printf("i = %d ", m);

  if(m > 0) printf ("ms[i] = %" PRIu64 " ", ms[m-1]);
  else printf ("ms[i] = %" PRIu64 " ", ms[4]);

  printf("ms[] = ");
  for(int i = 0; i < 5; ++i)
    printf ("%" PRIu64 " ", ms[i]%100);

  printf("\n");
  printf("Kraj ispisa\n\n" );

}

void ispisFile(double start, uint64_t ms[])
{

  FILE *fptr;
  fptr = fopen("readme.txt", "a");

  if (fptr == NULL)
  {
        fprintf(fptr, "Cannot open file \n");
        exit(0);
  }
  //fprintf(f, "Some text: %s\n", text);
  double ukupno = dsecnd();
  double elapsed = ukupno - start;
  fprintf(fptr, "%g ",elapsed);

  fprintf(fptr, "i = %d ", m);

  if(m > 0) fprintf (fptr, "ms[i] = %" PRIu64 " ", ms[m-1]);
  else fprintf (fptr, "ms[i] = %" PRIu64 " ", ms[4]);

  fprintf(fptr, "ms[] = ");
  for(int i = 0; i < 5; ++i)
    fprintf (fptr, "%" PRIu64 " ", ms[i]%100);

  fprintf(fptr,"\n");
  fclose(fptr);
}

int processRqst(double timeLapse, double start, uint64_t ms[])
{
  timeLapse = 2.0;
  if(timeLapse < 1.0) return 0; // 0 = nije kraj
  else
  {
    float newRqst = generate01();
    float end = generate01();
    printf("newRqst = %f, end = %f\n",newRqst,end);

    //newRqst je slucajno generirani float izmedu 0 i 1
    //ako je newRqst*100 > 50 (vjerojatnost 0.5) onda ispis
    if(newRqst*100 > 50.0)
    {
        ispis(start, ms);
        ispisFile(start,ms);
    }
    //end je slucajno generirani float izmedu 0 i 1
    //ako je end*100 > 90 (vjerojatnost 0.1) onda kraj
    //moglo se uzet i end*100 < 10
    if(end*100 > 90.0) //
      return 1; //kraj ovdje

    else return 0;

  }
}





// POMOĆNE FUNKCIJE ZA ISPIS BROJEVA
// void binarno(uint64_t n)
// {
//
//     int binaryNum[1000];
//     int i = 0;
//     while (n > 0)
//     {
//
//
//         binaryNum[i] = n % 2;
//         n = n / 2;
//         i++;
//     }
//
//
//     for (int j = i - 1; j >= 0; j--)
//          printf("%d",binaryNum[j] );
//
//       printf("\n" );
// }
//
//
//
// void binarno2(uint64_t n)
// {
//
//     for (int i = 63; i >= 0; i--)
//     {
//         int k = n >> i;
//         if (k & 1 == 1)
//             printf("1");
//         else
//             printf("0");
//     }
//
//     printf("\n");
// }











int main()
{
  srand(time(NULL));
  double t1,t2,start;
  t1 = start = dsecnd();


  uint64_t ms[5];
  for(int j = 0; j < 5; ++j)
    ms[j] = 0;



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////  OVDJE PRAVI KOD


  uint64_t rnd = time(NULL);
  uint64_t r;
  int ponavljaj;
  while(1)
  {
    r = generate(rnd);
    printf ("r = %" PRIu64 "\n", r);
    rnd = r;

    ponavljaj = 0;
    while((testBitova(r) == 0 || prost(r) == 0) && ponavljaj < 10000)
    {

        if(r <= maxBroj - 2) r +=2;
        else r = generate(rnd);

        ponavljaj++;
    }
    if(prost(r)) printf("broj %"PRIu64" je prost\n", r);
     ms[m] = r;

    if(m+1 == 5) m = 0;
    else m++;

    t2 = dsecnd();
    int kraj = processRqst(t2-t1, start, ms);
    t1 = t2;


    if(kraj) break;

  }

//// OVDJE KRAJ PRAVOG KODA
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //????????????????????????????????????????????????????
  // OVDJE TESTOVI


  // uint64_t rnd = time(NULL);
  // uint64_t r = generate(rnd);
  // printf ("r = %" PRIu64 "\n", r);
  // if(prost(r)) printf("broj %"PRIu64" je prost\n", r);
  // ms[m] = r;
  // printf("%d\n",m);
  // printf ("ms[0] = %" PRIu64 "\n", ms[m]);
  // m++;
  //
  //

  // uint64_t zn = (r>>0) & 1;
  // uint64_t zn1 = (r>>1) & 1;
  // uint64_t zn2 = (r>>2) & 1;
  // uint64_t zn3 = (r>>3) & 1;
  // uint64_t zn4 = (r>>4) & 1;
  // uint64_t zn5 = (r>>5) & 1;
  //
  //
  // printf("%" PRIu64 "\n",zn);
  // printf("%" PRIu64 "\n",zn1);
  // printf("%" PRIu64 "\n",zn2);
  // printf("%" PRIu64 "\n",zn3);
  // printf("%" PRIu64 "\n",zn3);
  // printf("%" PRIu64 "\n",zn3);


  //
  //
  //
  //
  // uint64_t r1 = generate(r);
  // printf ("r1 = %" PRIu64 "\n", r1);
  // if(prost(r1)) printf("broj %"PRIu64" je prost\n", r1);
  // ms[m] = r1;
  // printf("%d\n",m);
  // printf ("ms[1] = %" PRIu64 "\n", ms[m]);
  // m++;
  //
  //
  // uint64_t r2 = generate(r1);
  // printf ("r2 = %" PRIu64 "\n", r2);
  // if(prost(r2)) printf("broj %"PRIu64" je prost\n", r2);
  // ms[m] = r2;
  // printf("%d\n",m);
  // printf ("ms[2] = %" PRIu64 "\n", ms[m]);
  // m++;






  return 0;
}
