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
    nova->endereco = 0;
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
    nova->endereco = 0;
    nova->prox = NULL;
    datavalor->prox = nova;
    return;
}

void imprimir_val(DATAVALOR *datavalor) {

    DATAVALOR *auxiliar = datavalor;

    printf("Datavalor:\n");
    while (auxiliar != NULL) {
        printf("> %02x - %02x %02x\n", auxiliar->variavel, auxiliar->valor, auxiliar->endereco);
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

__uint8_t procurar_variavel(DATAVALOR *datavalor, __uint8_t var) {

    DATAVALOR *auxiliar = datavalor;
    __uint8_t endereco = 0;

    while(auxiliar != NULL) {

        if(auxiliar->variavel == var) {
            endereco = auxiliar->endereco;
            break;
        }
        auxiliar = auxiliar->prox;
    }

    return endereco;
}

void limpa_linha(char *linha) {
    for (int i = 0; linha[i] != 0; i++)
        linha[i] = 0;
    return;
}

void obter_tokens(FILE *entrada, DATACODIGO *datacodigo, DATAVALOR *datavalor) {

    char caractere[1];
    char linha[8] = {0};
    int i = 0;
    bool lendo_data;
    bool lendo_code;

    while(fread(caractere, 1, 1, entrada)) {
        
        // printf("-C %c\n", *caractere);

        if(*caractere != 32 && *caractere != 10)
        {
            linha[i] = *caractere;
            i++;
        }
        else
        {
            printf(">L %s\n", linha);

            if(strcmp(linha, ".DATA") == 0) {
                lendo_data = true;
                lendo_code = false;
            }
            else if (strcmp(linha, ".CODE") == 0) {
                lendo_data = false;
                lendo_code = true;
            } 
            else if(lendo_data)
            {
                if((linha[0] >= 0x41 && linha[0] <= 0x5a) || (linha[0] >= 0x61 && linha[0] <= 0x7a)) {
                    inserir_val(datavalor, linha[0], obter_dado_val(entrada));
                }
            }
            else if(lendo_code)
            {
                if(strcmp(linha, "STA") == 0) {
                    inserir_cod(datacodigo, STA, obter_dado_cod(entrada), true);
                }
                else if (strcmp(linha, "LDA") == 0) {
                    inserir_cod(datacodigo, LDA, obter_dado_cod(entrada), true);               
                }
                else if (strcmp(linha, "ADD") == 0) {
                    inserir_cod(datacodigo, ADD, obter_dado_cod(entrada), true);
                }
                else if (strcmp(linha, "HLT") == 0) {
                    inserir_cod(datacodigo, HLT, 0, false);
                }
            }

            limpa_linha(linha);
            i = 0;
        }
    }

    return;
}

bool criar_memoria() {

    FILE *memoria = fopen("memoria.mem", "wb");
    if(memoria == NULL) return false;

    __uint32_t memoriaID[] = {0x52444e03};  // Little Endian
    __uint8_t byteZerado[512] = {0x00};

    fwrite(memoriaID, 4, 1, memoria);
    fwrite(byteZerado, 1, 512, memoria);
    fclose(memoria);

    return true;
}

bool inserir_tokens(DATACODIGO *datacodigo, DATAVALOR *datavalor) {

    FILE *memoria = fopen("memoria.mem", "r+b");
    if(memoria == NULL) return false;

    DATACODIGO *datacodigo_aux = datacodigo->prox;
    DATAVALOR *datavalor_aux = datavalor->prox;
    __uint8_t endereco;

    int i = 4;

    while(datacodigo_aux != NULL) {

        if(datacodigo_aux->tem2bytes)
            i += 4;
        else
            i += 2;

        datacodigo_aux = datacodigo_aux->prox;
    }
    while(datavalor_aux != NULL) {

        fseek(memoria, i, SEEK_SET);
        fwrite(&datavalor_aux->valor, 1, 1, memoria);
        datavalor_aux->endereco = i;
        datavalor_aux = datavalor_aux->prox;
        i += 2;
    }
    
    i = 4;
    datacodigo_aux = datacodigo->prox;

    while(datacodigo_aux != NULL) {

        fseek(memoria, i, SEEK_SET);
        fwrite(&datacodigo_aux->instrucao, 1, 1, memoria);
        if(datacodigo_aux->tem2bytes)
        {
            i += 2;
            fseek(memoria, i, SEEK_SET);
            endereco = (procurar_variavel(datavalor->prox, datacodigo_aux->variavel) - 4) / 2;
            fwrite(&endereco, 1, 1, memoria);
        }
        datacodigo_aux = datacodigo_aux->prox;
        i += 2;
    }

    fclose(memoria);
    return true;
}

void imprimir_memoria() {

    FILE *memoria = fopen("memoria.mem", "rb");
    if(memoria == NULL) {
        printf("ERRO ao imprimir memoria.\n");
        return;
    }

    __uint8_t bytes[516];

    fread(bytes, 1, 516, memoria);
    fclose(memoria);
    
    printf("\nHEXDUMP:");
    for(int i = 0; i < 516; i++) {

        if (i % 2 == 0)
            printf(" ");
        if (i % 16 == 0)
            printf("\n%07x ", i);

        printf("%02x", bytes[i]);
    }
    printf("\n");

    return;
}

int main() {

    FILE *entrada = fopen("entrada.asm", "r");
    if(entrada == NULL) return -1;

    DATACODIGO *datacodigo = init_datacodigo();
    DATAVALOR *datavalor = init_datavalor();

    obter_tokens(entrada, datacodigo, datavalor);
    fclose(entrada);

    imprimir_cod(datacodigo);
    imprimir_val(datavalor);

    if(!criar_memoria())
        return -1;

    if(!inserir_tokens(datacodigo, datavalor))
        return -1;

    imprimir_val(datavalor);
    excluir_cod(datacodigo);
    excluir_val(datavalor);

    imprimir_memoria();
    
    return 0;
}