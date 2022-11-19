#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef struct
{
  int ID;
  char titulo[50];
  char usuario[20];
  char senha[20];
} Senha;

// Criei essa função porque eu e Klayvert vamos ter que usar esse trecho
// de código novamente na função alterarSenha().
int contarSenhas()
{
  FILE *f = fopen("senhas.bin", "rb"); // apenas leitura
  Senha senha;
  int contador = 0;

  if (f == NULL)
  {
    printf("\nErro! Nao foi possivel abrir o arquivo\n");
    exit(1);
  }

  // A cada senha encontrada, o contador é incrementado
  while (fread(&senha, sizeof(Senha), 1, f))
  {
    contador++;
  }
  return contador;
}

void listar(){
  // arq é o handle que usamos para acessar o arquivo
  FILE *f;

  // abre o arquivo em modo de leitura
  f = fopen("senhas.bin", "r");

  // fopen vai retornar NULL se o arquivo estiver vazio
  if (f != NULL){
    // Ler cada caracter do arquivo até o final do arquivo (EOF) is
    // EOF marca que foi alcançado o final do arquivo
    char c;
    while ( (c = fgetc(f)) != EOF )
    // Faz output de cada caracter para o console
      putchar(c);

    // fecha o gerenciador de arquivos
    fclose(f);
  }
  // Se teve um erro ao abrir o arquivo, avisa ao usuario
  else printf("Erro ao abrir arquivo.\n");
};    // principal

void adicionar(); // principal

// void barraDeBusca(); - Secundário
// void alterarSenha(); - Secundário

// Função para gerar uma senha aleatória com uma quantidade N de caracteres
void senhaAleatoria(int N, char *s)
{
	// Inicializa o contador
	int i = 0;

	int gerador = 0;

	// Alimentar o gerador aleatório
	// com o horario atual
	// para que os numeros sejam diferentes
	srand((unsigned int)(time(NULL)));

	// Array de numeros
	char numeros[] = "0123456789";

	// Array de letras minusculas
	char letras[] = "abcdefghijklmnoqprstuvwyzx";

	// Array de letras maiusculas
	char LETRAS[] = "ABCDEFGHIJKLMNOQPRSTUYWVZX";

	// Array de simbolos especiais
	char simbolos[] = "!@#$^&*?";

	// Fazer que ele percorra do indice 0 ao N-1
	for (i = 0; i < N; i++) {

		if (gerador == 1) {
			s[i] = numeros[rand() % 10];
			gerador = rand() % 4;
			printf("%c", s[i]);
		}
		else if (gerador == 2) {
			s[i] = simbolos[rand() % 8];
			gerador = rand() % 4;
			printf("%c", s[i]);
		}
		else if (gerador == 3) {
			s[i] = LETRAS[rand() % 26];
			gerador = rand() % 4;
			printf("%c", s[i]);
		}
		else {
			s[i] = letras[rand() % 26];
			gerador = rand() % 4;
			printf("%c", s[i]);
		}
	}
}

void deletar(int id, int totalDeSenhas)
{
  FILE *f;
  Senha senha;
  Senha listaDeSenhas[totalDeSenhas - 1];
  int deletados = 0;

  f = fopen("senhas.bin", "rb");

  if (f == NULL)
  {
    printf("\nErro! Nao foi possivel criar o arquivo\n");
    exit(2);
  }

  // Esse 'for' armazena todas as senhas em um vetor de senhas,
  // com exceção da que o usuário deseja deletar
  for (int i = 0; i < totalDeSenhas; i++)
  {
    fread(&senha, sizeof(Senha), 1, f);

    if (senha.ID != id)
    {
      listaDeSenhas[i - deletados] = senha;
    }
    else
    {
      deletados++;
    }
  }

  fclose(f);
  // fechando aruivo de leitura

  // abrindo arquivo de escrita. Esse método destroi
  // todo conteúdo do arquivo original, mas as informações
  // desejadas contiuam salvas no vetor 'listaDeSenhas[]'
  f = fopen("senhas.bin", "wb");

  if (f == NULL)
  {
    printf("\nErro! Nao foi possivel criar o arquivo\n");
    exit(3);
  }

  // escrevendo a informação no novo arquivo
  for (int i = 0; i < totalDeSenhas - 1; i++)
  {
    fwrite(&listaDeSenhas[i], sizeof(Senha), 1, f);
  }

  fclose(f);
  // fechando arquivo de escrita

  return;
}

int main()
{
  FILE *f = fopen("senhas.bin", "ab");
  int operacao;

  do
  {
    // prompt que questiona a intenção do usuário
    printf("\nPor favor, escolha uma das seguintes opcoes:\n\n"
           "- tecle '0' para encerrar o programa\n"
           "- tecle '1' para listar as senhas ja cadastradas\n"
           "- tecle '2' para deletar alguma senha ja cadastrada\n"
           "- tecle '3' para adicionar uma nova senha\n"
           "- tecle '4' para alterar dados de uma senha especifica\n\n"
           "Opcao escolhida: ");
    scanf("%d", &operacao);

    // As funções chamadas dependerão da escolha do usuário
    switch (operacao)
    {

    case 0: // encerrar programa
      break;

    case 1:
      printf("\nListar todas as senhas\n");
      // A FAZER
      //
      // listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário
      // barraDeBusca(); - aqui deve ficar a função que permitirá ao usuário buscar uma senha específica
      //
      break;

    case 2:
    {
      int id;
      int totalDeSenhas = contarSenhas();

      if (totalDeSenhas == 0)
      {
        printf("Nao existe nenhuma senha cadastrada\n");
        break;
      }

      do
      {
        // A FAZER
        //
        // listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário

        printf("\nQual das senhas acima voce deseja deletar?\n"
               "Selecione o id da senha escolhida: ");
        scanf("%d", &id);

        if (id > totalDeSenhas || id < 1)
        {
          printf("\nSenha invalida\n");
        }

      } while (id > totalDeSenhas || id < 1);

      deletar(id, totalDeSenhas); // função que permitirá ao usuário deletar uma das senhas listadas
      break;
    }
    case 3:
      printf("\nAdicionar senha\n");
      // A FAZER
      //
      // adicionar(); - enfim, ces já sabem
      //
      break;

    case 4:
      printf("\nAlterar senha!\n");
      // A FAZER
      //
      // listar(); - aqui deve ficar a função que irá mostras todas as senhas ao usuário
      // alterarSenha();
      //
      break;

    default:
      printf("\n\n\n\n\n\nOpcao invalida. Por favor, selecione uma das opcoes listadas.\n");
    }

  } while (operacao != 0); // o loop só encerra quando o usuário escolher encerrar

  printf("\nPrograma encerrado!\n\n");

  fclose(f);

  return 0;
}

void addSenhas()
{
  Senha p;
  char continuar;
  short tempID = 1;
  FILE *f = fopen("senhas.bin", "rb"); // lendo binario
  if (f)                               // Se o arquivo existir eu procuro o p.ID da ultima senha salva e somo 1
    while (fread(&p, sizeof(Senha), 1, f))
      tempID = (p.ID > 0) ? p.ID + 1 : 1;
  fclose(f);
  f = fopen("senhas.bin", "ab"); // abrindo/criando arquivo modo de escrita
  do
  {
    printf("Digite um titulo para a senha: ");
    fflush(stdin);
    gets(p.titulo);

    printf("Digite o seu login: ");
    fflush(stdin);
    gets(p.usuario);

    printf("Digite a sua senha: ");
    fflush(stdin);
    gets(p.senha);
    p.ID = tempID;
    tempID++;
    fwrite(&p, sizeof(Senha), 1, f); // Salvando as informações passadas pelo user

    printf("\nVoce deseja adicionar mais senhas? (s/n)");
    continuar = getchar();
    printf("\n");
  } while (continuar != 'n');

  fclose(f); // fecha o arquivo /

  f = fopen("senhas.bin", "rb"); // rb � o argumento de leitura para binario
  if (!f)
  { // (!f) testa se o resultado da opera��o foi NULL
    printf("Nao foi possivel abrir o arquivo\n");
  }

  while (fread(&p, sizeof(Senha), 1, f))
  { //
    printf("ID: %d\n", p.ID);
    printf("%s\n", p.titulo);
    printf("Login: %s\n", p.usuario);
    printf("Senha: %s\n", p.senha);
    printf("\n\n");
  }

  fclose(f);
}
