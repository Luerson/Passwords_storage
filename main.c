#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Estrutura de cada senha
typedef struct
{
  int ID;
  char titulo[50];
  char usuario[20];
  char senha[20];
} Senha;
//-----------------------

//--------------------------------------
// Funções principais
void listar();                               // Lista todas as senhas já cadastradas
void addSenhas();                            // Adiciona uma senha com dados informados pelo usuário
void barraDeBusca();                         // O usuário insere informação de uma das senhas para listar especificamente a que ele quer
void deletar(int totalDeSenhas);             // deleta uma senha escolhida pelo usuário
void alterarSenha(int totalDeSenhas);        // Altera os dados de uma senha selecionada pelo usuário
void alterarSenhaMestra(int totalDeSenhas);  // Altera a senha mestra

// Funções auxiliares
int contarSenhas();                                          // Conta o total de senhas cadastradas
void senhaAleatoria(int N, char *s);                         // Gera uma senha aleatória (Não uma struct Senha, mas uma variável Senha.senha)
void gerarArquivo(Senha listaDeSenhas[], int totalDeSenhas); // Cria cria um arquivo com os dados armazenados em 'listaDeSenhas'
void criptografar(char *input, char *dest, char *senha);     // Encripta o input usando uma senha e escreve o resultado na string dest.
void descriptografar(char *input, char *dest, char *senha);  // Encripta o input usando uma senha e escreve o resultado na string dest.
int acquireID(int totalDeSenhas);                            // usa o prompt para adquirir o id da senha de interesse do usuário
//--------------------------------------

//  --- CÓDIGO NÃO FINALIZADO
// Encripta o input usando uma senha e escreve o resultado na string dest.
void criptografar(char *input, char *dest, char *senha)
{
  int senhaIndex = 0;

  char senhaNumerica[100];
  sprintf(senhaNumerica, "%d", strlen(senha));

  char num[10];
  for (int i = 0; i < strlen(senha); i++)
  {
    sprintf(num, "%d", (int)senha[i]);
    strcat(senhaNumerica, num);
  }
  senhaNumerica[strlen(senhaNumerica)] = '\0';

  for (int i = 0; i < strlen(input); i++)
  {
    int num = senhaNumerica[senhaIndex] - '0';

    dest[i] = (char)((int)input[i] + num);

    if (senhaIndex == strlen(senha))
    {
      senhaIndex = 0;
    }
    else
    {
      senhaIndex++;
    }
  }
  dest[strlen(input)] = '\0';
}

void descriptografar(char *input, char *dest, char *senha)
{
  int senhaIndex = 0;

  char senhaNumerica[100];
  sprintf(senhaNumerica, "%d", strlen(senha));

  char num[10];
  for (int i = 0; i < strlen(senha); i++)
  {
    sprintf(num, "%d", (int)senha[i]);
    strcat(senhaNumerica, num);
  }
  senhaNumerica[strlen(senhaNumerica)] = '\0';

  for (int i = 0; i < strlen(input); i++)
  {
    int num = senhaNumerica[senhaIndex] - '0';

    dest[i] = (char)((int)input[i] - num);

    if (senhaIndex == strlen(senha))
    {
      senhaIndex = 0;
    }
    else
    {
      senhaIndex++;
    }
  }
  dest[strlen(input)] = '\0';
}

char senhaMestra[100];

int main()
{
  FILE *f = fopen("senhas.bin", "ab");
  FILE *fm = fopen("senha_mestra.bin", "rb+");
  if (fm == NULL)
  {
    fm = fopen("senha_mestra.bin", "wb+");
  }

  char senhaLida[100];
  char c;
  int count = 0;
  while (fread(&c, sizeof(char), 1, fm) == 1)
  {
    senhaLida[count] = c;
    count++;
  }

  fseek(fm, 0, SEEK_END);
  if (ftell(fm) == 0)
  {
    fseek(fm, 0, SEEK_SET);
    printf("\nBem vindo ao armazenador de senhas, já que esta é sua primeira vez acessando o programa, por favor, escolha uma senha mestra que será utilizada para acessar as outras senhas aqui guardadas: ");
    scanf("%s", senhaMestra);

    char senhaCriptografada[100];
    criptografar(senhaMestra, senhaCriptografada, senhaMestra);
    fwrite(senhaCriptografada, 1, strlen(senhaCriptografada) * sizeof(char), fm);
  }
  else
  {
    fseek(fm, 0, SEEK_SET);
    char senhaMestraInserida[100];
    printf("\nPara acessar este programa, por favor insira a senha mestra: ");
    scanf("%s", senhaMestraInserida);

    char senhaArmazenadaDescriptografada[100];
    descriptografar(senhaLida, senhaArmazenadaDescriptografada, senhaMestraInserida);

    while (strcmp(senhaArmazenadaDescriptografada, senhaMestraInserida) != 0)
    {
      printf("Senha incorreta.\n");
      printf("Para acessar este programa, por favor insira a senha mestra: ");
      scanf("%s", senhaMestraInserida);
      descriptografar(senhaLida, senhaArmazenadaDescriptografada, senhaMestraInserida);
    }

    printf("Senha correta.\n");
    strcpy(senhaMestra, senhaArmazenadaDescriptografada);
  }

  fclose(fm);

  int totalDeSenhas;
  int operacao;

  do
  {
    totalDeSenhas = contarSenhas();

    // prompt que questiona a intenção do usuário
    printf("\nPor favor, escolha uma das seguintes opcoes:\n\n"
           "- tecle [0] para encerrar o programa\n"
           "- tecle [1] para listar as senhas ja cadastradas\n"
           "- tecle [2] para deletar alguma senha ja cadastrada\n"
           "- tecle [3] para adicionar uma nova senha\n"
           "- tecle [4] para alterar dados de uma senha especifica\n"
           "- tecle [5] para buscar uma senha especifica\n"
           "- tecle [6] para modificar a senha mestra\n\n"
           "Opcao escolhida: ");
    scanf("%d", &operacao);

    // As funções chamadas dependerão da escolha do usuário
    switch (operacao)
    {
    // INICIO CASE 0-----------------------------
    case 0: // encerrar programa
      break;
    // FIM CASE 0------------------------------

    // INICIO CASE 1-----------------------------
    case 1:
      if (totalDeSenhas)
      {
        listar();
      }
      else
      {
        printf("\nNao existe nenhuma senha cadastrada\n");
      }
      break;
    // FIM CASE 1------------------------------

    // INICIO CASE 2-----------------------------
    case 2:
      if (totalDeSenhas)
      {
        deletar(totalDeSenhas);
      }
      else
      {
        printf("\nNao existe nenhuma senha cadastrada\n");
      }
      break;
    // FIM CASE 2------------------------------

    // INICIO CASE 3-----------------------------
    case 3:
      addSenhas();
      break;
    // FIM CASE 3------------------------------

    // INICIO CASE 4-----------------------------
    case 4:
      if (totalDeSenhas)
      {
        alterarSenha(totalDeSenhas);
      }
      else
      {
        printf("\nNao existe nenhuma senha cadastrada\n");
      }

      break;
    // FIM CASE 4------------------------------

    // INICIO CASE 4-----------------------------
    case 5:
      if (totalDeSenhas)
      {
        barraDeBusca();
      }
      else
      {
        printf("\nNao existe nenhuma senha cadastrada\n");
      }
      break;
    // FIM CASE 4------------------------------

    // INICIO CASE 6-----------------------------
    case 6:
      alterarSenhaMestra(totalDeSenhas);
      break;
    // FIM CASE 6------------------------------

    default:
      printf("\nOpcao invalida. Por favor, selecione uma das opcoes listadas.\n");
    }
  } while (operacao != 0); // o loop só encerra quando o usuário escolher encerrar

  printf("\nPrograma encerrado!\n\n");
  fclose(f);

  return 0;
}

//------------------------------------------------------
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
//------------------------------------------------------

//------------------------------------------------------
void listar()
{
  Senha p;

  FILE *f = fopen("senhas.bin", "rb");

  if (f == NULL)
  { // Testar se o arquivo e NULL
    printf("Nao foi possivel abrir o arquivo\n");
  }

  else
  { // Repetir enquanto o programa conseguir ler uma linha do arquivo
    while (fread(&p, sizeof(Senha), 1, f) == 1)
    {
      char senhaDescriptografada[20];
      descriptografar(p.senha, senhaDescriptografada, senhaMestra);

      printf("----------------------------\n");
      printf("ID: %d\n", p.ID);
      printf("%s\n", p.titulo);
      printf("Login: %s\n", p.usuario);
      printf("Senha: %s\n", senhaDescriptografada);
      printf("----------------------------");
      printf("\n\n");
    }
  }
  // Fecha o arquivo
  fclose(f);
}
//------------------------------------------------------

//------------------------------------------------------
void barraDeBusca()
{
  int opc;
  Senha p;
  char titulo[50];
  char usuario[20];

  printf("Escolha o tipo de busca: \n");
  printf("1 - Pesquisar por titulo. \n");
  printf("2 - Pesquisar por usuario. \n");
  scanf("%d", &opc);

  FILE *f = fopen("senhas.bin", "rb");

  if (f == NULL)
  { // Testa se o arquivo e NULL
    printf("Nao foi possivel abrir o arquivo\n");
  }
  else
  {
    if (opc == 1)
    {
      fflush(stdin);
      printf("Digite o titulo a pesquisar: ");
      gets(titulo);

      while (fread(&p, sizeof(Senha), 1, f) == 1)
      {
        if (strcmp(titulo, p.titulo) == 0)
        { // Compara duas strings e mostra quando elas forem iguais
          char senhaDescriptografada[20];
          descriptografar(p.senha, senhaDescriptografada, senhaMestra);

          printf("----------------------------\n");
          printf("ID: %d\n", p.ID);
          printf("%s\n", p.titulo);
          printf("Login: %s\n", p.usuario);
          printf("Senha: %s\n", senhaDescriptografada);
          printf("----------------------------");
          printf("\n\n");
        }
      }
    }

    else if (opc == 2)
    {
      printf("Digite o usuario a pesquisar: ");
      fflush(stdin);
      gets(usuario);

      while (fread(&p, sizeof(Senha), 1, f) == 1)
      {
        if (strcmp(usuario, p.usuario) == 0)
        {
          char senhaDescriptografada[20];
          descriptografar(p.senha, senhaDescriptografada, senhaMestra);

          printf("\n\n----------------------------\n");
          printf("ID: %d\n", p.ID);
          printf("%s\n", p.titulo);
          printf("Login: %s\n", p.usuario);
          printf("Senha: %s\n", senhaDescriptografada);
          printf("----------------------------");
          printf("\n\n");
        }
      }
    }
  }

  fclose(f);
}
//------------------------------------------------------

// Função para gerar uma senha aleatória com uma quantidade N de caracteres.
void senhaAleatoria(int N, char *s)
{
  // Inicializa o contador
  int i = 0;

  int gerador = 0;

  // Alimentar o gerador aleatório
  // com o horario atual,
  // para que os numeros sejam diferentes.
  srand((unsigned int)(time(NULL)));

  // Array de numeros.
  char numeros[] = "0123456789";

  // Array de letras minusculas.
  char letras[] = "abcdefghijklmnoqprstuvwyzx";

  // Array de letras maiusculas.
  char LETRAS[] = "ABCDEFGHIJKLMNOQPRSTUYWVZX";

  // Array de simbolos especiais.
  char simbolos[] = "!@#$^&*?";

  char tempSenha[20];

  // Fazer que ele percorra do indice 0 ao N-1
  for (i = 0; i < N; i++)
  {
    if (gerador == 1)
    {
      tempSenha[i] = numeros[rand() % 10];
      gerador = rand() % 4;
    }
    else if (gerador == 2)
    {
      tempSenha[i] = simbolos[rand() % 8];
      gerador = rand() % 4;
    }
    else if (gerador == 3)
    {
      tempSenha[i] = LETRAS[rand() % 26];
      gerador = rand() % 4;
    }
    else
    {
      tempSenha[i] = letras[rand() % 26];
      gerador = rand() % 4;
    }
  }

  criptografar(tempSenha, s, senhaMestra);
}
//------------------------------------------------------

//------------------------------------------------------
void deletar(int totalDeSenhas)
{
  FILE *f;
  Senha senha;
  Senha listaDeSenhas[totalDeSenhas - 1];
  int deletados = 0;
  int id;

  id = acquireID(totalDeSenhas);

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
      senha.ID = i + 1 - deletados;
      listaDeSenhas[i - deletados] = senha;
    }
    else
    {
      deletados++;
    }
  }

  fclose(f); // Fechando aruivo de leitura

  gerarArquivo(listaDeSenhas, totalDeSenhas - 1);

  return;
}
//------------------------------------------------------

//------------------------------------------------------
void addSenhas()
{
  Senha p;
  char continuar;
  short tempID = 1;
  FILE *f = fopen("senhas.bin", "rb"); // lendo binario

  // Se o arquivo existir, procurar o p.ID da ultima senha salva e somar 1.
  if (f)
  {
    while (fread(&p, sizeof(Senha), 1, f))
    {
      tempID = (p.ID > 0) ? p.ID + 1 : 1;
    }
  }

  fclose(f);
  f = fopen("senhas.bin", "ab"); // Abrindo/criando arquivo modo de escrita

  do
  {
    printf("Digite um titulo para a senha: ");
    fflush(stdin);
    gets(p.titulo);

    printf("Digite o seu login: ");
    fflush(stdin);
    gets(p.usuario);

    char tempSenha[20];
    printf("Digite a sua senha (deixe vazio para gerar uma senha aleatoria de 10 caracteres): ");
    fflush(stdin);
    gets(tempSenha);

    if (strlen(tempSenha) == 0)
    {
      senhaAleatoria(10, p.senha);
      printf("Senha gerada: %s", p.senha);
      printf("\n");
    }
    else
    {
      criptografar(tempSenha, p.senha, senhaMestra);
    }

    p.ID = tempID;
    tempID++;
    fwrite(&p, sizeof(Senha), 1, f); // Salvando as informações passadas pelo user.

    printf("\nVoce deseja adicionar mais senhas? (s/n): ");
    continuar = tolower(getchar()); // Converter para minusculo, para inputs em maíusculo (S/N) serem aceitos.
    printf("\n");
  } while (continuar != 'n');

  fclose(f); // fecha o arquivo.

  f = fopen("senhas.bin", "rb"); // rb eh o argumento de leitura para binario.
  if (!f)
  { // (!f) testa se o resultado da operacao foi NULL.
    printf("Nao foi possivel abrir o arquivo\n");
  }

  while (fread(&p, sizeof(Senha), 1, f))
  {
    char senhaDescriptografada[20];
    descriptografar(p.senha, senhaDescriptografada, senhaMestra);

    printf("ID: %d\n", p.ID);
    printf("%s\n", p.titulo);
    printf("Login: %s\n", p.usuario);
    printf("Senha: %s\n", senhaDescriptografada);
    printf("\n\n");
  }

  fclose(f);
}
//------------------------------------------------------

//------------------------------------------------------
void alterarSenha(int totalDeSenhas)
{
  FILE *f;
  Senha senha;
  Senha listaDeSenhas[totalDeSenhas];
  int id;

  id = acquireID(totalDeSenhas);

  f = fopen("senhas.bin", "rb");

  if (f == NULL)
  {
    printf("\nErro! Nao foi possivel criar o arquivo\n");
    exit(2);
  }

  // Esse 'for' armazena todas as senhas em um vetor de senhas,
  // mas altera a que o usuário selecionar
  for (int i = 0; i < totalDeSenhas; i++)
  {
    fread(&senha, sizeof(Senha), 1, f);

    if (senha.ID == id)
    {
      printf("Digite um novo titulo para a senha: ");
      fflush(stdin);
      gets(senha.titulo);

      printf("Digite o seu novo login: ");
      fflush(stdin);
      gets(senha.usuario);

      printf("Digite a sua nova senha: ");
      fflush(stdin);
      gets(senha.senha);

      senha.ID = id;
    }

    listaDeSenhas[i] = senha;
  }

  fclose(f);

  gerarArquivo(listaDeSenhas, totalDeSenhas);

  return;
}
//------------------------------------------------------

void alterarSenhaMestra(int totalDeSenhas)
{
  char novaSenhaMestra[100];
  Senha listaDeSenhas[totalDeSenhas];
  Senha p;
  FILE *f = fopen("senhas.bin", "rb+");
  FILE *fm;
  
  printf("\nInsira a nova senha mestra: ");
  scanf("%s", novaSenhaMestra);

  if (f == NULL)
  {
    // Testar se o arquivo e NULL
    printf("Nao foi possivel abrir o arquivo\n");
  }
  else
  {
    // Repetir enquanto o programa conseguir ler uma linha do arquivo
    int i = 0;
    while (fread(&p, sizeof(Senha), 1, f) == 1)
    {
      char senhaDescriptografada[20];

      descriptografar(p.senha, senhaDescriptografada, senhaMestra);
      printf("%s\n", senhaDescriptografada);

      criptografar(senhaDescriptografada, p.senha, novaSenhaMestra);
      listaDeSenhas[i] = p;
      i++;
    }

    fwrite(&p, sizeof(Senha), 1, f); // Salvando as informações passadas pelo user.

    strcpy(senhaMestra, novaSenhaMestra);
    printf("%s\n", senhaMestra);
    fm = fopen("senha_mestra.bin", "wb+");

    char senhaCriptografada[100];
    criptografar(senhaMestra, senhaCriptografada, senhaMestra);
    fwrite(senhaCriptografada, 1, strlen(senhaCriptografada) * sizeof(char), fm);

    gerarArquivo(listaDeSenhas, totalDeSenhas);
  }

  fclose(fm);
}

//------------------------------------------------------
void gerarArquivo(Senha listaDeSenhas[], int totalDeSenhas)
{
  FILE *f = fopen("senhas.bin", "wb");

  if (f == NULL)
  {
    printf("\nErro! Nao foi possivel criar o arquivo\n");
    exit(3);
  }

  // escrevendo a informação no novo arquivo
  for (int i = 0; i < totalDeSenhas; i++)
  {
    fwrite(&listaDeSenhas[i], sizeof(Senha), 1, f);
  }

  fclose(f);
  // fechando arquivo de escrita

  return;
}
//------------------------------------------------------

//------------------------------------------------------
int acquireID(int totalDeSenhas)
{
  int id;

  do
  {
    listar();

    printf("\nQual das senhas acima voce deseja selecionar?\n"
           "id da senha escolhida: ");
    scanf("%d", &id);

    if (id > totalDeSenhas || id < 1)
    {
      printf("\nSenha invalida\n");
    }

  } while (id > totalDeSenhas || id < 1);

  return id;
}