#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool short
#define true 1
#define false 0
#define equals(a, b) (((strcmp(a, b) == 0) ? true : false))
#define NUMENTRADA 1000
#define TAMLINHA 1000

/**
 * A função ehPalindromo verifica se uma palavra é um palíndromo.
 */
bool ehPalindromo(char palavra[])
{
    int tamanho = strlen(palavra);
    for (int i = 0; i < tamanho / 2; i++)
    {
        if (palavra[i] != palavra[tamanho - 1 - i])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    char palavra[TAMLINHA];
    while (1)
    {
        scanf("%s", palavra);
        if (equals(palavra, "FIM"))
        {
            break;
        }

        if (ehPalindromo(palavra))
        {
            printf("SIM\n");
        }
        else
        {
            printf("NAO\n");
        }
    }

    return 0;
}
