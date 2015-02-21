#include "Texas.h"

string stateNames[] = { "None", "Dealt", "Flop", "Turn", "River" };

void TexasHoldem::Process() {
	switch (me_State) {
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
	for (i = 0; i<mv_Players.size(); i++) {
		Player* p = mv_Players[i];

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
			if ((i + 1)<mv_Players.size()) {
				mv_Players[i + 1]->mb_CurrentTurn = true;
			}
		}
	}

	//ensure everyone has bet
	for (i = 0; i<mv_Players.size(); i++) {
		Player* p = mv_Players[i];
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
	mv_Players[0]->mb_CurrentTurn = true;
	for (int i = 0; i<mv_Players.size(); i++) {
		mv_Players[i]->me_Action = PA_NONE;
	}

	//throw down flop
	//burn
	mh_Cards.DealCard();
	flop1 = mh_Cards.DealCard();
	flop2 = mh_Cards.DealCard();
	flop3 = mh_Cards.DealCard();

	printf("The Flop:\n%s | %s | %s\n",
		flop1->shortName().c_str(),
		flop2->shortName().c_str(),
		flop3->shortName().c_str());

	me_State = THSE_FLOP;
}

void TexasHoldem::ProcessStateFlop() {
	//signal the next betting player 
	//this will change of course, depending on who stays in
	ProcessBets();
	mv_Players[0]->mb_CurrentTurn = true;
	for (int i = 0; i<mv_Players.size(); i++) {
		mv_Players[i]->me_Action = PA_NONE;
	}

	//burn
	mh_Cards.DealCard();

	//show turn card
	turn = mh_Cards.DealCard();
	printf("The Turn:\n%s | %s | %s | %s\n", 
		flop1->shortName().c_str(), 
		flop2->shortName().c_str(),
		flop3->shortName().c_str(),
		turn->shortName().c_str());

	me_State = THSE_TURN;
}

void TexasHoldem::ProcessStateNone() {
	int  i;
	Bet* b;
	bool lb_NeedsReIndex = false;

	//remove players that have no chips
	vector<Player*>::iterator it = mv_Players.begin();
	while (it != mv_Players.end()) {
		if ((*it)->mi_ChipCount == 0) {
			delete(*it);
			it = mv_Players.erase(it);
			lb_NeedsReIndex = true;
		} else {
			++it;
		}
	}

	//re-index player id
	if (lb_NeedsReIndex) {
		for (i = 0; i<mv_Players.size(); i++) {
			Player* p = mv_Players[i];
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
	for (i = 0; i<mv_Players.size(); i++) {
		Player* p = mv_Players[i];
		printf("%dUP: CHIPS:%8d | W:%5d | L:%5d | T:%5d\n", i + 1, p->ChipCount(), p->Wins(), p->Losses(), p->Ties());
		p->me_Action = PA_NONE;
		//WRONG *****
		//USE DEALER CHIP
		if (i == 0)p->mb_CurrentTurn = true;
	}
	mh_Cards.Shuffle();

	//deal 1st card (starting to left of dealer)
	for (i = mi_Dealer + 1; i<mv_Players.size(); i++) {
		Card* c = mh_Cards.DealCard();
		mv_Players[i]->cards[0] = c;
	}
	for (i = 0; i<mi_Dealer + 1; i++) {
		Card* c = mh_Cards.DealCard();
		mv_Players[i]->cards[0] = c;
	}

	//deal 2nd card (starting to left of dealer)
	for (i = mi_Dealer + 1; i<mv_Players.size(); i++) {
		Card* c = mh_Cards.DealCard();
		mv_Players[i]->cards[1] = c;
	}
	for (i = 0; i<mi_Dealer + 1; i++) {
		Card* c = mh_Cards.DealCard();
		mv_Players[i]->cards[1] = c;
	}

	//show player 1 cards
	Card* p1c1 = mv_Players[0]->cards[0];
	Card* p1c2 = mv_Players[0]->cards[1];
	printf("Deck shuffled. Your Cards:\n%s | %s\n\n",
		p1c1->shortName().c_str(),
		p1c2->shortName().c_str());
	me_State = THSE_DEALT;
}

void TexasHoldem::ProcessStateRiver() {
	int i;
	vector<PokerHand*> hands;

	for (i = 0; i<mv_Players.size(); i++) {
		ScoreHand(mv_Players[i]->Hand(), flop1, flop2, flop3, turn, river, mv_Players[i]->cards[0], mv_Players[i]->cards[1]);

		Card* p1c1 = mv_Players[i]->cards[0];
		Card* p1c2 = mv_Players[i]->cards[1];
		char  title[5] = { ' ', ' ', ' ', ' ', 0 };

		if (i == mi_Dealer)
			strcpy_s(title, 5, "DEAL");
		if (i == sblind)
			strcpy_s(title, 5, "SBLI");
		if (i == bblind)
			strcpy_s(title, 5, "BBLI");

		printf("%dUP(%s): %s | %s\n->%s\n", i + 1, title, 
			p1c1->shortName().c_str(),
			p1c2->shortName().c_str(),
			mv_Players[i]->Hand().HandName());
	}

	//find the winner(s)
	Player** people = (Player**)malloc(mv_Players.size() * sizeof(Player*));
	for (i = 0; i<mv_Players.size(); i++) {
		people[i] = mv_Players[i];
		mv_Players[i]->mb_Winner = false;
	}
	CalculateWinner(people, mv_Players.size());
	free(people);

	int winnercount = 0;
	int winnerplayer = -1;

	for (i = 0; i<mv_Players.size(); i++) {
		if (mv_Players[i]->mb_Winner) {
			winnercount++;
			winnerplayer = i;
		}
	}

	if (winnercount == 1) {
		printf("%dUP wins\n\n", winnerplayer + 1);
		//		mv_Players[winnerplayer]->mi_ChipCount += mi_Pot;

		for (i = 0; i<mv_Players.size(); i++) {
			if (i != winnerplayer) {
				mv_Players[i]->mi_ChipCount--;
				mv_Players[i]->mi_Losses++;
			} else {
				mv_Players[i]->mi_Wins++;
			}
		}
	} else {
		printf("split pot:\n");
		for (i = 0; i<mv_Players.size(); i++) {
			if (mv_Players[i]->mb_Winner) {
				printf("%dUP wins\n", i + 1);
				//			mv_Players[i]->mi_ChipCount += (mi_Pot / winnercount);
				mv_Players[i]->mi_Ties++;
			} else {
				mv_Players[i]->mi_ChipCount--;
				mv_Players[i]->mi_Losses++;
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

	me_State = THSE_NONE;
}

void TexasHoldem::ProcessStateTurn() {
	//signal the next betting player 
	//this will change of course, depending on who stays in
	ProcessBets();
	mv_Players[0]->mb_CurrentTurn = true;
	for (int i = 0; i<mv_Players.size(); i++) {
		mv_Players[i]->me_Action = PA_NONE;
	}

	//burn
	mh_Cards.DealCard();

	//show river card
	river = mh_Cards.DealCard();
	printf("The River:\n%s | %s | %s | %s | %s\n\n",
		flop1->shortName().c_str(),
		flop2->shortName().c_str(),
		flop3->shortName().c_str(),
		turn->shortName().c_str(),
		river->shortName().c_str());
	me_State = THSE_RIVER;
}

void TexasHoldem::RotateDealerButton() {
	//THIS LOGIC ISN"T CORRECT (I THINK?) DOUBLE CHECK
	//rotate dealer + blinds
	if (++mi_Dealer >= mv_Players.size()) {
		mi_Dealer = 0;
	}

	if (++sblind >= mv_Players.size()) {
		sblind = 0;
	}

	if (++bblind >= mv_Players.size()) {
		bblind = 0;
	}
}