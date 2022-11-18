#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int ID;
    char titulo[50];
    char usuario[20];
    char senha[20];
} Senha;

//Criei essa função porque eu e Klayvert vamos ter que usar esse trecho
//de código novamente na função alterarSenha().
int contarSenhas() {
    FILE *f = fopen("senhas.bin", "rb"); //apenas leitura
    Senha senha;
    int contador = 0;

    if (f == NULL) {
        printf("\nErro! Nao foi possivel abrir o arquivo\n");
        exit(1);
    }

    //A cada senha encontrada, o contador é incrementado
    while (fread(&senha, sizeof(Senha), 1, f)) {
        contador++;
    }
    return contador;
}


void listar(); // principal
void adicionar(); // principal

//void barraDeBusca(); - Secundário
//void alterarSenha(); - Secundário


//criarSenha() -- Se der tempo!!!!!

void deletar(int id, int totalDeSenhas) {
    FILE *f;
    Senha senha;
    Senha listaDeSenhas[totalDeSenhas-1];
    int deletados = 0;

    f = fopen("senhas.bin", "rb");

    if (f == NULL) {
        printf("\nErro! Nao foi possivel criar o arquivo\n");
        exit(2);
    }

    //Esse 'for' armazena todas as senhas em um vetor de senhas,
    //com exceção da que o usuário deseja deletar
    for (int i = 0; i < totalDeSenhas; i++) {
        fread(&senha, sizeof(Senha), 1, f);  

        if (senha.ID != id) {
            listaDeSenhas[i - deletados] = senha;
        } else {
            deletados++;
        }
    }

    fclose(f);
    //fechando aruivo de leitura


    //abrindo arquivo de escrita. Esse método destroi
    //todo conteúdo do arquivo original, mas as informações
    //desejadas contiuam salvas no vetor 'listaDeSenhas[]'
    f = fopen("senhas.bin", "wb");

    if (f == NULL) {
        printf("\nErro! Nao foi possivel criar o arquivo\n");
        exit(3);
    }

    //escrevendo a informação no novo arquivo
    for (int i = 0; i < totalDeSenhas-1; i++) {
        fwrite (&listaDeSenhas[i], sizeof(Senha), 1, f);
    }

    fclose(f);
    //fechando arquivo de escrita

    return;
}


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

            case 2: {
                int id;
                int totalDeSenhas = contarSenhas();

                do {
                    //A FAZER
                    //
                    //listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário

                    printf("\nQual das senhas acima voce deseja deletar?\n"
                           "Selecione o id da senha escolhida: ");
                    scanf("%d", &id);

                    if (id > totalDeSenhas || id < 1) {
                        printf("\nSenha invalida\n");
                    }

                } while (id > totalDeSenhas || id < 1);

                deletar(id, totalDeSenhas); //função que permitirá ao usuário deletar uma das senhas listadas
                break;
            }
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