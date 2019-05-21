#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>


int cnt = 45278;
int m = 0;
uint64_t maxBroj = 0xffffffffffffffffULL;
int broj[10] = {0};
int ulaz[10] = {0};
uint64_t ms[5] = {0};
int kraj = 0;
// sem_t semafor;
// sem_t novi;
pthread_mutex_t monitor;
pthread_cond_t red;
int polja = 0;


uint64_t generate(uint64_t rnd)
{

  uint64_t A = 8654098236518264769ULL;
  uint64_t B = 18116652324302926351ULL;

  uint64_t rnd1;
  rnd1 = ((rnd * A) % B) | 1;



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
  else cnt = 37642;

  for(int i = 2; i < cnt; ++i)
    if(rnd % i == 0) return 0;

  return 1;
}



int testBitova(uint64_t rnd)
{

  for (int i = 63; i >= 2; i--)
  {

      int k1 = rnd >> i;
      int k2 = rnd >> i-1;
      int k3 = rnd >> i-2;


      if ( ((k1 & 1 == 1) && (k2 & 1 == 1) && (k3 & 1 == 1)) ||
       ((k1 & 1 == 0) && (k2 & 1 == 0) && (k3 & 1 == 0)))
        return 0;

  }

  return 1;
}


int max()
{
    int maxi = broj[0];
    for(int i = 1; i < 10; ++i)
    {
        if(maxi < broj[i])
            maxi = broj[i];
    }

    return maxi;
}

void enter_KO(int id)
{
    ulaz[id] = 1;
    broj[id] = max() + 1;
    ulaz[id] = 0;

    for(int j = 0; j < 10; ++j)
    {
        while(ulaz[j] == 1);

        while(broj[j] != 0 &&
        (broj[j] < broj[id] || (broj[j] == broj[id] && j < id)));
    }
}


void exit_KO(int id)
{
    broj[id] = 0;
}



void *radnaDretva(void *x)
{
    int *id = (int*) x;
    uint64_t rnd = time(NULL);
    while(1)
    {
        sleep(3);
        uint64_t r = generate(rnd);
        rnd = r;
        int ponavljaj = 0;

        while((testBitova(r) == 0 || prost(r) == 0) && ponavljaj < 100000)
        {

            if(r <= maxBroj - 2 ) r +=2;
            else { r = generate(rnd); rnd = r; }

            ponavljaj++;
        }

        //enter_KO(*id);
        //sem_wait(&semafor);
        pthread_mutex_lock(&monitor);

        ms[m] = r;
        if(m+1 == 5) m = 0;
        else m++;

        polja++;

        pthread_cond_broadcast(&red);
        pthread_mutex_unlock(&monitor);

        // sem_post(&semafor);
        // sem_post(&novi);
        //exit_KO(*id);

       // sleep(3);

        if(kraj) break;


    }


}


void *dretvaProvjere(void* x)
{
    int *id = (int*)x;


    FILE *fptr;
    fptr = fopen("readmeMonitori.txt", "a");

    if (fptr == NULL)
    {
          fprintf(fptr, "Cannot open file \n");
          exit(0);
    }

    while(1)
    {
        //enter_KO(*id);


        // sem_wait(&novi);
        // sem_wait(&semafor);
        pthread_mutex_lock(&monitor);

        while(polja == 0)
            pthread_cond_wait(&red, &monitor);

        uint64_t y = ms[0];
        printf("id = %d, y =  %" PRIu64 ", uzeo broj\n",*id,y);
        fprintf(fptr,"id = %d, y =  %" PRIu64 ", uzeo broj\n",*id,y);

        polja--;

        pthread_mutex_unlock(&monitor);
        //sem_post(&semafor);
        //exit_KO(*id);

        uint64_t wait = y%5 + 1;
        //sleep(wait);
        //printf("wait = %d\n", y%5);
        printf("id = %d, y = %" PRIu64 ", potrosio broj\n",*id,y);
        fprintf(fptr,"id = %d, y =  %" PRIu64 ", potrosio broj\n",*id,y);

        if(kraj) break;

    }
    fclose(fptr);
}



int main()
{
    srand(time(NULL));
    double t1,t2,start;
    t1 = start = dsecnd();
    int br[10] = {0};


    // uint64_t ms[5];

    // for(int j = 0; j < 5; ++j)
    //     ms[j] = 0ULL;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////  OVDJE PRAVI KOD

    pthread_t threads[10];
    // sem_init(&semafor, 0, 1);
    // sem_init(&novi, 0, 0);


    for(int i = 0; i < 5; ++i)
    {
        br[i] = i;
        if (pthread_create(&threads[i], NULL, radnaDretva, &br[i]) != 0)
        {
            printf("Greska pri stvaranju dretve!\n");
   exit(1);
   }
    }

    sleep(2);

    for(int i = 5; i < 10; ++i)
    {
        br[i] = i;
        if (pthread_create(&threads[i], NULL, dretvaProvjere, &br[i]) != 0)
        {
            printf("Greska pri stvaranju dretve!\n");
   exit(1);
   }
    }

    sleep(30);
    kraj = 1;

    for(int j = 0; j < 10; ++j)
        pthread_join(threads[j],NULL);




FILE *fptr;
fptr = fopen("readmeMonitori.txt", "a");

if (fptr == NULL)
{
      fprintf(fptr, "Cannot open file \n");
      exit(0);
}
fprintf(fptr,"\n------------------------------");
fprintf(fptr,"----------------------------------------------\n");
fclose(fptr);


//// OVDJE KRAJ PRAVOG KODA


  return 0;
}
