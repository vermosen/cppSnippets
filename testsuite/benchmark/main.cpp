
u64 g_correctionFactor;  // number of clocks to offset after each measurement to remove the overhead of the measurer itself.
u64 g_accuracy;

static u64 const errormeasure = ~((u64)0);

#ifdef _MSC_VER
#pragma intrinsic(__rdtsc)
inline u64 GetRDTSC()
{
    int a[4];
    __cpuid(a, 0x80000000);  // flush OOO instruction pipeline
    return __rdtsc();
}

inline void WarmupRDTSC()
{
    int a[4];
    __cpuid(a, 0x80000000);  // warmup cpuid.
    __cpuid(a, 0x80000000);
    __cpuid(a, 0x80000000);

    // measure the measurer overhead with the measurer (crazy he..)
    u64 minDiff = LLONG_MAX;
    u64 maxDiff = 0;   // this is going to help calculate our PRECISION ERROR MARGIN
    for (int i = 0; i < 80; ++i)
    {
        u64 tick1 = GetRDTSC();
        u64 tick2 = GetRDTSC();
        minDiff = Aska::Min(minDiff, tick2 - tick1);   // make many takes, take the smallest that ever come.
        maxDiff = Aska::Max(maxDiff, tick2 - tick1);
    }
    g_correctionFactor = minDiff;

    printf("Correction factor %llu clocks\n", g_correctionFactor);

    g_accuracy = maxDiff - minDiff;
    printf("Measurement Accuracy (in clocks) : %llu\n", g_accuracy);
}
#endif

#include <iostream>

int main()
{
    WarmupRDTSC();


    std::cout << "Hello World!\n";
}
