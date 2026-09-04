#include<iostream>
#include<string>
#include<ctime>
#include<random>
#include<algorithm>
#include<Windows.h>
#include"drawCard.h" //卡牌的可视化

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

using namespace std;
HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

class Player{
public:
	Player(int num,bool pvpMode=false);
	~Player(){};
	void handout(int num);
	void print();
	void printPlayerCards();
	void simplePrint(int n);
	void layOut();
	void layOut(int mode);
	void run();
	void dealNPC(int m,int n);
	void dealPlayer();
	void dealPlayer(int player);
	
	//游戏结束
	int flag_over;
private:
	bool pvpMode;
	int pvpFirstPlayer;
	string pvpLastMove[4];
	int pvpHistoryCount;
	int pvpHistoryPlayer[60];
	string pvpHistoryText[60];
	int layoutMode[4];
	int flag_7;
	int cards[4][26];
	int playerNum;
	int point[4];
	int isCardhanded[53];
	int availableCards[53];
	int usedCards[53];
	int isCardFolded[53];
	int flag_champion_num;

	//for static parameters in printPlayerCards() function
	int flag_used_times;
	int flag_heart;
	int flag_diamond;
	int flag_spade;
	int flag_club;

	void runPVP();
	void showPVPResult();
	void addPVPHistory(int player,string text);
	int readPlayChoice(int player);
	void switchLayoutMode(int player);
	void printHand(int player);
	void pvpTransition(int nextPlayer);
	int cardOwner(int card);
	void getOpenedCards(int card,int opened[],int& count);
	int aiPlayScore(int player,int card);
	int aiFoldScore(int card);
	int handCount(int player);
};

int random(int n,int m){
	double base=((double)rand())/RAND_MAX;
	int res=n+(base*(double)(m-n+1));
	return res;
}

void checkExit(int value){
	if(value==-1){
		cout<<"游戏结束，感谢游玩！"<<endl;
		system("pause");
		exit(0);
	}
}

bool parseInteger(const string& input,int& value){
	if(input.empty()) return false;
	long long result=0;
	for(int i=0;i<(int)input.size();i++){
		char ch=input[i];
		if(ch<'0'||ch>'9') return false;
		result=result*10+(ch-'0');
		if(result>2147483647) return false;
	}
	value=(int)result;
	return true;
}

int readIntInput(){
	string input;
	while(true){
		cin>>input;
		if(!cin){
			bool isEOF=cin.eof();
			cin.clear();
			cin.ignore(10000,'\n');
			if(isEOF) checkExit(-1);
			cout<<"输入格式错误，请重新输入："<<endl;
			continue;
		}
		if(input=="-1") checkExit(-1);
		int value;
		if(parseInteger(input,value)) return value;
		cout<<"输入格式错误，请重新输入："<<endl;
	}
}

int cardRank(int card){
	int num=card%13;
	if(num==0) num=13;
	return num;
}

string cardText(int card){
	string suit;
	if(card>=1&&card<=13) suit="红桃";
	else if(card>=14&&card<=26) suit="方片";
	else if(card>=27&&card<=39) suit="黑桃";
	else if(card>=40&&card<=52) suit="梅花";
	int num=card%13;
	if(num==0) num=13;
	return suit+to_string(num);
}

// ========== AI 策略参数（可按需要调整） ==========
// 出牌评分权重
const int AI_PLAY_HIGH_BONUS        = 1;  // 点数高于 7 越大的牌越优先处理
const int AI_EDGE_SAFE_BONUS        = 4;  // A/K 不新开口，属于安全牌
const int AI_KEEP_SEVEN_PENALTY     = 10; // 非必要不出 7，保留 7 可以“憋”住其他玩家
const int AI_OPEN_SELF_BONUS        = 3;  // 打出后能让自己后续接上的牌，加分
const int AI_OPEN_OPPONENT_PENALTY  = 8;  // 打出后可能让对手接上的牌，扣分
const int AI_PLAY_RANDOM_TOLERANCE  = 4;  // 出牌时在“接近最优”的候选中随机，避免每局雷同
const int AI_FOLD_RANDOM_TOLERANCE  = 1;  // 扣牌时在“接近最优”的低分候选中随机

Player::Player(int num,bool pvp):playerNum(num){
	pvpMode=pvp;
	pvpFirstPlayer=0;
	for(int i=0;i<4;i++){
		pvpLastMove[i]="";
	}
	pvpHistoryCount=0;
	for(int i=0;i<4;i++){
		layoutMode[i]=0;
	}
	flag_7=0;
	flag_over=0;
	flag_champion_num=0;
	//static parameters
	flag_used_times=0;
	flag_heart=0;
	flag_diamond=0;
	flag_spade=0;
	flag_club=0;
	for(int i=0;i<4;i++){
		point[i]=0;
	}
	for(int i=0;i<53;i++){
		usedCards[i]=0;
		availableCards[i]=0;
		isCardhanded[i]=0;
		isCardFolded[i]=0;
	}
	availableCards[7]=1;
}

void Player::handout(int num){
	for(int i=0;i<num;i++){
		for(int j=0;j<52/num;j++){
			int c=random(1,52);
			while(isCardhanded[c]==1||(num==3&&c==7)) c=random(1,52);
			cards[i][j]=c;
			isCardhanded[c]=1;
		}
		sort(cards[i],cards[i]+52/playerNum);
	}
}

void Player::printPlayerCards(){
	cout<<"你的卡槽为:"<<endl;
	for(int j=0;j<52/playerNum;j++){
		if(isCardhanded[cards[0][j]]==1){

		if(cards[0][j]>=1&&cards[0][j]<=13){
				if(flag_heart==flag_used_times){cout<<"红桃:";
				flag_heart++;}
			}
		if(cards[0][j]>=14&&cards[0][j]<=26){
				if(flag_diamond==flag_used_times){cout<<"方片:";
				flag_diamond++;}
			}
		if(cards[0][j]>=27&&cards[0][j]<=39){
				if(flag_spade==flag_used_times){cout<<"黑桃:";
				flag_spade++;}
			}
		if(cards[0][j]>=40&&cards[0][j]<=52){
				if(flag_club==flag_used_times){cout<<"梅花:";
				flag_club++;}
			}
		int num=cards[0][j]%13;
		if(num==0) num=13;
		if(availableCards[cards[0][j]]==1) SetConsoleTextAttribute(hConsole,10);
		cout<<num<<"("<<j<<")  ";
		SetConsoleTextAttribute(hConsole,7);

		}

	}
	cout<<endl;
	flag_used_times++;
}

void Player::print(){
	cout<<"本局玩家:"<<playerNum<<"人。"<<endl;
	cout<<"牌面:"<<endl;
	for(int i=0;i<playerNum;i++){
		cout<<"玩家"<<i+1<<":";
		bool printedHearts = false;
		bool printedDiamonds = false;
		bool printedSpades = false;
		bool printedClubs = false;
		for(int j=0;j<52/playerNum;j++){
			if(cards[i][j]>=1&&cards[i][j]<=13){
				if(!printedHearts){
					cout<<"红桃:";
					printedHearts=1;
				}
			}
			else if(cards[i][j]>=14&&cards[i][j]<=26){
				if(!printedDiamonds){
					cout<<"方片:";
					printedDiamonds=1;
				}
			}
			else if(cards[i][j]>=27&&cards[i][j]<=39){
				if(!printedSpades){
					cout<<"黑桃:";
					printedSpades=1;
				}
			}
			else if(cards[i][j]>=40&&cards[i][j]<=52){
				if(!printedClubs){
					cout<<"梅花:";
					printedClubs=1;
				}
			}
			int num=cards[i][j]%13;
				if(num==0) num=13;
				if(isCardhanded[cards[i][j]]==1) cout<<num<<", ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void Player::simplePrint(int n){
	if(n>=1&&n<=13) cout<<"红桃";
	else if(n>=14&&n<=26) cout<<"方片";
	else if(n>=27&&n<=39) cout<<"黑桃";
	else if(n>=40&&n<=52) cout<<"梅花";
	else cout<<n<<":";
	int num=n%13;
	if(num==0) num=13;
	cout<<num;
}

void Player::layOut() {
	layOut(layoutMode[0]);
}

void Player::layOut(int mode) {
	if(mode==1){
		// 旧文字牌面展示：保留自之前注释掉的旧代码
		cout << "目前场上牌型:";
		SetConsoleTextAttribute(hConsole,11);
		int st = 0;
		for (int i = 1; i < 53; i++) {
			if (isCardhanded[i] == 0 && isCardFolded[i] == 0) {
				usedCards[st] = i;
				st++;
			}
		}
		sort(usedCards, usedCards + st);
		bool printedHearts = false;
		bool printedDiamonds = false;
		bool printedSpades = false;
		bool printedClubs = false;
		for (int i = 0; i < st; i++) {
			int card = usedCards[i];
			int num = card % 13;
			if (num == 0) num = 13; // 处理余数为0的情况

			if (card >= 1 && card <= 13) {
				if (!printedHearts) {
					cout << "\n红桃:";
					printedHearts = true;
				}
			} else if (card >= 14 && card <= 26) {
				if (!printedDiamonds) {
					cout << "\n方片:";
					printedDiamonds = true;
				}
			} else if (card >= 27 && card <= 39) {
				if (!printedSpades) {
					cout << "\n黑桃:";
					printedSpades = true;
				}
			} else if (card >= 40 && card <= 52) {
				if (!printedClubs) {
					cout << "\n梅花:";
					printedClubs = true;
				}
			}

			cout << num << "  ";
		}
		cout << endl;
		SetConsoleTextAttribute(hConsole,7);
	} else {
		// 图形牌面展示（当前使用的 drawCard 方案）
		cout<<"目前场上牌型："<<endl;
		int st=0;
		for(int i=0;i<53;i++){
			if(isCardhanded[i]==0&&isCardFolded[i]==0){
				usedCards[st++]=i;
			}
		}
		sort(usedCards,usedCards+st);
		bool heartsExist=0,diamondExist=0,spadesExist=0,clubExist=0;
		int heart[13]={0},diamond[13]={0},spade[13]={0},club[13]={0};
		int st1=0,st2=0,st3=0,st4=0;
		for(int i=0;i<st;i++){
			if(usedCards[i]>=1&&usedCards[i]<=13) heart[st1++]=usedCards[i];
			else if(usedCards[i]>=14&&usedCards[i]<=26) diamond[st2++]=usedCards[i];
			else if(usedCards[i]>=27&&usedCards[i]<=39) spade[st3++]=usedCards[i];
			else if(usedCards[i]>=40&&usedCards[i]<=52) club[st4++]=usedCards[i];
		}
		if(heart[0]!=0) drawCard(heart,0,--st1);
		if(diamond[0]!=0) drawCard(diamond,0,--st2);
		if(spade[0]!=0) drawCard(spade,0,--st3);
		if(club[0]!=0) drawCard(club,0,--st4);
	}
}

int Player::cardOwner(int card){
	if(isCardhanded[card]!=1) return -1;
	for(int i=0;i<playerNum;i++){
		for(int j=0;j<52/playerNum;j++){
			if(cards[i][j]==card) return i;
		}
	}
	return -1;
}

void Player::getOpenedCards(int card,int opened[],int& count){
	count=0;
	int rank=cardRank(card);
	if(rank==7){
		int left=card-1;
		int right=card+1;
		if(left>=1&&left<=52&&!isCardFolded[left]&&availableCards[left]==0) opened[count++]=left;
		if(right>=1&&right<=52&&!isCardFolded[right]&&availableCards[right]==0) opened[count++]=right;
	}
	else if(rank>=2&&rank<=6){
		int next=card-1;
		if(next>=1&&next<=52&&!isCardFolded[next]&&availableCards[next]==0) opened[count++]=next;
	}
	else if(rank>=8&&rank<=12){
		int next=card+1;
		if(next>=1&&next<=52&&!isCardFolded[next]&&availableCards[next]==0) opened[count++]=next;
	}
}

int Player::aiPlayScore(int player,int card){
	int score=0;
	int rank=cardRank(card);

	// 高点数牌如果一直留在手里，一旦被迫扣牌会扣很多分，因此适当优先处理。
	score += (rank-7)*AI_PLAY_HIGH_BONUS;

	// A/K 是边缘牌，打出不会给对手打开新路，属于较安全的出牌。
	if(rank==1||rank==13){
		score += AI_EDGE_SAFE_BONUS;
	}

	// 7 是“安全牌/关键牌”，打出后会打开一整条花色，通常应尽量保留以限制对手。
	if(rank==7){
		score -= AI_KEEP_SEVEN_PENALTY;
	}

	// 看这张牌会为谁打开新的可出牌。
	int opened[2]={0};
	int openCount=0;
	getOpenedCards(card,opened,openCount);
	for(int i=0;i<openCount;i++){
		int owner=cardOwner(opened[i]);
		if(owner==player){
			score += AI_OPEN_SELF_BONUS;
		}
		else if(owner>=0){
			score -= AI_OPEN_OPPONENT_PENALTY;
		}
	}
	return score;
}

int Player::aiFoldScore(int card){
	// 扣牌分数 = 点数，越小越好；这里返回负分便于统一按“高分优先”选取。
	int rank=cardRank(card);
	return -rank;
}

int Player::handCount(int player){
	int count=0;
	for(int j=0;j<52/playerNum;j++){
		if(isCardhanded[cards[player][j]]==1) count++;
	}
	return count;
}

void Player::dealNPC(int m,int n){
	for(int i=m;i<n;i++){
		bool cardPlayed=0;

		// 收集所有可出的牌并评分
		int candidates[26]={0};
		int scores[26]={0};
		int candidateCount=0;
		int bestScore=-1000000000;
		for(int j=0;j<52/playerNum;j++){
			int c=cards[i][j];
			if(isCardhanded[c]==1&&availableCards[c]==1){
				int s=aiPlayScore(i,c);
				candidates[candidateCount]=c;
				scores[candidateCount]=s;
				if(s>bestScore) bestScore=s;
				candidateCount++;
			}
		}

		int finalDecision=0;
		if(candidateCount>0){
			// 在“接近最优”的候选中加入适度随机，避免每局完全相同，也避免明显差牌。
			int top[26]={0};
			int topCount=0;
			for(int k=0;k<candidateCount;k++){
				if(scores[k]>=bestScore-AI_PLAY_RANDOM_TOLERANCE){
					top[topCount++]=candidates[k];
				}
			}
			finalDecision=top[random(0,topCount-1)];
		}

		if(finalDecision!=0){
			availableCards[finalDecision]=0;
			if(finalDecision%13<=6&&finalDecision%13>1){
				availableCards[finalDecision-1]=isCardFolded[finalDecision-1]?0:1;
			}
			else if(finalDecision%13>=8&&finalDecision%13!=0){
				availableCards[finalDecision+1]=isCardFolded[finalDecision+1]?0:1;
			}
			else if(finalDecision%13==7){
				availableCards[finalDecision-1]=isCardFolded[finalDecision-1]?0:1;
				availableCards[finalDecision+1]=isCardFolded[finalDecision+1]?0:1;
			}
			isCardhanded[finalDecision]=0;
			cout<<"玩家"<<i+1<<"已出牌:";
			simplePrint(finalDecision);
			cout<<endl;
			cardPlayed=1;
		}

		int flag_done=0;
		for(int j=0;j<52/playerNum;j++){
			if(isCardhanded[cards[i][j]]==1) flag_done++;
		}

		if(!cardPlayed&&flag_done!=0){
			// 无牌可出时，在低分（点数小）的候选中随机，避免总是扣同一张。
			int foldCandidates[26]={0};
			int foldScores[26]={0};
			int foldCount=0;
			int bestFoldScore=-1000000000;
			for(int j=0;j<52/playerNum;j++){
				int c=cards[i][j];
				if(isCardhanded[c]==1){
					int s=aiFoldScore(c);
					foldCandidates[foldCount]=c;
					foldScores[foldCount]=s;
					if(s>bestFoldScore) bestFoldScore=s;
					foldCount++;
				}
			}
			int topFold[26]={0};
			int topFoldCount=0;
			for(int k=0;k<foldCount;k++){
				if(foldScores[k]>=bestFoldScore-AI_FOLD_RANDOM_TOLERANCE){
					topFold[topFoldCount++]=foldCandidates[k];
				}
			}
			int finalDecision2=topFold[random(0,topFoldCount-1)];

			isCardhanded[finalDecision2]=0;
			isCardFolded[finalDecision2]=1;
			int num=cardRank(finalDecision2);
			point[i]+=num;
			SetConsoleTextAttribute(hConsole,13);
			cout<<"玩家"<<i+1<<"已扣牌。"<<endl;
			SetConsoleTextAttribute(hConsole,7);
		}
	}
}

void Player::dealPlayer(){
	printPlayerCards();
	int choice=readPlayChoice(0);
	int dealt=0;
	while(dealt==0){
		if(choice<0||choice>=52/playerNum||cards[0][choice]<=0||cards[0][choice]>52){
			cout<<"输入错误！请重新选择你要出的牌:"<<endl;
			choice=readPlayChoice(0);
		}
		else if(isCardhanded[cards[0][choice]]==0){
			cout<<"这张牌已经出过了！请重新输入你要出的牌:"<<endl;
			choice=readPlayChoice(0);
		}
		else if(availableCards[cards[0][choice]]!=1){
			cout<<"这张牌打不出去,要扣这张吗？(不要0/要1)"<<endl;
			int choiceFold=readIntInput();
			if(choiceFold==1){
				//检测有没有能出的牌
				int dealAbility=0;
				for(int j=0;j<52/playerNum;j++){
					if(availableCards[cards[0][j]]==1) dealAbility++;
				}
				if(dealAbility>0){
					cout<<"有可出的牌，无法扣牌！请重新输入你要出的牌:"<<endl;
					choice=readPlayChoice(0);
					continue;
				}
				int num=cards[0][choice]%13;
				if(num==0) num=13;
				point[0]+=num;
				SetConsoleTextAttribute(hConsole,4);
				system("cls");
				cout<<"你现在已经扣了"<<point[0]<<"分！"<<endl;
				SetConsoleTextAttribute(hConsole,7);
				dealt=1;
				isCardhanded[cards[0][choice]]=0;
				isCardFolded[cards[0][choice]]=1;
				availableCards[cards[0][choice]]=0;
			}
			else{
				cout<<"取消扣牌，请重新输入你要出的牌:"<<endl;
				choice=readPlayChoice(0);
			}
		}
		else{
			availableCards[cards[0][choice]]=0;
			if(cards[0][choice]%13<=6&&cards[0][choice]%13>1){
				availableCards[cards[0][choice]-1]=isCardFolded[cards[0][choice]-1]?0:1;
			}
			else if(cards[0][choice]%13>=8&&cards[0][choice]%13!=0){
				availableCards[cards[0][choice]+1]=isCardFolded[cards[0][choice]+1]?0:1;
			}
			else if(cards[0][choice]%13==7){
				availableCards[cards[0][choice]-1]=isCardFolded[cards[0][choice]-1]?0:1;
				availableCards[cards[0][choice]+1]=isCardFolded[cards[0][choice]+1]?0:1;
			}
			isCardhanded[cards[0][choice]]=0;
			dealt=1;
			system("cls");
			cout<<"玩家1已出牌:";
			simplePrint(cards[0][choice]);
			cout<<endl;
		}
	}
}


void Player::printHand(int player){
	cout<<"玩家"<<player+1<<"的手牌:"<<endl;
	bool printedHearts=false,printedDiamonds=false,printedSpades=false,printedClubs=false;
	for(int j=0;j<52/playerNum;j++){
		int c=cards[player][j];
		if(isCardhanded[c]!=1) continue;
		if(c>=1&&c<=13){
			if(!printedHearts){
				cout<<"红桃:";
				printedHearts=true;
			}
		}
		else if(c>=14&&c<=26){
			if(!printedDiamonds){
				cout<<"方片:";
				printedDiamonds=true;
			}
		}
		else if(c>=27&&c<=39){
			if(!printedSpades){
				cout<<"黑桃:";
				printedSpades=true;
			}
		}
		else if(c>=40&&c<=52){
			if(!printedClubs){
				cout<<"梅花:";
				printedClubs=true;
			}
		}
		int num=c%13;
		if(num==0) num=13;
		if(availableCards[c]==1) SetConsoleTextAttribute(hConsole,10);
		cout<<num<<"("<<j<<")  ";
		SetConsoleTextAttribute(hConsole,7);
	}
	cout<<endl;
}

void Player::switchLayoutMode(int player){
	layoutMode[player]=1-layoutMode[player];
	cout<<"已切换牌面显示方式，当前为："<<(layoutMode[player]==0?"图形":"文字")<<"模式"<<endl;
	layOut(layoutMode[player]);
	if(!pvpMode) printPlayerCards(); else printHand(player);
}

int Player::readPlayChoice(int player){
	while(true){
		cout<<"请选择你要出的牌:"<<endl;
		string input;
		cin>>input;
		if(!cin){
			bool isEOF=cin.eof();
			cin.clear();
			cin.ignore(10000,'\n');
			if(isEOF) checkExit(-1);
			cout<<"输入格式错误，请重新输入："<<endl;
			continue;
		}
		if(input=="-1") checkExit(-1);
		if(input=="c"||input=="C"){
			switchLayoutMode(player);
			continue;
		}
		int choice;
		if(parseInteger(input,choice)) return choice;
		cout<<"输入格式错误，请重新输入："<<endl;
	}
}

void Player::dealPlayer(int player){
	system("cls");
	SetConsoleTextAttribute(hConsole,14);
	cout<<"===== 轮到玩家"<<player+1<<" ====="<<endl;
	SetConsoleTextAttribute(hConsole,7);

	// 显示上一轮其他玩家已经出的牌；首轮无记录时正常留空
	// 必须按照实际出牌顺序显示，而不是按玩家编号显示
	cout<<"----- 上一轮其他玩家出牌 -----"<<endl;
	bool hasOtherMove=false;
	int lastSelfIndex=-1;
	for(int idx=0;idx<pvpHistoryCount;idx++){
		if(pvpHistoryPlayer[idx]==player) lastSelfIndex=idx;
	}
	int startIndex=0;
	if(lastSelfIndex!=-1) startIndex=lastSelfIndex+1;
	for(int idx=startIndex;idx<pvpHistoryCount;idx++){
		if(pvpHistoryPlayer[idx]==player) continue;
		if(!pvpHistoryText[idx].empty()){
			cout<<pvpHistoryText[idx]<<endl;
			hasOtherMove=true;
		}
	}
	if(!hasOtherMove) cout<<"（首轮暂无其他玩家出牌记录）"<<endl;
	cout<<"---------------------------"<<endl;

	layOut(layoutMode[player]);
	printHand(player);
	int choice=readPlayChoice(player);
	int dealt=0;
	while(dealt==0){
		if(choice<0||choice>=52/playerNum||cards[player][choice]<=0||cards[player][choice]>52){
			cout<<"输入错误！请重新选择你要出的牌:"<<endl;
			choice=readPlayChoice(player);
		}
		else if(isCardhanded[cards[player][choice]]==0){
			cout<<"这张牌已经出过了！请重新输入你要出的牌:"<<endl;
			choice=readPlayChoice(player);
		}
		else if(availableCards[cards[player][choice]]!=1){
			cout<<"这张牌打不出去,要扣这张吗？(不要0/要1)"<<endl;
			int choiceFold=readIntInput();
			if(choiceFold==1){
				//检测有没有能出的牌
				int dealAbility=0;
				for(int j=0;j<52/playerNum;j++){
					if(isCardhanded[cards[player][j]]==1&&availableCards[cards[player][j]]==1) dealAbility++;
				}
				if(dealAbility>0){
					cout<<"有可出的牌，无法扣牌！请重新输入你要出的牌:"<<endl;
					choice=readPlayChoice(player);
					continue;
				}
				int num=cardRank(cards[player][choice]);
				point[player]+=num;
				dealt=1;
				isCardhanded[cards[player][choice]]=0;
				isCardFolded[cards[player][choice]]=1;
				availableCards[cards[player][choice]]=0;
				pvpLastMove[player]="玩家"+to_string(player+1)+"已扣牌。";
				addPVPHistory(player,pvpLastMove[player]);
			}
			else{
				cout<<"取消扣牌，请重新输入你要出的牌:"<<endl;
				choice=readPlayChoice(player);
			}
		}
		else{
			int c=cards[player][choice];
			availableCards[c]=0;
			if(c%13<=6&&c%13>1){
				availableCards[c-1]=isCardFolded[c-1]?0:1;
			}
			else if(c%13>=8&&c%13!=0){
				availableCards[c+1]=isCardFolded[c+1]?0:1;
			}
			else if(c%13==7){
				availableCards[c-1]=isCardFolded[c-1]?0:1;
				availableCards[c+1]=isCardFolded[c+1]?0:1;
			}
			isCardhanded[c]=0;
			dealt=1;
			pvpLastMove[player]="玩家"+to_string(player+1)+"已出牌:"+cardText(c);
			addPVPHistory(player,pvpLastMove[player]);
		}
	}
}

void Player::pvpTransition(int nextPlayer){
	system("cls");
	cout<<"========================================"<<endl;
	cout<<"      请将屏幕交给玩家"<<nextPlayer+1<<"        "<<endl;
	cout<<"----------------------------------------"<<endl;
	cout<<"其他玩家请不要观看屏幕。"<<endl;
	cout<<"玩家"<<nextPlayer+1<<"准备好后，按回车开始。"<<endl;
	cout<<"输入 -1 可退出游戏。"<<endl;
	cout<<"========================================"<<endl;

	// 清掉上一次 cin>> 留下的换行符
	cin.ignore(10000,'\n');

	string line;
	while(true){
		getline(cin,line);
		if(line=="-1"){
			cout<<"游戏结束，感谢游玩！"<<endl;
			exit(0);
		}
		if(line.empty()) break;
		cout<<"请直接按回车开始，或输入 -1 退出。"<<endl;
	}
}

void Player::runPVP(){
	//PVP模式：所有玩家均为真人，在同一屏幕前轮流操作
	flag_over=0;
	if(flag_7==0){
		flag_7++;
		isCardhanded[7]=0;
		availableCards[7]=0;
		availableCards[6]=1;
		availableCards[8]=1;
		availableCards[20]=1;
		availableCards[33]=1;
		availableCards[46]=1;
		isCardFolded[7]=0;
		if(playerNum==3){
			//cout<<"系统已出牌:红桃7"<<endl;
			//cout<<"掷骰子决定先手为...";
			pvpFirstPlayer=random(0,playerNum-1);
			//cout<<"玩家"<<pvpFirstPlayer+1<<"!"<<endl;
		}
		else{
			for(int i=0;i<playerNum;i++){
				for(int j=0;j<52/playerNum;j++){
					if(cards[i][j]==7){
						//cout<<"玩家"<<i+1<<"已出牌:红桃7"<<endl;
						pvpLastMove[i]="玩家"+to_string(i+1)+"已出牌:红桃7";
						addPVPHistory(i,pvpLastMove[i]);
						pvpFirstPlayer=(i+1)%playerNum;
					}
				}
			}
		}
		//cout<<"--------------------\n";
	}
	for(int i=1;i<=52;i++){
		if(isCardhanded[i]==1) flag_over++;
	}
	if(flag_over==0){
		showPVPResult();
		return;
	}
	int p=pvpFirstPlayer;
	for(int count=0;count<playerNum;count++){
		// 手牌已空的玩家不再参与出牌，但仍占本轮顺序
		if(handCount(p)==0){
			p=(p+1)%playerNum;
			continue;
		}
		pvpTransition(p);
		dealPlayer(p);
		p=(p+1)%playerNum;
	}
	// 多人模式：必须等所有玩家手牌都为 0 后本局才结束
	flag_over=0;
	for(int i=1;i<=52;i++){
		if(isCardhanded[i]==1) flag_over++;
	}
	if(flag_over==0){
		showPVPResult();
		return;
	}
	cout<<"--------------------\n";
}

void Player::showPVPResult(){
	system("cls");
	flag_over=0;
	cout<<"游戏结束！"<<endl;
	layOut();
	for(int i=0;i<playerNum;i++){
		cout<<"玩家"<<i+1<<"扣分:"<<point[i]<<"分；"<<endl;
	}
	int minPoint=100;
	for(int i=0;i<playerNum;i++){
		if(point[i]<=minPoint) minPoint=point[i];
	}
	SetConsoleTextAttribute(hConsole,14);
	cout<<"冠军是玩家";
	for(int i=0;i<playerNum;i++){
		if(point[i]==minPoint){
			if(flag_champion_num>=1) cout<<"和玩家";
			cout<<i+1;
			flag_champion_num++;
		}
	}
	flag_champion_num=0;
	cout<<"!"<<endl;
	SetConsoleTextAttribute(hConsole,7);
}

void Player::addPVPHistory(int player,string text){
	if(pvpHistoryCount<60){
		pvpHistoryPlayer[pvpHistoryCount]=player;
		pvpHistoryText[pvpHistoryCount]=text;
		pvpHistoryCount++;
	}
}

void Player::run(){
	if(pvpMode){
		runPVP();
		return;
	}
	flag_over=0;
	if(flag_7==0){
		flag_7++;
		isCardhanded[7]=0;
		availableCards[7]=0;
		availableCards[6]=1;
		availableCards[8]=1;
		availableCards[20]=1;
		availableCards[33]=1;
		availableCards[46]=1;

		isCardFolded[7]=0;
		//设置第一出牌人
		for(int i=0;i<playerNum;i++){
			if(playerNum==3) break;
			for(int j=0;j<52/playerNum;j++){
				if(cards[i][j]==7){
					//cout<<"玩家"<<i+1<<"已出牌:红桃7"<<endl;
					if(i==0) dealNPC(1,playerNum);
					else if(i==1) dealNPC(2,playerNum);
					else if(i==2) dealNPC(3,playerNum);
				}
			}
		}
		//玩家人数为3的情况：红桃7默认已打出
		if(playerNum==3){
			cout<<"系统已出牌:红桃7"<<endl;
			cout<<"掷骰子决定先手为...";
			int firstHand=random(0,2);
			cout<<"玩家"<<firstHand+1<<"!"<<endl;
			layOut();
			if(firstHand==0){
				dealPlayer();
				dealNPC(1,playerNum);
			}
			else dealNPC(firstHand,playerNum);
		}

		cout<<"--------------------\n";
	}
	for(int i=1;i<=52;i++){
		if(isCardhanded[i]==1) flag_over++;
	}
	if(flag_over==0){
		cout<<"游戏结束！"<<endl;
		layOut();
		for(int i=0;i<playerNum;i++){
			cout<<"玩家"<<i+1<<"扣分:"<<point[i]<<"分；"<<endl;
		}
		int minPoint=100;
		for(int i=0;i<playerNum;i++){
			if(point[i]<=minPoint) minPoint=point[i];
		}
		SetConsoleTextAttribute(hConsole,14);
		cout<<"冠军是玩家";
		for(int i=0;i<playerNum;i++){
			if(point[i]==minPoint){
				if(flag_champion_num>=1) cout<<"和玩家";
				cout<<i+1;
				flag_champion_num++;
			}
		}
		flag_champion_num=0;
		cout<<"!"<<endl;
		SetConsoleTextAttribute(hConsole,7);
		return;
	}
	layOut();
	dealPlayer();
	dealNPC(1,playerNum);
	cout<<"--------------------\n";
}
int main(){
	// 让 Windows 控制台使用 UTF-8 显示中文和制表符
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	srand((unsigned)time(NULL));
	int cheat=0;
	while(true){
		system("cls");
		SetConsoleTextAttribute(hConsole,12);
		cout<<"憋七游戏demo"<<endl;
		SetConsoleTextAttribute(hConsole,7);
		cout<<"请选择游戏模式：1. 人机对战（原版）  2. PVP玩家对战（轮流操作）"<<endl;
		int pvpSelect=readIntInput();
		while(pvpSelect!=1&&pvpSelect!=2){
			cout<<"请输入正确模式编号(1或2):";
			pvpSelect=readIntInput();
		}
		bool pvpMode=(pvpSelect==2);
		cout<<"--------------------\n";
		cout<<"请选择参赛人数(2至4人):";
		int num=readIntInput();
		while(num%10!=2&&num%10!=3&&num%10!=4&&num%10!=1){
			cout<<"请输入正确参赛人数(2至4人):";
			num=readIntInput();
		}
		cout<<"--------------------\n";
		cout<<"游戏规则"<<endl;
		if(num%10==2||num%10==4){
			cout<<"出牌顺序：持有红桃7者自动打出红桃7，从下家开始循环出牌。\n出牌规则：各花色除点数7外，只能打出某一花色场上存在的相邻点数，点数7可随时打出。\n无牌可出时：选择一张牌扣到自己面前，并跳过出牌。\n游戏结束：所有玩家的手牌被打出或扣下时游戏结束，所扣点数最小者获胜。"<<endl;
		}
		else if(num%10==3){
			cout<<"出牌顺序：系统打出红桃7后，掷骰子决定先手，随后循环出牌。\n出牌规则：各花色除点数7外，只能出场上存在的某一花色的相邻点数，点数7可随时打出。\n无牌可出时：选择一张牌扣到自己面前，并跳过出牌。\n游戏结束：所有玩家的手牌被打出或扣下时游戏结束，所扣点数最小者获胜。"<<endl;
		}
		cout<<"--------------------\n";
		cout<<"按任意键开始游戏..."<<endl;
		system("pause");
		Player player(num%10,pvpMode);
		player.handout(num%10);
		if(num>10 && pvpSelect==1){
			cout<<"请输入作弊模式秘钥：";
			string code;
			cin>>code;
			if(!cin){
				bool isEOF=cin.eof();
				cin.clear();
				cin.ignore(10000,'\n');
				if(isEOF) checkExit(-1);
				cout<<"秘钥输入格式错误，作弊模式开启失败！"<<endl;
				code="";
			}
			if(code=="-1"){
				cout<<"游戏结束，感谢游玩！"<<endl;
				exit(0);
			}
			if(code=="cjx123"){
				SetConsoleTextAttribute(hConsole,13);
				cout<<"作弊模式已开启！"<<endl;
				SetConsoleTextAttribute(hConsole,7);
				cheat=1;
			}
			else{
				cout<<"秘钥错误，作弊模式开启失败！"<<endl;
			}
		}
		if(!pvpMode) player.printPlayerCards();
		while(1){
			if(cheat==1) player.print();
			player.run();
			if(player.flag_over==0) break;
		}
		cout<<"要再来一局吗？要1/不要0"<<endl<<"请输入您的选择:";
		int replay=readIntInput();
		while(true){
			if(replay==0){
				cout<<"游戏结束，感谢游玩！"<<endl;
				exit(0);
			}
			if(replay==1) break;
			cout<<"请输入正确选择(1继续/0退出):";
			replay=readIntInput();
		}
		system("cls");
	}
}