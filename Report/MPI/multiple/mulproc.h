#include <stdio.h>

// 桁数
#define KETA 2000

// 基数
#define BASE 10

// 符号
#define PLUS  1
#define MINUS -1

// 多倍長整数
struct NUMBER {
    int n[KETA]; // 桁
    int sign;    // 符号
};

// プロトタイプ宣言
void clearByZero(struct NUMBER *);
void setRnd(struct NUMBER *, int);
void copyNumber(const struct NUMBER *, struct NUMBER *);
void swap(struct NUMBER *, struct NUMBER *);
void dispNumber(const struct NUMBER *);
void saveNumber(FILE *fp, const struct NUMBER *);

int  isZero(const struct NUMBER *);
int  numComp(const struct NUMBER *, const struct NUMBER *);
int  isPrime(const struct NUMBER *);

void getAbs(const struct NUMBER *, struct NUMBER *);
int  setInt(struct NUMBER *, int);
int  setString(struct NUMBER *, char *);
int  setSign(struct NUMBER *, int);
int  getSign(const struct NUMBER *);

int  mulByBase(const struct NUMBER *, struct NUMBER *);
int  divByBase(const struct NUMBER *, struct NUMBER *);

int  add(const struct NUMBER *, const struct NUMBER *, struct NUMBER *);
int  sub(const struct NUMBER *, const struct NUMBER *, struct NUMBER *);
int  multiple(const struct NUMBER *, const struct NUMBER *, struct NUMBER *);
int  divide(const struct NUMBER *, const struct NUMBER *, struct NUMBER *, struct NUMBER *);
int  simpleDivide(const struct NUMBER *, const struct NUMBER *, struct NUMBER *, struct NUMBER *);

int  increment(const struct NUMBER *, struct NUMBER *);
int  decrement(const struct NUMBER *, struct NUMBER *);

int  squareRoot(const struct NUMBER *, struct NUMBER *);
