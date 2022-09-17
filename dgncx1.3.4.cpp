/*  
	TODO1 im:*** difficulty:***** account(.in?->https://www.luogu.com.cn/blog/z1e2k3i4/sang-shi-wei-ji
    TODO2 im:** difficulty:**** bizzard and Z(jin_shu_dao_dian)
    TODO3 im:**** difficulty:***** (look TODO3)sheng_ming_zhi fang_yu_li(attack->gong_ji_li)
	TODO4 im:** difficulty:****** windows or mac?
*/
#include <bits/stdc++.h>
#include <unistd.h>
#include <windows.h>
#include <random>
#include <iostream>
#include <conio.h>
#include <string.h>

using namespace std;

void printt(int wei,int hei);
bool pdwin(int wei,int hei);
void gameorplotorpic();
void plot();
void cdstory();
void cluestory();

int coin1, coin2, coin3;
int getcoin;
int coincnt = 0;
int coinxy[1000];
int gamecnt=0;
unsigned int cccoin;

bool clickflag = 0;
bool coincolor = false;
bool juqingcd1=0,juqingcd2=0,juqingcd3=0,juqingcd4=0;
bool picflag[1000][1000] = {0};
bool gameone = 0;

char role = '$';
char clouds = '$';
char pic[1000][1000];
char readd;

string s = "";

void SetColorAndBackground(int ForgC, int BackC){
	WORD wColor = ((BackC&0x0f) << 4) + (ForgC&0x0f);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}

void rgb_init(){
	HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwInMode,dwOutMode;
	GetConsoleMode(hIn,&dwInMode);
	GetConsoleMode(hOut,&dwOutMode);
	dwInMode |= 0x0200;
	dwOutMode |= 0x0004;
	SetConsoleMode(hIn,dwInMode);
	SetConsoleMode(hOut,dwOutMode);
}

void rgb_set(int wr,int wg,int wb,int br,int bg,int bb){
	printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",wr,wg,wb,br,bg,bb);
}

void slowprint(string slow){
	for(int i=0;i<slow.length();i++){
		cout<<slow[i];
		Sleep(85);
	}
}

void yan_hua(){
	rgb_init();
	srand((unsigned)time(NULL));
	int cd4_rd1;
	int cd4_rd2;
	int cd4_rd3;
	for(int k=0;k<100;k++){
		system("cls");
		for(int i=0;i<29;i++){
			for(int j=0;j<60;j++){
				cd4_rd1=rand()%256;
				cd4_rd2=rand()%256;
				cd4_rd3=rand()%256;
				rgb_set(255,255,255,cd4_rd1,cd4_rd2,cd4_rd3);
				cout<<"  ";	
			}
			cout<<"\n";
		}
		Sleep(50);
	}
	rgb_set(255,255,255,0,0,0);
}

bool drawcoin_pd(int a){
	for (int i =1;i <= cccoin; i++){
		if (a == coinxy[i]) return true;
	}
	return false;
}

bool rdcoin_pd(int a){
	for (int i = 1; i < a; i++){
		if (coinxy[i] == coinxy[a]) return true;
	}
	return false;
}

void rdcoin(int cccoin, int wei, int hei){
	srand((unsigned)time(NULL));
	for(int i = 1; i <= cccoin; i++){
		rdcoina:
		coinxy[i] = 1 + rand() % (wei*hei);
		if ((coinxy[i] == 1)||(rdcoin_pd(i))) goto rdcoina;
	}
}

void qkhcq(){
	cin.clear(); 
}

void printt(int wei,int hei){
	int printcnt = 0;
	SetColorAndBackground(15, 0);//6,0->deep yellow  14,0->not deep yellow(coins' color) 
	system("cls");
	for (int i = 0; i <= hei+1; i++){
		for (int j = 0; j <= wei+3; j++){
			if (pic[i][j] == '$'){
				if (coincolor){
					SetColorAndBackground(6, 0);
					coincolor = false;
				} else {
				SetColorAndBackground(14, 0);
				coincolor = true;
				}
				cout<<'$';
				SetColorAndBackground(15, 0);
				continue;
			}
			cout << pic[i][j];
		}
		if (i == 1 && getcoin == 1){
			SetColorAndBackground(6, 0);
			cout << "        you get a coin!";
			SetColorAndBackground(15, 0);
		}
		if (i == 2 && getcoin == 1){
			cout << "        you still need to get " << cccoin - coincnt << " coin";
			if (cccoin - coincnt > 1) cout << 's';
		}
		cout << '\n';
	}
	cout << "\nyou are " << role << "\n" << clouds << "s are the cells that you haven't been there yet\n'$' are coins\nyou need to control " << role << " by wsad\nyour goal is to collect all the coins as fast as possible\nand then clear all the clouds,the fewer steps the better\n";
}

bool pdwin(int wei,int hei){
	int cntflag = 0;
	for (int i = 1; i < hei + 1; i++){
		for (int j = 1; j < wei + 1; j++){
			if (picflag[i][j]) cntflag++;
		}
	} 
	if (cntflag == hei * wei){
		system("cls");
		cout << "you win!!!\n";
		return 1;	
	}
	return 0;
}

void welcome(){                           
	cout<<"\
	\n\
###  #######  ###          #########            ###   \n\
###  ### ###  ###          ###                  ###   \n\
###  ### ###  ###          #########            ###   \n\
### ###   ### ###          #########            ###   \n\
### ###   ### ###          ###                  ###   \n\
 #####     #####           ###                  #########\n\
  ###       ###            #########            #########\n\n\n\n\
";
	cout<<"\
    #####                   #####                  ###        ###             ###########\n\
  #########               #########               #####      #####            ###########\n\
##############          ##############           ### ###    ### ###           ###\n\
###        ###          ###        ###          ###   ###  ###   ###          ###\n\
###        ##           ###        ###         ###    ###  ###    ###         ###########\n\
###                     ###        ###         ###    ###  ###    ###         ###########\n\
###        ##           ###        ###         ###     ######     ###         ###\n\
###        ###          ###        ###         ###       ##       ###         ###\n\
##############          ##############         ###                ###         ###\n\
  #########               #########            ###                ###         ###########\n\
    #####                   #####              ###                ###         ###########\n\
	";
	sleep(1);
	system("cls");
	cout<<"\
	\n\
##############                ######    \n\
##############              ##########  \n\
     ####                 ##############\n\
     ####                 ###        ###\n\
     ####                 ###        ###\n\
     ####                 ###        ###\n\
     ####                 ##############\n\
     ####                   ##########  \n\
     ####                     ######    \n\
	";
	sleep(1);
	system("cls");
	cout << "\
__      _                      _      ____ _                      __\n\
| |    (_)                    | |    |  _ \ (_)                    | |\n\
| |     _ __________ _ _ __ __| |    | |_) |_ __________ _ _ __ __| |\n\
| |    | |_  /_  / _` | '__/ _` |    |  _ <| |_  /_  / _` | '__/ _` |\n\
| |____| |/ / / / (_| | | | (_| |    | |_) | |/ / / / (_| | | | (_| |\n\
|______|_/___/___\__,__|_|  \__,__|    |____/|_/___/___\_,___|_|  \__,__|\n\n\n\
";
	cout<<"\
                 ###\n\
                ###\n\
               ###\n\
              ###\n\
             ###\n\
            ##############\n\
            ##############\n\
                      ###\n\
                     ###\n\
                    ###\n\
                   ###\n\
                  ###\n\
	";
	sleep(1);
	system("cls");
}

void original_story(){
	string storyin ="More than a hundred years ago, \n\
a group of exquisite and lovely lizards lived on the miraculous land of Kashata. \n\
You are one of them. \n\
You have been living here safely for years. \n\
No one is bothering you. \n\
But suddenly one day, \n\
a dazzling yellow light came down from the air. \n\
After the yellow light goes out, \n\
you find yourself trapped in a small room. \n\
It's written on the wall: \n\
    p..play the game...me an..and f...fi..nd..d all th..the clues..s \n\
    t..to escap..ppe heee..re bit... by b..bit.";
	slowprint(storyin);
	sleep(2);
	system("cls");
	readd=getch();
	plot();
}

void storychoice(){
	cout<<"Read the story or not?(y or n)\n";
	char story_choice;
	cin_story_choice:
	qkhcq();
	cin>>story_choice;
	if(story_choice=='y'){
		original_story();
	}else if(story_choice=='n'){
		cout<<"OK.\n\n";
	}else{
		cout<<"wrong input!!!\n\n";
		goto cin_story_choice;
	}
}

void game(){
	int wronglettercnt = 0;
	clock_t start, end;
	clock_t start2, end2;
	start2 = clock();
	char tmp;
	int x = 1, y = 1;
	int clickcnt = 0; 
	cin_wh: 
	cout << "height?	";
	unsigned int hei;
	cin >> hei;
	cout << "width?	";
	unsigned int wei;
	cin >> wei;
	cout<<"how many coins?	";
	cin >> cccoin;
	if (cccoin > hei*wei-1){
		cout << "impossible!!!";
		goto cin_wh;
	}
	system("cls");
	rdcoin(cccoin, wei, hei);
	for (int i = 1; i <= hei; i++){//all space
		for (int j = 1; j <= wei; j++){
			pic[i][j] = ' ';
		}
	}
	for (int i = 0; i <= wei+1; i += (wei+1)){
		for (int j = 0;j <= hei+1; j++){//y
			pic[j][i] = '|';
		}
	}
	for (int i = 0;i <= hei+1; i+=(hei+1)){//square frame
		for (int j = 1; j <= wei; j++){
			pic[i][j] = '-';//x 
		}
	}
	int drawcoincnt = 0;
	int cdcnt1 = 0;
	while ((clouds == role) || (clouds == '$') || (role == '$')){
		cout << "select a role(not'$'):";
		cin >> role;
		qkhcq();
		cout << "select a symbol for clouds(not'$'):";
		clouds = getch();
		qkhcq();
		system("cls");
		cdcnt1++;
	}
	for (int i = 1; i <= hei; i++){
		for (int j = 1; j <= wei; j++){
			drawcoincnt++;
			if (drawcoin_pd(drawcoincnt)){
				pic[i][j] = '$';
				continue;
			}
			pic[i][j] = clouds;//cloud 
		}
	}
	picflag[1][1] = 1; 
	pic[1][1] = role;
	printt(wei,hei);
	start = clock();
	while (1){
		getcoin = 0;
		char cinn = getch();
		if (clickflag) {
			clickcnt++;
		}
		if (cinn == 'w' && x != 1){
			qkhcq();
			if (!picflag[x - 1][y]){//up
				if (pic[x - 1][y] == '$'){
					getcoin = 1;
					coincnt++;
					if (coincnt == cccoin){
						clickflag = 1;
						end = clock();
					}
				}
				picflag[x - 1][y] = 1; 
				pic[x - 1][y] = ' ';
				if (pdwin(wei,hei)){
					goto winn; 
				}
			}
			tmp = pic[x][y];
			pic[x][y] = pic[x - 1][y];
			pic[x - 1][y] = tmp;
			x--;
		} else if (cinn == 'a' && y != 1){
			qkhcq();
			if (!picflag[x][y - 1]){//left
				if (pic[x][y - 1] == '$'){
					getcoin = 1;
					coincnt++;
					if (coincnt == 3){
						clickflag = 1;
						end = clock();
					}
				}
				picflag[x][y - 1] = 1;
				pic[x][y - 1] = ' ';
				if (pdwin(wei,hei)){
					goto winn; 
				}
			}
			tmp = pic[x][y];
			pic[x][y] = pic[x][y-1];
			pic[x][y-1] = tmp;
			y--;
		} else if (cinn == 's' && x != hei){
			qkhcq();
			if (!picflag[x+1][y]){
				if (pic[x+1][y] == '$'){
					getcoin = 1;
					coincnt++;
					if (coincnt == 3){
						clickflag = 1;
						end = clock();
					}
				}
				picflag[x+1][y] = 1;
				pic[x+1][y] = ' ';//down
				if (pdwin(wei,hei)){
					goto winn; 
				}
			}
			tmp = pic[x][y];
			pic[x][y] = pic[x+1][y];
			pic[x+1][y] = tmp;
			x++;
		} else if (cinn == 'd' && y != wei){
			qkhcq();
			if (!picflag[x][y+1]){
				if (pic[x][y+1] == '$'){
					getcoin = 1;
					coincnt++;
					if (coincnt == 3){
						clickflag = 1;
						end = clock();
					}
				}
				picflag[x][y+1] = 1;
				pic[x][y+1] = ' ';//right
				if (pdwin(wei,hei)){
					goto winn;
				}
			}
			tmp = pic[x][y];
			pic[x][y] = pic[x][y+1];
			pic[x][y+1] = tmp;
			y++;
		}else{
			wronglettercnt++;
		}
		printt(wei, hei);
	}
	winn:
	gamecnt+=wei*hei;
	if ((clickcnt / 3 + (end - start) / CLK_TCK / 2) > 35) cout << "you got 100 score!!!\n";
	else if ((clickcnt / 3 + (end - start) / CLK_TCK / 2 ) >= 135) cout << "you got no score!!!\n";
	else cout << "you got " << 135 - (clickcnt / 3 + (end - start) / CLK_TCK / 2) << " score!!!\n";
	if(!gameone) gameone=1;
	sleep(2);
	if (cdcnt1 > 10){
		system("cls");
		sleep(1);
		cout << "you get the colorful egg nub1!\n";
		juqingcd1=true;
	}
	if (clouds == ' '){
		sleep(2);
		cout << "you get the colorful egg nub2!!\n";
		sleep(2);
		juqingcd2=true;
	}
	if (wronglettercnt >= 100){
		sleep(3);
		cout << "you get the colorful egg nub3!!!\n";
		juqingcd3=true;
		qkhcq();
		cin >> s;	
	}
	if (s == "!!!!"){
		char cd4hint[100]="but there is something different...\nthe game isn't over yet...*\n";
		end2 = clock();
		cout << "--------------------------------\nProcess exited after " << fixed << setprecision(2) << (double)(end2 - start2) / CLK_TCK << " seconds with return value 0\n请按任意键继续. . .\n";
		sleep(1);
		for(int i=0;i<100;i++){
			if(cd4hint[i]!='*'){
				cout<<cd4hint[i];
				Sleep(100);
			}else{
				break;
			}
		}
		sleep(4);
		system("cls");
		SetColorAndBackground(2, 14);
		cout << "\nWOW!!!!YOU GET THE COLORFUL EGG NUB4!!!!\n";
		juqingcd4=true;
		Sleep(2000);
		yan_hua();
	}
	gameorplotorpic();
}

void allstory(){
	original_story();
	cluestory();
	cdstory();	
	readd = getch();
	plot();
}

void cdstory(){
	string storycd1 ="thousands of years ago, \n\
there was a big nuclear explosion occur on Kashata. \n\
all the animals and plants mutate a little. \n\
some died and some become huge or tiny,etc. \n\
all the lizards included your ancestors became very cute. \n\
but, one plant became extremely dangerous...";

	string storycd2 ="there was a person called Medozig,\n\
he was the king of the Kashata before the big nuclear explosion.\n\
but after the explotion,\n\
everything mutated and he wasn't the king at all.\n\
no one listened to him.\n\
but he still wanted everyone listen to him,\n\
so, he...\n";

	string storycd3 ="Medozig found that everyone are now listen to a plant,\n\
because it's dangerous.\n\
it can kill anyone who doesn't listen to it.\n\
Medozig doesn't believe that a plant can be so powerful,\n\
so he decide to eliminate that plant.\n\
first, he asked others and knew that the plant was a Aesculus called Aquamerald.\n\
After mutated it became magical.\n\
it can throw leaves,  Aquamarine and Emerald \n\
that can make you be injured, sick and be poisoning(and listen to it).\n\
because of this, many animals and plants listen to it,\n\
and Medozig was going to destroy it...";

	string storycd4 ="Medozig found the Aesculus Aquamerald to destroy it.\n\
they fought with each other over several days and nights.\n\
finally in one day,\n\
Medozig had obvious advantages.\n\
he was thinking he was going to win,\n\
when Aquamerald suddenly said,\n\
if I can't defeat you,\n\
I'll blew myself up.\n\
then with a piercing explosion,\n\
Aquamerald exploded...";

	string storycd5 ="everyone in the Kashata land all affected by the explosion,\n\
some got sick and some even dead.\n\
but the lizards were very lucky.\n\
after all species sicked or bacome extinct,\n\
they were still alive.\n\
but, the explosion had a great impact on Kashata,\n\
and the dazzling yellow light would light up every other year...";
	if(juqingcd1){
		slowprint(storycd1);
		cout<<"\n";
		cout<<"\n";
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	sleep(1);
	if(juqingcd2){
		slowprint(storycd2);
		cout<<"\n";
		cout<<"\n";
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	sleep(1);
	if(juqingcd3){
		slowprint(storycd3);
		cout<<"\n";
		cout<<"\n";
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	sleep(1);
	if(juqingcd4){
		slowprint(storycd4);
		cout<<"\n";
		cout<<"\n";
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	sleep(1);
	if((juqingcd1) && (juqingcd2)&& (juqingcd3)&& (juqingcd4)){
		slowprint(storycd5);
		cout<<"\n";
		cout<<"\n";
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	readd = getch();
	plot();
}

void cluestory(){
	string cluestory1="a hint:\n\
	play more games to get more plots.\n\
";
	string cluestory2="you need to find a way to kill 'Medozig',\n\
so that 'Aquamerald' could calm down.\n\
";
	string cluestory3="but, you can't kill Medozig,\n\
because he was too powerful.\n\
";
	string cluestory4="so,you need to make Aquamerald believe\n\
that Medozig was die...\n\
";
	if(gamecnt>0){
		slowprint(cluestory1);
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	if(gamecnt>100){
		slowprint(cluestory2);
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	if(gamecnt>1000){
		slowprint(cluestory3);
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	if(gamecnt>1e7){
		slowprint(cluestory4);
	}else{
		cout<<"\n\n      ?      ?      ?      \n\n";
	}
	readd=getch();
	plot();
}

void plot(){
	cin_plotchoi:
	system("cls");
	cout<<"1. oringinal story\n2. story clues\n3. bonus story\n4. all the story\n5. quit\n\n";
	int plotchoi;
	string plot3pw;
	string plot4pw;
	cin>>plotchoi;
	switch (plotchoi) {
		case(1):
			original_story();
			break;
		case(2):
			if(gameone){
				cout<<"bonus story's read password is Erybcae";
				sleep(1);
				system("cls");
				cluestory();
			}else{
				cout<<"please play a game first!";
				readd = getch();
				goto cin_plotchoi;
			}
			break;
		case(3):
			cout<<"read password?\n";
			cin>>plot3pw;
			if(plot3pw=="Erybcae"){
				cout<<"all story's read password is dbGirehjBV";
				sleep(1);
				system("cls");
				cdstory();
			}else{
				cout<<"wrong password!";
				goto cin_plotchoi;
			}
			break;
		case(4):
			cout<<"read password?\n";	
			cin>>plot4pw;
			if(plot4pw=="dbGirehjBV"){
				allstory();
			}else{
				cout<<"wrong password!";
				goto cin_plotchoi;
			}
			break;
		case(5):
			gameorplotorpic();
			break;
		default:
			cout<<"wrong input!";
			qkhcq();
			goto cin_plotchoi;
	}
}
void lizpic(){
	picstart:
	cout<<"i have 4 pictures,\nplease choose one(nub1~4)(quit -> 5):\n";
	int picchoose;
	cinpicchoose:
	cin>>picchoose;
	cout<<"please wait for a minute.\n";
	sleep(1);
	switch(picchoose){
		case(1):
			system("start https://ts1.cn.mm.bing.net/th/id/R-C.cd9fe0f9de158cc712b249cf6045241d?rik=vjELD9Bk9T%2fLFg&riu=http%3a%2f%2fdpic.tiankong.com%2flh%2fdz%2fQJ6645258546.jpg&ehk=ag2yS04w2iPaL6OhOALnjLoT%2b1UbcNHio5fTRq8%2bZY4%3d&risl=&pid=ImgRaw&r=0");
			system("cls");
			readd=getch();
			break;
		case(2):
			system("start https://ts1.cn.mm.bing.net/th/id/R-C.b12049e51ed103f4631616726f8faa49?rik=wbttnezQBFdJVA&riu=http%3a%2f%2fdpic.tiankong.com%2fxv%2f0i%2fQJ8487519038.jpg&ehk=EHAJuNXRbwgOQOCKsqoJO7M%2bYVAa9TLhTSd59Y5eOUY%3d&risl=&pid=ImgRaw&r=0/");
			system("cls");
			readd=getch();
			break;
		case(3):
			system("start https://ts1.cn.mm.bing.net/th/id/R-C.3c5f87b476ea0d0805e2e0468be7b58e?rik=oaDdS1W4H2M%2bBQ&riu=http%3a%2f%2fpic2.58.com%2fzp_images%2fallimg%2f121016%2f5_121016152703_1.png&ehk=uq6tskorR1kcuGr%2fHWqHsHUa%2bb%2b3D20mtpqRN7jW83E%3d&risl=&pid=ImgRaw&r=0&sres=1&sresct=1/");
			system("cls");
			readd=getch();
			break;
		case(4):
			system("start https://tse4-mm.cn.bing.net/th/id/OIP-C.2aMuUCDPxOVvat6aRDZifgHaE6?pid=ImgDet&rs=1/");
			system("cls");
			readd=getch();
			break;
		case(5):
			cout<<"ok.";
			gameorplotorpic();
		default:
			cout<<"wrong input!!!";
			goto cinpicchoose;
	}
	goto picstart;
}
void gameorplotorpic(){
	system("cls");
	SetColorAndBackground(15, 0);	
	cout<<"1. play the game\n2. check the plot\n3. see some pictures\n";
	int g_or_p_choi;
	cin_g_or_p_choi:
	qkhcq();
	cin>>g_or_p_choi;
	switch(g_or_p_choi){
		case(1):
			game();
			break;
		case(2):
			plot();
			break;
		case(3):
			lizpic();
			break; 
		default:
			cout<<"wrong input!!!";
			qkhcq();
			goto cin_g_or_p_choi;
	}
}

int main(){
	welcome();
	storychoice();
	gameorplotorpic();
	return 0;
}
