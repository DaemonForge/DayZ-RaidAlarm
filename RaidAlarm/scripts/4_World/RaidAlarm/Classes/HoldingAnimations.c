modded class ModItemRegisterCallbacks
{	
    override void RegisterHeavy(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior){
		super.RegisterHeavy(pType, pBehavior);
	
		pType.AddItemInHandsProfileIK("RaidAlarm_PowerSupply", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,				 		"dz/anims/anm/player/ik/heavy/sea_chest.anm");
		pType.AddItemInHandsProfileIK("RaidAlarm_ServerCluster", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,				 		"dz/anims/anm/player/ik/heavy/car_tent_packed.anm");
		pType.AddItemInHandsProfileIK("RaidAlarm_CommunicationsArray", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,				 		"dz/anims/anm/player/ik/heavy/sea_chest.anm");
		pType.AddItemInHandsProfileIK("RaidAlarm_Server", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,				 		"dz/anims/anm/player/ik/heavy/sea_chest.anm");
		
	}
};