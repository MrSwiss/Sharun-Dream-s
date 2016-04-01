#include "Player.h"
#include <math.h>

float HP_MOD[CLASS_MAX][LV_Max+1];

void init_HP_Mod()
{
	for (int i=0; i<CLASS_MAX; i++) {
		HP_MOD[i][0] = 1;
		for (int j=1; j<LV_Max+1; j++)
			HP_MOD[i][j] = pow(HP_CLASS_MOD[i], j-1);
	}
	int i=0;
	for (int j=1; j<LV_Max+1; j++) {
		if (!Experience_LEVEL[j]) i = 1;
		if (i) Experience_LEVEL[j] = Experience_LEVEL[j-1] * 1.2;
	}
}
