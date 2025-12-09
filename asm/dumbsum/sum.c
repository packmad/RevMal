#include <stdio.h>

int dumbsum4args(int w, int x, int y, int z) {
	// Caller: dumbsum8args
	// Callee: dumbsum4args
	return w+x+y+z;
}

int dumbsum8args(int a, int b, int c, int d, int e, int f, int g, int h) {
	// Caller: main
	// Callee: dumbsum8args
	int partial_sum = dumbsum4args(a,b,c,d);
	partial_sum += dumbsum4args(e,f,g,h);
	return partial_sum;
}

int main() {
	printf("Sum=%d\n", dumbsum8args(1, 2, 4, 8, 16, 32, 64, 128));
}
