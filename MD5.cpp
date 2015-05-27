#include <iostream>
#include <string>

using namespace std;

// constants
unsigned const s[] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

// constants
unsigned const k[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// constants
#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define shift(x, n) ((x << n) | (x >> (32 - n)))

char const str16[] = "0123456789abcdef";

unsigned a0 = A;
unsigned b0 = B;
unsigned c0 = C;
unsigned d0 = D;
unsigned * N;

// transfer Decimal to Hex
string toHex(unsigned x) {

	string result = "";
	int i;

	for (i = 0; i < 4; i++) {
		result += str16[(x / 16) % 16];
		result += str16[x % 16];
		x /= 256;
	}

	return result;
}

// add bytes to the original string
unsigned add(string & str) {

	int sizeOfStr = str.size();
	int sizeOfN = sizeOfStr / 4;
	int i, j;

	sizeOfN++;

	while (sizeOfN % 16 != 14)
		sizeOfN++;

	N = new unsigned[sizeOfN + 2];

	for (i = 0; i < sizeOfStr / 4; i++) {
		N[i] = 0;
		for (j = 3; j >= 0; j--) {
			N[i] *= 256;
			N[i] += (unsigned)str[i * 4 + j];
		}
	}

	N[i] = 128;
	for (j = 1; (sizeOfStr - j + 4) % 4 != 3; j++) {
		N[i] *= 256;
		N[i] += str[sizeOfStr - j];
	}

	for (i++; i < sizeOfN; i++)
		N[i] = 0;

	N[i++] = sizeOfStr * 8;
	N[i] = 0;

	return sizeOfN + 2;
}

// main loop
void mainLoop (unsigned * M) {

	unsigned g;
	unsigned f;

	unsigned a = a0;
	unsigned b = b0;
	unsigned c = c0;
	unsigned d = d0;

	unsigned tmp;
	int i;

	for (i = 0; i < 64; i++) {
		if (i < 16) {
			f = (b & c) | (~b & d);
			g = i;
		} else if (i < 32) {
			f = (d & b) | (~d & c);
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			f = b ^ c ^ d;
			g = (3 * i + 5) % 16;
		} else {
			f = c ^ (b | ~d);
			g = (7 * i) % 16;
		}

		tmp = d;
		d = c;
		c = b;
		b += shift((a + f + k[i] + M[g]), s[i]);
		a = tmp;
	}

	a0 += a;
	b0 += b;
	c0 += c;
	d0 += d;
}

// MD5 function
string MD5(string & str) {

	int length = add(str);

	int i, j;
	unsigned M[16];

	for (i = 0; i < length; i += 16) {
		for (j = 0; j < 16; j++)
			M[j] = N[i + j];
		mainLoop(M);
	}

	delete N;

	return toHex(a0) + toHex(b0) + toHex(c0) + toHex(d0);
}

int main() {

// a test case
	string str = "0123456789abcdefghijklmnopqrstuvwxyz";

	cout << "Str: " << str << endl;
	cout << "MD5: " << MD5(str) << endl;

	return 0;
}
