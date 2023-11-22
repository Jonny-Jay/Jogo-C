/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"


struct square{
  char quadrado[4];
  int x;
  int y;
  struct square *next;
};

struct obj{
char img;
int x;
int y;
screenColor color;
};


struct square* push(char quadrado[4], int x, int y){
  struct square *newSquare = (struct square*)malloc(sizeof(struct square));
  strcpy(newSquare->quadrado, quadrado);
  newSquare->x = x;
  newSquare->y = y;
  newSquare->next = NULL;

  return newSquare;
}


void criar_matriz(struct square **head, int tamanho_x, int tamanho_y)
{
  int i, j;

  for(i = 0; i < tamanho_x; i++)
    {
      for(j = 0; j < tamanho_y; j++)
        {
          int pos_x = MAXX/2 - tamanho_x/2 + j;
          int pos_y = MAXY/2 - tamanho_y/2 + i;
          if(*head == NULL)
          {
            *head = push("_", pos_x, pos_y);
          }
          else
          {
            struct square *n = *head;
            while (n->next != NULL) {
              n = n->next;
            }
            struct square* newSquare = push("_", pos_x, pos_y);
            n->next = newSquare;
          }
         
        }
    }
}

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

void print_obj(struct obj *obj)
{
  screenSetColor(obj->color, YELLOW);
  screenGotoxy(obj->x, obj->y);
  printf("%c", obj->img);
}

void print_score(int score){
  screenSetColor(GREEN, BLACK);
  screenGotoxy(35, 19);
  printf("SCORE:");

  screenGotoxy(43, 19);
  printf("         ");

  screenGotoxy(43, 19);
  printf("%d ", score);
}

void print_highscore(int score){

  screenSetColor(GREEN, BLACK);
  screenGotoxy(30, 21);
  printf("HIGH SCORE:");

  screenGotoxy(43, 21);
  printf("                    ");

  screenGotoxy(43, 21);
  printf("%d", score);
  

}

int getx(int x, int max_x)
{
  return MAXX/2 - max_x/2 + x;
}

int gety(int y, int max_y)
{
  return MAXY/2 - max_y/2 + y;
}

void print_menu(){
  screenSetColor(GREEN, BLACK);
  
  screenGotoxy(36, 4);
  screenSetBold();
  //screenSetColor(GREEN, BLACK);
  printf("MAHARAJA");
  
  screenGotoxy(36, 8);
  //screenSetColor(GREEN, BLACK);
  printf("1 PLAYER");

  screenGotoxy(36, 10);
  //screenSetColor(GREEN, BLACK);
  printf("2 PLAYERS");
  
  screenGotoxy(36, 12);
  //screenSetColor(GREEN, BLACK);
  printf("CONTROLS");
  
  screenGotoxy(36, 14);
  //screenSetColor(GREEN, BLACK);
  printf("LEADERBOARD");
  
  screenGotoxy(36, 16);
  //screenSetColor(GREEN, BLACK);
  printf("EXIT");
}

void print_controles()
{
  struct obj select;
  select.img = '>';
  select.x = 28;
  select.y = 20;
  
  while(1)
    {
      screenGotoxy(36, 4);
      screenSetBold();
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
      printf("GAME ENDS AFTER 20 GOALS");

      screenGotoxy(30, 20);
      printf("BACK");

      print_obj(&select);

      static int ch = 0;

      if(keyhit())
      {
        ch = readch();
      }
      if(ch == 10)
      {
        screenClear();
        break;
      }
    }
}

void jogo(){
  srand(time(NULL));

  struct square *head = NULL;
  struct obj persona;
  struct obj goal;

  int score = 0;
  print_score(score);
  print_highscore(score);

  int cont_casas = 0;
  int cont_goals = 0;

  static int ch = 0;

  //tamanho da matriz
  int max_x = 5; 
  int max_y = 5;

  persona.img = 'x';
  persona.color = WHITE;
  persona.x = getx(max_x/2, max_x);
  persona.y = gety(max_y/2, max_y);

  goal.img = 'o';
  goal.color = YELLOW;


  screenInit(1);
  keyboardInit();
  timerInit(100);

  criar_matriz(&head, max_x, max_y);
  int lim_dir = getx(max_x, max_x); 
  int lim_esq = getx(0, max_x);
  int lim_cim = gety(0, max_y);
  int lim_bax = gety(max_y, max_y);

  goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
  goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;

  sleep(1);

  print_matriz(head);
  print_obj(&persona);
  print_obj(&goal);
  print_score(score);
  print_highscore(score);

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
        break;

        case 115:
        if((persona.y + 1) < lim_bax){
          persona.y += 1;
        } 
        break;

        case 97:
        if((persona.x - 1) >= lim_esq){
          persona.x -= 1;
        }
        break;

        case 100:
        if((persona.x + 1) < lim_dir){
          persona.x += 1;
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
      print_highscore(score);
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

    if (cont_goals == 3)
    {
      screenClear();
      
      break;

    }

    screenUpdate();

  }

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
          case 114:
          jogo();
          break;

          case 10:
          verif = 0;
          break;
        }
    }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();
  
}

void jogo_2(){
  srand(time(NULL));

  struct square *head = NULL;
  struct obj persona;
  struct obj goal;

  int score_a[2] = {0, 0};

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
  persona.x = getx(max_x/2, max_x);
  persona.y = gety(max_y/2, max_y);

  goal.img = 'o';
  goal.color = YELLOW;


  screenInit(1);
  keyboardInit();
  timerInit(100);

  for(int i=0; i<2; i++)
    {
      sleep(1);
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
            break;

            case 115:
            if((persona.y + 1) < lim_bax){
              persona.y += 1;
            } 
            break;

            case 97:
            if((persona.x - 1) >= lim_esq){
              persona.x -= 1;
            }
            break;

            case 100:
            if((persona.x + 1) < lim_dir){
              persona.x += 1;
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
          score_a[i] = score;
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

        if (cont_goals == 3)
        {
          screenClear();

          break;

        }

        screenUpdate();

      }

    }
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
  if(score_a[0] > score_a[1])
  {
    printf("PLAYER 1");
  }
  else if(score_a[0] < score_a[1])
  {
    printf("PLAYER 2");
  }
  else
  {
    printf("   DRAW");
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
          break; //te amo jota  vem cá
        }
    }

  

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

}

int main() 
{
  screenInit(1);
  keyboardInit();
  timerInit(100);

  static int caracter = 0;

  struct obj select;
  select.img = '>';
  select.x = 35;
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
      switch(caracter)
        {
          case 119: //W
          if(select.y > 8)
          {
            select.y -= 2;
          }
          break;

          case 115:
            if(select.y < 16) //S
            {
              select.y += 2;
            }
          break;

          case 10:
          enter_press = 1;
          break;

          default:
          mov_menu = 0;
          break;
        }//oi jotinho boa noite

        if(mov_menu)
        {
          screenClear();
          print_menu();
          print_obj(&select);
        }
      
        if(enter_press)
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
            //leaderboard
          }
          else if(select.y == 16)
          {
            screenClear();
            exit(1);
          }
        }
      caracter = 0;
      
    }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}


