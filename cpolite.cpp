#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#define PLAYERCOUNT 3
#define CARDSCOUNT 54
#define CURRENTPLAYER 0
#define VALUECOUNT 17
#define ERROR -1
istream I = cin
ostream O = cout
using namespace std;
	int n_puzzle_3[3][3] = {1,2,3,4,5,6,7,8,0};
	int n_puzzle[3][3] = {1,3,8,7,0,2,5,6,4};
	void outArr(){
		for(int i=0;i<3;++i)
		{
			for(int j=0;j<3;++j){
				if(n_puzzle[i][j] != 0){
					O << n_puzzle[i][j] << " ";
				}
				else{
				O <<"  ";
				}
			}
			O<<endl;			
		}	
	}
	void change(int *one,int *two){
		int temp;
		temp = *one;
		*one = *two;
		*two = temp;
	} 
int scnt=0;
const char toFigure[]="34567890JQKA 2YZ";
enum COLOR{  //��ɫ��ʾASCII: 3~6
eHEART=3,//���� 
eDIAMOND,//��Ƭ 
eCLUB,   //�ݻ� 
eSPADE   //���� 
};
class Card;
class CardsType;
class CardGroup;
class Player;
class Landlords;
class LastCards;
bool makeChoice(string tip);
bool cmp(Card* a,Card* b);
class Card{
public:
char figure;
COLOR color;
int value;
Card(char _figure,COLOR _color){
    figure=_figure;
    color=_color;
    value=calValue();
}
int calValue(){
    for(int i=0;toFigure[i];i++){
        if(toFigure[i]==figure){
            return i;
        }
    }
    return ERROR;
}
void print(){
    if(figure=='Z'){
        O<<"Z";
    }else if(figure=='Y'){
        O<<"Y";
    }else{
        O<<figure<<(char)color;
    }
    O<<' ';
}
};
class CardsType{
public:
//Ϊ�˹淶���Ҷ�Ӧ�Ƶķ���
//ͳһΪ3������cnt1��isContinuous��cnt2
int typeId;
string typeStr;
int cnt1,cnt2;
bool isContinuous;
CardsType(){
    typeId=ERROR;
}
bool operator ==(const CardsType& other)const{
    return this->typeId==other.typeId;
}
void init(char* _typeStr,int _typeId,int _cnt1,bool _isContinuous,int _cnt2){
    cnt1=_cnt1;
    isContinuous=_isContinuous;
    cnt2=_cnt2;
    typeStr=_typeStr;
    typeId=_typeId;
}
}; 
class CardGroup{
public:
vector<Card*> cards;
CardsType type;
void calType(){
    int i,n=cards.size();
    //init(typeStr,typeId,cnt1,isContinuous,cnt2)
    if(n==0){
        type.init("����",14,0,0,0);
        return;
    }
    if(n==2&&cards[0]->value==15&&cards[1]->value==14){
        type.init("��ը",0,0,0,0);
        return;
    }
    //ͳ��ͬ�������ж����� 
    int cntFlag[VALUECOUNT]={0};
    for(i=0;i<n;i++){
        cntFlag[cards[i]->value]++;
    }
    //ͳ�Ƶ����������ٵ��� 
    int maxCnt=0,minCnt=4;
    for(i=0;i<VALUECOUNT;i++){ 
        if(maxCnt<cntFlag[i]){
           maxCnt=cntFlag[i];
        }
        if(cntFlag[i]&&minCnt>cntFlag[i]){
           minCnt=cntFlag[i];
        }
    }
    if(n==4&&maxCnt==4){
        type.init("ը��",1,4,0,0);
        return;
    }
    if(n==1){
        type.init("����",2,1,0,0);
        return;
    }
    if(n==2&&maxCnt==2){
        type.init("����",3,2,0,0);
        return;
    }
    if(n==3&&maxCnt==3){
        type.init("���� ",4,3,0,0);
        return;
    }
    if(n==4&&maxCnt==3){
        type.init("����һ",5,3,0,1);
        return;
    }
    if(n==5&&maxCnt==3&&minCnt==2){
        type.init("����һ��",6,3,0,2);
        return;
    }
    if(n==6&&maxCnt==4){
        type.init("�Ĵ���",7,4,0,1);
        return;
    }
    if(n==8&&maxCnt==4&&minCnt==2){
        type.init("�Ĵ���",8,4,0,2);
        return;
    } 
    if(n>=5&&maxCnt==1&&cards[0]->value==cards[n-1]->value+n-1){
        type.init("˳��",9,1,1,0);
        return;
    }
    if(n>=6&&maxCnt==2&&minCnt==2&&cards[0]->value==cards[n-1]->value+n/2-1){
        type.init("����",10,2,1,0); 
        return;
    }
    int fjCnt;//ͳ�������Ҵ���3���ŵ��� 
    for(i=0;i<VALUECOUNT &&cntFlag[i]<3;i++);
    for(fjCnt=0;i<VALUECOUNT &&cntFlag[i]>=3;i++,fjCnt++);
    if(fjCnt>1){
        if(n==fjCnt*3)
            type.init("�ɻ�",11,3,1,0);
        else if(n==fjCnt*4)
            type.init("�ɻ�",12,3,1,1); 
        else if(n==fjCnt*5&&minCnt==2)
            type.init("�ɻ�",13,3,1,2); 
    }
}
void init(string inputStr, vector<Card*> &cardsHolded){
    this->cards.clear();
    //���� 
    if(inputStr=="N"){
        this->calType();
        return;
    }
    int i,j;
    //����Ϸ����ж� 
    for(i=0;i<inputStr.size();i++){
        bool find=false;
        for(j=0;toFigure[j];j++){ 
            if(inputStr[i]==toFigure[j]){
                find=true;
                break;
            }
        } 
        if(find==false){
            //�����ַ�����toFigure��
            return;
        }
    }
    //����������û����Щ�� 
    int visitFlag[20]={0};
    for(i=0;i<inputStr.size();i++){
        Card *find=NULL;
        for(j=0;j<cardsHolded.size();j++){
            if(!visitFlag[j]&&cardsHolded[j]->figure==inputStr[i]){
                visitFlag[j]=1;
                find=cardsHolded[j];
                break;
            }
        }
        if(find){
            this->cards.push_back(find);
        }else{
            O<<inputStr[i];
            O<<"û���ҵ�\t";
            this->cards.clear();
            return;
        }
    }//end for(i=0;i<inputStr.size();i++) 
    this->arrange();
}
void init(vector<Card*> newCards){
    this->cards=newCards;
    this->arrange();
}
bool isCanBeat(CardGroup &cardGroup){
    if(cardGroup.type.typeStr=="��ը"){
        return false;
    }else if(this->type.typeStr=="��ը"){
        return true;
    }else if(cardGroup.type==this->type &&this->type.typeStr=="ը��"){
        return value()>cardGroup.value();
    }else if(cardGroup.type.typeStr=="ը��"){
        return false;
    }else if(this->type.typeStr=="ը��"){
        return true;
    }else if(cardGroup.type==this->type &&this->cards.size()==cardGroup.cards.size()){
        return this->value()>cardGroup.value();
    }else{
        return false;
    }
}
int value(){
    //��������Ȩֵ 
    int i;
    if(type.typeStr=="����һ"||type.typeStr=="����һ��"||type.typeStr=="�ɻ�"){
        for(i=2;i<cards.size();i++){
            if(cards[i]->value==cards[i-2]->value){
                return cards[i]->value;
            }
        }
    }
    if(type.typeStr=="�Ĵ���"){
        for(i=3;i<cards.size();i++){
            if(cards[i]->value==cards[i-3]->value){
                return cards[i]->value;
            }
        }
    }
    return cards[0]->value;
}
void arrange(){
    //�������򡢼������� 
    sort(this->cards.begin(),this->cards.end(),cmp);
    this->calType();
}
};
class LastCards{
static LastCards *lastCards;
public:
Player *player;
CardGroup cardGroup;
static LastCards* inst(){//����ģʽ 
    if(lastCards==NULL){
        lastCards=new LastCards();
    }
    return lastCards;
}
vector<Card*> findCanBeatFrom(vector<Card*> &cardsHolded){
    //�����ܴ�ù����� 
    int i,j,k,n=cardsHolded.size(),m=cardGroup.cards.size();
    string typeStr=cardGroup.type.typeStr;
    vector<Card*> ret;
    if(typeStr=="��ը"||n<m){
        //�򲻹������ؿ����� 
        return ret;
    }
    int value=cardGroup.value();
    //ͳ�Ƹ����Ƴ��ֵĴ��� 
    int cntFlag[VALUECOUNT]={0};
    for(i=0;i<n;i++){ 
        cntFlag[cardsHolded[i]->value]++;
    } 
    int continuousCount=1;
    if(cardGroup.type.isContinuous){
        continuousCount=m/(cardGroup.type.cnt1+cardGroup.type.cnt2);
    }
    bool findFirstFigure;
    //cout<<"continuousCount="<<continuousCount<<endl;
    for(i=value+1;i<VALUECOUNT;i++){
        findFirstFigure=true;
        for(j=0;j<continuousCount;j++){ 
            if(cntFlag[i-j]<cardGroup.type.cnt1){
                findFirstFigure=false;
                break;
            }
        }
        if(findFirstFigure){
            ret.clear();
            int firstFigure=i;
            //cout<<"����"<<cardGroup.type.cnt1<<"��"<<firstFigure+3<<endl;
            for(k=0,j=0;k<cardsHolded.size() &&j<continuousCount;k++){ 
                if(cardsHolded[k]->value==firstFigure-j){
                    for(int kk=0;j>=0&&kk<cardGroup.type.cnt1;kk++){ 
                        ret.push_back(cardsHolded[k+kk]);
                    } 
                    j++; 
                }
            } 
            if(cardGroup.type.cnt2>0){
                int SecondFigures[5];
                int SecondCount=continuousCount;
                if(cardGroup.type.typeStr=="�Ĵ���")
                    SecondCount=2;
                bool findSecondFigure=true;
                for(j=0,k=-1;j<SecondCount &&findSecondFigure;j++){
                    findSecondFigure=false;
                    for(k++;k<VALUECOUNT;k++){
                        SecondFigures[j]=k;
                        if(cntFlag[k]>=cardGroup.type.cnt2 &&cntFlag[k]<cardGroup.type.cnt1){
                            findSecondFigure=true;
                            break;
                        }
                    }
                }
                if(findSecondFigure){
                    //cout<<"����SecondFigure "<<cardGroup.type.cnt2<<"��"<<SecondFigures[0]+3<<endl;
                    //cout<<"SecondCount= "<<SecondCount<<endl;
                    //for(i=0;i<SecondCount;i++)cout<<"SecondFigures["<<i<<"]="<<SecondFigures[i]<<endl;
                    for(i=0;i<SecondCount;i++){ 
                        for(j=0;j<cardsHolded.size();){ 
                            if(cardsHolded[j]->value==SecondFigures[i]){
                                for(k=0;k<cardGroup.type.cnt2;k++){
                                    //cout<<"���"<<cardsHolded[j]->value+3<<endl;
                                    ret.push_back(cardsHolded[j+k]);
                                }
                                do{
                                    j++;
                                }while(j<cardsHolded.size()&&cardsHolded[j]->value==SecondFigures[i]);
                            }else{
                                j++;
                            } 
                        }
                    } 
                    return ret;
                }//if(findSecondFigure) 
            }//end if(cardGroup.type.cnt2>0)
            else{
                return ret;
            }
        }//end if(findFirstFigure)
    }//end for(i=value+1;i<VALUECOUNT;i++)
    ret.clear();
    //û�ƴ�ù�ʱ������û��ը�� 
    if(typeStr!="ը��"){
        for(i=cardsHolded.size()-1;i>=3;i--){
            if(cardsHolded[i]->value==cardsHolded[i-3]->value){
                for(j=0;j<4;j++){
                    ret.push_back(cardsHolded[i-j]);
                }
                break;
            }
        }
    }
    return ret;   
}//end vector<Card*> findCanBeatFrom()
};
LastCards* LastCards::lastCards = NULL;
class Player{
public:
string name;
vector<Card*> cards;
void arrange(){
    sort(cards.begin(),cards.end(),cmp);
}
void print(){
    cout<<this->name<<":\t";
    for(int i=0;i<cards.size();i++){
        cards[i]->print();
    }
    cout<<"["<<cards.size()<<"]\n";
}
vector<Card*> tip(){
    //��ʾ����,ʹ�Լ���Сһ�����
    CardGroup ret;
    string temp;
    int j,k,m=cards.size();
    for(j=0;j<m;j++){
        temp="";
        for(k=j;k<m;k++){
            temp+=cards[k]->figure;
        }
        ret.init(temp,cards);
        if(ret.type.typeId!=ERROR){
            return ret.cards;
        }
    }
    ret.cards.clear();
    return ret.cards;
}
void chupai(CardGroup &cardGroup){
    //���� 
    cout<<this->name<<":\t";
    cout<<cardGroup.type.typeStr<<' ';
    for(int i=0;i<cardGroup.cards.size();i++){
        cardGroup.cards[i]->print();
        this->cards.erase(find(this->cards.begin(),this->cards.end(),cardGroup.cards[i])); 
    }
    cout<<"\t["<<this->cards.size()<<"]\n";
    if(cardGroup.type.typeStr!="����"){
        //��¼�� LastCards �� 
        LastCards::inst()->player=this;
        LastCards::inst()->cardGroup.init(cardGroup.cards);
    }
}
};
class Landlords{
Player *player[PLAYERCOUNT];
bool finished,youWin,landlordWin;
int landlordIndex;
Card *cards[CARDSCOUNT];
public:
Landlords(){
    int i,j,k;
    for(i=0;i<PLAYERCOUNT;i++){
        this->player[i]=new Player();
    }
    //54���Ƴ�ʼ�� 
    for(k=i=0;i<14;i++){
        if(toFigure[i]==' '){
            continue;
        }
        for(COLOR color=eHEART;color<=eSPADE;color=(COLOR)(color+1)){
            this->cards[k++]=new Card(toFigure[i],color);
        }
    }
    this->cards[k++]=new Card('Y',eSPADE);
    this->cards[k]=new Card('Z',eHEART);
}
~Landlords(){
    for(int i=0;i<PLAYERCOUNT;i++){
        delete this->player[i];
    }
    for(int i=0;i<CARDSCOUNT;i++){
        delete this->cards[i];
    }
} 
void init(){
    player[CURRENTPLAYER]->name="Bice";
    player[1]->name="���2";
    player[2]->name="���3";
    finished=false;
    youWin=false;
    landlordWin=false;
    //������
    landlordIndex=ERROR; 
    while(landlordIndex==ERROR){
        srand((int)time(0));
        shuffle();
        landlordIndex=chooseLandlord();
    }
    cout<<player[landlordIndex]->name<<"\t��Ϊ����\n\n";
    this->add3Cards();
    LastCards::inst()->player=player[landlordIndex];
}
void startGame(){
    string inputSrt;
    CardGroup inputCards;
    for(int iTurns=landlordIndex;!finished;iTurns++){
        if(iTurns>=PLAYERCOUNT){
            iTurns=0;
        }
        if(iTurns==CURRENTPLAYER){
            cout<<endl;
            player[iTurns]->print();
            cout<<"������ʾ��Z=���� Y=С�� 0=10 N=��Ҫ ��������� ����:JKQ0A9\n����ƣ�\t";
            do{
                cin>>inputSrt;
                inputCards.init(inputSrt,player[iTurns]->cards);
            }while(check(&inputCards)==false);
        }else{
            if(player[iTurns]==LastCards::inst()->player){
                //�����ϴγ��Ƶ����Լ���������ʾ���� 
                inputCards.init(player[iTurns]->tip());
            }else{
                //�����ܴ�ù��ϼҵ��� 
                inputCards.init(LastCards::inst()->findCanBeatFrom(player[iTurns]->cards));
            }
        }
        player[iTurns]->chupai(inputCards);//���� 
         if(player[iTurns]->cards.size()==0){
            //�������û���ˣ���Ϸ���� 
            finished=true;
            landlordWin=iTurns==landlordIndex;
            if(landlordWin){
                youWin=landlordIndex==CURRENTPLAYER;
            }else{
                youWin=landlordIndex!=CURRENTPLAYER;
            }
        }
    }
    O<<"\n_________________________ "<<(youWin?"You Win!":"You Lose!")<<" _________________________\n\n";
}
void add3Cards(){
    O<<"����3����:\t";
    for(int i=PLAYERCOUNT*17;i<CARDSCOUNT;i++){
        this->cards[i]->print();
        player[landlordIndex]->cards.push_back(cards[i]);
    }
    O<<endl;
    player[landlordIndex]->arrange();
}
int chooseLandlord(){
    O<<"\n_________________________ ������ _________________________\n\n";
    int first=-1,last,cnt=0,i,j=rand()%PLAYERCOUNT;
    bool decision;
    for(i=0;i<PLAYERCOUNT;i++,j==2?j=0:j++){
        if(j==CURRENTPLAYER){
            decision=makeChoice("�Ƿ���������(Y=��/N=����):");
        }else{ 
            decision=rand()%2;
        }
        if(decision){
            cnt++;
            last=j;
            if(first==-1){
                first=j;
            }
            O<<this->player[j]->name<<"\t������\n";
        }else{
            O<<this->player[j]->name<<"\tû����\n";
        }
    }
    if(cnt==0){
        O<<"û���������·���\n";
        return ERROR;
    }
    if(cnt==1){
        //��һ��ֻ��һ�������� 
        return first;
    }
    else{
        //���һ������ 
        if(first==CURRENTPLAYER){
            decision=makeChoice("�Ƿ���������(Y=��/N=����):");
        }else{
            decision=rand()%2;
        }
        if(decision){
            O<<this->player[first]->name<<"\t������\n";
            return first;
        }else{
            O<<this->player[first]->name<<"\tû����\n";
            return last;
        }
    }
}
void shuffle(){
    int i,j,k;    
    //ϴ�� 
    for(i=0;i<CARDSCOUNT;i++){
        swap(this->cards[i],this->cards[rand()%CARDSCOUNT]);
    }
     //���� 
    for(k=i=0;i<PLAYERCOUNT;i++){
        this->player[i]->cards.clear();
        for(j=0;j<17;j++){
            this->player[i]->cards.push_back(this->cards[k++]);
        }
        this->player[i]->arrange();//���� 
        this->player[i]->print();
    }
}
bool check(CardGroup *cardGroup){
    if(cardGroup->type.typeId==ERROR){
        cout<<"���ƴ�����������\n";
        return false;
    }else if(cardGroup->type.typeStr=="����"){
        return true;
    }else if(LastCards::inst()->player!=player[CURRENTPLAYER]&&!cardGroup->isCanBeat(LastCards::inst()->cardGroup)){
        cout<<"�򲻹�����������\n";
        return false;
    }else{
        return true;
    }
}
};
//=====================================================================0
void SetPos(COORD a)
{
	HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);
}
void SetPos(int i, int j)
{
	COORD pos={i, j};
	SetPos(pos);
}
void color(int a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}
void Pos()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}
void logo()
{
	color(12);
	printf("                   ��");
	color(10);
	printf("��\n");
	color(11);
	printf("                   ��");
	color(14);
	printf("��");
}
 
void logoing()
{
	color(12);
	printf("                   ��");
	_sleep(500);
	color(10);
	printf("��\n");
	_sleep(500);
	color(11);
	printf("                   ��");
	_sleep(500);
	color(14);
	printf("��");
	_sleep(500);
	system("Cls");
	
}
void lobby()
{
	system("cls");
	Pos();
	printf("\n");
	logo();
	SetPos(10,5);
	color(12);
	printf("��ӭʹ��");
	printf("�๦�ܳ���");
	SetPos(0,20);
	printf("-------------------------------------------\n");
	printf("       ��Ȩ���� Fj & OwenLMZ & a97");
	
	
	color(12);
	SetPos(1,7);
	printf("����������������������������������\n");
	printf(" ��               ��\n");
	printf(" ��  ������[a]    ��\n");
	printf(" ��               ��\n");
	printf(" ����������������������������������\n");
	
	color(10);
	SetPos(23,7);
	printf("����������������������������������\n");
	SetPos(23,8);
	printf("��               ��\n");
	SetPos(23,9);
	printf("�� ����С��Ϸ[s] ��\n");
	SetPos(23,10);
	printf("��               ��\n");
	SetPos(23,11);
	printf("����������������������������������\n");
	
	color(11);
	SetPos(1,13);
	printf("����������������������������������\n");
	printf(" ��               ��\n");
	printf(" ��  ȥ�����[d]  ��\n");
	printf(" ��               ��\n");
	printf(" ����������������������������������\n");
	
	color(14);
	SetPos(23,13);
	printf("����������������������������������\n");
	SetPos(23,14);
	printf("��               ��\n");
	SetPos(23,15);
	printf("��  ϵͳ����[f]  ��\n");
	SetPos(23,16);
	printf("��               ��\n");
	SetPos(23,17);
	printf("����������������������������������\n");
	
	Pos();
}
void calculator1()
{
	system("cls");
	printf("\n\n\n\n\n");
	logoing();
	system("cls");
	color(11);
	printf("|������|����1!1�˳�\n");
	color(14);
	double num1, num2;
	char op;
	double result;
	string caption1("Error!Divided by 0!\n");
	string caption2("Invalid opereator!\n");
	while (1)
	{
		color(7);
		I >> num1 >> op >> num2;
		color(15);
		switch (op)
		{
		case '+':
			result = num1 + num2;
			O << num1 << op << num2 << "=" << result << endl; break;
		case '-':
			result = num1 - num2;
			O << num1 << op << num2 << "=" << result << endl; break;
		case '*':
			result = num1 * num2;
			O << num1 << op << num2 << "=" << result << endl; break;
		case '/':
			if (fabs(num2)<1.0e-8)
			{
				O << caption1 << endl;
			}
			else
			{
				result = num1 / num2;
				O << num1 << op << num2 << "=" << result << endl; break;
			}
		case '!':
			goto loop_calculator1;
		default: O << caption2 << endl;
		}
	}
	loop_calculator1:
	int Loop;
}
 
void calculator2_1()
{
	system("cls");
	color(14);
	SetPos(1,1);
	printf("|�߾��ȼӷ�|����   �˳�\n");
	color(11);
	printf("ÿ���������У�һ��һ�������Զ����\n\n");
	color(10);
	SetPos(17,1);
	printf("[E]");
	SetPos(0,5);
	
	while (1)
	{
		color(8);
		char a1[100],b1[100];
		int a[100],b[100],c[100];
		int a1_len,b1_len,lenc,i,x;
		memset(a,0,sizeof(a)); 
		memset(b,0,sizeof(b)); 
		memset(c,0,sizeof(c));
		gets(a1);
		if (a1[0]=='E')
			break;
		gets(b1);
		color(15);
		a1_len=strlen(a1); 
		b1_len=strlen(b1); 
		for (i=0;i<=a1_len-1;i++) 
		{
			a[a1_len-i]=a1[i]-48; //������������a���� ��
		}
		for (i=0;i<=b1_len-1;i++)
		{
			b[b1_len-i]=b1[i]-48; //������������b���� 
		} 
  	  	lenc =1; 
		x=0; 
 	   	while(lenc <=a1_len || lenc <=b1_len) 
 	   	{ 
			c[lenc]=a[lenc]+b[lenc]+x; //������� ��
			x=c[lenc]/10;
			c[lenc]=c[lenc]%10;
			lenc++;
		}
		c[lenc]=x;
		if (c[lenc]==0)
		{
			lenc--; //������߽�λ  
		} 
		for (i=lenc;i>=1;i--)
		{
			O<<c[i]; //������ 	
		}
		O<<endl<<endl;
	}
}
 
void calculator2_2()
{
	system("cls");
	color(14);
	SetPos(1,1);
	printf("|�߾��ȼ���|����   �˳�\n");
	color(11);
	printf("ÿ���������У�һ��һ�������Զ����\n\n");
	color(10);
	SetPos(17,1);
	printf("[E]");
	SetPos(0,5);
	
	while (1)
	{
		color(7);
		int N=1001; 
 	    int a [ N ] , b [ N ] , c [ N ] , i ;
 	   	char n [ N ] , n1 [ N ] , n2 [ N ] ;
 	    memset ( a , 0 , sizeof ( a ) ) ;
		memset ( b , 0 , sizeof ( b ) ) ;
  	  	memset ( c , 0 , sizeof ( c ) ) ;
  	 	gets ( n1 ) ;
  	 	if (n1[0]=='E')
  	 		break;
 	   	gets ( n2 ) ;
 	   	color(15);
 	   	int lena = strlen ( n1 ) , lenb = strlen ( n2 ) ;
	    if ( lena < lenb || ( lena == lenb && strcmp ( n1 , n2 ) < 0 ) ) 
	    {
	        strcpy ( n , n1 ) ;
	        strcpy ( n1 , n2 ) ;
	        strcpy ( n2 , n ) ;
	        swap ( lena , lenb ) ;
	        printf ( "-" ) ;
	    }
	    for ( i = 0 ; i < lena ; i ++ ) a [ lena - i ] = int ( n1 [ i ] - '0' ) ;
	    for ( i = 0 ; i < lenb ; i ++ ) b [ lenb - i ] = int ( n2 [ i ] - '0' ) ;
	    i = 1 ;
	    while ( i <= lena || i<= lenb ) 
	    {
	        if ( a [ i ] < b [ i ] ) 
	        {
	            a [ i ] += 10 ;
	            a [ i + 1 ] -- ;
	        }
	        c [ i ] = a [ i ] - b [ i ] ;
	        i ++ ;
	    }       
	    int lenc = i ;
	    while ( c [ lenc ] == 0 && lenc > 1 ) 
			lenc -- ; 
	    for ( i = lenc ; i >= 1 ; i -- ) 
			printf ( "%d" , c [ i ] ) ;
		printf("\n\n");
	}
}
 
void calculator2_3()
{
	system("cls");
	color(14);
	SetPos(1,1);
	printf("|�߾��ȳ˷�|����   �˳�\n");
	color(11);
	printf("ÿ���������У�һ��һ�������Զ����\n\n");
	color(10);
	SetPos(17,1);
	printf("[E]");
	SetPos(0,5);
	
	while (1)
	{
		string str1,str2;
 	   	int a[250],b[250],c[500],len; 
  	  	int i,j;
  	  	memset(a,0,sizeof(a));
 	    memset(b,0,sizeof(b));
		
		color(8);
 	 	I>>str1;
 	 	if (str1[0]=='E')
 	 		break;
 	 	I>>str2;
 	 	color(15);
	    a[0]=str1.length();
	    for(i=1;i<=a[0];i++)
	        a[i]=str1[a[0]-i]-'0';
	    b[0]=str2.length();
	    for(i=1;i<=b[0];i++)
	        b[i]=str2[b[0]-i]-'0';
 
		memset(c,0,sizeof(c));
  	    for(i=1;i<=a[0];i++)
 	        for(j=1;j<=b[0];j++)
 	        {
 	        	c[i+j-1]+=a[i]*b[j];
    	    	c[i+j]+=c[i+j-1]/10;
    	    	c[i+j-1]%=10;
    	    }
	    len=a[0]+b[0]; 
 
	    while((c[len]==0)&&(len>1)) len--;
	    for(i=len;i>=1;i--)
	        cout<<c[i];
	    
	    printf("\n\n");
	}
}
 
void calculator2()
{
	system("cls");
	printf("\n\n\n\n\n");
	logoing();
	
	loop_calculator2_head:
	system("cls");
	
	color(10);
	SetPos(1,1);
	printf("|�߾��ȼ�����|��   ����");
	color(11);
	SetPos(17,1);
	printf("[`]");
	
	color(14);
	SetPos(1,3);
	printf("|�߾��ȼӷ�| [s]");
	SetPos(1,5);
	printf("|�߾��ȼ���| [d]");
	SetPos(1,7);
	printf("|�߾��ȳ˷�| [f]");
	
	char c;
	while (1)
		if (kbhit())
		{
			c=getch();
			switch (c)
			{
				case 's':
					{
						calculator2_1();
						goto loop_calculator2_head;
					}
				case 'd':
					{
						calculator2_2();
						goto loop_calculator2_head;
					}
				case 'f':
					{
						calculator2_3();
						goto loop_calculator2_head;
					}
				case '`':
					goto loop_calculator2_tail;
			}
		}
	
	loop_calculator2_tail:
	int Loop;
}
void Calculator()
{
	loop_calculator_head:
	system("cls");
	
	color(12);
	printf("\n");
	printf("����������������������������������\n");
	printf("�� ��ͨ������[f] ��\n");
	printf("����������������������������������");
	
	color(14);
	SetPos(20,1);
	printf("����������������������������������\n");
	SetPos(20,2);
	printf("�� �߾�������[f] ��\n");
	SetPos(20,3);
	printf("����������������������������������");
	
	color(8);
	SetPos(1,5);
	printf("��   ���ء�");
	color(15);
	SetPos(3,5);
	printf("[`]");
	
	char c;
	while (1)
		if (kbhit())
		{
			c=getch();
			if (c=='d')
			{
				calculator1();
				goto loop_calculator_head;
			}
			if (c=='f')
			{
				calculator2();
				goto loop_calculator_head;
			}
			if (c=='`')
				goto loop_calculator_tail;
		}
	loop_calculator_tail:
	int Loop;
}
 
void game1()
{
	color(6);
	char maze[18][24]= {"************** ********",//1
          				"*     #      * *      *",//2
          				"* ************ * ** * *",//3
          				"*            * **** * *",//4
          				"* ********** * *    * *",//5
          				"* **       *      *****",//6
          				"* ** ***** ***** * ** *",//7
          				"*        *          * *",//8
          				"****** * **********   *",//9
          				"*    * * *        * $ *",//10
          				"* **** * * ****** *****",//11
          				"* *    * * *    * *   *",//12
          				"* * ****** * ** * * * *",//13
          				"* *        * ** * * * *",//14
          				"* ********** **     * *",//15
          				"*               *     *",//16				  				  				            		
          				"*************** *******",//17 
						"***********************"};//18				
    int x,y;
    int count = 0;
    srand(time(0));
	x = rand() % 18;
	y = rand() % 23;
	while(maze[x][y] != ' '){
		x = rand() % 18;
		y = rand() % 23;
	} 
	maze[x][y] = 'i';
	int i,j;
    for(i = 0;i < 18;++i){
    	for(j = 0;j < 23;++j){
    		O << maze[i][j] << " ";
		}
		O << endl;
	}
	char c;
	while(true){
		c = getch();
		++count;
		system("cls");
		if(c == 'w'){
			if(maze[x-1][y] == ' ' || maze[x-1][y] == '$'){
				maze[x][y] = ' ';
				--x;
				maze[x][y] = 'i';
			}
			else{
				MessageBox(0,"��ײ����ǽ��/ը���ϣ��������Ϸ","   ����",MB_OK);
				break; 
			} 
		}
		if(c == 'a'){
			if(maze[x][y-1] == ' ' || maze[x][y-1] == '$'){
				maze[x][y] = ' ';
				--y;
				maze[x][y] = 'i';
			}
			else{
				MessageBox(0,"��ײ����ǽ��/ը���ϣ��������Ϸ","   ����",MB_OK);
				break; 
			} 
		}
		if(c == 's'){
			if(maze[x+1][y] == ' ' || maze[x+1][y] == '$'){
				maze[x][y] = ' ';
				++x;
				maze[x][y] = 'i';
			}
			else{
				MessageBox(0,"��ײ����ǽ��/ը���ϣ��������Ϸ","   ����",MB_OK);
				break; 
			} 
		}
		if(c == 'd'){
			if(maze[x][y+1] == ' ' || maze[x][y+1] == '$'){
				maze[x][y] = ' ';
				++y;
				maze[x][y] = 'i';
			}
			else{
				MessageBox(0,"��ײ����ǽ��/ը���ϣ��������Ϸ","   ����",MB_OK);
				break; 
			} 
		}
		for(i = 0;i < 18;++i){
	    	for(j = 0;j < 23;++j){
	    		O << maze[i][j] << " ";
			}
			O << endl;
		}
		if(maze[9][20] == 'i'){
			maze[0][14] = ' '; 
			MessageBox(0,"ȡ������","��ʾ",MB_OKCANCEL);
		}
		if(x == 0 && y == 14 && maze[9][20] == ' '){
			string steps = "�߳��Թ���ʹ�ò���Ϊ:";			//messagebox��ʾʹ�ò��� 
			char sum[100]; 
			itoa(count,sum,10);							//����¼��Ҳ����ı���sum��ת���ַ�������ֵ��iToA���� 
			steps += sum; 
			MessageBox(0,steps.c_str(),"��ʾ",MB_OKCANCEL);//����õ��ַ�����ʾ��messagebox�� 
			break; 
		}
		if(x<0){
			MessageBox(0,"�����¿�ʼ","   ����",MB_OK);
			break; 
		}	
	}	
	Sleep(2000);
}
 
void game2()
{
	color(6);
	int num[5][5] = {0};//16�ſ��� 
	int arr[5][5] = {0};//��ǿ����Ƿ񱻷��� 
	int ix,iy,ia,ib; 
	int cnt = 0;
	int flag[9] = {0};//1-8��num�����г��ֵĴ��� 
	int temp;//������ɵ����� 
	O<<"���俨�ƹ���:"<<endl;
	O<<"����16�ſ��ƣ�������1-8��ɣ�ÿ�����ֳ���2��"<<endl;
	O<<"��Ϸ����Ҫ�����Ʒ������棬�ҳ�������ͬ������"<<endl;
	O<<"ÿһ��ֻ�������ſ���"<<endl;
	O<<"�������������ͬ���򽫿����������ֵ�һ����ʾ����"<<endl;
	O<<"���򽫿��Ʒ������棬������һ��"<<endl;
	O<<"ϵͳ������Ϸ���ҳ�������ͬ���ֵĿ��������ѵĲ���(���ж�����)"<<endl;
	system("pause");
	srand(time(0));
	for(int i=1;i<=4;i++){
		for(int j=1;j<=4;j++){
			while(true){
				temp = rand()%8 +1;
				if(flag[temp]<2){
					num[i][j] = temp;	
					flag[temp]++;
					break;
				}
				else{
					continue;
				}		
			}

		}
	}
	for(int i=1;i<=4;i++){
		for(int j=1;j<=4;j++){
			O<<"*";
		}
		O<<endl;
	} 
	while(true){
		cnt++;
		int sum = 0;
		O<<"�������һ�η�����Ƭ������������:"<<endl;
		I>>ix>>iy;
		arr[ix][iy]++;
		system("cls");
		for(int i=1;i<=4;i++){
			for(int j=1;j<=4;j++){
				if(arr[i][j]>0) O<<num[i][j];
				else  O<<"*";
			}
			O<<endl;
		}
		O<<"������ڶ��η�����Ƭ������������:"<<endl;
		I>>ia>>ib;
		arr[ia][ib]++;
		system("cls");
		for(int i=1;i<=4;i++){
			for(int j=1;j<=4;j++){
				if(arr[i][j]>0) O<<num[i][j];
				else  O<<"*";
			}
			O<<endl;
		}
		if(num[ix][iy] == num[ia][ib]){
			O<<"��ϲ�������ַ��Ƴɹ�"<<endl;
		}
		else{
			O<<"���ź������ַ���ʧ��"<<endl;
			arr[ix][iy]--;
			arr[ia][ib]--;
		}
		cout<<"2��������һ��"<<endl;
		Sleep(2000);
		system("cls");
		for(int i=1;i<=4;i++){
			for(int j=1;j<=4;j++){
				if(arr[i][j]>0){
					sum+=1;
					O<<num[i][j];
				}
				else O<<"*";
			} 
			O<<endl;
		}
		if(sum==16){
			O<<endl;
			O<<"һ��ʹ��"<<cnt<<"��"<<endl;
			break;
		} 
	}	
	Sleep(2000); 
}
void game3(){
	outArr();
	int x=1,y=1,count=0;
	while(true){				
		O<<"�������ƶ�����(wasd)��"<<endl;
		char choose;	
		choose = getch();
		++count;
		if(choose == 'w')
		{
			if(x>=0 && x<2)
			{
				change(&n_puzzle[x][y],&n_puzzle[x+1][y]);
				++x;
			}
		}
		if(choose == 'a')
		{
			if(y>=0 && y<2)
			{
				change(&n_puzzle[x][y],&n_puzzle[x][y+1]);
				++y;
			}
		}
		if(choose == 's')
		{
			if(x>0 && x<=2)
			{
				change(&n_puzzle[x][y],&n_puzzle[x-1][y]);
				--x;
			}
		}
		if(choose == 'd')
		{
			if(y>0 && y<=2)
			{
				change(&n_puzzle[x][y],&n_puzzle[x][y-1]);
				--y;
			}
		}
		system("cls");
		outArr();
		int flag = 1;
		for(int i=0;i<3;++i)
		{
			for(int j=0;j<3;++j){
				if(n_puzzle[i][j] != n_puzzle_3[i][j])
				{
					flag=0;					
				}	
			}
		}
		if(flag == 1)
		{	
			string steps = "������ֻ��ݵ���ʹ�ò���Ϊ:";			//messagebox��ʾʹ�ò��� 
			char sum[100]; 
			itoa(count,sum,10);							//����¼��Ҳ����ı���sum��ת���ַ�������ֵ��iToA���� 
			steps += sum; 
			MessageBox(0,steps.c_str(),"��ʾ",MB_OK);//����õ��ַ�����ʾ��messagebox�� 
		//MessageBox(0,"��ϲ��������ֻ��ݵ�","��ʾ",MB_OK);
		break; 
	}		
}
}
void game4(){
//	��ʾ����
	O << "����һ������������" << endl;
	Sleep(500);
	O << "Ϊ�˲μ����µ�һ������" << endl;
	Sleep(500);
	O << "�����Ҫ����С�������Ϊ��һ�����ѵ" << endl;
	Sleep(500);
	O << "ֻ�������Գ���500���ܲμӴ��" << endl;
	Sleep(500);
	O << "��һ����,ÿ����С��������Խ���һ��̽��" << endl << endl;
	Sleep(500);
	O << "�ڿ����ӹ������ͽ�Ǯ�����ǻ���������ڹ�" << endl;
	Sleep(500);
	O << "�չأ�������ӻ���ٹ������ڹ������ǻ����Ľ�Ǯ" << endl;
	Sleep(500);
	O << "�Է������Ӵ������������ǻ����ĺܶ��Ǯ" << endl;
	Sleep(500);
	O << "���������Ŵ���������ս��" << endl;
	Sleep(500);
	O << "�й䣺����װ�������������ԣ����ǻ����Ľ�Ǯ" << endl << endl;
	Sleep(500);	
	O << "׼���ÿ�ʼð������" << endl;
	system("pause");
	
//�й价������װ������ 
	//װ��������
	char item[10][9] = {"����", "��ˮ��", "����",
					"��", "�����", "����֮ʯ",
					"�籩�޽�", "��������", "������װ",
					"�ƾ�"};
	//װ���۸� 
	int prices[10] = {10, 10, 10, 20, 20, 20, 50, 50, 100, 100};
	//װ�����ӵ�����
	int itemAtt[10][4] = {{10, 0, 0, 0}, {0, 0, 10, 0}, {0, 0, 0, 10},
						  {20, 0, 0, 0}, {0, 20, 0, 0}, {0, 10, 10 , 0},
					      {50, 0, 0, 0}, {0, 50, 0, 0}, {0, 100, 0, 0}, 
					      {100, 0, 0, 0}};		
	 
	int itemList[10];
	for(int i = 0;i < 10;++i){         //��װ���б��������Ԫ����Ϊ-1 
		itemList[i] = -1;
	}
//�����������ӵ������� 
	//�������� 
	char enemyName[5][5] = {"ë��", "ǿ��", "ɱ��", "ͷĿ", "����"};	
	//�������� 
	int enemyAtt[5][4] = {{60, 60, 50, 0},
					   	  {70, 60, 60, 0},
					      {90, 60, 50, 20},
					      {90, 70, 80, 30},
					      {100, 70, 90, 50}};	
	//�����ͽ� 
	int enemyMoney[5] = {30, 30, 50, 50, 60};
//��һ������ʾ�ĸ�������ɵ����Բ�ѡ����ɫ����-------------------------------------------------------------------------------------	
	//��һ������� 
	char playAtt[6][5] = {"����", "Ѫ��", "�ڹ�", "����", "��Ǯ"};
	//�������� 
	int player[6] = {};//0.���� 1.Ѫ�� 2.�ڹ� 3.���� 4.��Ǯ 
	//��ʼ������
	srand(time(0));
	while(true){
		system("cls");
		O << "��ѡ����ĳ�ʼ���ԣ�" << endl;
		//�����ɫ��4������
		for(int i = 0;i < 4;++i){
			player[i] = rand()%50+40;
		}
		player[4]=50; 
		//��ʾ��ɫ��4������
		O << "��ǰ��ɫ��Ϣ" << endl;
		for(int i = 0;i < 5;++i){
			if(player[i] >= 120) color(6);
			else if(player[i] >= 80) color(4);
			O<<playAtt[i]<<":"<<player[i]<<endl;
			color(7);
		}
		char user;//����һ��user������ȷ����ɫ����
		O << "�Ƿ�ѡ���ʼ���ԣ���y/n��" << endl;
		user = getch();
		if(user == 'y'){
			break;
		}
	} 
//�ڶ�����������ɫ����ʾ��ǰȷ���Ľ�ɫ����-------------------------------------------------------------------------------------
	//������ɫing���˴���Ϊ��ѭ���������������������������������������������� 
	system("cls");
	O << "���ڴ�����ɫ" << endl;
	for(int i=3;i>=1;--i){
		Sleep(1000);
		cout << i << endl;
	} 
	O << "��ɫ�����ɹ�����" << endl;
	//��ʾ��ɫ��Ϣ
	O << "��ǰ��ɫ��Ϣ" << endl;
	for(int i = 0;i < 5;++i){
		if(player[i] >= 120){
			color(6);
		}else if(player[i] >= 80){
			color(4);
		}else{
			color(7);
		}
		O << playAtt[i] << " :" << player[i] << endl;
		color(7);
	}
	system("pause");
//��������12���µ�����-------------------------------------------------------------------------------------	
	//��ǰ�·� 
	int month = 1;
	while(month <= 12){
		system("cls");
		while(true){
			O << "������" << month << "��" << endl;
			O << "q �鿴����" << endl;
			O << "a �ڿ� " << endl; 
			O << "s �չ� " << endl;
			O << "d �Է� " << endl;
			O << "f ���� " << endl;  
			O << "g �й� " << endl; 						
			O << "�����룺"; 
			char choose;//��������ѡ��
			I >> choose; 
			if(choose == 'q'){				
				system("cls");
				//��ʾ��ɫ����
				O << "��ǰ��ɫ��Ϣ" << endl;
				for(int i = 0;i < 5;++i){
					if(player[i] >= 120){
						color(6);
					}else if(player[i] >= 80){
						color(4);
					}else{
						color(7);
					}
					O << playAtt[i] << " :" << player[i] << endl;
					color(7);
				}
				//��ʾװ��
				O << "��ǰװ����" << endl;
				if(itemList[0] == -1){
					O << "û��װ��" << endl;
				}
				for(int i = 0;i < 10;++i){
					if(itemList[i] == -1){
						break;
					}
					//��ʾװ����ɫ
					if(itemList[i] < 3){
						color(15);
					}else if(itemList[i] < 6){
						color(10);
					}else if(itemList[i] < 8){
						color(1);
					}else{
						color(5);
					}
					O << item[itemList[i]] << endl;
					color(15);
				}				
			}
			else if(choose == 'a'){
				O << "С�������ڿ���" << endl;
				O << "���ݣ������ݣ�����Ǯ+5" << endl;
				Sleep(800);
				O << "���ݣ������ݣ�����Ǯ+5" << endl;
				Sleep(800);
				O << "С�����ǿ������ۣ�������+10" << endl;
				Sleep(800);
				O << "С����ķ����������ڹ�-5" << endl;
				Sleep(800);
				player[4] += 10;
				player[0] += 10;
				player[2] -= 5;
				break; 
			}	
			else if(choose == 's'){
				O << "С������������" << endl;
				char event[2][100]= {"�����������������������+20���ڹ�+20",
					  			"�߻���ħ���������������-15���ڹ�-15"}; 
				for(int i=0; i<3; ++i){
					int temp = rand()%2;
					O << event[temp] << endl;
					Sleep(1000);
					if(temp == 0){
						player[0] += 20;
						player[2] += 20;
					}else if (temp == 1){
						player[0] -= 15;
						player[2] -= 15;
					}else{
						player[4] += 30;
					}
				}
				break;
			}
			else if(choose == 'd'){
				O << "С������ܳ�" << endl;
				for(int i = 0;i < 3;++i){
					int temp = rand()%20+1;
					O << "С������Զ���" << endl;
					Sleep(2000);
					O << "Ѫ��+" << temp;
					O << ",��Ǯ-" << temp/2+1  << endl;
					player[1] += temp;
					player[4] -= temp / 2+1;
				}
				break; 
			}
			else if(choose == 'f'){
				//����
				O << "��ӭ���д��������ֵ��ˣ�" <<endl;
				int temp = rand()%5; 
				if(enemyAtt[temp][1] <= 0){
					temp = rand()%5;
				} 
				O << enemyName[temp] << endl; 				
				for(int i = 0;i<4;++i){ 
					O << playAtt[i] <<" : "<< enemyAtt[temp][i]<<endl;
				}
				O<<"�ͽ� : " << enemyMoney[temp] << endl; 
				
				O << "��ǰ��ɫ��Ϣ" << endl;
				for(int i = 0;i < 4;++i){
					if(player[i] >= 120){
						color(6);
					}else if(player[i] >= 80){
						color(12);	
					}else{
						color(15);
					}
					O << playAtt[i] << " ��" << player[i] << endl;
					color(15); 
				}
				O<<"�Ƿ�ӭս��(y/n)"<<endl; 
				char user;
				I >> user;
				if(user == 'y'){
					if(enemyAtt[temp][2]>player[2]){
						O << "�ڹ�������������ˡ������-20" << endl;
						player[4] -= 20;
					}else{
						while(enemyAtt[temp][1] > 0){
							if(player[1] <= 0){
								break;
							}
							int attack1 =  enemyAtt[temp][0] - player[3];
							if(attack1<0) attack1 = 0;
							cout<< enemyName[temp] << "��������,���" << attack1 << "�˺�" <<endl;
							player[1] -= attack1;							
							int attack2 = player[0] - enemyAtt[temp][3];
							if(attack2<0) attack2=0; 
							cout << "С�����������,���" << attack2 << "�˺�"<<endl;
							enemyAtt[temp][1] -= attack2;	
						}
						if(player[1]>0){
							O<<"ս��ʤ����������������ͽ�"<< enemyMoney[temp]<<endl; 
							player[4] += enemyMoney[temp];
							//��չ��ս��ʤ���󣬳��ͽ��⣬С������������������ 
							//int att = rand()%4;
							//int value = (rand()%3+1) * temp;
							//cout<< playAtt[att]<<"����"<<value<<endl;
							//player[att]+=value;
						}
					} 
				}
				else{
					O<<"�Ʋ����֣���Ǯ����10"<<endl;
					player[4]-=10; 
				}
				break;
			}
			else if(choose == 'g'){
				//�й�
				O << "С�����ɽ��ˮ�����̵�" << endl;
				O << "��ǰ��Ǯ��" << player[4] << endl;	
				int index[5] = {-1,-1,-1,-1,-1};	
				for(int i = 0;i<5;++i){
					index[i] = rand()%10;
					for(int j =0;j<i;++j){
						if(index[i] == index[j]){
							--i;
							break;
						}
					}
				}
				for(int i = 0 ; i < 5; ++i){
					if(index[i] < 3){
						color(15);
					}else if(index[i] < 6){
						color(10);
					}else if(index[i] < 8){
						color(1);
					}else{
						color(5);
					} 
					O << (i+1) <<"  "<< item[index[i]] <<" ";
					O << "�۸�:" << prices[index[i]] <<endl;
					O << "   "<<"װ������ֵ��";
					for(int j=0;j<4;++j){
						cout << itemAtt[index[i]][j] << "  ";
					} 
					O << endl;
					color(15);
				}
				O << "������Ҫ���򼸺�װ����" << endl;
				int user;
				I >> user;
				
				if(user>=1 && user<=5 && player[4]>=prices[index[user-1]]){
					//��װ��
					O << "�ɹ�����" << item[index[user-1]] << endl;
					player[4] -= prices[index[user-1]];
					//�����Ʒ					
					for(int i = 0;i < 10;++i){   //�������װ���ŵ�װ���б��� 
						if(itemList[i] == -1){
							itemList[i] = index[user-1];
							break;
						}
					}
					
					for(int i = 0;i < 4;++i){                                                                                      //����װ�����ԣ�������ɫ��Ӧ���� 
						if(itemAtt[index[user-1]][i] != 0){
							O <<"���"<< playAtt[i]<<itemAtt[index[user-1]][i]<<"��"<<endl;
						}
						player[i] += itemAtt[index[user-1]][i];
					}
				}
				else{
					O << "�������ҳ��ӵİɣ���!����ӭ��!"<<endl; 
				} 
				break;
			}
			
			else{
				system("cls");
				O<<"��δ��ͨ�˹��ܣ�����������"<<endl;
			}
		} 
		system("pause");  
		++month;
		for(int i=0;i<5;++i){
			if(player[i] < 0){
				O<<playAtt[i]<<"���㣬��Ϸ����"<<endl; 
			}
			if(player[i]<=10){
				MessageBox(0,"�뼰ʱ�鿴����ֵ��","����",MB_ICONEXCLAMATION);
				break;
			}			
		}
	}
	O << "��ǰ��ɫ��Ϣ" << endl;
	for(int i = 0;i < 5;++i){
		if(player[i] >= 120){
			color(6);
		}else if(player[i] >= 80){
			color(12);	
		}else{
			color(15);
		}
		O << playAtt[i] << " ��" << player[i] << endl;
		color(15); 
	}
	int sum=0;
	for(int i=0;i<5;++i){
		sum += player[i];
	} 
	O << "������ֵΪ��" << sum <<endl; 
	system("pause");
	O << "һ���......" <<endl; 
	//12���º���Ϸʤ��
	if(sum >= 500){
		if(player[0] >= 100){
			MessageBox(0,"��ϲ��ù������˳ƺ�","��Ϸʤ��",MB_OK);
		}
		if(player[1] >= 100){
			MessageBox(0,"��ϲ���Ѫ�����˳ƺ�","��Ϸʤ��",MB_OK);
		} 
		if(player[2] >= 100){
			MessageBox(0,"��ϲ����ڹ����˳ƺ�","��Ϸʤ��",MB_OK);
		} 
		if(player[3] >= 100){
			MessageBox(0,"��ϲ��÷������˳ƺ�","��Ϸʤ��",MB_OK);
		} 
		if(player[4] >= 100){
			MessageBox(0,"��ϲ��ý�Ǯ���˳ƺ�","��Ϸʤ��",MB_OK);
		}
	}
	//12���º���Ϸʧ��
	if(sum < 500){
		MessageBox(0,"��������ս��","��Ϸʧ��",MB_OK);
	} 
	Sleep(2000);
} 
void game5(){
	Landlords *landlords=new Landlords();
	do{
	    landlords->init();//���ơ������� 
	    landlords->startGame();//��Ϸ��ʼ 
	}while(makeChoice("\n�Ƿ������Ϸ����Y=����/N=������: "));
	delete landlords;
	}
	bool makeChoice(string tip){
	O<<tip;
	string input;
	I>>input;
	return input=="Y"||input=="y";
	}
	bool cmp(Card* a,Card* b){
	if(a->value==b->value){
	    return a->color>b->color;
	}else{
	    return a->value>b->value;
	}	
}
void Game()
{
	system("cls");
	loop_game_head:
	color(14);
	printf("\n");
	printf("������������������������������������������\n");
	printf("�� �´��Թ�[d]       ��\n");
	printf("������������������������������������������\n");
	color(10);
	SetPos(25,1);
	printf("������������������������������������������\n");
	SetPos(25,2);
	printf("�� ���俨��[f]       ��\n");
	SetPos(25,3);
	printf("������������������������������������������\n");
	color(8);
	SetPos(50,1);
	printf("������������������ ��������\n");
	SetPos(50,2);
	printf("�� ���ֻ��ݵ�[h]        ��\n");
	SetPos(50,3);
	printf("������������������ �� �� ��\n");	
	color(4);
	SetPos(70,1);
	printf("��������     ��������\n");
	SetPos(70,2);
	printf("�� С�뽭��[j]     ��\n");
	SetPos(70,3);
	printf("������ ��������������\n");		
	color(6);
	SetPos(80,1);
	printf("�������� ��������\n");
	SetPos(80,2);
	printf("�� ������[k]   ��\n");
	SetPos(80,3);
	printf("������ ����������\n");	
	color(3);
	printf("��   ����");
	color(15);
	SetPos(3,5);
	printf("[`]");
	printf("\n\n\n");
	char c;
	while (1)
		if (kbhit())
		{
			c=getch();
			if (c=='d')
			{
				game1();
				goto loop_game_head;
			}
			if (c=='f')
			{
				game2();
				goto loop_game_head;
			}
			if(c=='h'){
				game3();
				goto loop_game_head;
			} 
			if(c=='j'){
				game4();
				goto loop_game_head;
			} 	
			if(c=='k'){
				game5();
				goto loop_game_head;
			}		
			if (c=='`')
				goto loop_game_tail;
		}
	
	loop_game_tail:
	int Loop;
}
 
void solution1()
{
	ShellExecute(NULL,"open","explorer.exe","https://blog.csdn.net/u011815404/article/details/79324003",NULL,SW_SHOW);
}
 
void solution2()
{
	system("cls");
	_sleep(500);
	color(12);
	SetPos(1,1);
	printf("����������������������������\n");
	SetPos(1,2);
	printf("�� �ݲ�֧�֣� ��\n");
	SetPos(1,3);
	printf("����������������������������\n");
	_sleep(1000);
}
 
void solution3()
{
	system("cls");
	_sleep(500);
	color(12);
	SetPos(1,1);
	printf("����������������������������\n");
	SetPos(1,2);
	printf("�� �ݲ�֧�֣� ��\n");
	SetPos(1,3);
	printf("����������������������������\n");
	_sleep(1000);
}
 
void Solution()
{
	loop_solution_head:
	system("cls");
	
	color(14);
	SetPos(1,1);
	printf("����������������������������������\n");
	SetPos(1,2);
	printf("�� ĳ��ͨ���[s] ��\n");
	SetPos(1,3);
	printf("����������������������������������\n");
	color(13);
	SetPos(1,4);
	printf("����������������������������������\n");
	SetPos(1,5);
	printf("��  ĳ�����[d]  ��\n");
	SetPos(1,6);
	printf("����������������������������������\n");
	color(11);
	SetPos(1,7);
	printf("����������������������������������\n");
	SetPos(1,8);
	printf("��  ĳ�����[f]  ��\n");
	SetPos(1,9);
	printf("����������������������������������\n");
	color(8);
	SetPos(1,11);
	printf("��   ���ء�");
	color(15);
	SetPos(3,11);
	printf("[`]");
	
	char c;
	while (1)
		if (kbhit())
		{
			c=getch();
			if (c=='s')
			{
				solution1();
				goto loop_solution_head;
			}
			if (c=='d')
			{
				solution2();
				goto loop_solution_head;
			}
			if (c=='f')
			{
				solution3();
				goto loop_solution_head;
			}
			if (c=='`')
				goto loop_solution_tail;
		}
	
	loop_solution_tail:
	int Loop;
}
 
void System()
{
	loop_system_head:
	system("cls");
	
	color(15);
	printf("\n ׼����");
	_sleep(500);
	system("cls");
	printf("\n ������Windowsϵͳȡ����ϵ");
	_sleep(1200);
	system("cls");
	printf("\n ������ϵͳ��������");
	_sleep(2000);
	system("cls");
	printf("\n �������ù���ԱȨ��");
	_sleep(800);
	system("cls");
	printf("\n ����������Ӳ����������");
	_sleep(2400);
	system("cls");
	printf("\n ׼��������");
	_sleep(1000);
	system("cls");
	
	color(12);
	SetPos(3,1);
	printf("����������������������������\n");
	SetPos(3,2);
	printf("��   �ػ�[A]  ��\n");
	SetPos(3,3);
	printf("����������������������������\n");
	color(10);
	SetPos(3,4);
	printf("����������������������������\n");
	SetPos(3,5);
	printf("��   ����[S]  ��\n");
	SetPos(3,6);
	printf("����������������������������\n");
	color(11);
	SetPos(3,7);
	printf("����������������������������\n");
	SetPos(3,8);
	printf("��   ע��[D]  ��\n");
	SetPos(3,9);
	printf("����������������������������\n");
	color(14);
	SetPos(3,10);
	printf("����������������������������\n");
	SetPos(3,11);
	printf("��   ����[F]  ��\n");
	SetPos(3,12);
	printf("����������������������������\n");
	color(8);
	SetPos(1,14);
	printf("��   ���ء�");
	color(15);
	SetPos(3,14);
	printf("[`]");
	
	char c;
	while (1)
		if (kbhit())
		{
			c=getch();
			switch (c)
			{
				case 'a':
					{
						system("shutdown -s -t 0");
						goto loop_system_head;
					}
				case 's':
					{
						system("shutdown -r");
						goto loop_system_head;
					}
				case 'd':
					{
						system("shutdown -l");
						goto loop_system_head;
					}
				case 'f':
					{
						system("shutdown -h");
						goto loop_system_head;
					}
				case '`':
					goto loop_system_tail;
			}
		}
	
	loop_system_tail:
	int Loop;
}
void run()
{
	color(7);
	char c;
	while (1)
		if (kbhit())
		{
			c=getch();
			switch (c)
			{
				case 'a':
					{
						Calculator();
						goto loop_run_tail;
					}
				case 's':
					{
						Game();
						goto loop_run_tail;
					}
				case 'd':
					{
						Solution();
						goto loop_run_tail;
					}
				case 'f':
					{
						System();
						goto loop_run_tail;
					}
			}
		}
	
	loop_run_tail:
	int Loop;
}
int main(void)
{
	Pos();
	printf("\n");
	for (int i=1;i<=4;i++)
	{
		logoing();
		printf("\n");
	}
	while (1)
	{
		lobby();
		run();
	}
	return 0;
}
