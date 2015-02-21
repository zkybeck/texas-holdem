#ifndef POKER_H
#define POKER_H

#include "CardDeck.h"
#include "Player.h"

enum PokerHandWinner {
	POKER_WIN_P1,
	POKER_WIN_P2,
	POKER_SPLIT
};

class Poker {
	//members
protected:
	CardDeck        deck;
	vector<Player*> players;

	//methods
protected:
	void            CalculateWinner(Player** ap_Players, int ai_Count);
	PokerHandWinner CompareHand(PokerHand& ah_Hand1, PokerHand& ah_Hand2);
	void            ScoreHand(PokerHand& ah_Hand, Card* ap_Card1, Card* ap_Card2, Card* ap_Card3, Card* ap_Card4, Card* ap_Card5, Card* ap_Card6, Card* ap_Card7);
	void            UnloadPlayers();
public:
	virtual void    Load(char* apsz_Data, int ai_Length) = 0;
	virtual void    Process() = 0;
	virtual char*   Save(int* ap_Length) = 0;

	Poker() { }
	virtual ~Poker() { UnloadPlayers(); }
};

#endif