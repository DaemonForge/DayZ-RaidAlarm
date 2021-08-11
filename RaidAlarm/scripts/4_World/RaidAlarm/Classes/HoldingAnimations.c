modded class ModItemRegisterCallbacks
{
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterOneHanded(pType, pBehavior);
		//pType.AddItemInHandsProfileIK("RaidAlarm_Base", "dz/anims/workspaces/player/player_main/props/player_main_1h_crowbar.asi", pBehavior,	"dz/anims/anm/player/ik/gear/hatchet.anm");
	};
};