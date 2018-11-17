
#define CAT(X, Y) X Y

#ifndef _MSC_VER
#define NASM_COMMENT(X) __asm__(CAT(";", #X));
#endif

#define NASM_CPP(X) \
    NASM_COMMENT(X) \
    X

int main() {

	float arr1[10000], arr2[10000];

	for (int i = 0; i < 10000; i++) {
		arr1[i] = i;
		arr2[i] = i;
	}

	NASM_COMMENT("starts here")
	for (int i = 0; i < 10000; i++) {
		NASM_CPP(arr1[i] = +arr2[i];)
	}
	NASM_COMMENT("ends here")

	return 0;
}
