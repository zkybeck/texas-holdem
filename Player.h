//Player.h
//poker
//------------------------------------------
#ifndef PLAYER_H
#define PLAYER_H
//------------------------------------------
#include "PokerHand.h"
//------------------------------------------
enum PlayerAction
{
	PA_NONE,
	PA_RAISE,
	PA_CHECK,
	PA_FOLD,
	PA_ALLIN
};

enum PlayerType
{
	PT_HUMAN,
	PT_COMPUTER
};

enum AIType
{
	AI_BLUFFER,
	AI_CONSERVATIVE,
	AI_SMART
};
//------------------------------------------
class Player
{
	//friends
	friend class Poker;
	friend class TexasHoldem;

	//members
	private:
		int          mi_PlayerID;
		PokerHand    mh_Hand;
		bool         mb_Winner;
		int          mi_Wins,mi_Losses,mi_Ties;
		int          mi_ChipCount;
		PlayerType   me_PlayerType;
	public:
		PlayerAction me_Action;
		int          mi_BetAmount;
		bool         mb_CurrentTurn;
		bool         mb_ActionRequired;
		AIType       me_AIType;
		int          mi_InFor;
		Card*        cards[2];

	//methods
	public:
		int ChipCount(void){
			return(mi_ChipCount);
		}
		static bool ComparePlayer(const Player* a,const Player* b){
			return(a->mi_PlayerID<b->mi_PlayerID);
		}
		PokerHand& Hand(void){
			return(mh_Hand);
		}
		bool IsComputer(void){
			return(me_PlayerType==PT_HUMAN);
		}
		int Losses(void){
			return(mi_Losses);
		}
		int Ties(void){
			return(mi_Ties);
		}
		int Wins(void){
			return(mi_Wins);
		}

		Player(int ai_ID){
			mi_PlayerID = ai_ID;
			mb_Winner = false;
			mi_Wins = mi_Losses = mi_Ties = 0;
			mi_ChipCount = 0;
			me_Action = PA_NONE;
			mi_BetAmount = 0;
			mb_CurrentTurn=false;
			cards[0] = cards[1] = 0;

			mi_InFor=0;
		}

		Player(int ai_ID,AIType ae_AIType){
			mi_PlayerID = ai_ID;
			mb_Winner = false;
			mi_Wins = mi_Losses = mi_Ties = 0;
			mi_ChipCount = 0;
			me_Action = PA_NONE;
			mi_BetAmount = 0;
			mb_CurrentTurn=false;
			cards[0] = cards[1] = 0;

			me_AIType = ae_AIType;
			me_PlayerType = PT_COMPUTER;

			mi_InFor=0;
		}
};
//------------------------------------------
#endif