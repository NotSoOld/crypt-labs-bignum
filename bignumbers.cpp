#include <bignumbers.h>

void printBig(Big *n)
{
	printf("\nResulting big number:\nAA: ");
	for (int i = 0; i < 2; i++) {
		printf("%I64x        ", n->aa[i]);
	}
	printf("\nBB: ");
	for (int i = 0; i < 4; i++) {
		printf("%x    ", n->bb[i]);
	}
	printf("\nCC: ");
	for (int i = 0; i < 8; i++) {
		printf("%x  ", n->cc[i]);
	}
	printf("\nDD: ");
	for (int i = 0; i < 17; i++) {
		printf("%x ", n->dd[i]);
	}
	printf("\n");
}


Big *bigLShift(Big *num, int shift)
{
	uint64 result0 = 0;
	uint64 result1 = 0;
	Big *result = (Big *)calloc(1, sizeof(Big));

	// Check for overflow:
	if ((num->aa[0] >> (64 - shift)) != 0) {
		printf("\nOVERFLOW ERROR! Number's too big.\n");
		system("pause");
		exit(2);
	}
	// Shift the higher byte.
	result0 = num->aa[0] << shift;
	// We'll lose some digits while shifting the lower byte.
	// Save them and add to the higher byte.
	uint64 tempShift = num->aa[1] >> (64 - shift);
	result0 = result0 | tempShift;
	// Shift the lower byte.
	result1 = num->aa[1] << shift;

	result->aa[1] = result1;
	result->aa[0] = result0;
	return result;
}


Big *bigRShift(Big *num, int shift)
{
	uint64 result0 = 0;
	uint64 result1 = 0;
	Big *result = (Big *)calloc(1, sizeof(Big));

	// Shift the lower byte.
	result1 = num->aa[1] >> shift;
	// We'll lose some digits while shifting the higher byte.
	// Save them and add to the lower byte.
	uint64 tempShift = num->aa[0] << (64 - shift);
	result1 = result1 | tempShift;
	// Shift the higher byte.
	result0 = num->aa[0] >> shift;

	result->aa[1] = result1;
	result->aa[0] = result0;
	result->dd[16] = num->dd[16];
	if (result->dd[16] == 1) {
		result->aa[0] |= 0x8000000000000000;
	}
	return result;
}


Big *bigAdd(Big *left, Big *right)
{
	uint64 result0 = 0;
	uint64 result1 = 0;
	Big *result = (Big *)calloc(1, sizeof(Big));

	// Do array-by-array addition with adding digit
	// to the right array if we see overflow.
	if (left->aa[1] > (UINT64_MAX - right->aa[1])) {
		result0++;
	}
	result1 = left->aa[1] + right->aa[1];

	result0 += left->aa[0] + right->aa[0];

	result->aa[0] = result0;
	result->aa[1] = result1;
	return result;
}


Big *bigAdd(Big *left, Big *right, uint8 &CF)
{
	uint64 result0 = 0;
	uint64 result1 = 0;
	Big *result = (Big *)calloc(1, sizeof(Big));
	CF = 0;

	// Do array-by-array addition with adding digit
	// to the right array if we see overflow.
	if (left->aa[1] > (UINT64_MAX - right->aa[1])) {
		result0++;
	}
	result1 = left->aa[1] + right->aa[1];

	uint64 leftaa = left->aa[0];
	if (left->aa[0] > UINT64_MAX - 1) {
		CF = 1;
	}
	else {
		leftaa += result0;
		if (leftaa > (UINT64_MAX - right->aa[0])) {
			CF = 1;
		}
		else {
			CF = 0;
		}
	}
	result0 = leftaa + right->aa[0];

	result->aa[0] = result0;
	result->aa[1] = result1;
	return result;
}

// -1 === left < right
//  0 === left = right
//  1 === left > right
int bigCompare(Big *left, Big *right)
{
	int result = 0;

	if (left->aa[0] < right->aa[0]) {
		result = -1;
	}
	else if (left->aa[0] > right->aa[0]) {
		result = 1;
	}
	else {
		if (left->aa[1] < right->aa[1]) {
			result = -1;
		}
		else if (left->aa[1] > right->aa[1]) {
			result = 1;
		}
		else {
			result = 0;
		}
	}

	return result;
}


Big *bigNegate(Big *number)
{
	Big *negatedNumber = (Big *)calloc(1, sizeof(Big));
	negatedNumber->aa[0] = ~(number->aa[0]);
	negatedNumber->aa[1] = ~(number->aa[1]);
	Big *one = (Big *)calloc(1, sizeof(Big));
	one->aa[1] = 1;
	negatedNumber = bigAdd(negatedNumber, one);
	negatedNumber->dd[16] = (number->dd[16] == 0 ? 0 : 1);
	return negatedNumber;
}


Big *bigSAdd(Big *a, Big *b)
{
	Big *r = (Big *)calloc(1, sizeof(Big));
	if (a->dd[16] == 0 && b->dd[16] == 0) {
		uint8 i = 0;
		r = bigAdd(a, b, i);
		if (i == 1) {
			printf("unhandled overflow in case 1\n");
		}
		r->dd[16] = 0;
	}
	else if (a->dd[16] == 0 && b->dd[16] == 1) {
		if (bigCompare(a, bigSNeg(b)) == -1) {
			r = bigAdd(a, b);
			r->dd[16] = 1;
		}
		else {
			r = bigAdd(a, b);
			r->dd[16] = 0;
		}
	}
	else if (a->dd[16] == 1 && b->dd[16] == 0) {
		if (bigCompare(b, bigSNeg(a)) == -1) {
			r = bigAdd(b, a);
			r->dd[16] = 1;
		}
		else {
			r = bigAdd(b, a);
			r->dd[16] = 0;
		}
	}
	else if (a->dd[16] == 1 && b->dd[16] == 1) {
		uint8 i = 0;
		r = bigAdd(a, b, i);
		if (i == 0) {
			printf("unhandled overflow in case 6\n");
		}
		r->dd[16] = 1;
	}
	return r;
}


Big *bigSAdd(Big *a, Big *b, uint8 &cf)
{
	Big *r = (Big *)calloc(1, sizeof(Big));
	if (a->dd[16] == 0 && b->dd[16] == 0) {
		r = bigAdd(a, b, cf);

		r->dd[16] = 0;
	}
	else if (a->dd[16] == 0 && b->dd[16] == 1) {
		if (bigCompare(a, bigSNeg(b)) == -1) {
			r = bigAdd(a, b);
			r->dd[16] = 1;
		}
		else {
			r = bigAdd(a, b);
			r->dd[16] = 0;
		}
	}
	else if (a->dd[16] == 1 && b->dd[16] == 0) {
		if (bigCompare(b, bigSNeg(a)) == -1) {

			r = bigAdd(b, a);
			r->dd[16] = 1;
		}
		else {
			r = bigAdd(b, a);
			r->dd[16] = 0;
		}
	}
	else if (a->dd[16] == 1 && b->dd[16] == 1) {

		r = bigAdd(a, b, cf);
		if (cf == 0) {
			// Overflow when -a + (-b) lose most significant bit;
			// So, as it happens before Rshift, we don't need
			// any correction because most significant bit should
			// be 0 in dop. code (Rshift makes it =1 for all numbers < 0).
			cf = 6;
		}
		else if (cf == 1) {
			// If cf = 1 for negative numbers, it's all OK.
			cf = 0;
		}
		r->dd[16] = 1;
	}
	return r;
}


Big *bigSNeg(Big *n)
{
	Big *r = (Big *)calloc(1, sizeof(Big));
	r->aa[0] = ~(n->aa[0]);
	r->aa[1] = ~(n->aa[1]);
	r->dd[16] = ((n->dd[16] == 1) ? 0 : 1);
	Big *o = (Big *)calloc(1, sizeof(Big));
	o->aa[1] = 1;
	r = bigSAdd(r, o);
	return r;
}


void bigAssign(Big *n, Big *m)
{
	n->aa[0] = m->aa[0];
	n->aa[1] = m->aa[1];
	n->dd[16] = m->dd[16];
}


Big *loadBigNumber(char *fileName)
{
	int digitsCount = 0;
	int *digits = stringToIntArray(readNumber(fileName), digitsCount);
	return parseIntoBigNumber(digits, digitsCount);
}


char *readNumber(char *fileName)
{
	char *readedNumber = (char *)calloc(100, sizeof(char));
	FILE *f = fopen(fileName, "r");
	int digitsCnt = 0;

	if (f != NULL) {
		char readedSymbol = fgetc(f);
		while (readedSymbol != '\n') {
			readedNumber[digitsCnt] = readedSymbol;
			digitsCnt++;
			readedSymbol = fgetc(f);
		}
	}
	fclose(f);
	printf("%s: %s\n", fileName, readedNumber);
	return readedNumber;
}


int *stringToIntArray(char *digitsString, int &digitsCount)
{
	char readedDigit = digitsString[0];
	int digitsCnt = 0;
	int digitCharsCnt = 0;
	const char DG[11] = "0123456789";
	int *resultIntArray = (int *)calloc(100, sizeof(int));

	while (readedDigit != '\0') {
		if (strchr(DG, readedDigit) != 0) {
			uint16 digit = (uint16)(readedDigit - 0x30);
			resultIntArray[digitsCnt] = digit;
			digitsCnt++;
		}
		digitCharsCnt++;
		readedDigit = digitsString[digitCharsCnt];
	}
	digitsCount = digitsCnt;
	return resultIntArray;
}


Big *parseIntoBigNumber(int *intArray, int digitsCount)
{
	Big *addingSum = (Big *)calloc(1, sizeof(Big));
	Big *shiftedSum = (Big *)calloc(1, sizeof(Big));
	Big *finalBigNumber = (Big *)calloc(1, sizeof(Big));
	int numOfShifts = 0;
	uint8 overflowFlag = 0;

	// For every digit in big number, starting from least significant ones...
	for (int i = digitsCount - 1; i >= 0; i--) {
		// Initialize temporary numbers with this digit.
		addingSum->aa[0] = 0;
		addingSum->aa[1] = intArray[i];
		shiftedSum->aa[0] = 0;
		shiftedSum->aa[1] = 0;
		// Then we need to multiply the digit by 10 raised in power
		// which means the position of this digit in big number.
		for (int j = 0; j < numOfShifts; j++) {
			shiftedSum = bigLShift(addingSum, 1);
			addingSum = bigLShift(addingSum, 3);
			overflowFlag = 0;
			addingSum = bigAdd(addingSum, shiftedSum, overflowFlag);
			if (overflowFlag != 0) {
				printf("OVERFLOW ERROR! Number's too big.");
				system("pause>nul");
				exit(1);
			}
		}
		// After gaining the real value of this digit in big number
		// we add this value to overall sum.
		overflowFlag = 0;
		finalBigNumber = bigAdd(finalBigNumber, addingSum);
		if (overflowFlag != 0) {
			printf("OVERFLOW ERROR! Number's too big.");
			system("pause>nul");
			exit(1);
		}
		numOfShifts++;
	}
	return finalBigNumber;
}


Big *bigModSum(Big *A, Big *B, Big *M)
{
	uint8 overflowFlag = 0;
	Big *result = bigAdd(A, B, overflowFlag);
	if (overflowFlag != 0 || bigCompare(result, M) != -1) {
		Big *notM = bigNegate(M);
		result = bigAdd(result, notM);
	}
	return result;
}


Big *bigModMult(Big *A, Big *B, Big *M)
{
	Big *result = (Big *)calloc(1, sizeof(Big));
	for (int i = 0; i <= 1; i++) {
		uint64 mask2 = 0x8000000000000000;
		for (int j = 0; j < 64; j++) {
			result = bigModSum(result, result, M);
			if (B->aa[i] & mask2) {
				result = bigModSum(result, A, M);
			}
			mask2 >>= 1;
		}
	}
	return result;
}


Big *bigModPwr(Big *A, Big *B, Big *M)
{
	Big *result = (Big *)calloc(1, sizeof(Big));
	result->aa[1] = 1;
	for (int i = 0; i <= 1; i++) {
		uint64 mask2 = 0x8000000000000000;
		for (int j = 0; j < 64; j++) {
			result = bigModMult(result, result, M);
			if (B->aa[i] & mask2) {
				result = bigModMult(result, A, M);
			}
			mask2 >>= 1;
		}
	}
	return result;
}


// As( + Bt) = 1 mod B, s = A^-1
void extendedBinaryEuclidian(Big *A, Big *B, Big *s, Big *t)
{
	Big *x = (Big *)calloc(1, sizeof(Big));
	Big *y = (Big *)calloc(1, sizeof(Big));
	Big *u = (Big *)calloc(1, sizeof(Big));
	u->aa[0] = A->aa[0];
	u->aa[1] = A->aa[1];
	Big *v = (Big *)calloc(1, sizeof(Big));
	v->aa[0] = B->aa[0];
	v->aa[1] = B->aa[1];
	while (u->aa[1] % 2 == 0 && v->aa[1] % 2 == 0) {
		u = bigRShift(u, 1);
		v = bigRShift(v, 1);
	}
	x->aa[0] = u->aa[0];
	x->aa[1] = u->aa[1];
	y->aa[0] = v->aa[0];
	y->aa[1] = v->aa[1];
	Big *s2 = (Big *)calloc(1, sizeof(Big));
	s2->aa[1] = 1;
	Big *s1 = (Big *)calloc(1, sizeof(Big));
	Big *t2 = (Big *)calloc(1, sizeof(Big));
	Big *t1 = (Big *)calloc(1, sizeof(Big));
	t1->aa[1] = 1;
	uint8 cf = 0;
	while (true) {
		while (x->aa[1] % 2 == 0) {
			x = bigRShift(x, 1);
			if (s2->aa[1] % 2 == 0 && t2->aa[1] % 2 == 0) {
				s2 = bigRShift(s2, 1);
				t2 = bigRShift(t2, 1);
			}
			else {
				cf = 0;
				s2 = bigSAdd(s2, v, cf);
				s2 = bigRShift(s2, 1);
				if (cf == 1) {
					// Correct overflowed positive number after Rshifting.
					s2->aa[0] |= (uint64)0x8000000000000000;
				}
				else if (cf == 6) {
					// Undo Rshift correction of most sig. bit for neg. number.
					s2->aa[0] &= (uint64)0x7fffffffffffffff;
				}
				cf = 0;
				t2 = bigSAdd(t2, bigSNeg(u), cf);
				t2 = bigRShift(t2, 1);
				if (cf == 1) {
					// Correct overflowed positive number after Rshifting.
					t2->aa[0] |= (uint64)0x8000000000000000;
				}
				else if (cf == 6) {
					// Undo Rshift correction of most sig. bit for neg. number.
					t2->aa[0] &= (uint64)0x7fffffffffffffff;
				}
			}
		}
		while (y->aa[1] % 2 == 0) {
			y = bigRShift(y, 1);
			if (s1->aa[1] % 2 == 0 && t1->aa[1] % 2 == 0) {
				s1 = bigRShift(s1, 1);
				t1 = bigRShift(t1, 1);
			}
			else {
				cf = 0;
				s1 = bigSAdd(s1, v, cf);
				s1 = bigRShift(s1, 1);
				if (cf == 1) {
					// Correct overflowed positive number after Rshifting.
					s1->aa[0] |= (uint64)0x8000000000000000;
				}
				else if (cf == 6) {
					// Undo Rshift correction of most sig. bit for neg. number.
					s1->aa[0] &= (uint64)0x7fffffffffffffff;
				}
				cf = 0;
				t1 = bigSAdd(t1, bigSNeg(u), cf);
				t1 = bigRShift(t1, 1);
				if (cf == 1) {
					// Correct overflowed positive number after Rshifting.
					t1->aa[0] |= (uint64)0x8000000000000000;
				}
				else if (cf == 6) {
					// Undo Rshift correction of most sig. bit for neg. number.
					t1->aa[0] &= (uint64)0x7fffffffffffffff;
				}
			}
		}
		if (bigCompare(x, y) != -1) {
			x = bigSAdd(x, bigSNeg(y));
			s2 = bigSAdd(s2, bigSNeg(s1));
			t2 = bigSAdd(t2, bigSNeg(t1));
		}
		else {
			y = bigSAdd(y, bigSNeg(x));
			s1 = bigSAdd(s1, bigSNeg(s2));
			t1 = bigSAdd(t1, bigSNeg(t2));
		}
		if (x->aa[0] == 0 && x->aa[1] == 0) {
			s->aa[0] = s1->aa[0];
			s->aa[1] = s1->aa[1];
			s->dd[16] = s1->dd[16];
			t->aa[0] = t1->aa[0];
			t->aa[1] = t1->aa[1];
			t->dd[16] = t1->dd[16];
			return;
		}
	}
}


void printBigDecimal(Big *number)
{
	uint32 *intArray = (uint32 *)calloc(5, sizeof(uint32));
	for (int i = 0; i <= 63; i++) {
		uint64 t = (number->aa[0] << (63 - i));
		uint64 temp = t >> 63;
		if (temp != 0) {
			// Creating a number which equals (temp * 2 ^ i).
			uint32 *tempIntArray = (uint32 *)calloc(5, sizeof(uint32));
			tempIntArray[4] = temp;
			for (int j = 0; j < 64 + i; j++) {
				for (int k = 4; k >= 0; k--) {
					tempIntArray[k] *= 2;
				}
				for (int k = 4; k >= 1; k--) {
					if (tempIntArray[k] >= DEC_MAX) {
						tempIntArray[k - 1] += (uint32)(tempIntArray[k] / DEC_MAX);
						tempIntArray[k] %= DEC_MAX;
					}
				}
			}
			// Add created number to resulting array.
			for (int k = 4; k >= 1; k--) {
				intArray[k] += tempIntArray[k];
				if (intArray[k] >= DEC_MAX) {
					intArray[k - 1] += (uint32)(intArray[k] / DEC_MAX);
					intArray[k] %= DEC_MAX;
				}
			}
			intArray[0] += tempIntArray[0];
		}
	}

	for (int i = 0; i <= 63; i++) {
		uint64 t = (number->aa[1] << (63 - i));
		uint64 temp = t >> 63;
		if (temp != 0) {
			// Creating a number which equals (temp * 2 ^ i).
			uint32 *tempIntArray = (uint32 *)calloc(5, sizeof(uint32));
			tempIntArray[4] = temp;
			for (int j = 0; j < i; j++) {
				for (int k = 4; k >= 0; k--) {
					tempIntArray[k] *= 2;
				}
				for (int k = 4; k >= 1; k--) {
					if (tempIntArray[k] >= DEC_MAX) {
						tempIntArray[k - 1] += (uint32)(tempIntArray[k] / DEC_MAX);
						tempIntArray[k] %= DEC_MAX;
					}
				}
			}
			// Add created number to resulting array.
			for (int k = 4; k >= 1; k--) {
				intArray[k] += tempIntArray[k];
				if (intArray[k] >= DEC_MAX) {
					intArray[k - 1] += (uint32)(intArray[k] / DEC_MAX);
					intArray[k] %= DEC_MAX;
				}
			}
			intArray[0] += tempIntArray[0];
		}
	}

	printf("%u ", number->dd[16]);
	for (int i = 0; i < 5; i++) {
		printf("%03u %03u %03u ", intArray[i] / 1000000,
			(intArray[i] / 1000) % 1000, intArray[i] % 1000);
	}
	printf("\n");
}


void checkTypes()
{
	printf("Long long int: %d\n", sizeof(uint64));
	printf("Long int: %d\n", sizeof(uint32));
	printf("Int: %d\n", sizeof(uint16));
	printf("Short int: %d\n", sizeof(uint8));
}