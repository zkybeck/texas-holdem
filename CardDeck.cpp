//CardDeck.cpp
//poker
//------------------------------------------
#include "CardDeck.h"
//------------------------------------------
CardDeck::CardDeck(void)
{
	InitCards();
}
//------------------------------------------
bool CompareRandomIndex(const Card* a,const Card* b)
{
	return(a->RandomIndex<b->RandomIndex);
}
//------------------------------------------
Card* CardDeck::DealCard(void){
	if(Cards.size()>0){
		Card* p = Cards.back();
		Cards.pop_back();
		return(p);
	}
	return(0);
}
//------------------------------------------
Card* CardDeck::GetCardFromString(const char* apsz_CardName)
{
	int      index,i;
	CardType type;
	CardSuit suit;

	if(!apsz_CardName)return(0);

	//get card type
	index = String::IndexOf(apsz_CardName,"Ace of ");
	if(index!=-1){type = CTE_ACE;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Two of ");
	if(index!=-1){type = CTE_TWO;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Three of ");
	if(index!=-1){type = CTE_THREE;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Four of ");
	if(index!=-1){type = CTE_FOUR;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Five of ");
	if(index!=-1){type = CTE_FIVE;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Six of ");
	if(index!=-1){type = CTE_SIX;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Seven of ");
	if(index!=-1){type = CTE_SEVEN;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Eight of ");
	if(index!=-1){type = CTE_EIGHT;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Nine of ");
	if(index!=-1){type = CTE_NINE;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Ten of ");
	if(index!=-1){type = CTE_TEN;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Jack of ");
	if(index!=-1){type = CTE_JACK;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"Queen of ");
	if(index!=-1){type = CTE_QUEEN;goto FOUND_CARD;}
	index = String::IndexOf(apsz_CardName,"King of ");
	if(index!=-1){type = CTE_KING;goto FOUND_CARD;}
	return(0);

FOUND_CARD:
	index = String::IndexOf(apsz_CardName," of Spades");
	if(index!=-1){suit = CSE_SPADES;goto FOUND_SUIT;}
	index = String::IndexOf(apsz_CardName," of Clubs");
	if(index!=-1){suit = CSE_CLUBS;goto FOUND_SUIT;}
	index = String::IndexOf(apsz_CardName," of Hearts");
	if(index!=-1){suit = CSE_HEARTS;goto FOUND_SUIT;}
	index = String::IndexOf(apsz_CardName," of Diamonds");
	if(index!=-1){suit = CSE_DIAMONDS;goto FOUND_SUIT;}
	return(0);

FOUND_SUIT:
	for(i=0;i<52;i++){
		if((mh_Cards[i].Type == type)&&(mh_Cards[i].Suit == suit)){
			return(&mh_Cards[i]);
		}
	}

	return(0);
}
//------------------------------------------
char* CardDeck::GetCardNameShort(byte ac_Card)
{
	switch(ac_Card){
		case CTE_ACE:return("A");
		case CTE_TWO:return("2");
		case CTE_THREE:return("3");
		case CTE_FOUR:return("4");
		case CTE_FIVE:return("5");
		case CTE_SIX:return("6");
		case CTE_SEVEN:return("7");
		case CTE_EIGHT:return("8");
		case CTE_NINE:return("9");
		case CTE_TEN:return("10");
		case CTE_JACK:return("J");
		case CTE_QUEEN:return("Q");
		case CTE_KING:return("K");
	}

	return(0);
}
//------------------------------------------
void CardDeck::GetCardNameFull(Card* ap_Card,char* apsz_CardName)
{
	char lsz_CardName[64];

	if(!ap_Card)return;
	if(!apsz_CardName)return;

	apsz_CardName[0]=0;

	switch(ap_Card->Type){
		case CTE_ACE:strcpy(lsz_CardName,"Ace");break;
		case CTE_TWO:strcpy(lsz_CardName,"Two");break;
		case CTE_THREE:strcpy(lsz_CardName,"Three");break;
		case CTE_FOUR:strcpy(lsz_CardName,"Four");break;
		case CTE_FIVE:strcpy(lsz_CardName,"Five");break;
		case CTE_SIX:strcpy(lsz_CardName,"Six");break;
		case CTE_SEVEN:strcpy(lsz_CardName,"Seven");break;
		case CTE_EIGHT:strcpy(lsz_CardName,"Eight");break;
		case CTE_NINE:strcpy(lsz_CardName,"Nine");break;
		case CTE_TEN:strcpy(lsz_CardName,"Ten");break;
		case CTE_JACK:strcpy(lsz_CardName,"Jack");break;
		case CTE_QUEEN:strcpy(lsz_CardName,"Queen");break;
		case CTE_KING:strcpy(lsz_CardName,"King");break;
	}

	switch(ap_Card->Suit){
		case CSE_SPADES:strcat(lsz_CardName," of Spades");break;
		case CSE_CLUBS:strcat(lsz_CardName," of Clubs");break;
		case CSE_HEARTS:strcat(lsz_CardName," of Hearts");break;
		case CSE_DIAMONDS:strcat(lsz_CardName," of Diamonds");break;
	}

	strcpy(apsz_CardName,lsz_CardName);

	return;
}
//------------------------------------------
void CardDeck::InitCards(void)
{
	int  i,j;
	char index;

	Randomize();

	for(i=0;i<4;i++){
		for(j=0;j<13;j++){
			index = j+(i*13);
			
			switch(i){
				case 0:mh_Cards[index].Suit=CSE_SPADES;break;
				case 1:mh_Cards[index].Suit=CSE_CLUBS;break;
				case 2:mh_Cards[index].Suit=CSE_HEARTS;break;
				case 3:mh_Cards[index].Suit=CSE_DIAMONDS;break;
			}

			mh_Cards[index].Type=j;
			mh_Cards[index].Index=index;
		}
	}
	return;
}
//------------------------------------------
void CardDeck::Randomize(void)
{
	srand((unsigned int)time(0));
	return;
}
//------------------------------------------
void CardDeck::Shuffle(void)
{
	int         i,j,k,random;
	vector<int> randoms;

	Cards.clear();

	//initial shuffle
	for(i=0;i<52;i++){
		//note: every card must have a unique random index
	NEW_RANDOM_PLZ:
		random = rand();
		for(j=0;j<(int)randoms.size();j++){
			if(randoms[j]==random)break;
		}
		if(j!=randoms.size())goto NEW_RANDOM_PLZ;
		randoms.push_back(random);

		mh_Cards[i].RandomIndex = random;
		Cards.push_back(&mh_Cards[i]);
	}
	sort(Cards.begin(),Cards.end(),CompareRandomIndex);

	//shuffle 6 additional times
	for(i=0;i<6;i++){
		randoms.clear();

		for(j=0;j<52;j++){
			//note: every card must have a unique random index
		NEW_RANDOM_PLS:
			random = rand();
			for(k=0;k<(int)randoms.size();k++){
				if(randoms[k]==random)break;
			}
			if(k!=randoms.size())goto NEW_RANDOM_PLS;
			randoms.push_back(random);

			Cards[j]->RandomIndex = random;
		}
		sort(Cards.begin(),Cards.end(),CompareRandomIndex);
	}

	return;
}
//------------------------------------------