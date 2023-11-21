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

struct score{
  int points;
  struct score *next;
};

struct square* push(char quadrado[4], int x, int y){
  struct square *newSquare = (struct square*)malloc(sizeof(struct square));
  strcpy(newSquare->quadrado, quadrado);
  newSquare->x = x;
  newSquare->y = y;
  newSquare->next = NULL;

  return newSquare;
}

void push_highscore(struct score **head, int pontos){
  struct score *n = *head;
  struct score *novo = (struct score *)malloc(sizeof(struct score));

  novo->points = pontos;
  novo->next = NULL;

  if (*head == NULL) {
    *head = novo;
  } else if (n->points < pontos) { 
    novo->next = *head;
    *head = novo;
  } else {
    while (n->next != NULL && n->next->points > pontos) { 
      n = n->next;
    }
    if (n->next == NULL) {
      novo->next = NULL;
      n->next = novo;
    } else {
      novo->next = n->next;
      n->next = novo;
    }
  }
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

void print_highscore(struct score *head, int score){

  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(30, 21);
  printf("HIGH SCORE:");

  screenGotoxy(43, 21);
  printf("                    ");

  screenGotoxy(43, 21);
  if (head == NULL || score > head->points) printf("%d", score);
  else printf("%d", head->points);

}

int getx(int x, int max_x)
{
  return MAXX/2 - max_x/2 + x;
}

int gety(int y, int max_y)
{
  return MAXY/2 - max_y/2 + y;
}

int main() 
{
  srand(time(NULL));
  
  struct square *head = NULL;
  struct score *head_score = NULL;
  struct obj persona;
  struct obj goal;

  int score = 0;
  print_score(score);
  print_highscore(head_score, score);

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

  //screenGotoxy(1, 1);
  //printf("          ");

  print_matriz(head);
  print_obj(&persona);
  print_obj(&goal);
  print_score(score);
  print_highscore(head_score, score);

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
      score += cont_casas;
      print_score(score);
      print_highscore(head_score, score);
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
      screenClear();
      screenGotoxy(MAXX/2, MAXY/2);
      printf("GAME OVER\n");
      print_score(score);
      print_highscore(head_score, score);
      break;
      
    }

    screenUpdate();
    
  }


  keyboardDestroy();
  screenDestroy();
  timerDestroy();


  return 0;
}
