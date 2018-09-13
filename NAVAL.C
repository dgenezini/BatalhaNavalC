/*************************************************************
12/10/2005

- O ARQUIVO "EGAVGA.BGI" DEVE ESTAR NA PASTA DO PROJETO.
- PARA COMPILAR, A PASTA DO PROJETO DEVE ESTAR CONFIGURADA:
  FILE>CHANGE DIR.

**************************************************************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <alloc.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include "funcoes.c"

void main(){
	int gdriver=9, gmode=2; //Driver VGA e Modo 640x480

	clrscr();
	initgraph(&gdriver,&gmode,""); //Inicia o modo Grafico

	do{
		zera_variaveis();

		do{
			desenha_tela(MARGEM,MARGEM,0); //Desenha a Tela do Jogador
			desenha_tela(TELA2_X,MARGEM,1); //Desenha a Tela do Computador
			desenha_msgs(0); //Desenha o Quadro de Mensagens
			escreve_titulo();
			switch (fase){
				case 1:{
					cleardevice(); //Limpa a Tela
					posiciona_inimigos(); //Posiciona Navios Inimigos
					outtextxy(MARGEM*2,MSG_Y,"Pressione uma Tecla para Comecar.");
					escreve_instrucoes();
					fase=2;
					break;
				}
				case 2:{
					do{
						key=getch();
					}while((toupper(key)!='W')&&(toupper(key)!='A')&&
								 (toupper(key)!='S')&&(toupper(key)!='D')&&
								 (key!=13)&&(key!=32)&&(key!=27)); //Ignora Outras Teclas

					cleardevice(); //Limpa a Tela
					if (sel_navio==4){
						desenha_msgs(1);
						outtextxy(MARGEM*2,MSG_Y,"Digite a Posicao do Porta-Avioes:");
						movimenta_cursor(4);
					}
					if (sel_navio==3){
						desenha_msgs(1);
						outtextxy(MARGEM*2,MSG_Y,"Digite a Posicao do Submarino:");
						movimenta_cursor(3);
					}
					if (sel_navio==2){
						desenha_msgs(1);
						outtextxy(MARGEM*2,MSG_Y,"Digite a Posicao do Navio:");
						movimenta_cursor(2);
					}
					if (sel_navio==1){
						cursor.tela=1; //Troca o Cursor para a Tela do Inimigo
						cursor.tam=1; //Diminui o Tamanho do Cursor para 1 Casa
						cursor.x=0;
						cursor.y=0;
						fase=3;
					}
					break;
				}
				case 3:{
					if (pontos_jogador==TOTAL_PONTOS){
						desenha_msgs(1);
						outtextxy(MARGEM*2,MSG_Y,"Ladrao! Voce Venceu");
						getch();
						fim=1;
					}else{
						if (pontos_inimigo==TOTAL_PONTOS){
							desenha_msgs(1);
							outtextxy(MARGEM*2,MSG_Y,"Voce Nao e de Nada!");
							getch();
							fim=1;
						}else{
							if (turno==0){ //Turno do Jogador
								do{
									key=getch();
								}while((toupper(key)!='W')&&(toupper(key)!='A')&&
											 (toupper(key)!='S')&&(toupper(key)!='D')&&
											 (key!=13)&&(key!=27));

								cleardevice(); //Limpa a Tela
								desenha_msgs(1);
								outtextxy(MARGEM*2,MSG_Y,"Digite a Posicao Para Atacar");
								atirar();
							}else{ //Turno do Inimigo
								cleardevice(); //Limpa a Tela
								turno_inimigo();
							}
						}
					}
					break;
				}
			}
			if (key==27)
				fim=1;
		}while(fim==0);
	}while(key!=27); //Aguarda o ESC para sair
	closegraph(); //Termina o modo Grafico
}
