#include "Texas.h"

string stateNames[] = { "None", "Dealt", "Flop", "Turn", "River" };

void TexasHoldem::Process() {
	switch (state) {
		case THSE_NONE: ProcessStateNone(); break;
		case THSE_DEALT: ProcessStateDealt(); break;
		case THSE_FLOP: ProcessStateFlop(); break;
		case THSE_TURN: ProcessStateTurn(); break;
		case THSE_RIVER: ProcessStateRiver(); break;
	}
}

void TexasHoldem::ProcessBets() {
	int i;

PLACE_YOUR_BETS:
	for (i = 0; i<players.size(); i++) {
		Player* p = players[i];

		if (p->mb_CurrentTurn && p->me_Action != PA_ALLIN && p->me_Action != PA_FOLD) {
			Bet* b;

			p->mb_CurrentTurn = false;

			if (p->IsComputer()) {

				//p->me_Action = Computer();
				/*
				b = new Bet;
				if(p->mi_ChipCount>=mi_AmountToCall){
				b->ChipAmount = mi_AmountToCall;
				}
				b->State = me_State;
				b->PlayerID = i;
				p->me_Action = PA_CHECK;
				//CHECK FOR SIDE POT DAMMIT
				mh_Pot.BetHistory.push_back(b);*/
			}

			switch (p->me_Action) {
				case PA_CHECK:{
					int difference = (mp_CurrentPot->ToCall - p->mi_InFor);

					if (difference >= 0) {
						if (difference >= p->mi_ChipCount) {
							p->me_Action = PA_ALLIN;
							//...
							mp_CurrentPot->SplitPot();
						} else {
							mp_CurrentPot->Players.push_back(p);
							p->mi_InFor = mp_CurrentPot->ToCall;
							p->mi_ChipCount -= difference;
							p->mb_ActionRequired = false;
						}
					}

					/*Bet* b = new Bet;
					b->State = me_State;
					b->PlayerID = i;
					b->Action = p->me_Action;
					//CHECK FOR SIDE POT DAMMIT
					mh_Pot.BetHistory.push_back(b);	*/
				}break;
				case PA_RAISE:
					break;
			}

			//next player
			if ((i + 1)<players.size()) {
				players[i + 1]->mb_CurrentTurn = true;
			}
		}
	}

	//ensure everyone has bet
	for (i = 0; i<players.size(); i++) {
		Player* p = players[i];
		if (p->mb_CurrentTurn) {
			goto PLACE_YOUR_BETS;
		}
	}
}

void TexasHoldem::ProcessStateDealt() {
	char c;

	//signal the next betting player 
	//this will change of course, depending on who stays in
	ProcessBets();
	players[0]->mb_CurrentTurn = true;
	for (int i = 0; i<players.size(); i++) {
		players[i]->me_Action = PA_NONE;
	}

	//burn
	deck.DealCard();

	//throw down flop
	flop1 = deck.DealCard();
	flop2 = deck.DealCard();
	flop3 = deck.DealCard();

	printf("The Flop:\n%s | %s | %s\n",
		flop1->shortName().c_str(),
		flop2->shortName().c_str(),
		flop3->shortName().c_str());

	state = THSE_FLOP;
}

void TexasHoldem::ProcessStateFlop() {
	//signal the next betting player 
	//this will change of course, depending on who stays in
	ProcessBets();
	players[0]->mb_CurrentTurn = true;
	for (int i = 0; i<players.size(); i++) {
		players[i]->me_Action = PA_NONE;
	}

	//burn
	deck.DealCard();

	//show turn card
	turn = deck.DealCard();
	printf("The Turn:\n%s | %s | %s | %s\n", 
		flop1->shortName().c_str(), 
		flop2->shortName().c_str(),
		flop3->shortName().c_str(),
		turn->shortName().c_str());

	state = THSE_TURN;
}

void TexasHoldem::ProcessStateNone() {
	int  i;
	Bet* b;
	bool lb_NeedsReIndex = false;

	//remove players that have no chips
	vector<Player*>::iterator it = players.begin();
	while (it != players.end()) {
		if ((*it)->mi_ChipCount == 0) {
			delete(*it);
			it = players.erase(it);
			lb_NeedsReIndex = true;
		} else {
			++it;
		}
	}

	//re-index player id
	if (lb_NeedsReIndex) {
		for (i = 0; i<players.size(); i++) {
			Player* p = players[i];
			p->mi_PlayerID = i;
		}
	}


	//simulate blind collection
	//>small blinds
	b = new Bet;
	b->BlindCollectionSmall = true;
	b->PlayerID = sblind;
	b->BetID = mi_BetID++;
	b->ChipAmount = 1;
	mh_Pot.BetHistory.push_back(b);
	//	mv_Players[sblind]->mi_ChipCount--;

	//>big blinds
	b = new Bet;
	b->BlindCollectionBig = true;
	b->PlayerID = bblind;
	b->BetID = mi_BetID++;
	b->ChipAmount = 2;
	mh_Pot.BetHistory.push_back(b);
	//	mv_Players[bblind]->mi_ChipCount-=2;


	//deal
	printf("Stats:\n");
	for (i = 0; i<players.size(); i++) {
		Player* p = players[i];
		printf("%dUP: CHIPS:%8d | W:%5d | L:%5d | T:%5d\n", i + 1, p->ChipCount(), p->Wins(), p->Losses(), p->Ties());
		p->me_Action = PA_NONE;
		//WRONG *****
		//USE DEALER CHIP
		if (i == 0)p->mb_CurrentTurn = true;
	}
	deck.Shuffle();

	//deal 1st card (starting to left of dealer)
	for (i = mi_Dealer + 1; i<players.size(); i++) {
		Card* c = deck.DealCard();
		players[i]->cards[0] = c;
	}
	for (i = 0; i<mi_Dealer + 1; i++) {
		Card* c = deck.DealCard();
		players[i]->cards[0] = c;
	}

	//deal 2nd card (starting to left of dealer)
	for (i = mi_Dealer + 1; i<players.size(); i++) {
		Card* c = deck.DealCard();
		players[i]->cards[1] = c;
	}
	for (i = 0; i<mi_Dealer + 1; i++) {
		Card* c = deck.DealCard();
		players[i]->cards[1] = c;
	}

	//show player 1 cards
	Card* p1c1 = players[0]->cards[0];
	Card* p1c2 = players[0]->cards[1];
	printf("Deck shuffled. Your Cards:\n%s | %s\n\n",
		p1c1->shortName().c_str(),
		p1c2->shortName().c_str());

	state = THSE_DEALT;
}

void TexasHoldem::ProcessStateRiver() {
	int i;
	vector<PokerHand*> hands;

	for (i = 0; i<players.size(); i++) {
		ScoreHand(players[i]->Hand(), flop1, flop2, flop3, turn, river, players[i]->cards[0], players[i]->cards[1]);

		Card* p1c1 = players[i]->cards[0];
		Card* p1c2 = players[i]->cards[1];
		char  title[5] = { ' ', ' ', ' ', ' ', 0 };

		if (i == mi_Dealer)
			strcpy(title, "DEAL");
		if (i == sblind)  
			strcpy(title, "SBLI");
		if (i == bblind)  
			strcpy(title, "BBLI");

		printf("%dUP(%s): %s | %s\n->%s\n", i + 1, title, 
			p1c1->shortName().c_str(),
			p1c2->shortName().c_str(),
			players[i]->Hand().HandName());
	}

	//find the winner(s)
	Player** people = (Player**)malloc(players.size() * sizeof(Player*));
	for (i = 0; i<players.size(); i++) {
		people[i] = players[i];
		players[i]->mb_Winner = false;
	}
	CalculateWinner(people, players.size());
	free(people);

	int winnercount = 0;
	int winnerplayer = -1;

	for (i = 0; i<players.size(); i++) {
		if (players[i]->mb_Winner) {
			winnercount++;
			winnerplayer = i;
		}
	}

	if (winnercount == 1) {
		printf("%dUP wins\n\n", winnerplayer + 1);
		//		players[winnerplayer]->mi_ChipCount += mi_Pot;

		for (i = 0; i<players.size(); i++) {
			if (i != winnerplayer) {
				players[i]->mi_ChipCount--;
				players[i]->mi_Losses++;
			} else {
				players[i]->mi_Wins++;
			}
		}
	} else {
		printf("split pot:\n");
		for (i = 0; i<players.size(); i++) {
			if (players[i]->mb_Winner) {
				printf("%dUP wins\n", i + 1);
				//			mv_Players[i]->mi_ChipCount += (mi_Pot / winnercount);
				players[i]->mi_Ties++;
			} else {
				players[i]->mi_ChipCount--;
				players[i]->mi_Losses++;
			}
		}
		printf("\n");
	}

	RotateDealerButton();

	//reset pot
	mh_Pot.Reset();
	mp_CurrentPot = &mh_Pot;

	//reset values
	flop1 = 0;
	flop2 = 0;
	flop3 = 0;
	turn = 0;
	river = 0;

	mi_BetID = 0;

	state = THSE_NONE;
}

void TexasHoldem::ProcessStateTurn() {
	//signal the next betting player 
	//this will change of course, depending on who stays in
	ProcessBets();
	players[0]->mb_CurrentTurn = true;
	for (int i = 0; i<players.size(); i++) {
		players[i]->me_Action = PA_NONE;
	}

	//burn
	deck.DealCard();

	//show river card
	river = deck.DealCard();
	printf("The River:\n%s | %s | %s | %s | %s\n\n",
		flop1->shortName().c_str(),
		flop2->shortName().c_str(),
		flop3->shortName().c_str(),
		turn->shortName().c_str(),
		river->shortName().c_str());

	state = THSE_RIVER;
}

void TexasHoldem::RotateDealerButton() {
	//THIS LOGIC ISN"T CORRECT (I THINK?) DOUBLE CHECK
	//rotate dealer + blinds
	if (++mi_Dealer >= players.size()) {
		mi_Dealer = 0;
	}

	if (++sblind >= players.size()) {
		sblind = 0;
	}

	if (++bblind >= players.size()) {
		bblind = 0;
	}
}