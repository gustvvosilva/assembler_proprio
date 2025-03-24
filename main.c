#include "main.h"

DATACODIGO *init_datacodigo() {
    DATACODIGO *nova = (DATACODIGO *) malloc(sizeof(DATACODIGO));
    nova->instrucao = 0;
    nova->variavel = 0;
    nova->tem2bytes = false;
    nova->prox = NULL;
    return nova;
}

void inserir_cod(DATACODIGO *datacodigo, __uint8_t instrucao, __uint8_t variavel, bool tem2bytes) {

    if(datacodigo->prox != NULL) {
        inserir_cod(datacodigo->prox, instrucao, variavel, tem2bytes);
        return;
    }

    DATACODIGO *nova = (DATACODIGO *) malloc(sizeof(DATACODIGO));
    nova->instrucao = instrucao;
    nova->variavel = variavel;
    nova->tem2bytes = tem2bytes;
    nova->prox = NULL;
    datacodigo->prox = nova;
    return;
}

void imprimir_cod(DATACODIGO *datacodigo) {

    DATACODIGO *auxiliar = datacodigo;

    printf("Datacodigo:\n");
    while (auxiliar != NULL) {
        printf("> %02x - %02x\n", auxiliar->instrucao, auxiliar->variavel);
        auxiliar = auxiliar->prox;
    }
    printf("\n");
    return;
}

void excluir_cod(DATACODIGO *datacodigo) {

    DATACODIGO *apagar;
    DATACODIGO *auxiliar = datacodigo;

    while (auxiliar != NULL) {
        apagar = auxiliar;
        auxiliar = auxiliar->prox;
        free(apagar);
    }

    printf("Datacodigo apagado com sucesso.\n");
    return;
}

__uint8_t obter_dado_cod(FILE *file) {

    char leitura[1];
    __uint8_t variavel[1];

    for(;;) {

        fread(leitura, 1, 1, file);

        // printf("C %c\n", *leitura);

        if(*leitura != 32 && *leitura != 10)
        {
            variavel[0] = *leitura;
        }
        else
        {
            printf(">L %c = %02x\n", variavel[0], variavel[0]);
            // printf("kkkkkkk %d\n", strcmp(linha, "ADD"));
            break;
        }
    }
    return variavel[0];
}

DATAVALOR *init_datavalor() {
    DATAVALOR *nova = (DATAVALOR *) malloc(sizeof(DATAVALOR));
    nova->variavel = 0;
    nova->valor = 0;
    nova->prox = NULL;
    return nova;
}

void inserir_val(DATAVALOR *datavalor, __uint8_t variavel, __uint8_t valor) {

    if(datavalor->prox != NULL) {
        inserir_val(datavalor->prox, variavel, valor);
        return;
    }

    DATAVALOR *nova = (DATAVALOR *) malloc(sizeof(DATAVALOR));
    nova->variavel = variavel;
    nova->valor = valor;
    nova->prox = NULL;
    datavalor->prox = nova;
    return;
}

void imprimir_val(DATAVALOR *datavalor) {

    DATAVALOR *auxiliar = datavalor;

    printf("Datavalor:\n");
    while (auxiliar != NULL) {
        printf("> %02x - %02x\n", auxiliar->variavel, auxiliar->valor);
        auxiliar = auxiliar->prox;
    }
    printf("\n");
    return;
}

void excluir_val(DATAVALOR *datavalor) {

    DATAVALOR *apagar;
    DATAVALOR *auxiliar = datavalor;

    while (auxiliar != NULL) {
        apagar = auxiliar;
        auxiliar = auxiliar->prox;
        free(apagar);
    }

    printf("Datavalor apagado com sucesso.\n");
    return;
}

__uint8_t obter_dado_val(FILE *file2) {

    char caractere[1];
    char linha[8] = {0};
    int i = 0;
    __uint8_t valor;

    for(;;) {

        fread(caractere, 1, 1, file2);
        // printf("C %c\n", *caractere);

        if(*caractere != 32 && *caractere != 10)
        {
            linha[i] = *caractere;
            i++;
        }
        else
        {
            printf(">L %s\n", linha);
            // printf("kkkkkkk %d\n", strcmp(linha, "ADD"));

            if(strcmp(linha, "DB") != 0) {
                if(strcmp(linha, "?") == 0){
                    valor = 0;
                    break;
                }
                else
                {
                    valor = atoi(linha);
                    break;
                }
            }

            limpa_linha(linha);
            i = 0;
        }
    }
    return valor;
}

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

    DATACODIGO *datacodigo = init_datacodigo();

    for(;;) {

        if(!fread(caractere, 1, 1, file))
            break;

        // printf("C %c\n", *caractere);

        if(*caractere != 32 && *caractere != 10)
        {
            linha[i] = *caractere;
            i++;
        }
        else
        {
            printf(">L %s\n", linha);
            // printf("kkkkkkk %d\n", strcmp(linha, "ADD"));

            if(strcmp(linha, "STA") == 0) {
                inserir_cod(datacodigo, STA, obter_dado_cod(file), true);
            }
            else if (strcmp(linha, "LDA") == 0) {
                inserir_cod(datacodigo, LDA, obter_dado_cod(file), true);               
            }
            else if (strcmp(linha, "ADD") == 0) {
                inserir_cod(datacodigo, ADD, obter_dado_cod(file), true);
            }
            else if (strcmp(linha, "HLT") == 0) {
                inserir_cod(datacodigo, HLT, 0, false);
            }

            limpa_linha(linha);
            i = 0;
        }
    }
    fclose(file);

    FILE *file2 = fopen("entrada2.txt", "r");
    if(file2 == NULL) return -1;

    DATAVALOR *datavalor = init_datavalor();

    limpa_linha(linha);
    i = 0;
    for(;;) {

        if(!fread(caractere, 1, 1, file2))
            break;

        // printf("C %c\n", *caractere);

        if(*caractere != 32 && *caractere != 10)
        {
            linha[i] = *caractere;
            i++;
        }
        else
        {
            printf(">L %s\n", linha);
            // printf("kkkkkkk %d\n", strcmp(linha, "ADD"));

            if((linha[0] >= 0x41 && linha[0] <= 0x5a) || (linha[0] >= 0x61 && linha[0] <= 0x7a)) {
                inserir_val(datavalor, linha[0], obter_dado_val(file2));
            }

            limpa_linha(linha);
            i = 0;
        }
    }
    fclose(file2);

    imprimir_cod(datacodigo);
    imprimir_val(datavalor);
    
    // FILE *file3 = fopen("soma.mem", "rb");
    // if(file3 == NULL) return -1;

    // __uint8_t bytes[516];

    // fread(bytes, 1, 516, file);
    // fclose(file3);
    
    // printf("\nHEXDUMP:");
    // for(int i = 0; i < 516; i++) {

    //     if (i % 2 == 0)
    //         printf(" ");
    //     if (i % 16 == 0)
    //         printf("\n%07x ", i);

    //     printf("%02x", bytes[i]);
    // }
    // printf("\n");    

    FILE *memoria = fopen("memoria.mem", "wb");
    if(memoria == NULL) return -1;

    __uint8_t byteZerado[512] = {0x00};

    __uint32_t memoriaID[] = {0x52444e03};  // Little Endian

    fwrite(memoriaID, 4, 1, memoria);

    fwrite(byteZerado, 1, 512, memoria);

    fclose(memoria);



    excluir_cod(datacodigo);
    excluir_val(datavalor);

    return 0;
}