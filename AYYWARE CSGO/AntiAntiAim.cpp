/*
Rest In Peace ApocalypseCheats
*/

// Credits to Valve and Shad0w
#include "Interfaces.h"
#include "Menu.h"
#include "Hacks.h"
#include "Proxies.h"
// Shad0ws Yaw fix
// (FIX ME UP LATER)
void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	static Vector vLast[65];
	static bool bShotLastTime[65];
	static bool bJitterFix[65];

	float *flPitch = (float*)((DWORD)pOut - 4);
	float flYaw = pData->m_Value.m_Float;
	bool bHasAA;
	bool bSpinbot;
	
	static float OldLowerBodyYaws[65];
	static float OldYawDeltas[65];
	IClientEntity* player = (IClientEntity*)pStruct;
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!player || !pLocal || pLocal == player || player->GetTeamNum() == pLocal->GetTeamNum()) {
		return;
	}

	if (Menu::Window.RageBotTab.AimbotExtraResolver.GetState()) {

		float CurYaw = player->GetLowerBodyYaw();
		if (OldLowerBodyYaws[player->GetIndex()] != CurYaw) {
			OldYawDeltas[player->GetIndex()] = player->GetEyeAngles().y - CurYaw;
			OldLowerBodyYaws[player->GetIndex()] = CurYaw;
			//*(float*)((uintptr_t)player + offsets.m_angEyeAngles + 4) = CurYaw;
			enemyLBYDelta[player->GetIndex()] = CurYaw;
			*(float*)(pOut) = CurYaw;

			return;
		}
		else {
			//*(float*)((uintptr_t)player + offsets.m_angEyeAngles + 4) = player->GetEyeAngles().y - OldYawDeltas[i];
			enemyLBYDelta[player->GetIndex()] = player->GetEyeAngles().y - OldYawDeltas[player->GetIndex()];
			*(float*)(pOut) = player->GetEyeAngles().y - OldYawDeltas[player->GetIndex()];

		}

		float yClamped = player->GetEyeAngles().y;
		while (yClamped < -180.0f)
			yClamped += 360.0f;

		while (yClamped > 180.0f)
			yClamped -= 360.0f;
		//*(float*)((uintptr_t)player + offsets.m_angEyeAngles + 4) = yClamped;
		enemyLBYDelta[player->GetIndex()] = yClamped;
		*(float*)(pOut) = yClamped;
	}
	else {
		*(float*)(pOut) = flYaw;
	}
		/*
		bHasAA = ((*flPitch == 90.0f) || (*flPitch == 270.0f));
		bSpinbot = false;

		if (!bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()]
			&& (fabsf(flYaw - vLast[((IClientEntity*)(pStruct))->GetIndex()].y) > 15.0f) && !bHasAA)
		{
			flYaw = vLast[((IClientEntity*)(pStruct))->GetIndex()].y;
			bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()] = true;
		}
		else
		{
			if (bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()]
				&& (fabsf(flYaw - vLast[((IClientEntity*)(pStruct))->GetIndex()].y) > 15.0f))
			{
				bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()] = true;
				bSpinbot = true;
			}
			else
			{
				bShotLastTime[((IClientEntity*)(pStruct))->GetIndex()] = false;
			}
		}

		vLast[((IClientEntity*)(pStruct))->GetIndex()].y = flYaw;


		bool bTmp = bJitterFix[((IClientEntity*)(pStruct))->GetIndex()];

		bJitterFix[((IClientEntity*)(pStruct))->GetIndex()] = (flYaw >= 180.0f && flYaw <= 360.0f);

		if (bTmp && (flYaw >= 0.0f && flYaw <= 180.0f))
		{
			flYaw += 359.0f;
		}

		*/
		
	//*(float*)(pOut) = flYaw;
}
// Simple fix for Fake-Down
void FixX(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	float* ang = (float*)pOut;
	*ang = pData->m_Value.m_Float;

	if (!Menu::Window.RageBotTab.AccuracyResolver.GetIndex()) return;

	if (pData->m_Value.m_Float > 180.0f)
		*ang -= 360.0f;
	else if (pData->m_Value.m_Float < -180.0f)
		*ang += 360.0f;

	if (pData->m_Value.m_Float > 89.0f && pData->m_Value.m_Float < 91.0f)
		*ang -= 90.0f;
	else if (pData->m_Value.m_Float > -89.0f && pData->m_Value.m_Float < -91)
		*ang += 90.0f;

	if (*ang < -179.648438f || *ang > 179.648438f)
	{
		*ang = -10.0f;
	}

	if (*ang <= -88.945313f && *ang >= -179.648438f)
	{
		*ang = -89.0f;
	}

	if (*ang >= 88.945313f && *ang <= 179.648438f)
	{
		*ang = 89.0f;
	}


	if (*ang > 89)
	{
		*ang = 270;
	}
	if (*ang < -89)
	{
		*ang = 90;
	}

	if (*ang > 36000088.000000)
	{
		*ang = 89;
	}

	if (*ang > 1800089)
	{
		*ang = 89;
	}

}



RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	int default_t = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	// Get local player (just to stop replacing spectators knifes)
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Menu::Window.SkinchangerTab.SkinEnable.GetState() && pLocal)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == iBowie))
		{
			// Set whatever knife we want
			if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 1)
				pData->m_Value.m_Int = iBowie;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 2)
				pData->m_Value.m_Int = iButterfly;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 3)
				pData->m_Value.m_Int = iFalchion;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 4)
				pData->m_Value.m_Int = iFlip;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 5)
				pData->m_Value.m_Int = iGut;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 6)
				pData->m_Value.m_Int = iHuntsman;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 7)
				pData->m_Value.m_Int = iKarambit;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 8)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 9)
				pData->m_Value.m_Int = iDagger;
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}
#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
void LowerBodyYawHook(const CRecvProxyData* data, void *pStruct, void *out)
{
	

	IClientEntity* player = (IClientEntity*)pStruct;
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	static float savedLBY;


	if (!pLocal)
		return;
	
	if (player != pLocal && player->GetTeamNum() != pLocal->GetTeamNum()) { // Gegnger sind nicht wir und ist auch nicht in unserem Team ...
		
		float NowLBY = data->m_Value.m_Float;
		float testLBY = NowLBY;

		if (NowLBY > 180.0)
		{
			do
				testLBY = testLBY - 360.0;
			while (testLBY > 180.0);
			NowLBY = testLBY;
		}

		if (testLBY < -180.0)
		{
			do
				testLBY = testLBY + 360.0;
			while (testLBY < -180.0);
			NowLBY = testLBY;
		}

		enemysLastProxyTimer[player->GetIndex()] = Interfaces::Globals->curtime - nci->GetAvgLatency(FLOW_OUTGOING);

		*(float*)out = NowLBY;

	}



	if (player == pLocal) {
		float NowLBY = data->m_Value.m_Float;
		static float lowerBodyYawUpdateTime;
		static float StoredTime;
		
		// HIER WURDE LBY GEUPDATED DAS LETZE MAL...
		consoleProxyLbyLASTUpdateTime = pLocal->GetTickBase() - TIME_TO_TICKS(nci->GetAvgLatency(FLOW_OUTGOING));
		
		
		//lowerBodyYawUpdateTime = pCmd->tick_count - TIME_TO_TICKS(nci->GetAvgLatency(FLOW_OUTGOING));
		lowerBodyYawUpdateTime = Interfaces::Globals->curtime - nci->GetAvgLatency(FLOW_OUTGOING);

		float testLBY;
		testLBY = NowLBY;


		if (NowLBY > 180.0)
		{
			do
				testLBY = testLBY - 360.0;
			while (testLBY > 180.0);
			NowLBY = testLBY;
		}

		if (testLBY < -180.0)
		{
			do
				testLBY = testLBY + 360.0;
			while (testLBY < -180.0);
			NowLBY = testLBY;
		}

		//testFloat1 = fsnLBY;
		testFloat2 = lowerBodyYawUpdateTime;
		//testFloat3 = LastLBYUpdateTime;
		//fsnLBY = NowLBY;
		*(float*)out = NowLBY;

	}




			
}



void ApplyAAAHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, "m_flLowerBodyYawTarget"))
				{
					pProp->m_ProxyFn = LowerBodyYawHook;
				}

				// Pitch Fix
				if (!strcmp(name, "m_angEyeAngles[0]"))
				{
					pProp->m_ProxyFn = FixX;
				}

				// Yaw Fix
				if (!strcmp(name, "m_angEyeAngles[1]"))
				{
					Utilities::Log("Yaw Fix Applied");
					pProp->m_ProxyFn = FixY;
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
			for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
				if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
					// Search for the 'm_nModelIndex' property.
					RecvTable* pClassTable = pClass->m_pRecvTable;

					for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
						RecvProp* pProp = &pClassTable->m_pProps[nIndex];

						if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
							continue;

						// Store the original proxy function.
						fnSequenceProxyFn = (RecvVarProxyFn)pProp->m_ProxyFn;

						// Replace the proxy function with our sequence changer.
						pProp->m_ProxyFn = SetViewModelSequence;

						break;
					}

					break;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}