#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sha.h>

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
void listar();                        // Lista todas as senhas já cadastradas
void addSenhas();                     // Adiciona uma senha com dados informados pelo usuário
void barraDeBusca();                  // O usuário insere informação de uma das senhas para listar especificamente a que ele quer
void deletar(int totalDeSenhas);      // deleta uma senha escolhida pelo usuário
void alterarSenha(int totalDeSenhas); // Altera os dados de uma senha selecionada pelo usuário

// Funções auxiliares
int contarSenhas();                                          // Conta o total de senhas cadastradas
void senhaAleatoria(int N, char *s);                         // Gera uma senha aleatória (Não uma struct Senha, mas uma variável Senha.senha)
void gerarArquivo(Senha listaDeSenhas[], int totalDeSenhas); // Cria cria um arquivo com os dados armazenados em 'listaDeSenhas'
int acquireID(int totalDeSenhas);                            // usa o prompt para adquirir o id da senha de interesse do usuário
//--------------------------------------

//  --- CÓDIGO NÃO FINALIZADO
// Criptografar plaintext e escrever em ciphertext usando uma senha e sha-512
void encrypt(char* password, char* plaintext, char* ciphertext) {
  unsigned char salt[8] = {0};
  unsigned char key[64];
  unsigned char iv[16];
  unsigned char tag[16];
  unsigned char aad[20];
  unsigned char* pt = (unsigned char*)plaintext;
  unsigned char* ct = (unsigned char*)ciphertext;
  int ptlen = strlen(plaintext);
  int ctlen, tmplen;
  int i;

  // gerar salt
  RAND_bytes(salt, 8);

  // gerar key
  PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), salt, 8, 100000, 64, key);

  // gerar iv
  RAND_bytes(iv, 16);

  // gerar aad
  RAND_bytes(aad, 20);

  // criptografar
  EVP_CIPHER_CTX* ctx;
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL);
  EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
  EVP_EncryptUpdate(ctx, NULL, &tmplen, aad, 20);
  EVP_EncryptUpdate(ctx, ct, &tmplen, pt, ptlen);
  ctlen = tmplen;
  EVP_EncryptFinal_ex(ctx, ct + ctlen, &tmplen);
  ctlen += tmplen;
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);
  EVP_CIPHER_CTX_free(ctx);

  // escrever salt, iv, tag, e ciphertext em ciphertext
  memcpy(ciphertext, salt, 8);
  memcpy(ciphertext + 8, iv, 16);
  memcpy(ciphertext + 24, tag, 16);
  memcpy(ciphertext + 40, ct, ctlen);
  ciphertext[40 + ctlen] = '\0';
}

// decriptografar usando sha-512
void decrypt(char* password, char* ciphertext, char* plaintext) {
  unsigned char salt[8];
  unsigned char key[64];
  unsigned char iv[16];
  unsigned char tag[16];
  unsigned char aad[20];
  unsigned char* ct = (unsigned char*)ciphertext + 40;
  unsigned char* pt = (unsigned char*)plaintext;
  int ptlen, tmplen;
  int i;

  // ler salt, iv, tag, e ciphertext do ciphertext
  memcpy(salt, ciphertext, 8);
  memcpy(iv, ciphertext + 8, 16);
  memcpy(tag, ciphertext + 24, 16);

  // gerar key
  PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), salt, 8, 100000, 64, key);

  // descriptografar
  EVP_CIPHER_CTX* ctx;
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL);
  EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);
  EVP_DecryptUpdate(ctx, NULL, &tmplen, aad, 20);
  EVP_DecryptUpdate(ctx, pt, &tmplen, ct, strlen(ciphertext) - 40);
  ptlen = tmplen;
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag);
  EVP_DecryptFinal_ex(ctx, pt + ptlen, &tmplen);
  EVP_CIPHER_CTX_free(ctx);

  plaintext[ptlen] = '\0';
}
//  --- CÓDIGO NÃO FINALIZADO

int main()
{ 
  char ciphertext[1024];
  encrypt("password", "plaintext", ciphertext);
  printf("ciphertext: %s\n", ciphertext);
  char plaintext[1024];
  decrypt("password", ciphertext, plaintext);
  printf("plaintext: %s\n", plaintext);

  FILE *f = fopen("senhas.bin", "ab");
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
      // FIM CASE 4------------------------------

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
      printf("----------------------------\n");
      printf("ID: %d\n", p.ID);
      printf("%s\n", p.titulo);
      printf("Login: %s\n", p.usuario);
      printf("Senha: %s\n", p.senha);
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
          printf("----------------------------\n");
          printf("ID: %d\n", p.ID);
          printf("%s\n", p.titulo);
          printf("Login: %s\n", p.usuario);
          printf("Senha: %s\n", p.senha);
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
          printf("\n\n----------------------------\n");
          printf("ID: %d\n", p.ID);
          printf("%s\n", p.titulo);
          printf("Login: %s\n", p.usuario);
          printf("Senha: %s\n", p.senha);
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

  // Fazer que ele percorra do indice 0 ao N-1
  for (i = 0; i < N; i++)
  {
    if (gerador == 1)
    {
      s[i] = numeros[rand() % 10];
      gerador = rand() % 4;
    }
    else if (gerador == 2)
    {
      s[i] = simbolos[rand() % 8];
      gerador = rand() % 4;
    }
    else if (gerador == 3)
    {
      s[i] = LETRAS[rand() % 26];
      gerador = rand() % 4;
    }
    else
    {
      s[i] = letras[rand() % 26];
      gerador = rand() % 4;
    }
  }
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
      strcpy(p.senha, tempSenha);
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
    printf("ID: %d\n", p.ID);
    printf("%s\n", p.titulo);
    printf("Login: %s\n", p.usuario);
    printf("Senha: %s\n", p.senha);
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