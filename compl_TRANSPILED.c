#include <stdio.h>
float sub(float a, float b) { float result;
result = a - b;
return result;
}
void print_float(float a) {
printf("%.2f and ",a);
}
void print_int(int a) {
printf("%d\n",a);
}
int main() {
float foo = 69.42;
print_float(sub(foo,21 / 21));
int x = 32;
print_int(x);
printf("And of course, Hello World!");
return 0;
}
