#ifndef TEXAS_H
#define TEXAS_H

#include "Poker.h"
#include "xml/XML.h"
using namespace Kahnadex;
using namespace Kahnadex::XML;

enum TexasHoldemState {
	THSE_NONE,
	THSE_DEALT,
	THSE_FLOP,
	THSE_TURN,
	THSE_RIVER
};

string stateNames[];

class Bet {
public:
	int              BetID;
	int              PlayerID;
	int              ChipAmount;
	int              Round;
	TexasHoldemState State;
	PlayerAction     Action;
	bool             BlindCollectionBig;
	bool             BlindCollectionSmall;

public:
	Bet() {
		BetID = -1;
		PlayerID = -1;
		ChipAmount = 0;
		Round = 0;
		State = THSE_NONE;
		Action = PA_NONE;
		BlindCollectionBig = false;
		BlindCollectionSmall = false;
	}
};

class Pot {
public:
	int             PotID;
	int             BetMax;
	int             ToCall;
	vector<Bet*>    BetHistory;
	vector<Pot*>    SidePots;
	vector<Player*> Players;

public:
	int  GetPotValue() { return(0); }
	void Reset() {
		int i;

		for (i = 0; i<SidePots.size(); i++) {
			delete SidePots[i];
		}
		/*mh_Pot.SidePots.clear();
		mh_Pot.Players.clear();
		for(i=0;i<mh_Pot.BetHistory.size();i++){
		delete mh_Pot.BetHistory[i];
		}
		mh_Pot.BetHistory.clear();
		mh_Pot.ToCall=-1;
		mh_Pot.BetMax=-1;
		mh_Pot.PotID=-1;*/
	}
	void SplitPot() {
		Pot* p = new Pot;
		//p->PotID = mp_CurrentPot->PotID+1;
		//....
	}

	Pot() {
		PotID = -1;
		BetMax = -1;
		ToCall = -1;
	}
	~Pot() {
		Reset();
	}
};

class TexasHoldem : public Poker {
	//members
private:
	TexasHoldemState me_State;
	int              mi_Dealer, sblind, bblind;
	Card*            flop1, *flop2, *flop3, *turn, *river;
	Pot              mh_Pot;
	int              mi_BetID;
	Pot*             mp_CurrentPot;

	//methods
private:
	void  ProcessBets();
	void  ProcessStateDealt();
	void  ProcessStateFlop();
	void  ProcessStateNone();
	void  ProcessStateRiver();
	void  ProcessStateTurn();
	void  RotateDealerButton();
public:
	void  Load(char* apsz_Data, int ai_Length);
	void  Process();
	char* Save(int* ap_Length);

	TexasHoldem() {
		me_State = THSE_NONE;

		//players (this is temporary; it's hard coded for now)
		Player* p = new Player(0);
		mv_Players.push_back(p);
		p = new Player(1, AI_BLUFFER);
		mv_Players.push_back(p);
		p = new Player(2, AI_CONSERVATIVE);
		mv_Players.push_back(p);
		p = new Player(3, AI_SMART);
		mv_Players.push_back(p);

		//dealer button / blinds
		mi_Dealer = 0;
		sblind = 1;
		bblind = 2;

		//bet index
		mi_BetID = 0;

		//pot
		mp_CurrentPot = &mh_Pot;
		mh_Pot.PotID = 0;

		//community cards
		flop1 = flop2 = flop3 = turn = river = 0;
	}
};

#endif