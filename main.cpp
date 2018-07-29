#include "bignumbers.h"

int main(void)
{
	/* El Gamal: */
	printf("-------- El Gamal: ---------\n");
	Big *Q = loadBigNumber("ElG_Q.txt");
	Big *M;
	Big *Ca = loadBigNumber("ElG_Ca.txt");
	Big *Cb = loadBigNumber("ElG_Cb.txt");
	Big *Da = loadBigNumber("ElG_Da.txt");
	Big *Db = (Big *)calloc(1, sizeof(Big));
	Big *mes = loadBigNumber("ElG_Mes.txt");

	Big *minusOne = (Big *)calloc(1, sizeof(Big));
	minusOne->aa[1] = 1;
	minusOne = bigNegate(minusOne);
	M = bigAdd(Q, minusOne);
	printf("M: ");
	printBigDecimal(M);
	Big *another = (Big *)calloc(1, sizeof(Big));
	extendedBinaryEuclidian(Cb, M, Db, another);
	printf("Db: ");
	printBigDecimal(Db);
	printf("another: ");
	printBigDecimal(bigNegate(another));
	printf("Changing message...\nW0 = ");
	printBigDecimal(mes);
	mes = bigModPwr(mes, Ca, Q);
	printf("W1 = ");
	printBigDecimal(mes);
	mes = bigModPwr(mes, Cb, Q);
	printf("W2 = ");
	printBigDecimal(mes);
	mes = bigModPwr(mes, Da, Q);
	printf("W3 = ");
	printBigDecimal(mes);
	mes = bigModPwr(mes, Db, Q);
	printf("W4 = ");
	printBigDecimal(mes);
	system("pause>nul");

	/* RSA: */
	printf("\n\n--------- RSA: ----------\n");
	Big *P = loadBigNumber("RSA_P.txt");
	Q = loadBigNumber("RSA_Q.txt");
	mes = loadBigNumber("RSA_Mes.txt");
	Big *D = loadBigNumber("RSA_D.txt");
	// Found with help of another program
	// after calculating value of M here.
	Big *E = loadBigNumber("RSA_E.txt");
	Big *fakeM = (Big *)calloc(1, sizeof(Big));
	fakeM->aa[0] = UINT64_MAX;
	fakeM->aa[1] = UINT64_MAX;
	Big *N = bigModMult(P, Q, fakeM);
	printf("N: ");
	printBigDecimal(N);
	bigAssign(Ca, bigAdd(P, minusOne));		// (P - 1)
	bigAssign(Cb, bigAdd(Q, minusOne));		// (Q - 1)
	M = bigModMult(Ca, Cb, fakeM);
	printf("M: ");
	printBigDecimal(M);
	printf("Changing message...\nm0 = ");
	printBigDecimal(mes);
	mes = bigModPwr(mes, E, N);
	printf("m1 = ");
	printBigDecimal(mes);
	mes = bigModPwr(mes, D, N);
	printf("m2 = ");
	printBigDecimal(mes);
	system("pause>nul");

	return 0;
}