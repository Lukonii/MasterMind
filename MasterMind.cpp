#include "defkey.h"
#include <string.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getkey();
char *alltrim(char *s);
void Menu(int color, ...);
bool DrawMask(char *File, char *Items[]);
char *Line(char *s, int n, int FlagLine);
char *replicate(char *s, unsigned int n);
void outtxt(char *s, int x, int y, int color);
int LineEdit(char *s, int x, int y, int color);
void MaskEdit(char *First, int Length[], int color, int *randomNumber);
void Frame(int x1, int y1, int x2, int y2, int color, int Flag);
void ScrollMenu(int d[], char *c, char *o[], int *x, int *y, int n);
void Grid(int m, int n, int w, int h, int x, int y, int color);
void PasteScore(char *score, int x, int y, int space);
void DrawMovesBoard();//crta tebelu gde se unose brojevi
void DrawScoreBoard();//crta tabelu sa rezultatima
void DrawResultBoard();//crta tabelu sa rezultatima pogadjanja
void EnterName();//prvo se poziva ovaj metod radi unosa imena pa tek onda krece igra
void ChooseAction(int action);//nako zavrsene igde se bira akcija New, Ignore, Exit
int *GenerateRandomNumber();
void NewGame();
bool CheckInput(char *input, int *randomNumber,int x, int y,int moves);
void ReadTops();//uzima iz fajla top 10
bool CheckRepeat();
void NewRound();
void CheckScoreBoard();//proverava poziciju na tabeli
void WriteScore();//upise rezultate u file

//Global
char names[20][14];
char points[10][2];
char score[5];
int numberOfGames;
char player[17];


main()
{
	int i;
	numberOfGames = 0;
	NewGame();
}
//------------------------------------------------------------------
void ReadTops(){
	FILE *top;
	top = fopen("Mind.txt","r");
	int y,i,k;
	y = 9;
	k = 0;
	for(i=0;i<10;i++){
		fgets(names[k],sizeof(names[k]),top);
		outtxt("        ",40,y,WHITE + MAGENTA * 16);
		outtxt(names[k],40,y,WHITE + MAGENTA * 16);
		y++;
		k++;
	}
	y = 9;
	for(i=10;i<20;i++){
		fgets(names[k],sizeof(names[k]),top);
		outtxt(names[k],72,y,BLUE + LIGHTGRAY * 16);
		y++;
		k++;
	}
	fclose(top);
}
//------------------------------------------------------------------
void NewGame(){
	int Length[] = {4,4,4,4,4,4,4,4,4,0};//duzine polja za unos i broj polja
	char *Items[] = { "01","02","03","04","05","06","07","08","09","10","" };//redni brojevi
	clrscr();

	int *randomNumber;
	int i;
	randomNumber = GenerateRandomNumber();
	if (DrawMask("Masters of Mind",Items)) {
		PasteScore("Score",6,8,2); // stampa score
		numberOfGames = 0;
		ReadTops();
		MaskEdit(Items[0],Length,WHITE + 16 * MAGENTA, randomNumber);
	}
}
void NewRound(){
	int Length[] = {4,4,4,4,4,4,4,4,4,0};//duzine polja za unos i broj polja
	char *Items[] = { "01","02","03","04","05","06","07","08","09","10","" };//redni brojevi
	int *randomNumber;
	int i,x,y;
	randomNumber = GenerateRandomNumber();
	x=21; y=10;
	for (i=0;i<9;i++){
		gotoxy(x,y);
		cprintf("%c",32);
		x++;
		gotoxy(x,y);
		cprintf("%c",32);
		x++;
		gotoxy(x,y);
		cprintf("%c",32);
		x++;
		gotoxy(x,y);
		cprintf("%c",32);
		
		x=21;
		y++;
	}
	x=32;y=10;
	for(i=0;i<9;i++){
		gotoxy(x,y);
		cprintf("%c",32);
		x++;
		gotoxy(x,y);
		cprintf("%c",32);
		
		x=32;
		y++;
	}

	MaskEdit(Items[0],Length,WHITE + 16 * MAGENTA, randomNumber);
}
//------------------------------------------------------------------
int *GenerateRandomNumber(){
	//the Knuth algoritam
	static int array[4];
	int i,j;
	srand((unsigned)time(NULL)); 
	int in,im;
	im = 0;
	for(in = 0; in < 9 && im < 4; ++in){
		int rn = 9 - in;
		int rm = 4 - im;
		if(rand() % rn < rm){
			array[im++] = in + 1;
		}
	}
	int x;
	i = rand() % 2 + 1;
	if(i <= 1 ){x = array[0];
		array[0] = array[2];
		array[2] = x;
		x = array[1];
		array[1] = array[3];
		array[3] = x;}
	else {
		x = array[0];
		array[0] = array[3];
		array[3] = x;
		x = array[1];
		array[1] = array[2];
		array[2] = x;
	}
	return array;
}
//------------------------------------------------------------------
void PasteScore(char score[], int x, int y, int space){
	Grid(5,1,1,1,5,7,CYAN);//5kolona-1vrsta-2siri-1duzina-5x-5y
	int i,n;
	n = (int)strlen(score);
	for(i=0;i<n;i++){
		gotoxy(x,y);
		cprintf("%c",score[i]);
		x = x + space;
	}
}
//-------------------------------------------------------------------
char *alltrim(char *p)
{
	char *q, *r;
	int n;
	while (isspace(*p)) p++;
	if (*p == 0) {
		q = new char[1];
		*q = 0;
		return q;
	}
	q = p + strlen(p);
	while (isspace(*q)) q--;
	n = q - p + 1;
	r = new char [n + 1];
	while (p <= q) *r++ = *p++;
	*r = 0;
	return (r - n);
}
//------------------------------------------------------------------
void Bs (char *s,int Data[])
{
	if (!Data[0]) return; 
	if (Data[0]-- > Data[2]) return;
	Data[4] = 1; Data[1] = Data[2];
	movmem(&s[Data[0] + 1],&s[Data[0]],Data[1] - Data[0]);
	s[Data[1]] = ' ';
	if (s[Data[2]] == ' ') Data[4] = 9;
	return;
}
//------------------------------------------------------------------
void Del (char *s,int Data[])
{
	if (Data[0] == Data[2] && s[Data[2]] != ' ') {
		s[Data[2]] = ' '; Data[4] = 9;
		return;
	}
	if (Data[2] <= Data[0]) return;
	Data[4] = 1; Data[1] = Data[2];
	movmem (&s[Data[0] + 1], &s[Data[0]], Data[1] - Data[0]);
	s[Data[1]] = ' ';
	if (Data[1] == Data[2]) Data[4] = 9;
	return;
}
//------------------------------------------------------------------
void Default (char *s,int Data[])
{
	if (Data[3] < ' ' || Data[3] > 'z') return;
	Data[3] = toupper (Data[3]);
	Data[1] = Data[0];
	if (Data[2] <= Data[0]) {
		s[Data[0]] = Data[3];
		Data[0]++;
		if (s[Data[1]] != ' ') {
			Data[2] = Data[0]; Data[4] = 1;
		}
		return;
	}
	Data[4] = 1; Data[1] = Data[2];
	movmem (&s[Data[0]], &s[Data[0] + 1],Data[1] - Data[0]);
	s[Data[0]] = Data[3];
	Data[0]++; Data[2]++; int len = strlen(s);
	if (Data[2] < len) return;
	Data[2]--;
	if (isspace(s[Data[2]])) Data[4] = 9;
	return;
}
//------------------------------------------------------------------
void ScrollMenu(int d[], char *c, char *o[], int *x, int *y, int n)
{
	int i, j, k, key;
	_setcursortype(_NOCURSOR);
	for (i = 0; i < n; i++) {
		textattr(d[2]); gotoxy(x[i],y[i]); cprintf("%s\n", o[i]);
		Frame(x[i] - 1, y[i] - 1,x[i] + d[5],y[i] + 1,d[0],1);
	}
	textattr (d[1]); gotoxy (x[0],y[0]); cputs (o[0]);
	i = 0; k = 0;
	while (1) {
		key = getkey();
		if (isalpha(key)) key = toupper(key);
		switch (key) {
			case ESC:
				_setcursortype(_NORMALCURSOR);
				gotoxy(1,1);
				return;
			case CR: //ako je pretisnu Enter na dugme
				//printf("%d\n", k);
				ChooseAction(k);
				//break;
			case UP:
				if (d[6]) if (--k < 0) k = n - 1;
				break;
			case LEFT:
				if (!d[6]) if (--k < 0) k = n - 1;
				break;
			case DOWN:
				if (d[6]) if (++k == n) k = 0;
				break;
			case RIGHT:
				if (!d[6]) if (++k == n) k = 0;
				break;
			case HOME:
				k = 0;
				break;
			case END:
				k = n - 1;
				break;
			default:
				for (j = 0; j < n; j++) if (key == c[j]) k = j;
		}
		if (i == k) continue;
		textattr (d[2]); gotoxy (x[i],y[i]); cputs (o[i]);
		textattr (d[1]); gotoxy (x[k],y[k]); cputs (o[k]);
		i = k;
	}
}
//------------------------------------------------------------------
void Frame(int x1, int y1, int x2, int y2, int color, int Flag)
{
	int CoorBox[] = {
	218,191,192,217,196,179,
	201,187,200,188,205,186
	};//ovo su vrednosti karaktera za isrctavanje
	int i, FlagDefault;
	int *p;
	FlagDefault = 0;
	textattr(color);
	if (Flag <= 1) Flag = 0;
	while(1) {
		if (x1 < 1 || y1 < 1 || x2 > 80 || y2 > 25) {
			FlagDefault = 1;
			break;
		}
		if (x2 <= x1 || y2 <= y1) {
			FlagDefault = 1;
			break;
		}
		break;
	}
	if (FlagDefault) {
		x1 = 1; y1 = 1; x2 = 80; y2 = 25;
	}
	if (Flag) Flag = 6;
	p = CoorBox + Flag;
	gotoxy(x1,y1); putch(*p++);
	gotoxy(x2,y1); putch(*p++);
	gotoxy(x1,y2); putch(*p++);
	gotoxy(x2,y2); putch(*p++);
	for (i = x1 + 1; i < x2; i++) {
		gotoxy (i,y1); putch(*p);
		gotoxy (i,y2); putch(*p);
	}
	p++;
	for (i = y1 + 1; i < y2; i++) {
		gotoxy (x1,i); putch(*p);
		gotoxy (x2,i); putch(*p);
	}
	gotoxy(x1,y1);
}
//------------------------------------------------------------------
int LineEdit (char *s, int x, int y, int color)
{
	void Del(char *s, int Data[]); 
	void Bs(char *s, int Data[]);
	void Default(char *s, int Data[]);
	int len, Data[5];
	len = strlen(s); if (!len) return 0;
	Data[2] = len - 1;
	if (isspace (s[Data[2]])) Data[4] = 9; else Data[4] = 1;
	Data[0] = 0;
	while(Data[0] < len) {
		if (Data[4]) {
		outtxt(s,x,y,color);
		if (Data[4] == 9) {
			while(isspace(s[Data[2]]) && Data[2] > 0) Data[2]--;
			if (!isspace (s[Data[2]])) Data[2]++;
		}
		Data[4] = 0;
		}
		gotoxy(x + Data[0],y);
		switch (Data[3] = getkey()) {
			case HOME:
				Data[0] = 0;
				break;
			case END:
				Data[0] = Data[2];
				break;
			case LEFT:
				if (--Data[0] < 0) Data[0] = 0;
				break;
			case RIGHT:
				if (++Data[0] == len) Data[0]--;
				break;
			case CR: case ESC: case UP: case DOWN: case PAGEDOWN:
			case CTRLHOME: case CTRLEND:
				return Data[3];
			case BS: Bs(s,Data); break;
			case DEL: Del(s,Data); break;
			default:
				Default(s,Data);
				break;
		}
	}
	outtxt(s,x,y,color);
	return CR;
}
//------------------------------------------------------------------
void outtxt (char *s, int x, int y, int color)
{
	textattr (color); gotoxy (x,y);
	if (s) cputs (s);
}
//------------------------------------------------------------------
char *replicate(char *s, unsigned int n)
{
	unsigned int i, j, Length;
	char *p;
	Length = strlen(s);
	p = new char [Length * n + 1];
	for (i = 0; i < n; i++) {
	for (j = 0; j < Length; j++) *p++ = s[j];
	}
	*p = 0;
	return (p - (Length * n));
}
//------------------------------------------------------------------
char *Line(char *s, int n, int FlagLine)
{
	int i, len, Start, End;
	char *p, *q, *r;
	q = alltrim(s); len = strlen(q);
	if (len > n) n = len;
	p = new char [n + 1];
	for (i = 0; i < n; i++) p[i] = ' ';
	p[n] = 0;
	switch(FlagLine) {
		case 0:
			Start = 0;
			break;
		case 1:
			Start = (n - len) / 2;
			break;
		default:
			Start = n - len;
			break;
	}
	End = Start + len;
	r = q;
	for (i = Start; i < End; i++) p[i] = *r++;
	delete [] q;
	return p;
}
//------------------------------------------------------------------
bool DrawMask(char *File, char *Items[])
{
	static char *o[] = {
	"New",
	"Ignore",
	"Exit"
	};
	int i, j, n, x1, x2, len, Len;
	char *p;
	p = replicate (" ",22);
	Frame(1,1,80,3,CYAN,1); 
	Frame(1,5,80,20,MAGENTA,1);
	outtxt(p,2,2,RED * 16); 
	outtxt(p,58,2,RED * 16);
	delete [] p;
	p = Line(File,34,1); outtxt(p,24,2,BLUE + LIGHTGRAY * 16);
	delete [] p;
	for (i = 0; i < 3; i++) {
		x1 = 1 + i * 27; x2 = x1 + 25;
		Frame(x1,22, x2,24,MAGENTA,1);
		p = Line(o[i],24,1);
		outtxt(p,x1 + 1,23,BROWN);
		delete [] p;
	}
	len = strlen(Items[0]); Len = len;
	n = 0;
	while (len) {
		if (len != Len) return false;
		len = strlen(Items[++n]);
	}
	if (n > 12) return false;//kraj igre
	DrawScoreBoard();//crta tablu za autora i poteze
	DrawMovesBoard();
	DrawResultBoard();
	for (i = 0; i < n; i++) {
		outtxt(Items[i],54, 9 + i,WHITE + MAGENTA * 16);//ovde se pomeraju redni brojevi
		outtxt(Items[i],58, 9 + i,BLUE + LIGHTGRAY * 16);//lista rednih brojeva
	}
	return true;
}
//------------------------------------------------------------------
void DrawScoreBoard(){
	char *bcg;
	outtxt(" Player:          ",39,7,BLUE + LIGHTGRAY * 16);
	outtxt("                  ",57,7,BROWN + MAGENTA * 16);
	bcg = replicate(" ",18);
	int i;
	for(i=0;i<12;i++){
		outtxt(bcg,57,8+i,LIGHTGRAY * 16);
	}
	for(i=0;i<12;i++){
		outtxt(bcg,39,8+i,MAGENTA * 16);
	}
}
//------------------------------------------------------------------
void DrawMovesBoard(){
	Grid(1,1,6,1,19,7,CYAN);
	Grid(1,1,6,11,19,7,CYAN);//5kolona-1vrsta-2siri-1duzina-5x-5y
	//prvo cemo obojiti polja a potom dodati tekst na njih
	gotoxy(26, 9); putch(180);//dodamo linije na tabelu
	gotoxy(19, 9); putch(195);
	char *p;
	p = replicate (" ",6);
	outtxt(p,20,8,CYAN * 16);
	char message[5] = "Data";
	outtxt(message,21,8,BLACK + CYAN * 16); 



}
//------------------------------------------------------------------
void DrawResultBoard(){
	Grid(1,1,4,1,30,7,CYAN);
	Grid(1,1,4,11,30,7,CYAN);//5kolona-1vrsta-2siri-1duzina-5x-5y
	gotoxy(35, 9); putch(180);//dodamo linije na tabelu
	gotoxy(30, 9); putch(195);
	char *p;
	p = replicate (" ",4);
	outtxt(p,31,8,CYAN * 16);
	char message[3] = "nm";
	outtxt(message,32,8,BLACK + CYAN * 16); 
	
}
//------------------------------------------------------------------
void EnterName(){
	int i, n, k, key, x[12], y[12];
	char *name;
	name = replicate(" ",14);
	while(1){
		key = LineEdit(name,58,7,WHITE + 16 * MAGENTA);
		k = i;
		switch (key) {
			case ESC: return;
			case CR: 
				gotoxy(1,24); 
				outtxt(name,58,7,BROWN + MAGENTA * 16);
				strcpy (player, name);
				return;
			/*case CR:
				i = i + 1;
				break;*/
			case DOWN:
				if (i == n - 1) i = 0; else i = i + 1;
				break;
			case UP:
				if (!i) i = n - 1; else i = i - 1;
				break;
			case PAGEDOWN:
				i = n;
				break;
			case CTRLHOME:
				i = 0;
				break;
			case CTRLEND:
				i = n - 1;
				break;
		}
		if (i == k) continue;
		outtxt(name,58,7,BROWN + MAGENTA * 16);//nakon unosa vrednosti za ime
		if (i != n) continue;
		Menu (MAGENTA, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,
		"New", "Ignore", "Exit", "");
		i = 0;
	}
}
//------------------------------------------------------------------
void ChooseAction(int action){

	if(action==0){
		strcpy(score,"Score");
		PasteScore("Score",6,8,2); // stampa score
		numberOfGames = 0;
		NewRound();
		gotoxy(58,7);
	} else if(action==1){

	} else if(action==2){
		exit(1);
	}

}
//------------------------------------------------------------------
void CheckScoreBoard(){
	//proveri da li je neki rekord oboren
	int sum = 0;
	int i,j;
	for (i=0;i<5;i++){
		sum = sum + (score[i] - 48);
	}
	char sc[5];
	itoa(sum,sc,10);
	for(i=10;i<20;i++){
		if(strcmp(sc,names[i])>0){
			for(j=8;j>=(i-10);j--){
				strcpy(names[j+1]," ");
				strcpy(names[j+1],names[j]);
			}
			for(j=18;j>=i;j--){
				strcpy(names[j+1]," ");
				strcpy(names[j+1],names[j]);
			}
			if(player[14]=='\0' && player[2]==' ') { //ako je prazan strin od 14 karatkera,tako mora, onda stavi unk
				strcpy(names[i-10],"Unknown"); 
				names[i-10][7]='\n';
			}
			//if(len <= 2) strcpy(names[i-10],"Unknown");
			else {
				int len;
				len = strlen(player);
				player[len-1] = '\0';
				//player[len] = '\0';
				strcpy(names[i-10]," ");
				strcpy(names[i-10],player);
			}
			strcpy(names[i]," ");
			strcpy(names[i],sc);
			//names[i-10][strlen(names[i-10])]='\n';
			names[i][strlen(names[i])]='\n';

			//strcpy(names[i-10],player);
			break;
		}
	}
	int y;
	y = 9;
	for(i=0;i<10;i++){
		outtxt("            ",40,y,WHITE + MAGENTA * 16);
		outtxt(names[i],40,y,WHITE + MAGENTA * 16);
		y++;
	}
	y = 9;
	for(i=10;i<20;i++){
		outtxt(names[i],72,y,BLUE + LIGHTGRAY * 16);
		y++;
	}
	
	WriteScore();

}
//------------------------------------------------------------------
void WriteScore(){
	FILE *top;
	int i;
	top = fopen("Mind.txt","w");
	for(i=0;i<20;i++){
		fprintf(top, "%s", names[i]);
	}
	fclose(top);
}
//------------------------------------------------------------------
void MaskEdit(char *First, int Length[], int color, int *randomNumber)
{
	if(numberOfGames == 0)
		EnterName();
	int i, n, k, key, x[12], y[12];
	char *Items[9];//9 pokusaja 
	
	x[0] = strlen(First) + 19;//ovde se povecava razlika izmedju polja za unos i rednog broja
	for (i = 0; i < 12; i++) {
	if (!Length[i]) break;
		Items[i] = replicate (" ",Length[i]);
		if (x[0] + Length[i] > 80) return;
		x[i] = x[0]; y[i] = i + 10; //ovde se pomera visina
	}
	if (!i) return;
	n = i; i = 0;
	while (1) {
		key = LineEdit(Items[i],x[i],y[i],color);
		k = i;
		switch (key) {
			case ESC: gotoxy(1,24); return;
			case CR:
				i = i + 1;
				break;
			case DOWN:
				if (i == n - 1) i = 0; else i = i + 1;
				break;
			case UP:
				if (!i) i = n - 1; else i = i - 1;
				break;
			case PAGEDOWN:
				i = n;
				break;
			case CTRLHOME:
				i = 0;
				break;
			case CTRLEND:
				i = n - 1;
				break;
		}
		if (i == k) continue;
		CheckRepeat(/*Items[k]*/);
		if(CheckInput(Items[k],randomNumber,x[k],y[k],i)){
			//nakon svakog unosa proveri unete brojeve
				if(numberOfGames == 5){
					i = n;
					CheckScoreBoard();
				} else {
					NewRound();
				}
		}
		outtxt(Items[k],x[k],y[k],BROWN);//nakon entera vrednosti
		if (i != n) continue; //ako se ovo ne desi on ce da ode na new
		Menu (MAGENTA, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,"New", "Ignore", "Exit", ""); // ponovo nacrta meni i fokusira ga
		//i = 0;
	}
}
//------------------------------------------------------------------
bool CheckInput(char *numbers, int *randomNumber,int x, int y,int moves){
	int onPlace = 0;
	int inNumber = 0;
	char onP[1]; 
	char inN[1];
	int i,j,br=1000;
	int randomN = 0;
	for (i=0;i<4;i++){
		randomN = randomN + randomNumber[i]*br;
		br = br/10;
	}

	char r[4];
	itoa(randomN,r,10);
	for (i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(r[i]==numbers[j]){
				inNumber++;
			}
		}
	}
	for(i=0;i<4;i++){
		if(r[i]==numbers[i]){
			onPlace++;
		}
	}
	itoa(inNumber,inN,10);
	itoa(onPlace,onP,10);
	outtxt(inN,x+11,y,BROWN);
	outtxt(onP,x+12,y,BROWN);
	outtxt(r,58,2,RED * 16);
	//outtxt(numbers,58,3,RED * 16);
	if(inN[0] >= '4' && onP[0] >= '4'){
		int rez;
		rez = 10 - moves;
		score[numberOfGames] = rez + 48;
		numberOfGames++;
		PasteScore(score,6,8,2);
		return true;
	}
	return false;
}
bool CheckRepeat(){
	return false;
}
//------------------------------------------------------------------
void Menu(int color, ...)
{
	int i, n, NewX, NewY, len, Data[7], CoorX[64], CoorY[64];
	char *p, *Items[64], FirstLetter[64];
	va_list ap;
	va_start (ap,color);
	Data[0] = color;
	for (i = 1; i < 7; i++) Data[i] = va_arg(ap, int);
	if (Data[4] + 2 > 25 || Data[3] + Data[5] >= 80) {
		va_end (ap);
		return;
	}
	Items[0] = va_arg(ap, char*); len = strlen(Items[0]);
	n = 1;
	while(len) {
		Items[n] = va_arg(ap, char*);
		len = strlen(Items[n++]);
	}
	va_end (ap); if (!--n) return;
	for (i = 0; i < n; i++) {
		p = alltrim(Items[i]); len = strlen(p);
		FirstLetter[i] = p[0];
		if (len > Data[5] - 2) Data[5] = len + 2;
		delete [] p;
	}
	NewX = Data[3] + 1; NewY = Data[4] + 1;
	for (i = 0; i < n; i++) {
		Items[i] = Line(Items[i],Data[5],1);
		CoorX[i] = NewX; CoorY[i] = NewY;
		if (Data[6]) {
			NewY = NewY + 3;
			if (NewY > 24) {
				NewX = NewX + Data[5] + 3; NewY = Data[4] + 1;
				if (NewX + Data[5] > 80) return;
			}
			continue;
		}
		NewX = NewX + Data[5] + 3;
		if (NewX + Data[5] <= 80 || i == n - 1) continue;
		NewX = Data[3] + 1; NewY = NewY + 3;
		if (NewY > 24) return;
	}
	ScrollMenu(Data,FirstLetter,Items,CoorX,CoorY,n);
	for (i = 0; i < n; i++) delete [] Items[i];
}
//------------------------------------------------------------------
int getkey() {
	int c, key;
	key = (c = getch()) > 0 ? c : 256 + getch();
	return key;
}
void Grid(int m, int n, int w, int h, int x, int y, int color)
{
	int i, j, k, newx, newy, Char, FlagDefault;
	FlagDefault = 0;
	textattr(color);
	if (m < 1 || n < 1 || w < 1 || h < 1) FlagDefault = 1;
	if (x < 1 || y < 1) FlagDefault = 1;
	if (x + m * (w + 1) > 80) FlagDefault = 1;
	if (y + n * (h + 1) > 25) FlagDefault = 1;
	if (FlagDefault) {m = 26; n = 12; w = 2; h = 1; x = 1; y = 1;}
	for (i = 0; i <= n; i++) {
		newy = y + i * (h + 1);
		for (j = 0; j <= m; j++) {
			newx = x + j * (w + 1);
			while(1) {
				if (i == 0 && j == 0) {Char = 218; break;}
				if (i == 0 && j == m) {Char = 191; break;}
				if (i == n && j == 0) {Char = 192; break;}
				if (i == n && j == m) {Char = 217; break;}
				if (i != 0 && j == 0) {Char = 195; break;}
				if (i != 0 && j == m) {Char = 180; break;}
				if (i == n) {Char = 193; break;}
				if (i == 0) {Char = 194; break;}
				Char = 197;
				break;
			}
			gotoxy(newx,newy); putch(Char);
			if (j == m) break;
			for (k = 1; k <= w; k++) {
				gotoxy(newx + k, newy); putch(196);
			}
		}
		if (i == n) break;
		for (j = 0; j < h; j++) {
			newx = x; newy = newy + 1;
			for (k = 0; k <= m; k++) {
				gotoxy(newx + k * (w + 1),newy); putch(179);
			}
		}
	}
	gotoxy(x,y);
}
//------------------------------------------------------------------