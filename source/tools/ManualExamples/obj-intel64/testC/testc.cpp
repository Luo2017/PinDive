#include<cstdio>
int main(int argc, char* argv[])
{
    double d[10];
    d[0] = 1.0;
    d[1] = 2.0;
    for (int i = 2; i < 10; i++) {
        d[i] = d[i - 1] * d[i - 2];
        printf("%lf\n", d[i]);
    }
    return 0;
}