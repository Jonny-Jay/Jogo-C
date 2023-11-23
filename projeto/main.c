#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// struct lista encadeada (elementos são as casinhas do tabuleiro)
struct square{
  char quadrado[4];
  int x;
  int y;
  struct square *next;
};

// struct de objetos (setinha do menu, persona e goal)
struct obj{
char img;
int x;
int y;
screenColor color;
};

struct square *push(char quadrado[4], int x, int y);

void criar_matriz(struct square **head, int tamanho_x, int tamanho_y);

void print_matriz(struct square *head);

void print_obj(struct obj *obj);

void print_score(int score);

int getx(int x, int max_x);

int gety(int y, int max_y);

void print_menu();

void print_controles();

void jogo();

void jogo_2();

void free_list(struct square *head);

//chama o menu e permite a movimentação do jogador pelo menu
int main() 
{
  screenInit(1);
  keyboardInit();
  timerInit(100);

  static int caracter = 0;

  struct obj select;
  select.img = '>';
  select.x = 34;
  select.y = 8;

  print_menu();
  print_obj(&select);

  while(1)
    {
      print_menu();
      int enter_press = 0;
      int mov_menu = 1;

      if(keyhit())
      {
        caracter = readch();
      }
      switch(caracter)  //jogador se movimenta para cima ou baixo e aperta enter para confirmar
        {
          case 119: //W
          if(select.y > 8)
          {
            select.y -= 2;
          }
          break;

          case 115:
            if(select.y < 14) //S
            {
              select.y += 2;
            }
          break;

          case 32:
          enter_press = 1;
          break;

          default:
          mov_menu = 0;
          break;
        }

        if(mov_menu)
        {
          screenClear();
          print_menu();
          print_obj(&select);
        }

        if(enter_press)   //verifica a opção selecionada
        {
          if(select.y == 8)
          {
            screenClear();
            jogo();
          }
          else if(select.y == 10)
          {
            screenClear();
            jogo_2();
          }
          else if(select.y == 12)
          {
            screenClear();
            print_controles();
          }
          else if(select.y == 14)
          {
            screenClear();
            break;
          }
        }
      caracter = 0;

    }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}

// função que cria uma nova casinha no tabuleiro e retorna essa casa (elemento da lista)
struct square* push(char quadrado[4], int x, int y){
  struct square *newSquare = (struct square*)malloc(sizeof(struct square)); //aloca espaço
  strcpy(newSquare->quadrado, quadrado); 
  newSquare->x = x;
  newSquare->y = y;
  newSquare->next = NULL;

  return newSquare;
}


//função que cria o tabuleiro (lista encadeada) colocando a posição no meio da tela e adicionando cada elemento no final da lista
void criar_matriz(struct square **head, int tamanho_x, int tamanho_y)
{
  int i, j;

  for(i = 0; i < tamanho_x; i++)
    {
      for(j = 0; j < tamanho_y; j++)
        {
          int pos_x = MAXX/2 - tamanho_x/2 + j; //coloca o quadrado no meio da tela, para poder printar o tabuleiro centralizado
          int pos_y = MAXY/2 - tamanho_y/2 + i; 
          if(*head == NULL)
          {
            *head = push("_", pos_x, pos_y); //se lista estiver vazia, cria o elemento e head aponta para ele
          }
          else
          {
            struct square *n = *head;
            while (n->next != NULL) { //percorre a lista
              n = n->next;
            }
            struct square* newSquare = push("_", pos_x, pos_y); //se não, cria o elemento e adiciona ao final da lista
            n->next = newSquare; //último elemento aponta para o novo elemento criado
          }

        }
    }
}

//função para printar a matriz (tabuleiro)
void print_matriz(struct square *head)
{
  struct square *n = head;

  while(n != NULL)
    {
      screenSetColor(GREEN, BLACK);
      screenGotoxy(n->x, n->y);
      printf("%s", n->quadrado);
      n = n->next;
    }
}


//função para printar um objeto (persona, goal, etc)
void print_obj(struct obj *obj)
{
  screenSetColor(obj->color, YELLOW);
  screenGotoxy(obj->x, obj->y);
  printf("%c", obj->img);
}


//função para printar a pontuação do jogador
void print_score(int score){
  screenSetColor(GREEN, BLACK);
  screenGotoxy(35, 19);
  printf("SCORE:");

  screenGotoxy(43, 19);
  printf("         ");

  screenGotoxy(43, 19);
  printf("%d ", score);
}


//calcular posição x com relação a matriz no centro
int getx(int x, int max_x)
{
  return MAXX/2 - max_x/2 + x;
}


//calcular a posição y com relação a matriz no centro
int gety(int y, int max_y)
{
  return MAXY/2 - max_y/2 + y;
}



//função para printar o menu inicial
void print_menu(){
  screenGotoxy(36, 4);
  printf("MAHARAJA");

  screenGotoxy(36, 8);
  printf("1 PLAYER");

  screenGotoxy(36, 10);
  printf("2 PLAYERS");

  screenGotoxy(36, 12);
  printf("CONTROLS");

  screenGotoxy(36, 14);
  printf("EXIT");
}


//função para printar os controles do jogo
void print_controles()
{
  struct obj select;
  select.img = '>';
  select.x = 28;
  select.y = 20;

  while(1) //loop para repetir até o jogador apertar enter
    {
      screenGotoxy(36, 4);
      printf("MAHARAJA");

      screenGotoxy(30, 6);
      printf("X -> PLAYER");

      screenGotoxy(30, 7);
      printf("O -> GOAL");

      screenGotoxy(30, 9);
      printf("MOVEMENTS: WASD");

      screenGotoxy(30, 11);
      printf("PRESS SPACE ON TOP OF GOAL ");

      screenGotoxy(30, 13);
      printf("THE MORE YOU WALK, MORE POINTS YOU GET");

      screenGotoxy(30, 15);
      printf("BE CAREFUL WITH THE TIME");

      screenGotoxy(30, 17);
      printf("GAME ENDS AFTER 10 GOALS");

      screenGotoxy(30, 20);
      printf("BACK");

      print_obj(&select);

      static int ch = 0;

      if(keyhit()) //detecta se alguma tecla foi pressionada
      {
        ch = readch(); //lê a tecla
      }
      if(ch == 32)
      {
        screenClear();
        break; //se for enter, sai do loop, consequentemente da função
      }
    }
}

//função para iniciar o jogo
void jogo(){
  srand(time(NULL)); //inicializa o gerador de números aleatórios

  struct square *head = NULL; //cria o head da lista encadeada
  struct obj persona;
  struct obj goal;

  //inicia o score com 0 e coloca para printar, para deixar a tela do jogo nas cores corretas
  int score = 0; 
  print_score(score); //foi pra ficar bonito

  int cont_casas = 0; //contador de casas andadas
  int cont_goals = 0; //contador de goals já passados

  static int ch = 0;

  //tamanho da matriz
  int max_x = 5; 
  int max_y = 5;

  persona.img = 'x';
  persona.color = WHITE;
  persona.x = getx(max_x/2, max_x); //define a posição do jogador para o meio do tabuleiro
  persona.y = gety(max_y/2, max_y);

  goal.img = 'o';
  goal.color = YELLOW;


  screenInit(1);
  keyboardInit();
  timerInit(100);


  //cria o tabuleiro e define as colisões
  criar_matriz(&head, max_x, max_y);
  int lim_dir = getx(max_x, max_x); 
  int lim_esq = getx(0, max_x);
  int lim_cim = gety(0, max_y);
  int lim_bax = gety(max_y, max_y);


  //gera a posição do goal em algum lugar aleatório do tabuleiro
  goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
  goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;

  sleep(1);


  //printa tudo
  print_matriz(head);
  print_obj(&persona);
  print_obj(&goal);
  print_score(score);

  timerUpdateTimer(4000); //define o timer como 4 segundos

  while(1) //while true enquanto o jogo não acabar
  {
    if(keyhit())
    {
      ch = readch();
    }

    int mov = 1; //verifica se o usuário andou
    int space_press = 0; //verifica se o usuário apertou espaço

    switch(ch) //WASD
      {
        case 119: //w
        if((persona.y - 1) >= lim_cim){  //verifica se o jogador não está na primeira linha do tabuleiro
          persona.y --; //atualiza a posição do jogador
        }
        else{
          mov = 0; //evita que pontue se o jogador bater na parede
        }
        break;

        case 115:
        if((persona.y + 1) < lim_bax){  //verifica se o jogador não está na última linha do tabuleiro
          persona.y ++;
        } 
        else{
          mov = 0; 
        }
        break;

        case 97:
        if((persona.x - 1) >= lim_esq){  //verifica se o jogador não está na primeira coluna do tabuleiro
          persona.x --;
        }
        else{
          mov = 0;
        }
        break;

        case 100:
        if((persona.x + 1) < lim_dir){  //verifica se o jogador não está na última coluna do tabuleiro
          persona.x ++;
        }
        else{
          mov = 0;
        }
        break;

        case 32:  //se o usuário apertar espaço
        space_press = 1;
        break;

        default:  //por padrão o jogador não anda
        mov = 0;
        break;
      }
    if (mov){  //se o jogador andou, aumenta o contador de número de casas e atualiza os prints
      cont_casas ++;
      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);
    }

    if(persona.x == goal.x && persona.y == goal.y && space_press)  //se o jogador está em cima do goal e apertou espaço
    {
      goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;  //gera uma nova posição do goal
      goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;
      score += cont_casas - 1; //aumenta a pontuação pelo número de casas andadas
      print_score(score);
      cont_casas = 0; //reseta o número de casas andadas
      cont_goals ++; //aumenta o número de goals passados


      //atualiza os prints
      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);

      timerUpdateTimer(4000); //reseta o timer
    }

    if (timerTimeOver()) //se o tempo acabar, gera uma nova posição e não marca ponto
    {
      goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
      goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;
      cont_casas = 0;
      cont_goals ++;

      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);

      timerUpdateTimer(4000);
    }

    ch = 0; //reseta a tecla apertada
    screenGotoxy(60, 12);
    float time = (4000 - (float)getTimeDiff())/1000; //printa o tempo que falta para acabar o goal em contagem regressiva e em segundos
    printf("Timer: %.2f", time);

    if (cont_goals == 10) //se 10 goals passarem, sai do loop, encerrando o jogo
    {
      screenClear();

      break;

    }

    screenUpdate(); //atualiza a tela para printar o que não foi printado

  }


  //printa a tela de fim de jogo, com pontuação obtida e opção de reiniciar ou sair
  screenSetColor(GREEN, BLACK);
  screenGotoxy(35, 4);
  printf("GAME OVER\n");

  screenGotoxy(35, 8);
  printf("SCORE:");
  screenGotoxy(43, 8);
  printf("         ");
  screenGotoxy(43, 8);
  printf("%d ", score);

  screenGotoxy(30, 10);
  printf("HIGH SCORE:");
  screenGotoxy(43, 10);
  printf("                    ");
  screenGotoxy(43, 10);
  printf("%d", score);

  screenGotoxy(30, 18);
  printf("PRESS 'R' TO RESTART");

  screenGotoxy(30, 20);
  printf("PRESS 'ENTER' TO EXIT");

  screenUpdate();

  int verif = 1;
  while(verif) 
    {
      if(keyhit())
      {
        ch = readch();
      }

      switch(ch)
        {
          case 114: //se apertar r, roda novamente o jogo
          jogo();
          break;

          case 10:  //se apertar enter, sai do loop, consequentemente do jogo, e volta para o menu
          verif = 0;
          break;
        }
    }

  free_list(head);

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

}


// igual ao jogo 1, mas com 2 jogadores
void jogo_2(){
  srand(time(NULL));

  struct square *head = NULL;
  struct obj persona;
  struct obj goal;

  int score_a[2] = {0, 0}; //array para armazenar as pontuações dos 2 jogadores

  int score = 0;
  print_score(score);

  int cont_casas = 0;
  int cont_goals = 0;

  static int ch = 0;

  //tamanho da matriz
  int max_x = 5; 
  int max_y = 5;

  persona.img = 'x';
  persona.color = WHITE;

  goal.img = 'o';
  goal.color = YELLOW;


  screenInit(1);
  keyboardInit();
  timerInit(100);

  for(int i=0; i<2; i++) //repetir o jogo 2 vezes
    {
      persona.x = getx(max_x/2, max_x);
      persona.y = gety(max_y/2, max_y);
      
      screenClear();
      screenGotoxy(35, 12);
      printf("PLAYER %d", i+1);
      screenGotoxy(35, 14);
      printf("PRESS 'ENTER' TO START");
      screenUpdate();
      while(1) 
        {
          if(keyhit() )
          {
            ch = readch();
          }
          if(ch == 10)
          {
            break;
          }
        }
          

      screenClear();
      screenUpdate();

      score = 0;
      cont_casas = 0;
      cont_goals = 0;

      criar_matriz(&head, max_x, max_y);
      int lim_dir = getx(max_x, max_x); 
      int lim_esq = getx(0, max_x);
      int lim_cim = gety(0, max_y);
      int lim_bax = gety(max_y, max_y);

      goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
      goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;

      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);
      print_score(score);

      timerUpdateTimer(4000);

      while(1)
      {

        if(keyhit())
        {
          ch = readch();
        }

        int mov = 1;
        int space_press = 0;

        switch(ch) //WASD
          {
            case 119:
            if((persona.y - 1) >= lim_cim){
              persona.y -= 1;
            }
            else{
              mov = 0;
            }
            break;

            case 115:
            if((persona.y + 1) < lim_bax){
              persona.y += 1;
            }
            else{
              mov = 0;
            }
            break;

            case 97:
            if((persona.x - 1) >= lim_esq){
              persona.x -= 1;
            }
            else{
              mov = 0;
            }
            break;

            case 100:
            if((persona.x + 1) < lim_dir){
              persona.x += 1;
            }
            else{
              mov = 0;
            }
            break;

            case 32:
            space_press = 1;
            break;

            default:
            mov = 0;
            break;
          }
        if (mov){
          cont_casas ++;
          print_matriz(head);
          print_obj(&persona);
          print_obj(&goal);
        }

        if(persona.x == goal.x && persona.y == goal.y && space_press)
        {
          goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
          goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;
          score += cont_casas - 1;
          print_score(score);
          cont_casas = 0;
          cont_goals ++;

          print_matriz(head);
          print_obj(&persona);
          print_obj(&goal);

          timerUpdateTimer(4000);
        }

        if (timerTimeOver())
        {
          goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
          goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;
          cont_casas = 0;
          cont_goals ++;

          print_matriz(head);
          print_obj(&persona);
          print_obj(&goal);

          timerUpdateTimer(4000);
        }

        ch = 0;
        screenGotoxy(60, 12);
        float time = (4000 - (float)getTimeDiff())/1000;
        printf("Timer: %.2f", time);

        if (cont_goals == 10)
        {
          score_a[i] = score; //armazena a pontuação do jogador i no array
          screenClear();

          break;

        }

        screenUpdate();

      }

    }

  //printa a tela de fim de jogo e as pontuações dos 2 jogadores
  screenSetColor(GREEN, BLACK);
  screenGotoxy(35, 4);
  printf("GAME OVER\n");

  screenGotoxy(28, 8);
  printf("SCORE PLAYER 1:  ");
  screenGotoxy(47, 8);
  printf("%d ", score_a[0]);

  screenGotoxy(28, 10);
  printf("SCORE PLAYER 2:  ");
  screenGotoxy(47, 10);
  printf("%d ", score_a[1]);

  screenGotoxy(30, 14);
  printf("WINNER:");
  screenGotoxy(39, 14);
  if(score_a[0] > score_a[1]) //se jogador 1 tiver mais pontos, ele vence
  {
    printf("PLAYER 1");
  }
  else if(score_a[0] < score_a[1]) //se jogador 2 tiver mais pontos, ele vence
  {
    printf("PLAYER 2");
  }
  else  //os 2 tem a mesma quantidade de pontos
  {
    printf("DRAW");
  }

  screenGotoxy(30, 20);
  printf("PRESS 'R' TO RESTART");

  screenGotoxy(30, 22);
  printf("PRESS 'ENTER' TO EXIT");

  screenUpdate();

  int verif = 1;
  while(verif)
    {
      if(keyhit())
      {
        ch = readch();
      }

      switch(ch)
        {
          case 114:
          jogo_2();
          break;

          case 10:
          verif = 0;
          break;
        }
    }

  free_list(head);



  keyboardDestroy();
  screenDestroy();
  timerDestroy();

}


void free_list(struct square *head) {
    struct square *n = head;
    struct square *next;

    while (n != NULL) {
        next = n->next;
        free(n);
        n = next;
    }
}