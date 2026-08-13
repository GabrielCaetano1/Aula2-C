#include <stdio.h>
int main() {
    float C;
    float F;

    printf("Digite a temperatura: ");
    scanf("%f", &C);

    F = ( C * 9/5 ) + 32; 
    
    printf("A conversao da temperatura para farenheit fica: %.2f graus.",  F);
    return 0;
}