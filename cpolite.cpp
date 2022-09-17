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
enum COLOR{  //花色显示ASCII: 3~6
eHEART=3,//红桃 
eDIAMOND,//方片 
eCLUB,   //草花 
eSPADE   //黑桃 
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
//为了规范查找对应牌的方法
//统一为3个参数cnt1、isContinuous、cnt2
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
        type.init("不出",14,0,0,0);
        return;
    }
    if(n==2&&cards[0]->value==15&&cards[1]->value==14){
        type.init("王炸",0,0,0,0);
        return;
    }
    //统计同点数牌有多少张 
    int cntFlag[VALUECOUNT]={0};
    for(i=0;i<n;i++){
        cntFlag[cards[i]->value]++;
    }
    //统计点数最多和最少的牌 
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
        type.init("炸弹",1,4,0,0);
        return;
    }
    if(n==1){
        type.init("单牌",2,1,0,0);
        return;
    }
    if(n==2&&maxCnt==2){
        type.init("对子",3,2,0,0);
        return;
    }
    if(n==3&&maxCnt==3){
        type.init("三张 ",4,3,0,0);
        return;
    }
    if(n==4&&maxCnt==3){
        type.init("三带一",5,3,0,1);
        return;
    }
    if(n==5&&maxCnt==3&&minCnt==2){
        type.init("三带一对",6,3,0,2);
        return;
    }
    if(n==6&&maxCnt==4){
        type.init("四带二",7,4,0,1);
        return;
    }
    if(n==8&&maxCnt==4&&minCnt==2){
        type.init("四带二",8,4,0,2);
        return;
    } 
    if(n>=5&&maxCnt==1&&cards[0]->value==cards[n-1]->value+n-1){
        type.init("顺子",9,1,1,0);
        return;
    }
    if(n>=6&&maxCnt==2&&minCnt==2&&cards[0]->value==cards[n-1]->value+n/2-1){
        type.init("连对",10,2,1,0); 
        return;
    }
    int fjCnt;//统计连续且大于3三张的牌 
    for(i=0;i<VALUECOUNT &&cntFlag[i]<3;i++);
    for(fjCnt=0;i<VALUECOUNT &&cntFlag[i]>=3;i++,fjCnt++);
    if(fjCnt>1){
        if(n==fjCnt*3)
            type.init("飞机",11,3,1,0);
        else if(n==fjCnt*4)
            type.init("飞机",12,3,1,1); 
        else if(n==fjCnt*5&&minCnt==2)
            type.init("飞机",13,3,1,2); 
    }
}
void init(string inputStr, vector<Card*> &cardsHolded){
    this->cards.clear();
    //不出 
    if(inputStr=="N"){
        this->calType();
        return;
    }
    int i,j;
    //输入合法性判断 
    for(i=0;i<inputStr.size();i++){
        bool find=false;
        for(j=0;toFigure[j];j++){ 
            if(inputStr[i]==toFigure[j]){
                find=true;
                break;
            }
        } 
        if(find==false){
            //输入字符不在toFigure中
            return;
        }
    }
    //查找手中有没有这些牌 
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
            O<<"没有找到\t";
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
    if(cardGroup.type.typeStr=="王炸"){
        return false;
    }else if(this->type.typeStr=="王炸"){
        return true;
    }else if(cardGroup.type==this->type &&this->type.typeStr=="炸弹"){
        return value()>cardGroup.value();
    }else if(cardGroup.type.typeStr=="炸弹"){
        return false;
    }else if(this->type.typeStr=="炸弹"){
        return true;
    }else if(cardGroup.type==this->type &&this->cards.size()==cardGroup.cards.size()){
        return this->value()>cardGroup.value();
    }else{
        return false;
    }
}
int value(){
    //计算牌组权值 
    int i;
    if(type.typeStr=="三带一"||type.typeStr=="三带一对"||type.typeStr=="飞机"){
        for(i=2;i<cards.size();i++){
            if(cards[i]->value==cards[i-2]->value){
                return cards[i]->value;
            }
        }
    }
    if(type.typeStr=="四带二"){
        for(i=3;i<cards.size();i++){
            if(cards[i]->value==cards[i-3]->value){
                return cards[i]->value;
            }
        }
    }
    return cards[0]->value;
}
void arrange(){
    //整理：排序、计算类型 
    sort(this->cards.begin(),this->cards.end(),cmp);
    this->calType();
}
};
class LastCards{
static LastCards *lastCards;
public:
Player *player;
CardGroup cardGroup;
static LastCards* inst(){//单例模式 
    if(lastCards==NULL){
        lastCards=new LastCards();
    }
    return lastCards;
}
vector<Card*> findCanBeatFrom(vector<Card*> &cardsHolded){
    //查找能打得过的牌 
    int i,j,k,n=cardsHolded.size(),m=cardGroup.cards.size();
    string typeStr=cardGroup.type.typeStr;
    vector<Card*> ret;
    if(typeStr=="王炸"||n<m){
        //打不过，返回空数组 
        return ret;
    }
    int value=cardGroup.value();
    //统计各点牌出现的次数 
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
            //cout<<"查找"<<cardGroup.type.cnt1<<"个"<<firstFigure+3<<endl;
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
                if(cardGroup.type.typeStr=="四带二")
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
                    //cout<<"查找SecondFigure "<<cardGroup.type.cnt2<<"个"<<SecondFigures[0]+3<<endl;
                    //cout<<"SecondCount= "<<SecondCount<<endl;
                    //for(i=0;i<SecondCount;i++)cout<<"SecondFigures["<<i<<"]="<<SecondFigures[i]<<endl;
                    for(i=0;i<SecondCount;i++){ 
                        for(j=0;j<cardsHolded.size();){ 
                            if(cardsHolded[j]->value==SecondFigures[i]){
                                for(k=0;k<cardGroup.type.cnt2;k++){
                                    //cout<<"添加"<<cardsHolded[j]->value+3<<endl;
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
    //没牌打得过时查找有没有炸弹 
    if(typeStr!="炸弹"){
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
    //提示功能,使自己最小一张连最长
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
    //出牌 
    cout<<this->name<<":\t";
    cout<<cardGroup.type.typeStr<<' ';
    for(int i=0;i<cardGroup.cards.size();i++){
        cardGroup.cards[i]->print();
        this->cards.erase(find(this->cards.begin(),this->cards.end(),cardGroup.cards[i])); 
    }
    cout<<"\t["<<this->cards.size()<<"]\n";
    if(cardGroup.type.typeStr!="不出"){
        //记录到 LastCards 中 
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
    //54张牌初始化 
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
    player[1]->name="玩家2";
    player[2]->name="玩家3";
    finished=false;
    youWin=false;
    landlordWin=false;
    //抢地主
    landlordIndex=ERROR; 
    while(landlordIndex==ERROR){
        srand((int)time(0));
        shuffle();
        landlordIndex=chooseLandlord();
    }
    cout<<player[landlordIndex]->name<<"\t成为地主\n\n";
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
            cout<<"输入提示：Z=大王 Y=小王 0=10 N=不要 输入可无序 例如:JKQ0A9\n请出牌：\t";
            do{
                cin>>inputSrt;
                inputCards.init(inputSrt,player[iTurns]->cards);
            }while(check(&inputCards)==false);
        }else{
            if(player[iTurns]==LastCards::inst()->player){
                //若是上次出牌的是自己，启用提示功能 
                inputCards.init(player[iTurns]->tip());
            }else{
                //查找能打得过上家的牌 
                inputCards.init(LastCards::inst()->findCanBeatFrom(player[iTurns]->cards));
            }
        }
        player[iTurns]->chupai(inputCards);//出牌 
         if(player[iTurns]->cards.size()==0){
            //玩家手中没牌了，游戏结束 
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
    O<<"地主3张牌:\t";
    for(int i=PLAYERCOUNT*17;i<CARDSCOUNT;i++){
        this->cards[i]->print();
        player[landlordIndex]->cards.push_back(cards[i]);
    }
    O<<endl;
    player[landlordIndex]->arrange();
}
int chooseLandlord(){
    O<<"\n_________________________ 抢地主 _________________________\n\n";
    int first=-1,last,cnt=0,i,j=rand()%PLAYERCOUNT;
    bool decision;
    for(i=0;i<PLAYERCOUNT;i++,j==2?j=0:j++){
        if(j==CURRENTPLAYER){
            decision=makeChoice("是否抢地主？(Y=抢/N=不抢):");
        }else{ 
            decision=rand()%2;
        }
        if(decision){
            cnt++;
            last=j;
            if(first==-1){
                first=j;
            }
            O<<this->player[j]->name<<"\t抢地主\n";
        }else{
            O<<this->player[j]->name<<"\t没有抢\n";
        }
    }
    if(cnt==0){
        O<<"没人抢，重新发牌\n";
        return ERROR;
    }
    if(cnt==1){
        //第一轮只有一人抢地主 
        return first;
    }
    else{
        //最后一次争抢 
        if(first==CURRENTPLAYER){
            decision=makeChoice("是否抢地主？(Y=抢/N=不抢):");
        }else{
            decision=rand()%2;
        }
        if(decision){
            O<<this->player[first]->name<<"\t抢地主\n";
            return first;
        }else{
            O<<this->player[first]->name<<"\t没有抢\n";
            return last;
        }
    }
}
void shuffle(){
    int i,j,k;    
    //洗牌 
    for(i=0;i<CARDSCOUNT;i++){
        swap(this->cards[i],this->cards[rand()%CARDSCOUNT]);
    }
     //分牌 
    for(k=i=0;i<PLAYERCOUNT;i++){
        this->player[i]->cards.clear();
        for(j=0;j<17;j++){
            this->player[i]->cards.push_back(this->cards[k++]);
        }
        this->player[i]->arrange();//整理 
        this->player[i]->print();
    }
}
bool check(CardGroup *cardGroup){
    if(cardGroup->type.typeId==ERROR){
        cout<<"出牌错误，重新输入\n";
        return false;
    }else if(cardGroup->type.typeStr=="不出"){
        return true;
    }else if(LastCards::inst()->player!=player[CURRENTPLAYER]&&!cardGroup->isCanBeat(LastCards::inst()->cardGroup)){
        cout<<"打不过，重新输入\n";
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
	printf("                   ■");
	color(10);
	printf("■\n");
	color(11);
	printf("                   ■");
	color(14);
	printf("■");
}
 
void logoing()
{
	color(12);
	printf("                   ■");
	_sleep(500);
	color(10);
	printf("■\n");
	_sleep(500);
	color(11);
	printf("                   ■");
	_sleep(500);
	color(14);
	printf("■");
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
	printf("欢迎使用");
	printf("多功能程序");
	SetPos(0,20);
	printf("-------------------------------------------\n");
	printf("       版权所有 Fj & OwenLMZ & a97");
	
	
	color(12);
	SetPos(1,7);
	printf("┌───────────────┐\n");
	printf(" │               │\n");
	printf(" │  计算器[a]    │\n");
	printf(" │               │\n");
	printf(" └───────────────┘\n");
	
	color(10);
	SetPos(23,7);
	printf("┌───────────────┐\n");
	SetPos(23,8);
	printf("│               │\n");
	SetPos(23,9);
	printf("│ 各种小游戏[s] │\n");
	SetPos(23,10);
	printf("│               │\n");
	SetPos(23,11);
	printf("└───────────────┘\n");
	
	color(11);
	SetPos(1,13);
	printf("┌───────────────┐\n");
	printf(" │               │\n");
	printf(" │  去抄题解[d]  │\n");
	printf(" │               │\n");
	printf(" └───────────────┘\n");
	
	color(14);
	SetPos(23,13);
	printf("┌───────────────┐\n");
	SetPos(23,14);
	printf("│               │\n");
	SetPos(23,15);
	printf("│  系统操作[f]  │\n");
	SetPos(23,16);
	printf("│               │\n");
	SetPos(23,17);
	printf("└───────────────┘\n");
	
	Pos();
}
void calculator1()
{
	system("cls");
	printf("\n\n\n\n\n");
	logoing();
	system("cls");
	color(11);
	printf("|计算器|输入1!1退出\n");
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
	printf("|高精度加法|输入   退出\n");
	color(11);
	printf("每次输入两行，一行一个数，自动相加\n\n");
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
			a[a1_len-i]=a1[i]-48; //将操作数放入a数组 　
		}
		for (i=0;i<=b1_len-1;i++)
		{
			b[b1_len-i]=b1[i]-48; //将操作数放入b数组 
		} 
  	  	lenc =1; 
		x=0; 
 	   	while(lenc <=a1_len || lenc <=b1_len) 
 	   	{ 
			c[lenc]=a[lenc]+b[lenc]+x; //两数相加 　
			x=c[lenc]/10;
			c[lenc]=c[lenc]%10;
			lenc++;
		}
		c[lenc]=x;
		if (c[lenc]==0)
		{
			lenc--; //处理最高进位  
		} 
		for (i=lenc;i>=1;i--)
		{
			O<<c[i]; //输出结果 	
		}
		O<<endl<<endl;
	}
}
 
void calculator2_2()
{
	system("cls");
	color(14);
	SetPos(1,1);
	printf("|高精度减法|输入   退出\n");
	color(11);
	printf("每次输入两行，一行一个数，自动相减\n\n");
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
	printf("|高精度乘法|输入   退出\n");
	color(11);
	printf("每次输入两行，一行一个数，自动相乘\n\n");
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
	printf("|高精度计算器|按   返回");
	color(11);
	SetPos(17,1);
	printf("[`]");
	
	color(14);
	SetPos(1,3);
	printf("|高精度加法| [s]");
	SetPos(1,5);
	printf("|高精度减法| [d]");
	SetPos(1,7);
	printf("|高精度乘法| [f]");
	
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
	printf("┌───────────────┐\n");
	printf("│ 普通计算器[f] │\n");
	printf("└───────────────┘");
	
	color(14);
	SetPos(20,1);
	printf("┌───────────────┐\n");
	SetPos(20,2);
	printf("│ 高精计算器[f] │\n");
	SetPos(20,3);
	printf("└───────────────┘");
	
	color(8);
	SetPos(1,5);
	printf("按   返回。");
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
				MessageBox(0,"你撞到了墙上/炸弹上，输掉了游戏","   警告",MB_OK);
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
				MessageBox(0,"你撞到了墙上/炸弹上，输掉了游戏","   警告",MB_OK);
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
				MessageBox(0,"你撞到了墙上/炸弹上，输掉了游戏","   警告",MB_OK);
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
				MessageBox(0,"你撞到了墙上/炸弹上，输掉了游戏","   警告",MB_OK);
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
			MessageBox(0,"取到宝箱","提示",MB_OKCANCEL);
		}
		if(x == 0 && y == 14 && maze[9][20] == ' '){
			string steps = "走出迷宫，使用步数为:";			//messagebox显示使用步数 
			char sum[100]; 
			itoa(count,sum,10);							//将记录玩家步数的变量sum，转成字符串，赋值给iToA数组 
			steps += sum; 
			MessageBox(0,steps.c_str(),"提示",MB_OKCANCEL);//将获得的字符串显示在messagebox上 
			break; 
		}
		if(x<0){
			MessageBox(0,"请重新开始","   警告",MB_OK);
			break; 
		}	
	}	
	Sleep(2000);
}
 
void game2()
{
	color(6);
	int num[5][5] = {0};//16张卡牌 
	int arr[5][5] = {0};//标记卡牌是否被翻出 
	int ix,iy,ia,ib; 
	int cnt = 0;
	int flag[9] = {0};//1-8在num数组中出现的次数 
	int temp;//随机生成的数字 
	O<<"记忆卡牌规则:"<<endl;
	O<<"现有16张卡牌，由数字1-8组成，每个数字出现2次"<<endl;
	O<<"游戏者需要将卡牌翻至背面，找出所有相同的数字"<<endl;
	O<<"每一轮只允许翻两张卡牌"<<endl;
	O<<"如果卡牌数字相同，则将卡牌上有数字的一面显示出来"<<endl;
	O<<"否则将卡牌翻回正面，进行下一轮"<<endl;
	O<<"系统测量游戏者找出所有相同数字的卡牌所花费的步数(进行多少轮)"<<endl;
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
		O<<"请输入第一次翻开卡片的行数和列数:"<<endl;
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
		O<<"请输入第二次翻开卡片的行数和列数:"<<endl;
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
			O<<"恭喜您，此轮翻牌成功"<<endl;
		}
		else{
			O<<"很遗憾，此轮翻牌失败"<<endl;
			arr[ix][iy]--;
			arr[ia][ib]--;
		}
		cout<<"2秒后进入下一轮"<<endl;
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
			O<<"一共使用"<<cnt<<"步"<<endl;
			break;
		} 
	}	
	Sleep(2000); 
}
void game3(){
	outArr();
	int x=1,y=1,count=0;
	while(true){				
		O<<"请输入移动方向(wasd)："<<endl;
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
			string steps = "完成数字华容道，使用步数为:";			//messagebox显示使用步数 
			char sum[100]; 
			itoa(count,sum,10);							//将记录玩家步数的变量sum，转成字符串，赋值给iToA数组 
			steps += sum; 
			MessageBox(0,steps.c_str(),"提示",MB_OK);//将获得的字符串显示在messagebox上 
		//MessageBox(0,"恭喜你完成数字华容道","提示",MB_OK);
		break; 
	}		
}
}
void game4(){
//	显示规则
	O << "这是一个武侠的世界" << endl;
	Sleep(500);
	O << "为了参加天下第一武道大会" << endl;
	Sleep(500);
	O << "玩家需要操作小码君进行为期一年的特训" << endl;
	Sleep(500);
	O << "只有总属性超过500才能参加大会" << endl;
	Sleep(500);
	O << "在一年中,每个月小码君都可以进行一次探险" << endl << endl;
	Sleep(500);
	O << "挖矿：增加攻击力和金钱，但是会减少少量内功" << endl;
	Sleep(500);
	O << "闭关：随机增加或减少攻击和内功，但是会消耗金钱" << endl;
	Sleep(500);
	O << "吃饭：增加大量生命，但是会消耗很多金钱" << endl;
	Sleep(500);
	O << "闯荡：出门闯荡，触发战斗" << endl;
	Sleep(500);
	O << "闲逛：购买装备增加自身属性，但是会消耗金钱" << endl << endl;
	Sleep(500);	
	O << "准备好开始冒险了吗？" << endl;
	system("pause");
	
//闲逛环节增加装备详情 
	//装备的名称
	char item[10][9] = {"铁剑", "蓝水晶", "布甲",
					"大剑", "红玛瑙", "贤者之石",
					"风暴巨剑", "力量腰带", "霸者重装",
					"破军"};
	//装备价格 
	int prices[10] = {10, 10, 10, 20, 20, 20, 50, 50, 100, 100};
	//装备增加的属性
	int itemAtt[10][4] = {{10, 0, 0, 0}, {0, 0, 10, 0}, {0, 0, 0, 10},
						  {20, 0, 0, 0}, {0, 20, 0, 0}, {0, 10, 10 , 0},
					      {50, 0, 0, 0}, {0, 50, 0, 0}, {0, 100, 0, 0}, 
					      {100, 0, 0, 0}};		
	 
	int itemList[10];
	for(int i = 0;i < 10;++i){         //将装备列表里得所有元素设为-1 
		itemList[i] = -1;
	}
//闯荡环节增加敌人详情 
	//敌人名称 
	char enemyName[5][5] = {"毛贼", "强盗", "杀手", "头目", "擂主"};	
	//敌人属性 
	int enemyAtt[5][4] = {{60, 60, 50, 0},
					   	  {70, 60, 60, 0},
					      {90, 60, 50, 20},
					      {90, 70, 80, 30},
					      {100, 70, 90, 50}};	
	//敌人赏金 
	int enemyMoney[5] = {30, 30, 50, 50, 60};
//第一步，显示四个随机生成的属性并选定角色属性-------------------------------------------------------------------------------------	
	//玩家基本属性 
	char playAtt[6][5] = {"攻击", "血量", "内功", "防御", "金钱"};
	//人物属性 
	int player[6] = {};//0.攻击 1.血量 2.内功 3.防御 4.金钱 
	//初始化人物
	srand(time(0));
	while(true){
		system("cls");
		O << "请选择你的初始属性：" << endl;
		//随机角色的4个属性
		for(int i = 0;i < 4;++i){
			player[i] = rand()%50+40;
		}
		player[4]=50; 
		//显示角色的4个属性
		O << "当前角色信息" << endl;
		for(int i = 0;i < 5;++i){
			if(player[i] >= 120) color(6);
			else if(player[i] >= 80) color(4);
			O<<playAtt[i]<<":"<<player[i]<<endl;
			color(7);
		}
		char user;//定义一个user，用来确定角色属性
		O << "是否选择初始属性？（y/n）" << endl;
		user = getch();
		if(user == 'y'){
			break;
		}
	} 
//第二步，创建角色并显示当前确定的角色属性-------------------------------------------------------------------------------------
	//创建角色ing（此处改为了循环！！！！！！！！！！！！！！！！！！！！！） 
	system("cls");
	O << "正在创建角色" << endl;
	for(int i=3;i>=1;--i){
		Sleep(1000);
		cout << i << endl;
	} 
	O << "角色创建成功！！" << endl;
	//显示角色信息
	O << "当前角色信息" << endl;
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
//第三步，12个月的修炼-------------------------------------------------------------------------------------	
	//当前月份 
	int month = 1;
	while(month <= 12){
		system("cls");
		while(true){
			O << "现在是" << month << "月" << endl;
			O << "q 查看属性" << endl;
			O << "a 挖矿 " << endl; 
			O << "s 闭关 " << endl;
			O << "d 吃饭 " << endl;
			O << "f 闯荡 " << endl;  
			O << "g 闲逛 " << endl; 						
			O << "请输入："; 
			char choose;//用来输入选项
			I >> choose; 
			if(choose == 'q'){				
				system("cls");
				//显示角色属性
				O << "当前角色信息" << endl;
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
				//显示装备
				O << "当前装备：" << endl;
				if(itemList[0] == -1){
					O << "没有装备" << endl;
				}
				for(int i = 0;i < 10;++i){
					if(itemList[i] == -1){
						break;
					}
					//显示装备颜色
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
				O << "小码君疯狂挖矿中" << endl;
				O << "嘿咻！！嘿咻！！金钱+5" << endl;
				Sleep(800);
				O << "嘿咻！！嘿咻！！金钱+5" << endl;
				Sleep(800);
				O << "小码君加强了麒麟臂！！攻击+10" << endl;
				Sleep(800);
				O << "小码君荒废了练功，内功-5" << endl;
				Sleep(800);
				player[4] += 10;
				player[0] += 10;
				player[2] -= 5;
				break; 
			}	
			else if(choose == 's'){
				O << "小码君疯狂修炼中" << endl;
				char event[2][100]= {"感悟天道，功力大增，攻击+20，内功+20",
					  			"走火入魔，功力大减，攻击-15，内功-15"}; 
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
				O << "小码君贼能吃" << endl;
				for(int i = 0;i < 3;++i){
					int temp = rand()%20+1;
					O << "小码君疯狂吃东西" << endl;
					Sleep(2000);
					O << "血量+" << temp;
					O << ",金钱-" << temp/2+1  << endl;
					player[1] += temp;
					player[4] -= temp / 2+1;
				}
				break; 
			}
			else if(choose == 'f'){
				//闯荡
				O << "欢迎进行闯荡，发现敌人：" <<endl;
				int temp = rand()%5; 
				if(enemyAtt[temp][1] <= 0){
					temp = rand()%5;
				} 
				O << enemyName[temp] << endl; 				
				for(int i = 0;i<4;++i){ 
					O << playAtt[i] <<" : "<< enemyAtt[temp][i]<<endl;
				}
				O<<"赏金 : " << enemyMoney[temp] << endl; 
				
				O << "当前角色信息" << endl;
				for(int i = 0;i < 4;++i){
					if(player[i] >= 120){
						color(6);
					}else if(player[i] >= 80){
						color(12);	
					}else{
						color(15);
					}
					O << playAtt[i] << " ：" << player[i] << endl;
					color(15); 
				}
				O<<"是否迎战？(y/n)"<<endl; 
				char user;
				I >> user;
				if(user == 'y'){
					if(enemyAtt[temp][2]>player[2]){
						O << "内功不够，被打劫了。。金币-20" << endl;
						player[4] -= 20;
					}else{
						while(enemyAtt[temp][1] > 0){
							if(player[1] <= 0){
								break;
							}
							int attack1 =  enemyAtt[temp][0] - player[3];
							if(attack1<0) attack1 = 0;
							cout<< enemyName[temp] << "发动攻击,造成" << attack1 << "伤害" <<endl;
							player[1] -= attack1;							
							int attack2 = player[0] - enemyAtt[temp][3];
							if(attack2<0) attack2=0; 
							cout << "小码君发动攻击,造成" << attack2 << "伤害"<<endl;
							enemyAtt[temp][1] -= attack2;	
						}
						if(player[1]>0){
							O<<"战斗胜利！！！！！获得赏金"<< enemyMoney[temp]<<endl; 
							player[4] += enemyMoney[temp];
							//拓展：战斗胜利后，除赏金外，小码君获得其他属性增益 
							//int att = rand()%4;
							//int value = (rand()%3+1) * temp;
							//cout<< playAtt[att]<<"增加"<<value<<endl;
							//player[att]+=value;
						}
					} 
				}
				else{
					O<<"破财免灾，金钱减少10"<<endl;
					player[4]-=10; 
				}
				break;
			}
			else if(choose == 'g'){
				//闲逛
				O << "小码君跋山涉水来到商店" << endl;
				O << "当前金钱：" << player[4] << endl;	
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
					O << "价格:" << prices[index[i]] <<endl;
					O << "   "<<"装备属性值：";
					for(int j=0;j<4;++j){
						cout << itemAtt[index[i]][j] << "  ";
					} 
					O << endl;
					color(15);
				}
				O << "请问你要购买几号装备？" << endl;
				int user;
				I >> user;
				
				if(user>=1 && user<=5 && player[4]>=prices[index[user-1]]){
					//买装备
					O << "成功购买" << item[index[user-1]] << endl;
					player[4] -= prices[index[user-1]];
					//添加物品					
					for(int i = 0;i < 10;++i){   //将买完的装备放到装备列表中 
						if(itemList[i] == -1){
							itemList[i] = index[user-1];
							break;
						}
					}
					
					for(int i = 0;i < 4;++i){                                                                                      //根据装备属性，提升角色对应属性 
						if(itemAtt[index[user-1]][i] != 0){
							O <<"获得"<< playAtt[i]<<itemAtt[index[user-1]][i]<<"点"<<endl;
						}
						player[i] += itemAtt[index[user-1]][i];
					}
				}
				else{
					O << "你是来砸场子的吧！！!不欢迎你!"<<endl; 
				} 
				break;
			}
			
			else{
				system("cls");
				O<<"暂未开通此功能，请重新输入"<<endl;
			}
		} 
		system("pause");  
		++month;
		for(int i=0;i<5;++i){
			if(player[i] < 0){
				O<<playAtt[i]<<"不足，游戏结束"<<endl; 
			}
			if(player[i]<=10){
				MessageBox(0,"请及时查看属性值！","警告",MB_ICONEXCLAMATION);
				break;
			}			
		}
	}
	O << "当前角色信息" << endl;
	for(int i = 0;i < 5;++i){
		if(player[i] >= 120){
			color(6);
		}else if(player[i] >= 80){
			color(12);	
		}else{
			color(15);
		}
		O << playAtt[i] << " ：" << player[i] << endl;
		color(15); 
	}
	int sum=0;
	for(int i=0;i<5;++i){
		sum += player[i];
	} 
	O << "总属性值为：" << sum <<endl; 
	system("pause");
	O << "一年后......" <<endl; 
	//12个月后，游戏胜利
	if(sum >= 500){
		if(player[0] >= 100){
			MessageBox(0,"恭喜获得攻击达人称号","游戏胜利",MB_OK);
		}
		if(player[1] >= 100){
			MessageBox(0,"恭喜获得血量达人称号","游戏胜利",MB_OK);
		} 
		if(player[2] >= 100){
			MessageBox(0,"恭喜获得内功达人称号","游戏胜利",MB_OK);
		} 
		if(player[3] >= 100){
			MessageBox(0,"恭喜获得防御达人称号","游戏胜利",MB_OK);
		} 
		if(player[4] >= 100){
			MessageBox(0,"恭喜获得金钱达人称号","游戏胜利",MB_OK);
		}
	}
	//12个月后，游戏失败
	if(sum < 500){
		MessageBox(0,"请来年再战！","游戏失败",MB_OK);
	} 
	Sleep(2000);
} 
void game5(){
	Landlords *landlords=new Landlords();
	do{
	    landlords->init();//发牌、抢地主 
	    landlords->startGame();//游戏开始 
	}while(makeChoice("\n是否继续游戏？（Y=继续/N=结束）: "));
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
	printf("┌───────────────────┐\n");
	printf("│ 勇闯迷宫[d]       │\n");
	printf("└───────────────────┘\n");
	color(10);
	SetPos(25,1);
	printf("┌───────────────────┐\n");
	SetPos(25,2);
	printf("│ 记忆卡牌[f]       │\n");
	SetPos(25,3);
	printf("└───────────────────┘\n");
	color(8);
	SetPos(50,1);
	printf("┌──────── ───┐\n");
	SetPos(50,2);
	printf("│ 数字华容道[h]        │\n");
	SetPos(50,3);
	printf("└──────── ─ ─ ┘\n");	
	color(4);
	SetPos(70,1);
	printf("┌───     ───┐\n");
	SetPos(70,2);
	printf("│ 小码江湖[j]     │\n");
	SetPos(70,3);
	printf("└── ──────┘\n");		
	color(6);
	SetPos(80,1);
	printf("┌─── ───┐\n");
	SetPos(80,2);
	printf("│ 斗地主[k]   │\n");
	SetPos(80,3);
	printf("└── ────┘\n");	
	color(3);
	printf("按   返回");
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
	printf("┌────────────┐\n");
	SetPos(1,2);
	printf("│ 暂不支持！ │\n");
	SetPos(1,3);
	printf("└────────────┘\n");
	_sleep(1000);
}
 
void solution3()
{
	system("cls");
	_sleep(500);
	color(12);
	SetPos(1,1);
	printf("┌────────────┐\n");
	SetPos(1,2);
	printf("│ 暂不支持！ │\n");
	SetPos(1,3);
	printf("└────────────┘\n");
	_sleep(1000);
}
 
void Solution()
{
	loop_solution_head:
	system("cls");
	
	color(14);
	SetPos(1,1);
	printf("┌───────────────┐\n");
	SetPos(1,2);
	printf("│ 某本通题解[s] │\n");
	SetPos(1,3);
	printf("└───────────────┘\n");
	color(13);
	SetPos(1,4);
	printf("┌───────────────┐\n");
	SetPos(1,5);
	printf("│  某谷题解[d]  │\n");
	SetPos(1,6);
	printf("└───────────────┘\n");
	color(11);
	SetPos(1,7);
	printf("┌───────────────┐\n");
	SetPos(1,8);
	printf("│  某扣题解[f]  │\n");
	SetPos(1,9);
	printf("└───────────────┘\n");
	color(8);
	SetPos(1,11);
	printf("按   返回。");
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
	printf("\n 准备中");
	_sleep(500);
	system("cls");
	printf("\n 正在与Windows系统取得联系");
	_sleep(1200);
	system("cls");
	printf("\n 正在与系统建立关联");
	_sleep(2000);
	system("cls");
	printf("\n 正在启用管理员权限");
	_sleep(800);
	system("cls");
	printf("\n 正在与计算机硬件建立关联");
	_sleep(2400);
	system("cls");
	printf("\n 准备以启动");
	_sleep(1000);
	system("cls");
	
	color(12);
	SetPos(3,1);
	printf("┌────────────┐\n");
	SetPos(3,2);
	printf("│   关机[A]  │\n");
	SetPos(3,3);
	printf("└────────────┘\n");
	color(10);
	SetPos(3,4);
	printf("┌────────────┐\n");
	SetPos(3,5);
	printf("│   重启[S]  │\n");
	SetPos(3,6);
	printf("└────────────┘\n");
	color(11);
	SetPos(3,7);
	printf("┌────────────┐\n");
	SetPos(3,8);
	printf("│   注销[D]  │\n");
	SetPos(3,9);
	printf("└────────────┘\n");
	color(14);
	SetPos(3,10);
	printf("┌────────────┐\n");
	SetPos(3,11);
	printf("│   休眠[F]  │\n");
	SetPos(3,12);
	printf("└────────────┘\n");
	color(8);
	SetPos(1,14);
	printf("按   返回。");
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
