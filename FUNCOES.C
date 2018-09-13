#define MARGEM 20
#define TAM_CASA 28
#define TAM_NAVIO 10
#define TAM_CHAMINE 5
#define CASAS_TELA 10
#define COR_BARCO_F DARKGRAY
#define COR_BARCO_B LIGHTGRAY
#define COR_BARCO_C LIGHTGRAY
#define COR_TELA_F WHITE
#define COR_TELA_B LIGHTBLUE
#define COR_TITULO RED
#define COR_CURSOR_F YELLOW
#define COR_CURSOR_B YELLOW
#define COR_ERRO RED
#define PORTA_AVIOES 1
#define SUBMARINOS 2
#define NAVIOS 4
#define TOTAL_NAVIOS (PORTA_AVIOES+SUBMARINOS+NAVIOS)
#define NAVIOS_JOGO (TOTAL_NAVIOS*2)
#define TELA2_X TAM_CASA*CASAS_TELA+(MARGEM*3)
#define MSG_Y ((TAM_CASA*CASAS_TELA)+(MARGEM*3))
#define TITULO_X ((TAM_CASA*CASAS_TELA)+MARGEM+(MARGEM/2))
#define TOTAL_PONTOS ((NAVIOS*2)+(SUBMARINOS*3)+(PORTA_AVIOES*4))

//Declaracao de Estruturas
struct Tposicoes{
	int x,y,codnavio,atingido;
};

struct Tcursor{
	int x,y,tam,tela;
	char pos;
};

struct Tnavio{
	int x1,y1,tam,tela,atingido;
	char pos;
};

struct Tposicoes posicoes[10][10][2];
struct Tposicoes ultima_jogada;
struct Tcursor cursor;
struct Tnavio navio[NAVIOS_JOGO+1];

int key,cont_navios,sel_navio,fase,fim,turno,tentativas;
int pontos_jogador,pontos_inimigo, tempo_parado;
int I,X,Y; //Contadores
int qtd_navios[3];

void desenha_msgs(int apaga){
	delay(tempo_parado);
	tempo_parado=0;
	if (apaga){
		setfillstyle(SOLID_FILL,getbkcolor());
		bar(MARGEM,TAM_CASA*CASAS_TELA+(MARGEM*2),getmaxx()-MARGEM,getmaxy()-MARGEM);
	}
	rectangle(MARGEM,TAM_CASA*CASAS_TELA+(MARGEM*2),getmaxx()-MARGEM,getmaxy()-MARGEM);
}

void movimenta_cursor(int tam){
	int erro;

	cursor.tam=tam;

	if (key==13){
		erro=0;
		for (I=0;I<tam;I++){
			if (cursor.pos=='V'){
				if (posicoes[cursor.x][cursor.y+I][0].codnavio!=0){
					erro=1;
				}
			}else{
				if (posicoes[cursor.x+I][cursor.y][0].codnavio!=0){
					erro=1;
				}
			}
		}
		if (erro==0){
			navio[cont_navios].tam=tam;
			navio[cont_navios].pos=cursor.pos;
			navio[cont_navios].x1=cursor.x;
			navio[cont_navios].y1=cursor.y;
			navio[cont_navios].tela=0;

			if (cursor.pos=='V'){
				for(I=0;I<tam;I++){
					posicoes[cursor.x][cursor.y+I][0].codnavio=cont_navios;
				}
			}else{
				for(I=0;I<tam;I++){
					posicoes[cursor.x+I][cursor.y][0].codnavio=cont_navios;
				}
			}
			cont_navios++;
			qtd_navios[tam-2]--;
			if (qtd_navios[tam-2]==0){
				sel_navio--;
			}
			sound(2000);
			delay(50);
			nosound();
		}else{
			desenha_msgs(1);
			outtextxy(MARGEM*2,MSG_Y,"Ja Existe um Navio Nesta Casa");
		}
	}

	//Teste de Colisao
	if (key==32){
		if (cursor.pos=='V'){
			cursor.pos='H';
		}else{
			cursor.pos='V';
		}
	}

	if (cursor.pos=='V'){
    if (toupper(key)=='W'){
			cursor.y--;
		}
		if (toupper(key)=='S'){
			cursor.y++;
		}
		if (toupper(key)=='A'){
			cursor.x--;
		}
		if (toupper(key)=='D'){
			cursor.x++;
		}
		if (cursor.y+(tam-1)>9){
			cursor.y=9-(tam-1);
		}
		if (cursor.x>9){
			cursor.x=9;
		}
		if (cursor.y<0){
			cursor.y=0;
		}
		if (cursor.x<0){
			cursor.x=0;
		}
	}else{
		if (toupper(key)=='W'){
			cursor.y--;
		}
		if (toupper(key)=='S'){
			cursor.y++;
		}
		if (toupper(key)=='A'){
			cursor.x--;
		}
		if (toupper(key)=='D'){
			cursor.x++;
		}
		if (cursor.y>9){
			cursor.y=9;
		}
		if (cursor.x+(tam-1)>9){
			cursor.x=9-(tam-1);
		}
		if (cursor.y<0){
			cursor.y=0;
		}
		if (cursor.x<0){
			cursor.x=0;
		}
	}
}

void atirar(){
	if (key==13){
		posicoes[cursor.x][cursor.y][1].atingido=1;
		desenha_msgs(1);
		if (posicoes[cursor.x][cursor.y][1].codnavio!=0){
			navio[posicoes[cursor.x][cursor.y][1].codnavio].atingido++;
			outtextxy(MARGEM*2,MSG_Y,"Navio Atingido!");
			pontos_jogador++;
			sound(2000);
		}else{
			outtextxy(MARGEM*2,MSG_Y,"Agua!");
			sound(1000);
		}
		delay(50);
		nosound();
		tempo_parado=1300;
		turno=1;
	}

	//Teste de Colisao
	if (toupper(key)=='W'){
		cursor.y--;
	}
	if (toupper(key)=='S'){
		cursor.y++;
	}
	if (toupper(key)=='A'){
		cursor.x--;
	}
	if (toupper(key)=='D'){
		cursor.x++;
	}
	if (cursor.y>9){
		cursor.y=9;
	}
	if (cursor.x>9){
		cursor.x=9;
	}
	if (cursor.y<0){
		cursor.y=0;
	}
	if (cursor.x<0){
		cursor.x=0;
	}
}

void turno_inimigo(){
	int x,y,erro;

	randomize();
	do{
		erro=0;
		if ((ultima_jogada.x>-1)&&(ultima_jogada.y>-1)){
			switch(tentativas){
				case 0:{
					if (ultima_jogada.y>0){
						if (posicoes[ultima_jogada.x][ultima_jogada.y-1][0].atingido==0){
							x=ultima_jogada.x;
							y=ultima_jogada.y-1;
							tentativas++;
							break;
						}else{
							tentativas++;
						}
					}else{
						tentativas++;
					}
				}
				case 1:{
					if (ultima_jogada.x<9){
						if (posicoes[ultima_jogada.x+1][ultima_jogada.y][0].atingido==0){
							x=ultima_jogada.x+1;
							y=ultima_jogada.y;
							tentativas++;
							break;
						}else{
							tentativas++;
						}
					}else{
						tentativas++;
					}
				}
				case 2:{
					if (ultima_jogada.y<9){
						if (posicoes[ultima_jogada.x][ultima_jogada.y+1][0].atingido==0){
							x=ultima_jogada.x;
							y=ultima_jogada.y+1;
							tentativas++;
							break;
						}else{
							tentativas++;
						}
					}else{
						tentativas++;
					}
				}
				case 3:{
					if (ultima_jogada.x>0){
						if (posicoes[ultima_jogada.x-1][ultima_jogada.y][0].atingido==0){
							x=ultima_jogada.x-1;
							y=ultima_jogada.y;
							tentativas++;
							break;
						}
					}
				}
				default:{
					tentativas=0;
					ultima_jogada.x=-1;
					ultima_jogada.y=-1;
					x=random(CASAS_TELA);
					y=random(CASAS_TELA);
				}
			}
		}else{
			x=random(CASAS_TELA);
			y=random(CASAS_TELA);
		}

		if (posicoes[x][y][0].atingido==1){
			erro=1;
		}else{
			posicoes[x][y][0].atingido=1;
			desenha_msgs(1);
			if (posicoes[x][y][0].codnavio!=0){
				ultima_jogada.x=x;
				ultima_jogada.y=y;
				tentativas=0;
				navio[posicoes[x][y][0].codnavio].atingido++;
				outtextxy(MARGEM*2,MSG_Y,"Navio Atingido!");
				pontos_inimigo++;
				sound(2000);
			}else{
				if ((tentativas==0)||(tentativas==4)){
					ultima_jogada.x=-1;
					ultima_jogada.y=-1;
					tentativas=0;
				}
				outtextxy(MARGEM*2,MSG_Y,"Agua!");
				sound(1000);
			}
			delay(50);
			nosound();
		}
	}while(erro==1);
	turno=0;
}

void desenha_navio(int x, int y, int casas, int tela, char pos){
	int x1,y1,x2,y2,chamines,chaminesx,chaminesy;

	x1=posicoes[x][y][tela].x;
	y1=posicoes[x][y][tela].y;

	chamines=casas-2; //Sem Chamines Nas Duas Pontas
	chaminesx=x1;
	chaminesy=y1;
	x2=x1;
	y2=y1;

	if (toupper(pos)=='V') {
		//Vertical
		setfillstyle(SOLID_FILL,COR_BARCO_B); //Cor de Fundo do Navio
		setcolor(COR_BARCO_F); //Cor da Linha do navio
		y2=y1+((casas-1)*TAM_CASA); //Calcula a Posicao da Ultima Casa do Navio
		//Desenha o Fundo do Navio
		fillellipse(x1,y1,TAM_NAVIO,TAM_NAVIO);
		fillellipse(x2,y2,TAM_NAVIO,TAM_NAVIO);
		bar(x1-TAM_NAVIO,y1,x2+TAM_NAVIO,y2);
		//Desenha o Contorno do Navio
		arc(x1,y1,0,180,TAM_NAVIO);
		line(x1-TAM_NAVIO,y1,x1-TAM_NAVIO,y2);
		arc(x1,y2,180,0,TAM_NAVIO);
		line(x1+TAM_NAVIO,y1,x1+TAM_NAVIO,y2);
		//Desenha as Chamines no Meio do Navio
		setfillstyle(SOLID_FILL,COR_BARCO_C); //Cor das Chamines do Navio
		while (chamines>0){
			chaminesy=chaminesy+TAM_CASA; //Incrementa a Posicao
			fillellipse(chaminesx,chaminesy,TAM_CHAMINE,TAM_CHAMINE);
			circle(chaminesx,chaminesy,TAM_CHAMINE);
			chamines--;
		}
		if (casas==2){
			chaminesy=chaminesy+(TAM_CASA/2);
			fillellipse(chaminesx,chaminesy,TAM_CHAMINE,TAM_CHAMINE);
			circle(chaminesx,chaminesy,TAM_CHAMINE);
		}
	}else{
		//Horizontal
		setfillstyle(SOLID_FILL,COR_BARCO_B); //Cor de Fundo do Navio
		setcolor(COR_BARCO_F); //Cor da Linha do navio
		x2=x1+((casas-1)*TAM_CASA); //Calcula a Posicao da Ultima Casa do Navio
		//Desenha o Fundo do Navio
		fillellipse(x1,y1,TAM_NAVIO,TAM_NAVIO);
		fillellipse(x2,y2,TAM_NAVIO,TAM_NAVIO);
		bar(x1,y1-TAM_NAVIO,x2,y2+TAM_NAVIO);
		//Desenha o Contorno do Navio
		arc(x1,y1,90,270,TAM_NAVIO);
		line(x1,y1-TAM_NAVIO,x2,y1-TAM_NAVIO);
		arc(x2,y1,270,90,TAM_NAVIO);
		line(x1,y1+TAM_NAVIO,x2,y1+TAM_NAVIO);
		setfillstyle(SOLID_FILL,COR_BARCO_C); //Cor das Chamines do Navio
		//Desenha as Chamines no Meio do Navio
		while (chamines>0){
			chaminesx=chaminesx+TAM_CASA; //Incrementa a Posicao
			fillellipse(chaminesx,chaminesy,TAM_CHAMINE,TAM_CHAMINE);
			circle(chaminesx,chaminesy,TAM_CHAMINE);
			chamines--;
		}
		if (casas==2){
			chaminesx=chaminesx+(TAM_CASA/2);
			fillellipse(chaminesx,chaminesy,TAM_CHAMINE,TAM_CHAMINE);
			circle(chaminesx,chaminesy,TAM_CHAMINE);
		}
	}
	setbkcolor(COR_TELA_B); //Retorna para a Cor da Tela
	setcolor(COR_TELA_F); //Retorna para a Cor da Linha
}

void posiciona_inimigos(){
	int x, y, pos, tam, erro, N;
	int cont_navio=NAVIOS, cont_sub=SUBMARINOS, cont_porta=PORTA_AVIOES;

	randomize();

	for (I=1;I<=TOTAL_NAVIOS;I++){
		if (cont_porta>0){
			tam=4;
			cont_porta--;
		}
		else if (cont_sub>0){
			tam=3;
			cont_sub--;
		}
		else if (cont_navio>0){
			tam=2;
			cont_navio--;
		}

		do{
			erro=0;
			pos=random(2);
			if (pos==0){
				x=random(CASAS_TELA-tam);
				y=random(CASAS_TELA);
				for(N=0;N<=(tam-1);N++){
					if (posicoes[x+N][y][1].codnavio!=0){
						erro=1;
						break;
					}
				}
			}else{
				x=random(CASAS_TELA);
				y=random(CASAS_TELA-tam);
				for(N=0;N<=(tam-1);N++){
					if (posicoes[x][y+N][1].codnavio!=0){
						erro=1;
						break;
					}
				}
			}
		}while(erro==1);

		for(N=0;N<tam;N++){
			navio[cont_navios].x1=x;
			navio[cont_navios].y1=y;
			navio[cont_navios].tam=tam;
			navio[cont_navios].tela=1;
			navio[cont_navios].atingido=0; //Voltar para 0

			if (pos==0){
				navio[cont_navios].pos='H';
				posicoes[x+N][y][1].codnavio=cont_navios;
			}else{
				navio[cont_navios].pos='V';
				posicoes[x][y+N][1].codnavio=cont_navios;
			}
		}
		cont_navios++;
	}
}

void desenha_tela(int x,int y,int tela){
	int T,x1,x2,y1,y2;
	char letra[2];
	char numero[2];

	setbkcolor(COR_TELA_B);
	setcolor(COR_TELA_F);

	x1=x;
	x2=x1+TAM_CASA;
	y1=y;
	y2=y1+TAM_CASA;
	for(Y=0;Y<=9;Y++){ //Desenha as Linhas
		for(X=0;X<=9;X++){ //Desenha os Quadrados da Linha
			posicoes[X][Y][tela].x=x1+(TAM_CASA/2); //Guarda o Centro de Todas as Casas
			posicoes[X][Y][tela].y=y1+(TAM_CASA/2); //Guarda o Centro de Todas as Casas
			letra[0]=65+X; //Incrementa a Letra da Coluna
			letra[1]='\0';
			numero[0]=48+Y; //Incrementa o Numero da Linha
			numero[1]='\0';

			rectangle(x1,y1,x2,y2); //Desenha a Casa

			if (posicoes[X][Y][tela].atingido==1){ //Marca Casa Atingida
				if (posicoes[X][Y][tela].codnavio>0){
					setfillstyle(SOLID_FILL,COR_ERRO);
					bar(x1,y1,x2,y2);
				}else{
					setcolor(COR_ERRO);
					line(x1,y1,x2,y2);
					line(x2,y1,x1,y2);
				}
			}
			setcolor(COR_TELA_F);


			if ((cursor.pos=='V')&&(cursor.tela==tela)){
				if ((X==cursor.x)&&((Y>=cursor.y)&&(Y<cursor.y+cursor.tam))){
					setcolor(COR_CURSOR_F);
					setfillstyle(SOLID_FILL,COR_CURSOR_B);
					bar(x1+1,y1+1,x2-1,y2-1);
					rectangle(x1+1,y1+1,x2-1,y2-1); //Desenha o Cursor
					setcolor(COR_TELA_F);
				}
			}
			if ((cursor.pos=='H')&&(cursor.tela==tela)){
				if ((Y==cursor.y)&&((X>=cursor.x)&&(X<cursor.x+cursor.tam))){
					setcolor(COR_CURSOR_F);
					setfillstyle(SOLID_FILL,COR_CURSOR_B);
					bar(x1+1,y1+1,x2-1,y2-1);
					rectangle(x1+1,y1+1,x2-1,y2-1); //Desenha o Cursor
					setcolor(COR_TELA_F);
				}
			}
			outtextxy(x1-4+(TAM_CASA/2),y-14,letra); //Escreve a Letra da Coluna

			x1=x1+TAM_CASA; //Posicao da Proxima Casa
			x2=x1+TAM_CASA; //Posicao da Proxima Casa
		}
		outtextxy(x-14,y1-4+(TAM_CASA/2),numero); //Escreve o Numero da Linha
		x1=x;
		x2=x1+TAM_CASA;
		y1=y1+TAM_CASA;
		y2=y2+TAM_CASA;
	}
	for(I=1;I<=NAVIOS_JOGO;I++){
		if ((navio[I].tam>0)&&(navio[I].tela==tela)){
			if ( ((tela==0)) || ((tela==1)&&(navio[I].atingido==navio[I].tam)) ){
					desenha_navio(navio[I].x1,navio[I].y1,navio[I].tam,navio[I].tela,navio[I].pos);
			}
		}
	}
}

void escreve_titulo(){
	setcolor(COR_TITULO);
	outtextxy(TITULO_X,MARGEM+(12*0), "B");
	outtextxy(TITULO_X,MARGEM+(12*1), "A");
	outtextxy(TITULO_X,MARGEM+(12*2), "T");
	outtextxy(TITULO_X,MARGEM+(12*3), "A");
	outtextxy(TITULO_X,MARGEM+(12*4), "L");
	outtextxy(TITULO_X,MARGEM+(12*5), "H");
	outtextxy(TITULO_X,MARGEM+(12*6), "A");

	outtextxy(TITULO_X,MARGEM+(12*8), "N");
	outtextxy(TITULO_X,MARGEM+(12*9), "A");
	outtextxy(TITULO_X,MARGEM+(12*10),"V");
	outtextxy(TITULO_X,MARGEM+(12*11),"A");
	outtextxy(TITULO_X,MARGEM+(12*12),"L");
	setcolor(COR_TELA_F);
}

void escreve_instrucoes(){
	setfillstyle(SOLID_FILL,getbkcolor());
	bar(getmaxy()-MARGEM-140,TAM_CASA*CASAS_TELA+(MARGEM*2),getmaxx()-MARGEM,getmaxy()-MARGEM);
	rectangle(getmaxy()-MARGEM-140,TAM_CASA*CASAS_TELA+(MARGEM*2),getmaxx()-MARGEM,getmaxy()-MARGEM);
	outtextxy(getmaxy()-MARGEM-120,TAM_CASA*CASAS_TELA+(MARGEM*3),"W - Cima");
	outtextxy(getmaxy()-MARGEM-120,TAM_CASA*CASAS_TELA+(MARGEM*4),"A - Esquerda");
	outtextxy(getmaxy()-MARGEM-120,TAM_CASA*CASAS_TELA+(MARGEM*5),"S - Baixo");
	outtextxy(getmaxy()-MARGEM-120,TAM_CASA*CASAS_TELA+(MARGEM*6),"D - Direita");
	outtextxy(getmaxy()-MARGEM-120,TAM_CASA*CASAS_TELA+(MARGEM*7),"Enter - Confirma");
	outtextxy(getmaxy()-MARGEM-120,TAM_CASA*CASAS_TELA+(MARGEM*8),"Espaco - Gira");
}

void zera_variaveis(){
	//Zerando Variaveis
	ultima_jogada.x=-1;
	ultima_jogada.y=-1;
	pontos_jogador=0;
	pontos_inimigo=0;
	cursor.x=0;
	cursor.y=0;
	cursor.pos='V';
	cursor.tela=0;
	cursor.tam=0;

	for(I=0;I<=NAVIOS_JOGO;I++){
		navio[I].x1=0;
		navio[I].y1=0;
		navio[I].tam=0;
		navio[I].tela=0;
		navio[I].pos='H';
		navio[I].atingido=0;
	}
	for(Y=0;Y<CASAS_TELA;Y++){
		for(X=0;X<CASAS_TELA;X++){
			posicoes[X][Y][0].x=0;
			posicoes[X][Y][0].y=0;
			posicoes[X][Y][0].codnavio=0;
			posicoes[X][Y][0].atingido=0;
			posicoes[X][Y][1].x=0;
			posicoes[X][Y][1].y=0;
			posicoes[X][Y][1].codnavio=0;
			posicoes[X][Y][1].atingido=0;
		}
	}
	qtd_navios[0]=NAVIOS;
	qtd_navios[1]=SUBMARINOS;
	qtd_navios[2]=PORTA_AVIOES;
	sel_navio=4;
	cont_navios=1;
	fase=1;
	fim=0;
	turno=0;
	tentativas=0;
	tempo_parado=0;
}
