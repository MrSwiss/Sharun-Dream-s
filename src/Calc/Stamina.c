#include "Player.h"

float Stamina(player_t *player)
{
	if (player->Stats.Stamina[1] >= 120)
	      return 1.3;
	switch (player->Stats.Stamina[1])
	{
		case 119: return 1.299; break;
		case 118: return 1.292; break;
		case 117: return 1.291; break;
		case 116: return 1.285; break;
		case 115: return 1.28; break;
		case 114: return 1.279; break;
		case 113: return 1.274; break;
		case 112: return 1.269; break;
		case 111: return 1.264; break;
		case 110: return 1.259; break;
		case 109: return 1.254; break;
		case 108: return 1.254; break;
		case 107: return 1.249; break;
		case 106: return 1.242; break;
		case 105: return 1.237; break;
		case 104: return 1.234; break;
		case 103: return 1.232; break;
		case 102: return 1.226; break;
		case 101: return 1.222; break;
		case 100: return 1.219; break;
		case 99: return 1.212; break;
		case 98: return 1.209; break;
		case 97: return 1.207; break;
		case 96: return 1.202; break;
		case 95: return 1.197; break;
		case 94: return 1.194; break;
		case 93: return 1.191; break;
		case 92: return 1.184; break;
		case 91: return 1.18; break;
		case 90: return 1.178; break;
		case 89: return 1.171; break;
		case 88: return 1.168; break;
		case 87: return 1.164; break;
		case 86: return 1.158; break;
		case 85: return 1.155; break;
		case 84: return 1.153; break;
		case 83: return 1.148; break;
		case 82: return 1.141; break;
		case 81: return 1.138; break;
		case 80: return 1.135; break;
		case 79: return 1.13; break;
		case 78: return 1.126; break;
		case 77: return 1.12; break;
		case 76: return 1.117; break;
		case 75: return 1.113; break;
		case 74: return 1.11; break;
		case 73: return 1.107; break;
		case 72: return 1.098; break;
		case 71: return 1.098 ; break;
		case 70: return 1.092; break;
		case 69: return 1.088; break;
		case 68: return 1.085; break;
		case 67: return 1.08; break;
		case 66: return 1.077; break;
		case 65: return 1.074; break;
		case 64: return 1.066; break;
		case 63: return 1.064; break;
		case 62: return 1.062 ; break;
		case 61: return 1.056; break;
		case 60: return 1.044; break;
		case 59: return 1.044; break;
		case 58: return 1.044; break;
		case 57: return 1.039; break;
		case 56: return 1.035; break;
		case 55: return 1.03; break;
		case 54: return 1.027; break;
		case 53: return 1.023; break;
		case 52: return 1.02; break;
		case 51: return 1.015; break;
		case 50: return 1.009; break;
		case 49: return 1.04; break;
		case 48: return 1.02; break;
		case 47: return 1.0; break;
		case 46: return 1.0; break;
		case 45: return 1.0; break;
		case 44: return 1.0; break;
		case 43: return 1.0; break;
		case 42: return 1.0; break;
		case 41: return 1.0; break;
		case 40: return 1.0; break;
		case 39: return 1.0; break;
		case 38: return 1.0; break;
		case 37: return 1.0; break;
		case 36: return 1.0; break;
		case 35: return 1.0; break;
		case 34: return 1.0; break;
		case 33: return 1.0; break;
		case 32: return 1.0; break;
		case 31: return 1.0; break;
		case 30: return 1.0; break;
		case 29: return 1.0; break;
		case 28: return 1.0; break;
		case 27: return 1.0; break;
		case 26: return 1.0; break;
		case 25: return 1.0; break;
		case 24: return 0.998; break;
		case 23: return 0.996; break;
		case 22: return 0.99; break;
		case 21: return 0.973; break;
		case 20: return 0.962; break;
		case 19: return 0.96; break;
		case 18: return 0.953; break;
		case 17: return 0.947; break;
		case 16: return 0.937; break;
		case 15: return 0.927; break;
		case 14: return 0.92; break;
		case 13: return 0.915; break;
		case 12: return 0.904; break;
		case 11: return 0.897; break;
		case 10: return 0.89; break;
		case 9: return 0.877; break;
		case 8: return 0.87; break;
		case 7: return 0.865; break;
		case 6: return 0.852; break;
		case 5: return 0.843; break;
		case 4: return 0.837; break;
		case 3: return 0.83; break;
		case 2: return 0.824; break;
		case 1: return 0.81; break;
	}
	return 0.8;
}

float Stamina_power(player_t *player)
{
	if (player->Stats.Stamina[1] >= 75) return 5.255;
	if (player->Stats.Stamina[1] >= 70) return 5.1;
	if (player->Stats.Stamina[1] >= 67) return 5.0;
	if (player->Stats.Stamina[1] >= 65) return 4.95;
	if (player->Stats.Stamina[1] >= 55) return 4.65;
	if (player->Stats.Stamina[1] >= 50) return 4.6;
	return 4.0;
}

float Stamina_defense(player_t *player)
{
	if (player->Stats.Stamina[1] >= 78) return 1.28;
	if (player->Stats.Stamina[1] >= 76) return 1.26;
	if (player->Stats.Stamina[1] >= 75) return 1.25;
	if (player->Stats.Stamina[1] >= 73) return 1.2;
	if (player->Stats.Stamina[1] >= 63) return 1.1;
	if (player->Stats.Stamina[1] >= 35) return 0.9;
	return 0.81;
}

float Stamina_owerturnPower(player_t *player)
{
	if (player->Stats.Stamina[1] >= 90) return 1.1;
	if (player->Stats.Stamina[1] >= 80) return 1.25;
	if (player->Stats.Stamina[1] >= 60) return 1.6;
	if (player->Stats.Stamina[1] >= 50) return 2.0;
	if (player->Stats.Stamina[1] >= 40) return 2.6;
	return 3;
}

float Stamina_owerturnRcpt(player_t *player)
{
	if (player->Stats.Stamina[1] >= 70) return 1.45;
	if (player->Stats.Stamina[1] >= 60) return 1.5;
	if (player->Stats.Stamina[1] >= 50) return 2.0;
	if (player->Stats.Stamina[1] >= 40) return 2.3;
	if (player->Stats.Stamina[1] >= 30) return 3.25;
	if (player->Stats.Stamina[1] >= 25) return 3;
	return 5;
}
