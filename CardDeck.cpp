#include "CardDeck.h"

string cardTypes[] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };
char cardShortName[] = { '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A' };
string cardSuits[] = { " of Spades", " of Clubs", " of Hearts", " of Diamonds" };

string Card::shortName() {
	string cardName = "";
	cardName += cardShortName[(int)type];
	cardName += (char)suit;
	return cardName;
}

string Card::longName() {
	string cardName = cardTypes[(int)type];
	cardName += cardSuits[(int)suit];
	return cardName;
}

Card* CardDeck::DealCard() {
	if (Cards.size() > 0) {
		Card* p = Cards.back();
		Cards.pop_back();
		return p;
	}
	return 0;
}

Card* CardDeck::GetCardFromString(const char* apsz_CardName) {
	int      index, i;
	CardType type;
	CardSuit suit;

	if (!apsz_CardName)return(0);

	//get card type
	index = String::IndexOf(apsz_CardName, "Ace of ");
	if (index != -1) { type = CTE_ACE; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Two of ");
	if (index != -1) { type = CTE_TWO; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Three of ");
	if (index != -1) { type = CTE_THREE; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Four of ");
	if (index != -1) { type = CTE_FOUR; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Five of ");
	if (index != -1) { type = CTE_FIVE; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Six of ");
	if (index != -1) { type = CTE_SIX; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Seven of ");
	if (index != -1) { type = CTE_SEVEN; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Eight of ");
	if (index != -1) { type = CTE_EIGHT; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Nine of ");
	if (index != -1) { type = CTE_NINE; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Ten of ");
	if (index != -1) { type = CTE_TEN; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Jack of ");
	if (index != -1) { type = CTE_JACK; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "Queen of ");
	if (index != -1) { type = CTE_QUEEN; goto FOUND_CARD; }
	index = String::IndexOf(apsz_CardName, "King of ");
	if (index != -1) { type = CTE_KING; goto FOUND_CARD; }
	return 0;

FOUND_CARD:
	index = String::IndexOf(apsz_CardName, " of Spades");
	if (index != -1) { suit = CSE_SPADES; goto FOUND_SUIT; }
	index = String::IndexOf(apsz_CardName, " of Clubs");
	if (index != -1) { suit = CSE_CLUBS; goto FOUND_SUIT; }
	index = String::IndexOf(apsz_CardName, " of Hearts");
	if (index != -1) { suit = CSE_HEARTS; goto FOUND_SUIT; }
	index = String::IndexOf(apsz_CardName, " of Diamonds");
	if (index != -1) { suit = CSE_DIAMONDS; goto FOUND_SUIT; }
	return 0;

FOUND_SUIT:
	for (i = 0; i<52; i++) {
		if ((mh_Cards[i].type == type) && (mh_Cards[i].suit == suit)) {
			return &mh_Cards[i];
		}
	}

	return 0;
}

void CardDeck::InitCards() {
	Randomize();

	for (int i = 0; i<4; i++) {
		for (int j = 0; j<13; j++) {
			char index = j + (i * 13);

			switch (i) {
				case 0:mh_Cards[index].suit = CSE_SPADES; break;
				case 1:mh_Cards[index].suit = CSE_CLUBS; break;
				case 2:mh_Cards[index].suit = CSE_HEARTS; break;
				case 3:mh_Cards[index].suit = CSE_DIAMONDS; break;
			}

			mh_Cards[index].type = (CardType)j;
		}
	}
}

void CardDeck::Randomize() {
	srand((unsigned int)time(0));
}

void CardDeck::Shuffle() {
	int         i, j, k, random;
	vector<int> randoms;

	Cards.clear();

	//initial shuffle
	for (i = 0; i<52; i++) {
		//note: every card must have a unique random index
	NEW_RANDOM_PLZ:
		random = rand();
		for (j = 0; j<(int)randoms.size(); j++) {
			if (randoms[j] == random)break;
		}
		if (j != randoms.size())goto NEW_RANDOM_PLZ;
		randoms.push_back(random);

		mh_Cards[i].sortIndex = random;
		Cards.push_back(&mh_Cards[i]);
	}

	sort(Cards.begin(), Cards.end(), [](const Card* a, const Card* b) -> bool {
		return(a->sortIndex < b->sortIndex);
	});

	//shuffle 6 additional times
	for (i = 0; i<6; i++) {
		randoms.clear();

		for (j = 0; j<52; j++) {
			//note: every card must have a unique random index
		NEW_RANDOM_PLS:
			random = rand();
			for (k = 0; k<(int)randoms.size(); k++) {
				if (randoms[k] == random)break;
			}
			if (k != randoms.size())goto NEW_RANDOM_PLS;
			randoms.push_back(random);

			Cards[j]->sortIndex = random;
		}

		sort(Cards.begin(), Cards.end(), [](const Card* a, const Card* b) -> bool {
			return(a->sortIndex < b->sortIndex);
		});
	}
}