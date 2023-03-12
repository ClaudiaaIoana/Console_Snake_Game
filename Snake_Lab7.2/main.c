#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <conio.h>
#include<time.h>

 //SNAKE

//dimensiunile chenarului
#define DIM_X 20
#define DIM_Y 20

#define CHR_STANGA_SUS 201
#define CHR_STANGA_JOS 200
#define CHR_DREAPTA_SUS 187
#define CHR_DREAPTA_JOS 188
#define CHR_BARA_VERTICALA 186
#define CHR_BARA_ORIZONTALA 205

#define CHR_CORP_SARPE '*'

#define for_i for(int i=0;i<DIM_X;i++)
#define for_j for(int j=0;j<DIM_Y;j++)

char scena[DIM_X][DIM_Y];

int lungime_sarpe;
int sarpe_x[20];
int sarpe_y[20];
int viteza_x = 0;
int viteza_y = 1;
int prag_viteza = 2;
int fruct_x, fruct_y;
int scor = 0;

//Functie folosita pentru a ascunde cursorul. 
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

//Functie care muta cursorul la pozitia x,y in consola
void gotoXY(int x, int y)
{
	COORD coord = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void initializeaza_scena()
{
	//to do 0: initializeaza matricea cu ' '(spatiu)
	for_i
		for_j
		scena[i][j] = ' ';

	//seteaza barele orizontale (sus si jos)
	for_j
		scena[0][j] = scena[DIM_X - 1][j] = CHR_BARA_ORIZONTALA;


	//seteaza barele verticale (stanga si dreapta)
	for_i
		scena[i][0] = scena[i][DIM_Y - 1] = CHR_BARA_VERTICALA;


	//seteaza colturile chenarului
	scena[0][0] = CHR_STANGA_SUS;
	scena[0][DIM_Y - 1] = CHR_DREAPTA_SUS;
	scena[DIM_X - 1][0] = CHR_STANGA_JOS;
	scena[DIM_X - 1][DIM_Y - 1] = CHR_DREAPTA_JOS;

}

void afiseaza_scena()
{
	system("cls");
	//To do 1: afiseaza scena impreuna cu sarpele
	for_i
	{ for_j
		printf("%c", scena[i][j]);
	printf("\n"); }
	printf("Scorul tau este: %d", scor);
}

void pozitioneaza_sarpe()
{
	for (int i = 0; i < lungime_sarpe; i++)
		scena[sarpe_x[i]][sarpe_y[i]] = CHR_CORP_SARPE;
}

void curata_scena()
{
	//To do 2: 'curata' tot chenarul
	//Acest lucru se realizeaza prin completarea cu ' '(spatiu) in interiorul matricii
	for (int i = 1; i < DIM_X - 1; i++)
		for (int j = 1; j < DIM_Y - 1; j++)
			scena[i][j] = ' ';
	scena[fruct_x][fruct_y] = 'o';
}

void initializeaza_joc()
{
	//aceasta functie seteaza lungimea sarpelui
	//si ii initializeaza pozitia
	lungime_sarpe = 4;

	sarpe_x[0] = DIM_X / 2;
	sarpe_y[0] = DIM_Y / 2;

	sarpe_x[1] = DIM_X / 2;
	sarpe_y[1] = DIM_Y / 2 - 1;

	sarpe_x[2] = DIM_X / 2;
	sarpe_y[2] = DIM_Y / 2 - 2;
	
	sarpe_x[3] = DIM_X / 2;
	sarpe_y[3] = DIM_Y / 2 - 3;

}

void calculeaza_pozitia_sarpelui(int x, int y)
{
	//aceasta functie calculeaza unde ar trebui sa fie localizate noile puncte ale sarpelui
	//atentie, nu le si deseneaza pe ecran.
	//Modificarile vor fi salvate in cei doi vectori, sarpe_x si sarpe_y
	for (int i = lungime_sarpe - 1; i > 0; i--)
	{
		sarpe_x[i] = sarpe_x[i - 1];
		sarpe_y[i] = sarpe_y[i - 1];
	}

	sarpe_x[0] += x;
	sarpe_y[0] += y;
	
	if (sarpe_x[0] == 0 || sarpe_x[0] == -1) sarpe_x[0] = DIM_X - 2;
	if (sarpe_x[0] == DIM_X + 1|| sarpe_x[0] == DIM_X ) sarpe_x[0] = 0;


	if (sarpe_y[0] == 0 || sarpe_y[0] == -1) sarpe_y[0] = DIM_Y - 2;
	if (sarpe_y[0] == DIM_Y + 1 || sarpe_y[0] == DIM_Y) sarpe_y[0] = 0;
}
int joc_pierdut()
{
	//To do 3: verifica daca jocul a fost pierdut
	 /*if (sarpe_x[0] == DIM_X - 1 || sarpe_x[0] == 0 || sarpe_y[0] == DIM_Y - 1 || sarpe_y[0] == 0)
		 return 1;
	 if (sarpe_x[0] == DIM_X || sarpe_x[0] == -1 || sarpe_y[0] == DIM_Y || sarpe_y[0] == -1)
		 return 1;*/
	for (int i = 1; i < lungime_sarpe; i++)
		if (sarpe_x[0] == sarpe_x[i] && sarpe_y[0] == sarpe_y[i])
			return 1;
	return 0;
}

void verifica_tastele()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'a':
			viteza_x = 0;
			viteza_y = -1;
			break;
		case 'd':
			viteza_x = 0;
			viteza_y = 1;
			break;
		case 'w':
			viteza_x = -1;
			viteza_y = 0;
			break;
		case 's':
			viteza_x = 1;
			viteza_y = 0;
			break;
		case 'l':
			viteza_x *= 2;
			viteza_y *= 2;
			break;
		case 'k':
			if (viteza_x == 2 || viteza_x == -2 || viteza_y == 2 || viteza_y == -2)
			{
				viteza_x /= 2;
				viteza_y /= 2;
			}
			break;
		}
	}
}
void refacere_chenar()
{
	//seteaza barele orizontale (sus si jos)
	for_j
		scena[0][j] = scena[DIM_X - 1][j] = CHR_BARA_ORIZONTALA;


	//seteaza barele verticale (stanga si dreapta)
	for_i
		scena[i][0] = scena[i][DIM_Y - 1] = CHR_BARA_VERTICALA;


	//seteaza colturile chenarului
	scena[0][0] = CHR_STANGA_SUS;
	scena[0][DIM_Y - 1] = CHR_DREAPTA_SUS;
	scena[DIM_X - 1][0] = CHR_STANGA_JOS;
	scena[DIM_X - 1][DIM_Y - 1] = CHR_DREAPTA_JOS;
}
void aparitie_fruct()
{
	srand(time(NULL));
	fruct_x = rand() % (DIM_X-2)+1;
	fruct_y = rand() % (DIM_Y-2)+1;
	scena[fruct_x][fruct_y] = 'o';

}void la_mancare()
{
	if (sarpe_x[0] == fruct_x && sarpe_y[0] == fruct_y)
	{
		lungime_sarpe++;
		for (int i = lungime_sarpe - 1; i > 0; i--)
		{
			sarpe_x[i] = sarpe_x[i - 1];
			sarpe_y[i] = sarpe_y[i - 1];
		}
		sarpe_x[0] = fruct_x + viteza_x;
		sarpe_y[0] = fruct_y + viteza_y;
		scor++;
		aparitie_fruct();
	}
}
void main()
{
	int k = 0;

	hidecursor();
	initializeaza_scena();
	initializeaza_joc();

	while (!joc_pierdut())
	{
		curata_scena();  
		pozitioneaza_sarpe();
		calculeaza_pozitia_sarpelui(viteza_x, viteza_y);
		while (k == 0)
		{
			aparitie_fruct();
			k++;
		}
		refacere_chenar();
		la_mancare();
        afiseaza_scena();
        
		for (int i = 0; i < 10000 / prag_viteza; i++)
		{
			verifica_tastele();//daca sunt apasate tastele wasd se modifica directia de deplasare a sarpelui
			Sleep(0.1);
		}
	}//sfarsitul buclei principale a programului


	//Afisarea mesajului de final in cazul pierderii jocului.
	curata_scena();
	for_i
	{ for_j
		scena[i][j]='$';
	}
	afiseaza_scena();
	gotoXY(DIM_X / 4, DIM_Y / 2);//se deplaseaza cursorul pentru afisarea mesajului
	printf("Joc pierdut!");

	_getch();
}