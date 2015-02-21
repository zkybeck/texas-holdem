#include "Texas.h"

void TexasHoldem::Load(char* apsz_Data, int ai_Length) {
	XmlDocument d;
	int i, j, k, l, m;

	UnloadPlayers();
	deck.Cards.clear();

	d.LoadXML(apsz_Data, ai_Length);

	for (i = 0; i<d.ChildNodes.Count(); i++) {
		if (!_strcmpi(d.ChildNodes.Item(i)->Name.c_str(), "CASINO")) {
			XmlNode* x = d.ChildNodes.Item(i);

			for (j = 0; j<x->ChildNodes().Count(); j++) {
				XmlNode* y = x->ChildNodes().Item(j);

				if (!_strcmpi(y->Name.c_str(), "STATE")) {
					if (!_strcmpi(y->InnerText.c_str(), "NONE")) {
						state = THSE_NONE;
					}
					if (!_strcmpi(y->InnerText.c_str(), "DEALT")) {
						state = THSE_DEALT;
					}
					if (!_strcmpi(y->InnerText.c_str(), "FLOP")) {
						state = THSE_FLOP;
					}
					if (!_strcmpi(y->InnerText.c_str(), "TURN")) {
						state = THSE_TURN;
					}
					if (!_strcmpi(y->InnerText.c_str(), "RIVER")) {
						state = THSE_RIVER;
					}
				}

				if (!_strcmpi(y->Name.c_str(), "AMOUNTTOCALL")) {
					//mi_AmountToCall = atoi(y->InnerText.c_str());
				}

				if (!_strcmpi(y->Name.c_str(), "COMMUNITYCARDS")) {
					for (k = 0; k<y->ChildNodes().Count(); k++) {
						XmlNode* z = y->ChildNodes().Item(k);

						if (!_strcmpi(z->Name.c_str(), "FLOP1")) {
							flop1 = deck.GetCardFromString(z->InnerText.c_str());
						}
						if (!_strcmpi(z->Name.c_str(), "FLOP2")) {
							flop2 = deck.GetCardFromString(z->InnerText.c_str());
						}
						if (!_strcmpi(z->Name.c_str(), "FLOP3")) {
							flop3 = deck.GetCardFromString(z->InnerText.c_str());
						}
						if (!_strcmpi(z->Name.c_str(), "TURN")) {
							turn = deck.GetCardFromString(z->InnerText.c_str());
						}
						if (!_strcmpi(z->Name.c_str(), "RIVER")) {
							river = deck.GetCardFromString(z->InnerText.c_str());
						}
					}
				}

				if (!_strcmpi(y->Name.c_str(), "CARDDECK")) {
					for (k = 0; k<y->ChildNodes().Count(); k++) {
						XmlNode* z = y->ChildNodes().Item(k);

						if (!_strcmpi(z->Name.c_str(), "CARD")) {
							Card* card = deck.GetCardFromString(z->InnerText.c_str());
							if (card) deck.Cards.push_back(card);
						}
					}
				}

				if (!_strcmpi(y->Name.c_str(), "PLAYERS")) {
					for (k = 0; k<y->ChildNodes().Count(); k++) {
						XmlNode* z = y->ChildNodes().Item(k);

						if (!_strcmpi(z->Name.c_str(), "PLAYER")) {
							Player* p = new Player(-1);

							for (l = 0; l<z->ChildNodes().Count(); l++) {
								XmlNode* zz = z->ChildNodes().Item(l);

								if (!_strcmpi(zz->Name.c_str(), "PLAYERID")) {
									p->mi_PlayerID = atoi(zz->InnerText.c_str());
								}

								if (!_strcmpi(zz->Name.c_str(), "HAND")) {
									for (m = 0; m<zz->ChildNodes().Count(); m++) {
										XmlNode* zzz = zz->ChildNodes().Item(m);

										if (!_strcmpi(zzz->Name.c_str(), "TYPE")) {
											p->mh_Hand.me_Hand = PokerHand::GetHandFromString(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "THREEOFAKIND")) {
											p->mh_Hand.threeofakind = atoi(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "FOUROFAKIND")) {
											p->mh_Hand.fourofakind = atoi(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "PAIR1")) {
											p->mh_Hand.pair1 = atoi(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "PAIR2")) {
											p->mh_Hand.pair2 = atoi(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "STRAIGHTHIGHCARD")) {
											p->mh_Hand.straightHighCard = atoi(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "FULLHOUSE3")) {
											p->mh_Hand.fullhouse3 = atoi(zzz->InnerText.c_str());
										}
										if (!_strcmpi(zzz->Name.c_str(), "FULLHOUSE2")) {
											p->mh_Hand.fullhouse2 = atoi(zzz->InnerText.c_str());
										}
									}
								}

								if (!_strcmpi(zz->Name.c_str(), "WINNER")) {
									if (!_strcmpi(zz->InnerText.c_str(), "TRUE"))
										p->mb_Winner = true;
									else
										p->mb_Winner = false;
								}
								if (!_strcmpi(zz->Name.c_str(), "WINS")) {
									p->mi_Wins = atoi(zz->InnerText.c_str());
								}
								if (!_strcmpi(zz->Name.c_str(), "LOSSES")) {
									p->mi_Losses = atoi(zz->InnerText.c_str());
								}
								if (!_strcmpi(zz->Name.c_str(), "TIES")) {
									p->mi_Ties = atoi(zz->InnerText.c_str());
								}
								if (!_strcmpi(zz->Name.c_str(), "CHIPCOUNT")) {
									p->mi_ChipCount = atoi(zz->InnerText.c_str());
								}
								if (!_strcmpi(zz->Name.c_str(), "TURN")) {
									for (m = 0; m<zz->Attributes().Count(); m++) {
										if (!_strcmpi(zz->Attributes().Item(m)->Name.c_str(), "ACTION")) {
											const char* text = zz->Attributes().Item(m)->Value.c_str();

											if (!_strcmpi(text, "RAISE")) {
												p->me_Action = PA_RAISE;
											} else if (!_strcmpi(text, "CHECK")) {
												p->me_Action = PA_CHECK;
											} else if (!_strcmpi(text, "FOLD")) {
												p->me_Action = PA_FOLD;
											} else if (!_strcmpi(text, "ALLIN")) {
												p->me_Action = PA_ALLIN;
											} else {
												p->me_Action = PA_NONE;
											}
										}

										if (!_strcmpi(zz->Attributes().Item(m)->Name.c_str(), "AMOUNT")) {
											p->mi_BetAmount = atoi(zz->Attributes().Item(m)->Value.c_str());
										}

										if (!_strcmpi(zz->Attributes().Item(m)->Name.c_str(), "CURRENTTURN")) {
											if (!_strcmpi(zz->Attributes().Item(m)->Value.c_str(), "TRUE"))
												p->mb_CurrentTurn = true;
										}
									}


								}
								if (!_strcmpi(zz->Name.c_str(), "CARDS")) {
									for (m = 0; m<zz->ChildNodes().Count(); m++) {
										XmlNode* zzz;

										if (m>1)break;

										zzz = zz->ChildNodes().Item(m);
										if (!_strcmpi(zzz->Name.c_str(), "CARD")) {
											Card* card = deck.GetCardFromString(zzz->InnerText.c_str());
											if (card)p->cards[m] = card;
										}
									}
								}
							}

							players.push_back(p);
						}
					}
				}
			}
		}
	}

	//sort players in order of their ID (just in case)
	if (players.size()>0) {
		sort(players.begin(), players.end(), Player::ComparePlayer);
	}

	return;
}

//save the state of the game as XML
char* TexasHoldem::Save(int* ap_Length) {
	XmlDocument   d;
	XmlNode*      root, *players, *cards, *x;
	XmlAttribute* a;
	int           i;
	char          lsz_Temp[64];
	string        sTemp;

	if (!ap_Length) return 0;
	*ap_Length = 0;

	//document element
	root = new XmlNode();
	root->Name = "Casino";
	a = new XmlAttribute();
	a->Name = "GameType";
	a->Value = "TexasHoldem";
	root->Attributes().AddAttribute(a);

	//state
	x = new XmlNode();
	x->Name = "State";
	x->InnerText = stateNames[(int)state];
	root->ChildNodes().AddNode(x);

	//amount to call
	//x = new XmlNode();
	//x->Name = "AmountToCall";
	//sprintf(lsz_Temp,"%d",mi_AmountToCall);
	//x->InnerText = lsz_Temp;
	//root->ChildNodes().AddNode(x);

	//community cards
	cards = new XmlNode();
	cards->Name = "CommunityCards";
	//..
	if (flop1) {
		x = new XmlNode();
		x->Name = "Flop1";
		x->InnerText = flop1->longName();
		cards->ChildNodes().AddNode(x);
	}

	if (flop2) {
		x = new XmlNode();
		x->Name = "Flop2";
		x->InnerText = flop2->longName();
		cards->ChildNodes().AddNode(x);
	}

	if (flop3) {
		x = new XmlNode();
		x->Name = "Flop3";
		x->InnerText = flop3->longName();
		cards->ChildNodes().AddNode(x);
	}

	if (turn) {
		x = new XmlNode();
		x->Name = "Turn";
		x->InnerText = turn->longName();
		cards->ChildNodes().AddNode(x);
	}

	if (river) {
		x = new XmlNode();
		x->Name = "River";
		x->InnerText = river->longName();
		cards->ChildNodes().AddNode(x);
	}
	//..
	root->ChildNodes().AddNode(cards);

	//card deck
	cards = new XmlNode();
	cards->Name = "CardDeck";
	for (i = 0; i<deck.Cards.size(); i++) {
		x = new XmlNode();
		x->Name = "Card";
		x->InnerText = deck.Cards[i]->longName();
		cards->ChildNodes().AddNode(x);
	}
	root->ChildNodes().AddNode(cards);

	//players
	players = new XmlNode();
	players->Name = "Players";
	root->ChildNodes().AddNode(players);

	for (i = 0; i<this->players.size(); i++) {
		XmlNode* player, *hand;
		Player*  p = this->players[i];

		player = new XmlNode();
		player->Name = "Player";

		x = new XmlNode();
		x->Name = "PlayerID";
		sprintf(lsz_Temp, "%d", p->mi_PlayerID);
		x->InnerText = lsz_Temp;
		player->ChildNodes().AddNode(x);

		hand = new XmlNode();
		hand->Name = "Hand";
		//..
		x = new XmlNode();
		x->Name = "Type";
		x->InnerText = p->mh_Hand.HandName();
		hand->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "ThreeOfAKind";
		sprintf(lsz_Temp, "%d", p->mh_Hand.threeofakind);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "FourOfAKind";
		sprintf(lsz_Temp, "%d", p->mh_Hand.fourofakind);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "Pair1";
		sprintf(lsz_Temp, "%d", p->mh_Hand.pair1);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "Pair2";
		sprintf(lsz_Temp, "%d", p->mh_Hand.pair2);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);

		//high cards

		x = new XmlNode();
		x->Name = "StraightHighCard";
		sprintf(lsz_Temp, "%d", p->mh_Hand.straightHighCard);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "FullHouse3";
		sprintf(lsz_Temp, "%d", p->mh_Hand.fullhouse3);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "FullHouse2";
		sprintf(lsz_Temp, "%d", p->mh_Hand.fullhouse2);
		x->InnerText = lsz_Temp;
		hand->ChildNodes().AddNode(x);
		//..
		player->ChildNodes().AddNode(hand);

		x = new XmlNode();
		x->Name = "Winner";
		x->InnerText = p->mb_Winner ? "True" : "False";
		player->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "Wins";
		sprintf(lsz_Temp, "%d", p->mi_Wins);
		x->InnerText = lsz_Temp;
		player->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "Losses";
		sprintf(lsz_Temp, "%d", p->mi_Losses);
		x->InnerText = lsz_Temp;
		player->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "Ties";
		sprintf(lsz_Temp, "%d", p->mi_Ties);
		x->InnerText = lsz_Temp;
		player->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "ChipCount";
		sprintf(lsz_Temp, "%d", p->mi_ChipCount);
		x->InnerText = lsz_Temp;
		player->ChildNodes().AddNode(x);

		//turn
		x = new XmlNode();
		x->Name = "Turn";
		a = new XmlAttribute();
		a->Name = "Action";
		lsz_Temp[0] = 0;
		switch (p->me_Action) {
			case PA_NONE:
				strcpy(lsz_Temp, "None");
				break;
			case PA_RAISE:
				strcpy(lsz_Temp, "Raise");
				break;
			case PA_CHECK:
				strcpy(lsz_Temp, "Check");
				break;
			case PA_FOLD:
				strcpy(lsz_Temp, "Fold");
				break;
			case PA_ALLIN:
				strcpy(lsz_Temp, "AllIn");
				break;
		}
		a->Value = lsz_Temp;
		x->Attributes().AddAttribute(a);
		if (p->mi_BetAmount>0) {
			a = new XmlAttribute();
			a->Name = "Amount";
			sprintf(lsz_Temp, "%d", p->mi_BetAmount);
			a->Value = lsz_Temp;
			x->Attributes().AddAttribute(a);
		}
		if (p->mb_CurrentTurn) {
			a = new XmlAttribute();
			a->Name = "CurrentTurn";
			a->Value = "True";
			x->Attributes().AddAttribute(a);
		}
		player->ChildNodes().AddNode(x);

		//cards
		cards = new XmlNode();
		cards->Name = "Cards";
		//..
		x = new XmlNode();
		x->Name = "Card";
		x->InnerText = p->cards[0]->longName();
		cards->ChildNodes().AddNode(x);

		x = new XmlNode();
		x->Name = "Card";
		x->InnerText = p->cards[1]->longName();
		cards->ChildNodes().AddNode(x);
		//..
		player->ChildNodes().AddNode(cards);

		players->ChildNodes().AddNode(player);
	}

	d.ChildNodes.AddNode(root);

	char* temp = d.SaveToMemory(ap_Length);
	return temp;
}