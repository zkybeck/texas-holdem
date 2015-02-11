//CardDeck.h
//poker
//------------------------------------------
#ifndef CARDDECK_H
#define CARDDECK_H
//------------------------------------------
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "xml/String.h"
using namespace std;
using namespace Kahnadex;
//------------------------------------------
typedef unsigned char byte;

enum CardSuit
{
	CSE_SPADES=0x06,
	CSE_CLUBS=0x05,
	CSE_HEARTS=0x03,
	CSE_DIAMONDS=0x04
};

enum CardType
{
	CTE_TWO,
	CTE_THREE,
	CTE_FOUR,
	CTE_FIVE,
	CTE_SIX,
	CTE_SEVEN,
	CTE_EIGHT,
	CTE_NINE,
	CTE_TEN,
	CTE_JACK,
	CTE_QUEEN,
	CTE_KING,
	CTE_ACE
};

struct Card
{
	byte Suit;
	byte Type;
	byte Index;
	int  RandomIndex;
};
//------------------------------------------
class CardDeck
{
	//friends
	friend class TexasHoldem;

	//members
	protected:
		Card          mh_Cards[52];
	public:
		vector<Card*> Cards;
	
	//methods
	protected:
		void         InitCards(void);
	public:
		Card*        DealCard(void);
		Card*        GetCardFromString(const char* apsz_CardName);
		static char* GetCardNameShort(byte ac_Card);
		static void  GetCardNameFull(Card* ap_Card,char* apsz_CardName);
		void         Randomize(void);
		void         Shuffle(void);

		CardDeck(void);
		virtual ~CardDeck(){}
};
//------------------------------------------
#endif