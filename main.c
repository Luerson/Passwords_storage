#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int ID;
    char titulo[50];
    char usuario[20];
    char senha[20];
} senha;


void listar(); // principal
void adicionar(); // principal
void deletar(); // principal


//void barraDeBusca(); - Secundário
//void alterarSenha(); - Secundário


//criarSenha() -- Se der tempo!!!!!


int main() {
    FILE *f = fopen("senhas.bin", "ab");
    int operacao;

    do {
        //prompt que questiona a intenção do usuário
        printf("\nPor favor, escolha uma das seguintes opcoes:\n\n"
            "- tecle '0' para encerrar o programa\n"
            "- tecle '1' para listar as senhas ja cadastradas\n"
            "- tecle '2' para deletar alguma senha ja cadastrada\n"
            "- tecle '3' para adicionar uma nova senha\n"
            "- tecle '4' para alterar dados de uma senha especifica\n\n"
            "Opcao escolhida: ");
        scanf("%d", &operacao);


        //As funções chamadas dependerão da escolha do usuário
        switch (operacao) {

            case 0: // encerrar programa
                break;

            case 1:
                printf("\nListar todas as senhas\n");
                //A FAZER
                //
                //listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário
                //barraDeBusca(); - aqui deve ficar a função que permitirá ao usuário buscar uma senha específica
                //
                break;

            case 2:
                printf("\nDeletar senha\n");
                //A FAZER
                //
                //listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário
                //deletar(); - função que permitirá ao usuário deletar uma das senhas listadas
                //
                break;

            case 3:
                printf("\nAdicionar senha\n");
                //A FAZER
                //
                //adicionar(); - enfim, ces já sabem
                //
                break;

            case 4:
            printf("\nAlterar senha!\n");
                //A FAZER
                //
                //listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário
                //alterarSenha();
                //
                break;

            default:
                printf("\n\n\n\n\n\nOpcao invalida. Por favor, selecione uma das opcoes listadas.\n");
        }

    }while (operacao != 0); // o loop só encerra quando o usuário escolher encerrar

    printf("\nPrograma encerrado!\n\n");

    fclose(f);

    return 0;
}