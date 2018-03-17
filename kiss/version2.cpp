#include <stdio.h>
#include <iostream>

static unsigned long long
x=1234567890987654321ULL,c=123456123456123456ULL,
y=362436362436362436ULL,z=1066149217761810ULL,t;

#define MWC (t=(x<<58)+c, c=(x>>6), x+=t, c+=(x<t), x)
#define XSH ( y^=(y<<13), y^=(y>>17), y^=(y<<43) )
#define CNG ( z=6906969069LL*z+1234567 )
#define KISS (MWC+XSH+CNG)

int main(void) {
    int i;
    for(int i=0;i<100000000;i++) {
	t = KISS;
	if(i < 5) {
	    std::cout << t << std::endl;
	}
    }

    if(t==1666297717051644203ULL) {
	printf("100 million uses of KISS OK");
	return 0;
    } else {
	printf("Fail");
	return -1;
    }
}
