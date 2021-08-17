
class RaidAlarm_ServerBattery extends TruckBattery{}
class RaidAlarm_Dish extends ItemBase{}

class RaidAlarm_Server extends RaidAlarm_Base{
	
	protected bool m_IsDeployed = false;
	
	protected int slot_ServerBattery = InventorySlots.INVALID;
	
	protected int TimeOfLastDrain = 0;
	
	void RaidAlarm_Server(){
		RegisterNetSyncVariableBool("m_IsDeployed");
		slot_ServerBattery = InventorySlots.GetSlotIdFromString("BatteryServer");
	}
	
	override bool NameOverride(out string output)
    {
        if(GetGame().IsClient() && CanSetOffAlarm()) {
            output = ConfigGetString("displayName") + " (ONLINE)";
            return true;
        } else if (GetGame().IsClient()) {
            output = ConfigGetString("displayName") + " (OFFLINE)";
            return true;
		}
        return super.NameOverride(output);
    }
	
	bool HasDish(){
		if (GetInventory() && RaidAlarm_Dish.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
	
	override int GetMinTimeBetweenTiggers() {
		return 300 * 1000;
	}
		
	override string GetAlarmSoundSet(){
		//return "RaidAlarmBellShortRange_SoundShader";
		return "RaidAlarmBellLongRange_SoundSet";
	}
	
	override void AfterStoreLoad()
	{	
		super.AfterStoreLoad();	
		TimeOfLastDrain = GetGame().GetTime();
	}
	
	override int GetRARadius(){
		return GameConstants.REFRESHER_RADIUS;
	}
	
	override bool CanSetOffAlarm(){	
		return (HasDish() && GetCompEM().CanWork(1) && m_IsDeployed);
	}
	
	void UpdateRABattery(){
		int curtime = GetGame().GetTime();
		if (GetInventory()){
			RaidAlarm_ServerBattery batt = GetServerBattery();
			if (GetGame().IsServer() && batt && TimeOfLastDrain != 0 && curtime > TimeOfLastDrain){
				float difsecs = Math.Floor((curtime - TimeOfLastDrain) / 1000);
				TimeOfLastDrain = curtime;
				float eReduction = difsecs * 0.01;
				float curEnergy = batt.GetCompEM().GetEnergy();
				if (curEnergy > eReduction){
					batt.GetCompEM().AddEnergy(-eReduction);
				} else {
					batt.GetCompEM().SetEnergy(0);
				}
				ConvertEnergyToQuantity();
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		
		if (slot_name == "BatteryServer" && item){
			item.GetCompEM().SwitchOff();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.GetCompEM().SwitchOff);
			SetSynchDirty();
		}
	}
		
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
				
		if (slot_name == "BatteryServer" && item){
			item.GetCompEM().SwitchOn();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.GetCompEM().SwitchOn);
			SetSynchDirty();
		}
	}
	
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if (!ctx.Read(m_IsDeployed)){
			return false;
		}

		return true;
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		UpdateRABattery();
		
		super.OnStoreSave( ctx );
		
		ctx.Write(m_IsDeployed);
	}
	
	
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
	
	
	RaidAlarm_ServerBattery GetServerBattery(){
		return RaidAlarm_ServerBattery.Cast(GetInventory().FindAttachment(slot_ServerBattery));
	}
	
}