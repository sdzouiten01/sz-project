#ifndef MT_H
#define MT_H

#define N 624
#define M 397
#define MATRIX_A 0x9908b0df
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff

#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y>>11)
#define TEMPERING_SHIFT_S(y) (y<<7)
#define TEMPERING_SHIFT_T(y) (y<<15)
#define TEMPERING_SHIFT_L(y) (y>>18)

static unsigned long mt[N];
static int mti=N+1;
static unsigned long actual_seed=0;

void Inputseed(unsigned long * input);
void Outputseed(unsigned long * output);
void Mosesrand();

//void sgenrand(unsigned long seed);

double genrand(/*unsigned long Seed*/);

static unsigned long mt_Moses[N];

#endif