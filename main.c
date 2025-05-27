#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Necessário para toupper

// --- Protótipos de Funções ---
FILE* abrirArq(char nome[], char modo[]);
FILE* fecharArq(FILE *arq);
void adicionarPalavra();
void adicionarDica();
void sorteio();
void pausa();
void menu();
void init();
void exibeboneco(int membros);
void resultado(unsigned char palavra[201], bool res); // Alterado para unsigned char
void jogar();
// Assinatura de procletra alterada para usar unsigned char*
bool procletra(unsigned char letra, unsigned char palavradit[201], unsigned char *esconde, unsigned char palavra[201]);

// --- Funções Auxiliares ---

// Função para limpar o buffer de entrada de forma portátil
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Implementação portátil de strupr
char* my_strupr(unsigned char *str) {
    unsigned char *p = str;
    while (*p) {
        *p = toupper((unsigned char)*p);
        p++;
    }
    return (char*)str; // Retorna char* para compatibilidade com strcmp, etc.
}

// --- Variáveis Globais ---
unsigned int palavraraso;

// --- Estruturas ---
typedef struct registro{
    unsigned int idDica;
    unsigned char palavra[201];
} registro;

typedef struct dica{
    unsigned char dica[201];
} dica;

// --- Implementação das Funções ---

int main(){
    init();
    menu();
    return 0;
}

// Função para abrir arquivo
FILE* abrirArq(char nome[], char modo[]){
    FILE *f = fopen(nome, modo);
    if(f == NULL){
        printf("Erro ao abrir o arquivo %s\n", nome);
    }
    return f;
}

// Função para fechar arquivo
FILE* fecharArq(FILE *arq){
    if(arq != NULL){
        fclose(arq);
    }
    return NULL;
}

void init(){
    FILE *temp;
    temp = abrirArq("dicas.fc", "a");
    temp = fecharArq(temp);
    temp = abrirArq("palavras.fc", "a");
    temp = fecharArq(temp);
}

void menu(){
    char opt;
    do{
        system("cls");
        printf("===========================================\n");
        printf("|              JOGO DA FORCA              |\n");
        printf("===========================================\n");
        printf("| 1. INICIAR JOGO\n");
        printf("| 2. ADICIONAR DICA \n");
        printf("| 3. ADICIONAR PALAVRA \n");
        printf("| 4. SAIR \n");
        printf("===========================================\n");
        printf("Escolha uma opcao: ");
        scanf("%c", &opt);
        limparBuffer(); // Limpa o buffer após a leitura de char

        if(opt == '1'){
            jogar();
        }else if(opt == '2'){
            adicionarDica();
        }else if(opt == '3'){
            adicionarPalavra();
        }
    }while(opt != '4');
}

void jogar(){
    registro guarda;
    size_t i; // Alterado para size_t
    size_t j; // Alterado para size_t
    int tamanho = 0;
    int qtdErro = 0;
    bool fim = false;
    bool ganhou = false;
    bool achou = false;
    unsigned char letra;
    unsigned char letDig[100]; // Não precisa inicializar com '0' se vai terminar com '\0'
    unsigned char palavraEdit[201];
    unsigned char esconde[201];
    dica gua;
    FILE *arq;

    sorteio();

    arq = abrirArq("palavras.fc", "r");
    if (arq == NULL) {
        printf("Erro fatal: Nao foi possivel abrir o arquivo de palavras para jogar.\n");
        pausa();
        return;
    }
    fseek(arq, sizeof(guarda) * palavraraso, SEEK_SET);
    fread(&guarda, sizeof(guarda), 1, arq);
    arq = fecharArq(arq);

    arq = abrirArq("dicas.fc", "r");
    if (arq == NULL) {
        printf("Erro fatal: Nao foi possivel abrir o arquivo de dicas para jogar.\n");
        pausa();
        return;
    }
    fseek(arq, sizeof(gua) * guarda.idDica, SEEK_SET);
    fread(&gua, sizeof(gua), 1, arq);
    arq = fecharArq(arq);

    letDig[0] = '\0'; // Garante terminação nula

    for(i = 0; i < strlen((char*)guarda.palavra); i++){
        if(guarda.palavra[i] == (unsigned char)181 || guarda.palavra[i] == (unsigned char)183 || guarda.palavra[i] == (unsigned char)199 || guarda.palavra[i] == (unsigned char)182 || guarda.palavra[i] == (unsigned char)142){
            palavraEdit[i] = (unsigned char)65; // 'A'
        } else if(guarda.palavra[i] == (unsigned char)144 || guarda.palavra[i] == (unsigned char)212 || guarda.palavra[i] == (unsigned char)210 || guarda.palavra[i] == (unsigned char)211){
            palavraEdit[i] = (unsigned char)69; // 'E'
        } else if(guarda.palavra[i] == (unsigned char)214 || guarda.palavra[i] == (unsigned char)222 || guarda.palavra[i] == (unsigned char)225 || guarda.palavra[i] == (unsigned char)216){
            palavraEdit[i] = (unsigned char)73; // 'I'
        } else if(guarda.palavra[i] == (unsigned char)224 || guarda.palavra[i] == (unsigned char)227 || guarda.palavra[i] == (unsigned char)229 || guarda.palavra[i] == (unsigned char)153){
            palavraEdit[i] = (unsigned char)79; // 'O'
        } else if(guarda.palavra[i] == (unsigned char)233 || guarda.palavra[i] == (unsigned char)235 || guarda.palavra[i] == (unsigned char)234 || guarda.palavra[i] == (unsigned char)154){
            palavraEdit[i] = (unsigned char)85; // 'U'
        } else {
            palavraEdit[i] = guarda.palavra[i];
        }
    }
    palavraEdit[i] = '\0';

    for(i = 0; i < strlen((char*)palavraEdit); i++){
        if(palavraEdit[i] == ' '){
            esconde[i] = ' ';
        } else if(palavraEdit[i] == '-' || palavraEdit[i] == '_'){
            esconde[i] = '-';
        } else {
            esconde[i] = '_';
        }
        if(palavraEdit[i] != ' ' && palavraEdit[i] != '-' && palavraEdit[i] != '_'){
            tamanho++;
        }
    }
    esconde[i] = '\0';
    j = 0;

    while(fim == false){
        system("cls");
        exibeboneco(qtdErro);
        printf("\n\t\t\t");
        for(i = 0; i < strlen((char*)esconde); i++){
            printf("%c ", esconde[i]);
        }

        printf("\n\n\tLETRAS DIGITADAS: ");
        for(i = 0; i < strlen((char*)letDig); i++){
            printf("%c ", letDig[i]);
        }

        printf("\n\n\tDICA: %s", (char*)gua.dica); // Cast para printf
        printf("\n\n\tTAMANHO: %d", tamanho);
        printf("\n\tDIGITE UMA LETRA: ");
        scanf("%c", &letra);
        limparBuffer();

        if(letra >= (unsigned char)'a' && letra <= (unsigned char)'z'){ // Cast para comparação
            letra = (unsigned char)toupper(letra); // Cast para atribuição
        } else if(letra == (unsigned char)135){ // Caractere 'Ç' minúsculo
            letra = (unsigned char)128; // 'Ç' maiúsculo
        }

        achou = false;
        for(i = 0; i < j; i++){
            if(letra == letDig[i]){
                achou = true;
                break;
            }
        }

        if(achou == false){
            letDig[j] = letra;
            letDig[j+1] = '\0';
            j++;
            // Não precisa de casts aqui se os parâmetros de procletra foram ajustados
            if(procletra(letra, palavraEdit, esconde, guarda.palavra) == true){
                qtdErro++;
            }
        }

        if(qtdErro == 6){
            fim = true;
            ganhou = false;
        } else if(strcmp((char*)guarda.palavra, (char*)esconde) == 0){
            ganhou = true;
            fim = true;
        }
    }
    resultado(guarda.palavra, ganhou); // Argumento já é unsigned char[]
}

// Implementação de procletra com parâmetros unsigned char*
bool procletra(unsigned char letra, unsigned char palavraEdit[201], unsigned char *esconde, unsigned char palavra[201]){
    bool erro = true;
    size_t i; // Alterado para size_t
    for(i = 0; i < strlen((char*)palavraEdit); i++){
        if(letra == palavraEdit[i]){
            erro = false;
            esconde[i] = palavra[i];
        }
    }
    return erro;
}

// Assinatura de resultado alterada para receber unsigned char
void resultado(unsigned char palavra[201], bool res){
    system("cls");
    printf("-----RESULTADO-----\n");
    printf("\t PALAVRA: %s\n", (char*)palavra); // Cast para printf
    if(res == true){
        printf("\tVOCE GANHOU!\n");
    } else {
        printf("\tVOCE PERDEU!\n");
    }
    pausa();
}

void exibeboneco(int membros){
    switch(membros){
        case 1:
            printf("\n\n");
            printf("             __________\n");
            printf("            | /        |\n");
            printf("            |/         O\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("           _L_\n");
            break;
        case 2:
            printf("\n\n");
            printf("             __________\n");
            printf("            | /        |\n");
            printf("            |/         O\n");
            printf("            |          |\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("           _L_\n");
            break;
        case 3:
            printf("\n\n");
            printf("             __________\n");
            printf("            | /        |\n");
            printf("            |/         O\n");
            printf("            |         /|\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("           _L_\n");
            break;
        case 4:
            printf("\n\n");
            printf("             __________\n");
            printf("            | /        |\n");
            printf("            |/         O\n");
            printf("            |         /|\\\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("           _L_\n");
            break;
        case 5:
            printf("\n\n");
            printf("             __________\n");
            printf("            | /        |\n");
            printf("            |/         O\n");
            printf("            |         /|\\\n");
            printf("            |         /\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("           _L_\n");
            break;
        case 6:
            printf("\n\n");
            printf("             __________\n");
            printf("            | /        |\n");
            printf("            |/         O\n");
            printf("            |         /|\\\n");
            printf("            |         / \\\n");
            printf("            |\n");
            printf("            |\n");
            printf("            |\n");
            printf("           _L_\n");
            break;
    }
}

void pausa(){
    printf("\nAPERTTE UMA TECLA PARA VOLTAR AO MENU...");
    getchar();
    limparBuffer();
}

void sorteio(){
    srand((unsigned int)time(NULL)); // Cast para unsigned int
    FILE *arq;
    registro temp;
    unsigned int qtd = 0;

    arq = abrirArq("palavras.fc", "r");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo de palavras para sorteio.\n");
        exit(1);
    }

    while(fread(&temp, sizeof(temp), 1, arq) == 1){
        qtd++;
    }
    arq = fecharArq(arq);

    if (qtd == 0){
        printf("Nenhuma palavra cadastrada para sortear.\n");
        pausa();
        exit(1);
    }

    palavraraso = rand() % qtd;
}

void adicionarDica(){
    dica guarda, temp;
    char resp;
    bool erro = false;
    do{
        system("cls");
        printf("\nDIGITE UMA DICA DE ATE 200 CARACTERES: ");
        scanf("%[^\n]", guarda.dica);
        limparBuffer();

        printf("\nDESEJA CONTINUAR COM A DICA %s S/N? ", (char*)guarda.dica); // Cast para printf
        resp = getchar();
        limparBuffer();
    }while(resp != 's' && resp != 'S');

    FILE *arq;
    my_strupr(guarda.dica);
    arq = abrirArq("dicas.fc", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de dicas para verificar duplicidade.\n");
        pausa();
        return;
    }

    while(fread(&temp, sizeof(temp), 1, arq) == 1){
        if(strcmp((char*)guarda.dica, (char*)temp.dica) == 0){
            printf("\nEssa dica ja existe!");
            erro = true;
            break;
        }
    }
    arq = fecharArq(arq);

    if(erro == false){
        arq = abrirArq("dicas.fc", "a");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo de dicas para adicionar.\n");
            pausa();
            return;
        }
        fwrite(&guarda, sizeof(guarda), 1, arq);
        arq = fecharArq(arq);
    }
    pausa();
}

void adicionarPalavra(){
    dica gua, regTemp;
    char resp;
    bool erro;
    FILE *temp;
    registro guarda, temporario;
    do{
        system("cls");
        printf("\nDIGITE UMA PALAVRA DE ATE 200 CARACTERES: ");
        scanf("%[^\n]", guarda.palavra);
        limparBuffer();

        printf("DIGITE A DICA: ");
        scanf("%[^\n]", gua.dica);
        limparBuffer();

        printf("\nDESEJA CONTINUAR COM A PALAVRA %s S/N? ", (char*)guarda.palavra); // Cast para printf
        resp = getchar();
        limparBuffer();
    }while(resp != 's' && resp != 'S');

    my_strupr(guarda.palavra);
    my_strupr(gua.dica);

    temp = abrirArq("dicas.fc", "r");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo de dicas para buscar ID.\n");
        pausa();
        return;
    }

    erro = true;
    guarda.idDica = 0;
    while(fread(&regTemp, sizeof(regTemp), 1, temp) == 1){
        if(strcmp((char*)gua.dica, (char*)regTemp.dica) == 0){
            erro = false;
            break;
        }
        guarda.idDica++;
    }
    temp = fecharArq(temp);

    if(erro == true){
        printf("\nNAO FOI POSSIVEL ENCONTRAR DICA!");
        printf("\nAPERTTE UMA TECLA PARA VOLTAR AO MENU...");
        getchar();
        limparBuffer();
        return;
    }

    temp = abrirArq("palavras.fc", "r");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo de palavras para verificar duplicidade.\n");
        pausa();
        return;
    }

    erro = false;
    while(fread(&temporario, sizeof(temporario), 1, temp) == 1){
        if(strcmp((char*)guarda.palavra, (char*)temporario.palavra) == 0){
            erro = true;
            break;
        }
    }
    temp = fecharArq(temp);

    if(erro == true){
        printf("\nPALAVRA JA EXISTE!");
        printf("\nAPERTTE UMA TECLA PARA VOLTAR AO MENU...");
        getchar();
        limparBuffer();
        return;
    }

    temp = abrirArq("palavras.fc", "a");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo de palavras para adicionar.\n");
        pausa();
        return;
    }
    if(fwrite(&guarda, sizeof(guarda), 1, temp) != 1){
        printf("\nNAO FOI POSSIVEL ADICIONAR PALAVRA!");
        printf("\nAPERTTE UMA TECLA PARA VOLTAR AO MENU...");
        getchar();
        limparBuffer();
        return;
    }
    temp = fecharArq(temp);
    pausa();
}
