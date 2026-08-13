#include <stdio.h>
int main() {
    int nota1;
    int nota2;
    int media;
    
    //O %d espera um inteiro, ele que recebe o valor da variavel
    // Pra float tem que ser %f
    printf("Digite a nota 1: ");
    scanf("%f", &nota1);

    printf("Digite a nota 2: ");
    scanf("%f", &nota2);

    media = (nota1 + nota2) / 2;
    printf("A media e: %.2f. A nota1:%.2f. nota2:%.2f", media, nota1, nota2);
    return 0;
}