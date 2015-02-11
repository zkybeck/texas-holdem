//Poker.cpp
//poker
//------------------------------------------
#include "Poker.h"
//------------------------------------------
void Poker::CalculateWinner(Player** ap_Players,int ai_Count)
{
	int     i;
	Player* currentWinner=0;
	bool    tie=false;

	for(i=0;i<ai_Count;i++){
		if(i+2>ai_Count)break;
		
		PokerHandWinner winner = CompareHand(ap_Players[i]->Hand(),ap_Players[i+1]->Hand());
		
		switch(winner){
			case POKER_WIN_P1:
				if(currentWinner && currentWinner!=ap_Players[i]){
					winner = CompareHand(currentWinner->Hand(),ap_Players[i]->Hand());
					switch(winner){
						case POKER_WIN_P2:
							tie=false;
							currentWinner = ap_Players[i];
							break;
						case POKER_SPLIT:
							tie=true;
							break;
					}
				}else currentWinner = ap_Players[i];
				break;

			case POKER_WIN_P2:
				if(currentWinner){
					winner = CompareHand(currentWinner->Hand(),ap_Players[i+1]->Hand());
					switch(winner){
						case POKER_WIN_P2:
							tie=false;
							currentWinner = ap_Players[i+1];
							break;
						case POKER_SPLIT:
							tie=true;
							break;
					}
				}else currentWinner = ap_Players[i+1];
				break;

			case POKER_SPLIT:
				tie=true;
				currentWinner = ap_Players[i];
				break;
		}
	}

	//now, a winning hand has been determined.
	if(tie){
		//split pot with all people who tied winner
		currentWinner->mb_Winner=true;

		for(i=0;i<ai_Count;i++){
			PokerHandWinner winner = CompareHand(currentWinner->Hand(),ap_Players[i]->Hand());
			if(winner == POKER_SPLIT){
				ap_Players[i]->mb_Winner = true;
			}
		}
	}else{
		currentWinner->mb_Winner = true;
	}

	return;
}
//------------------------------------------
PokerHandWinner Poker::CompareHand(PokerHand& ah_Hand1,PokerHand& ah_Hand2)
{
	int i,bestof5;

	if(ah_Hand1.me_Hand == ah_Hand2.me_Hand){	
		switch(ah_Hand1.me_Hand){
			case POKER_HIGHCARD:
				//best of 5
				for(i=0;i<5;i++){
					if(ah_Hand1.high[i] > ah_Hand2.high[i])return(POKER_WIN_P1);
					if(ah_Hand1.high[i] < ah_Hand2.high[i])return(POKER_WIN_P2);
				}
				return(POKER_SPLIT);

			case POKER_PAIR:
				if(ah_Hand1.pair1 > ah_Hand2.pair1)return(POKER_WIN_P1);
				if(ah_Hand1.pair1 < ah_Hand2.pair1)return(POKER_WIN_P2);
				//best of 5
				bestof5=3;i=0;
				while(bestof5>0){
					if(ah_Hand1.high[i] != ah_Hand1.pair1){
						if(ah_Hand1.high[i] > ah_Hand2.high[i])return(POKER_WIN_P1);
						if(ah_Hand1.high[i] < ah_Hand2.high[i])return(POKER_WIN_P2);
						bestof5--;
					}
					i++;
				}
				return(POKER_SPLIT);

			case POKER_TWO_PAIRS:
				if(ah_Hand1.pair1 > ah_Hand2.pair1)return(POKER_WIN_P1);
				if(ah_Hand1.pair1 < ah_Hand2.pair1)return(POKER_WIN_P2);
				if(ah_Hand1.pair2 > ah_Hand2.pair2)return(POKER_WIN_P1);
				if(ah_Hand1.pair2 < ah_Hand2.pair2)return(POKER_WIN_P2);
				//best of 5
				bestof5=1;i=0;
				while(bestof5>0){
					if(ah_Hand1.high[i] != ah_Hand1.pair1 && ah_Hand1.high[i] != ah_Hand1.pair2){
						if(ah_Hand1.high[i] > ah_Hand2.high[i])return(POKER_WIN_P1);
						if(ah_Hand1.high[i] < ah_Hand2.high[i])return(POKER_WIN_P2);
						bestof5--;
					}
					i++;
				}
				return(POKER_SPLIT);

			case POKER_THREE_OF_A_KIND:
				if(ah_Hand1.threeofakind > ah_Hand2.threeofakind)return(POKER_WIN_P1);
				if(ah_Hand1.threeofakind < ah_Hand2.threeofakind)return(POKER_WIN_P2);
				//best of 5
				bestof5=2;i=0;
				while(bestof5>0){
					if(ah_Hand1.high[i] != ah_Hand1.threeofakind){
						if(ah_Hand1.high[i] > ah_Hand2.high[i])return(POKER_WIN_P1);
						if(ah_Hand1.high[i] < ah_Hand2.high[i])return(POKER_WIN_P2);
						bestof5--;
					}
					i++;
				}
				return(POKER_SPLIT);

			case POKER_STRAIGHT:
				if(ah_Hand1.straightHighCard > ah_Hand2.straightHighCard)return(POKER_WIN_P1);
				if(ah_Hand1.straightHighCard < ah_Hand2.straightHighCard)return(POKER_WIN_P2);
				return(POKER_SPLIT);

			case POKER_FLUSH:
				//best of 5
				for(i=0;i<5;i++){
					if(ah_Hand1.high[i] > ah_Hand2.high[i])return(POKER_WIN_P1);
					if(ah_Hand1.high[i] < ah_Hand2.high[i])return(POKER_WIN_P2);
				}
				return(POKER_SPLIT);

			case POKER_FULL_HOUSE:
				if(ah_Hand1.fullhouse3 > ah_Hand2.fullhouse3)return(POKER_WIN_P1);
				if(ah_Hand1.fullhouse3 < ah_Hand2.fullhouse3)return(POKER_WIN_P2);
				if(ah_Hand1.fullhouse2 > ah_Hand2.fullhouse2)return(POKER_WIN_P1);
				if(ah_Hand1.fullhouse2 < ah_Hand2.fullhouse2)return(POKER_WIN_P2);
				return(POKER_SPLIT);

			case POKER_FOUR_OF_A_KIND:
				if(ah_Hand1.fourofakind > ah_Hand2.fourofakind)return(POKER_WIN_P1);
				if(ah_Hand1.fourofakind < ah_Hand2.fourofakind)return(POKER_WIN_P2);
				//best of 5
				bestof5=1;i=0;
				while(bestof5>0){
					if(ah_Hand1.high[i] != ah_Hand1.fourofakind){
						if(ah_Hand1.high[i] > ah_Hand2.high[i])return(POKER_WIN_P1);
						if(ah_Hand1.high[i] < ah_Hand2.high[i])return(POKER_WIN_P2);
						bestof5--;
					}
					i++;
				}
				return(POKER_SPLIT);

			case POKER_STRAIGHT_FLUSH:
				if(ah_Hand1.straightHighCard > ah_Hand2.straightHighCard)return(POKER_WIN_P1);
				if(ah_Hand1.straightHighCard < ah_Hand2.straightHighCard)return(POKER_WIN_P2);
				return(POKER_SPLIT);

			case POKER_ROYAL_FLUSH:
				return(POKER_SPLIT);
		}
	}else if(ah_Hand1.me_Hand > ah_Hand2.me_Hand){
		return(POKER_WIN_P1);
	}else{
		return(POKER_WIN_P2);
	}

	return(POKER_SPLIT);
}
//------------------------------------------
void Poker::ScoreHand(PokerHand& ah_Hand,Card* ap_Card1,Card* ap_Card2,Card* ap_Card3,Card* ap_Card4,Card* ap_Card5,Card* ap_Card6,Card* ap_Card7)
{
	//make this work with either 5 OR 7
	//check for null
	Card* p[7]={ap_Card1,ap_Card2,ap_Card3,ap_Card4,ap_Card5,ap_Card6,ap_Card7};
	int  i,j;
	int  pair=0;

	ah_Hand.Clear();

	//non-flush card count
	int count2[13]={0};

	for(i=0;i<7;i++){
		//keep track of high card
		for(j=0;j<5;j++){
			if(p[i]->Type > ah_Hand.high[j]){
				memmove(ah_Hand.high+j+1,ah_Hand.high+j,(4-j)*sizeof(int));
				ah_Hand.high[j]=p[i]->Type;
				break;
			}
		}
		//card count
		count2[p[i]->Type]++;
	}

	//score cards
	for(i=0;i<13;i++){
		if(count2[i]==4){
			ah_Hand.me_Hand = POKER_FOUR_OF_A_KIND;
			ah_Hand.fourofakind = i;
		}

		if(count2[i]==3){
			if(ah_Hand.me_Hand == POKER_PAIR){
				ah_Hand.me_Hand = POKER_FULL_HOUSE;
				ah_Hand.fullhouse3 = i;
				ah_Hand.fullhouse2 = ah_Hand.pair1;

			}else if(ah_Hand.me_Hand == POKER_TWO_PAIRS){
				ah_Hand.me_Hand = POKER_FULL_HOUSE;
				ah_Hand.fullhouse3 = i;
				if(ah_Hand.pair1 > ah_Hand.pair2){
					ah_Hand.fullhouse2 = ah_Hand.pair1;
				}else{
					ah_Hand.fullhouse2 = ah_Hand.pair2;
				}

			}else if(ah_Hand.me_Hand < POKER_THREE_OF_A_KIND){
				ah_Hand.me_Hand = POKER_THREE_OF_A_KIND;
				ah_Hand.threeofakind = i;

			}else if(ah_Hand.me_Hand==POKER_THREE_OF_A_KIND){
				//full house
				ah_Hand.me_Hand = POKER_FULL_HOUSE;
				if(i > ah_Hand.threeofakind){
					ah_Hand.fullhouse3 = i;
					ah_Hand.fullhouse2 = ah_Hand.threeofakind;
				}else{
					ah_Hand.fullhouse3 = ah_Hand.threeofakind;
					ah_Hand.fullhouse2 = i;
				}
			}
		}

		if(count2[i]==2){
			if(ah_Hand.me_Hand == POKER_FULL_HOUSE){
				//check the 2nd pair 
				if(i > ah_Hand.fullhouse2){
					ah_Hand.fullhouse2 = i;
				}
			}
			if(ah_Hand.me_Hand == POKER_THREE_OF_A_KIND){
				ah_Hand.me_Hand = POKER_FULL_HOUSE;
				ah_Hand.fullhouse2 = i;

			}else if(ah_Hand.me_Hand==POKER_TWO_PAIRS){						
				if(i > ah_Hand.pair1){
					ah_Hand.pair2 = ah_Hand.pair1;
					ah_Hand.pair1 = i;
				}else if(i > ah_Hand.pair2){
					ah_Hand.pair2 = i;
				}

			}else if(ah_Hand.me_Hand == POKER_PAIR){
				ah_Hand.me_Hand = POKER_TWO_PAIRS;
				
				if(ah_Hand.pair1 > i){
					ah_Hand.pair2 = i;
				}else{
					ah_Hand.pair2 = ah_Hand.pair1;
					ah_Hand.pair1 = i;
				}
			}else{
				if(ah_Hand.me_Hand < POKER_PAIR){
					ah_Hand.me_Hand = POKER_PAIR;
					ah_Hand.pair1 = i;
				}
			}
		}
	}

	//look for a straight
	int cardsInaRow=0;
	int straightHigh=-1;
	bool straight=false;

	for(i=0;i<13;i++){
		if(count2[i]>0){
			if(++cardsInaRow>=5){
				straight=true;
				straightHigh = i;
			}else{
				//ace exception in A,2,3,4,5
				if(cardsInaRow==4 && i == CTE_FIVE){
					if(count2[CTE_ACE]>0){
						cardsInaRow++;
						straight = true;
						straightHigh = i;
					}
				}
			}
		}
		else
			cardsInaRow=0;
	}
	if((straight) && (ah_Hand.me_Hand < POKER_STRAIGHT)){
		ah_Hand.me_Hand = POKER_STRAIGHT;
		ah_Hand.straightHighCard = straightHigh;
	}

	//Suit count
	bool flush=false;
	int  count[4]={0};
	CardSuit flushtype;

	//general count
	for(i=0;i<7;i++){
		if(p[i]&&p[i]->Suit==CSE_SPADES)count[0]++;
		if(p[i]&&p[i]->Suit==CSE_CLUBS)count[1]++;
		if(p[i]&&p[i]->Suit==CSE_HEARTS)count[2]++;
		if(p[i]&&p[i]->Suit==CSE_DIAMONDS)count[3]++;
	}
	//flush?
	for(i=0;i<4;i++){
		if(count[i]>=5){
			flush=true;
			switch(i){
				case 0:flushtype=CSE_SPADES;break;
				case 1:flushtype=CSE_CLUBS;break;
				case 2:flushtype=CSE_HEARTS;break;
				case 3:flushtype=CSE_DIAMONDS;break;
			}
			break;
		}
	}

	if((flush) && (ah_Hand.me_Hand < POKER_FLUSH)){
		ah_Hand.me_Hand = POKER_FLUSH;

		//find the flush high cards
		for(i=0;i<7;i++){
			if(p[i]->Suit!=flushtype)continue;

			//keep track of high card
			for(j=0;j<5;j++){
				if(p[i]->Type > ah_Hand.high[j]){
					memmove(ah_Hand.high+j+1,ah_Hand.high+j,(4-j)*sizeof(int));
					ah_Hand.high[j]=p[i]->Type;
					break;
				}
			}
		}
	}

	//check for straight flush & royal flush
	if(flush&&straight){
		int count3[13][4]={0};
		int flushtypeint;

		//the Suit of flush the player has
		switch(flushtype){
			case CSE_SPADES:flushtypeint=0;break;
			case CSE_CLUBS:flushtypeint=1;break;
			case CSE_HEARTS:flushtypeint=2;break;
			case CSE_DIAMONDS:flushtypeint=3;break;
		}
		
		//gather info; Suit count per card type
		for(i=0;i<7;i++){
			switch(p[i]->Suit){
				case CSE_SPADES:
					count3[p[i]->Type][0]++;
					break;
				case CSE_CLUBS:
					count3[p[i]->Type][1]++;
					break;
				case CSE_HEARTS:
					count3[p[i]->Type][2]++;
					break;
				case CSE_DIAMONDS:
					count3[p[i]->Type][3]++;
					break;
			}
		}

		//check for royal / straight flush
		cardsInaRow=0;
		straightHigh=-1;
		bool straightflush=false;
		for(i=0;i<13;i++){
			//remember.. the person can have a straight flush 
			//& a straight with a high card at the same time
			if(count3[i][flushtypeint]>0){
				if(++cardsInaRow>=5){
					straightflush=true;
					straightHigh = i;
				}else{
					//ace exception in A,2,3,4,5
					if(cardsInaRow==4 && i == CTE_FIVE){
						if(count2[CTE_ACE]>0){
							cardsInaRow++;
							straightflush = true;
							straightHigh = i;
						}
					}
				}
			}else cardsInaRow = 0;
		}

		if(straightflush){
			ah_Hand.me_Hand = POKER_STRAIGHT_FLUSH;
			ah_Hand.straightHighCard = straightHigh;

			//royal flush
			if(ah_Hand.straightHighCard == CTE_ACE){
				ah_Hand.me_Hand = POKER_ROYAL_FLUSH;
			}
		}
	}

	return;
}
//------------------------------------------
void Poker::UnloadPlayers(void)
{
	int i;
	for(i=0;i<mv_Players.size();i++){
		delete mv_Players[i];
	}
	mv_Players.clear();
}
//------------------------------------------