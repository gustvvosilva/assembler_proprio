#include "main.h"

void limpa_linha(char *linha) {
    for (int i = 0; linha[i] != 0; i++)
        linha[i] = 0;
    return;
}

int main() {
    
    FILE *file = fopen("entrada.txt", "r");
    if(file == NULL) return -1;

    char caractere[1];
    char linha[8];
    int i = 0;

    for(;;) {

        if(!fread(caractere, 1, 1, file))
            break;

        printf("C %c\n", *caractere);

        if(*caractere != 32 && *caractere != 10)
        {
            linha[i] = *caractere;
            i++;
        }
        else
        {
            printf(">L %s\n", linha);
            // printf("kkkkkkk %d\n", strcmp(linha, "ADD"));



            limpa_linha(linha);
            i = 0;
        }
    }

    fclose(file);
    return 0;
}