modded class BaseBuildingBase extends ItemBase
{
	
	protected RaidAlarm_Base m_Alarm;
	
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef){		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		if (GetRaidAlarmConfig().VanillaDamageThresold >= 0 && !GetGame().IsClient() && !(damageType == DT_CLOSE_COMBAT && source.IsMan()) && damageResult && damageResult.GetHighestDamage("") > GetRaidAlarmConfig().VanillaDamageThresold){
			SetOffAlarm();
		}
	}
	
	void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		string itemType = item.GetType();
		itemType.ToLower();
		if (item.IsRuined() && (itemType == "codelock" || itemType == "Padlock_Heavy" || itemType == "Padlock" || itemType == "CombinationLock" || itemType == "CombinationLock4")){
			SetOffAlarm();
		}
	}
	
	void ~BaseBuildingBase(){
		if (GetGame() && GetGame().IsServer()){
			//Call Later to prevent it from going off on shutdown
			Print("[RAIDALARM] ~BaseBuildingBase SetOffAlarm");
			if (m_Alarm){
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_Alarm.SetOffAlarm, 50);
			}
		}
	}
	
	//Returns true if it set successfully 
	bool SetRaidAlarm(RaidAlarm_Base alarm){
		if (!m_Alarm){
			m_Alarm = RaidAlarm_Base.Cast(alarm);
			return true;
		}
		return false;
	}
	
	
	bool SetOffAlarm(){
		if (!m_Alarm){
			return false;
		}
		return m_Alarm.TriggerAlarm();
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false ){
		super.OnPartDestroyedServer( player, part_name, action_id, destroyed_by_connected_part );
		
		Print("[RAIDALARM] OnPartDestroyedServer");
		SetOffAlarm();
	}
	
}
