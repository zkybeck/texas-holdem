#ifndef POKERHAND_H
#define POKERHAND_H

enum PokerHandType {
	POKER_HIGHCARD = 0,
	POKER_PAIR,
	POKER_TWO_PAIRS,
	POKER_THREE_OF_A_KIND,
	POKER_STRAIGHT,
	POKER_FLUSH,
	POKER_FULL_HOUSE,
	POKER_FOUR_OF_A_KIND,
	POKER_STRAIGHT_FLUSH,
	POKER_ROYAL_FLUSH
};

class PokerHand {
	//friends
	friend class Poker;
	friend class TexasHoldem;

	//members
private:
	PokerHandType me_Hand;
	int           threeofakind;
	int           fourofakind;
	int           pair1;
	int           pair2;
	int           high[5];
	int           straightHighCard;
	int           fullhouse3;
	int           fullhouse2;

	//methods
public:
	void Clear() {
		me_Hand = POKER_HIGHCARD;
		fourofakind = -1;
		pair1 = -1;
		pair2 = -1;
		threeofakind = -1;
		fullhouse3 = -1;
		fullhouse2 = -1;
		straightHighCard = -1;
		memset(high, -1, 5 * sizeof(int));
	}
	static PokerHandType GetHandFromString(const char* apsz_HandName) {

		if (!_strcmpi(apsz_HandName, "HIGH CARD"))return(POKER_HIGHCARD);
		if (!_strcmpi(apsz_HandName, "PAIR"))return(POKER_PAIR);
		if (!_strcmpi(apsz_HandName, "TWO PAIRS"))return(POKER_TWO_PAIRS);
		if (!_strcmpi(apsz_HandName, "THREE OF A KIND"))return(POKER_THREE_OF_A_KIND);
		if (!_strcmpi(apsz_HandName, "STRAIGHT"))return(POKER_STRAIGHT);
		if (!_strcmpi(apsz_HandName, "FLUSH"))return(POKER_FLUSH);
		if (!_strcmpi(apsz_HandName, "FULL HOUSE"))return(POKER_FULL_HOUSE);
		if (!_strcmpi(apsz_HandName, "FOUR OF A KIND"))return(POKER_FOUR_OF_A_KIND);
		if (!_strcmpi(apsz_HandName, "STRAIGHT FLUSH"))return(POKER_STRAIGHT_FLUSH);
		if (!_strcmpi(apsz_HandName, "ROYAL FLUSH"))return(POKER_ROYAL_FLUSH);

		return(POKER_HIGHCARD);
	}

	static const char* GetHandName(PokerHandType ae_Hand) {
		switch (ae_Hand) {
			case POKER_HIGHCARD:return("High Card");
			case POKER_PAIR:return("Pair");
			case POKER_TWO_PAIRS:return("Two Pairs");
			case POKER_THREE_OF_A_KIND:return("Three of a Kind");
			case POKER_STRAIGHT:return("Straight");
			case POKER_FLUSH:return("Flush");
			case POKER_FULL_HOUSE:return("Full House");
			case POKER_FOUR_OF_A_KIND:return("Four of a Kind");
			case POKER_STRAIGHT_FLUSH:return("Straight Flush");
			case POKER_ROYAL_FLUSH:return("Royal Flush");
		}

		return("");
	}
	const char* HandName() {
		return(PokerHand::GetHandName(me_Hand));
	}
	PokerHandType Hand() {
		return(me_Hand);
	}

	PokerHand() {
		Clear();
	}
};

#endif