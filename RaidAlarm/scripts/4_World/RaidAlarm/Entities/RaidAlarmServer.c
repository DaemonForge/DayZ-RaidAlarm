class RaidAlarm_ServerBattery extends TruckBattery{}

class RaidAlarm_Dish extends ItemBase{}

class RaidAlarm_Server extends RaidAlarm_Base{
	
	
	protected int slot_ServerBattery = InventorySlots.INVALID;
	protected int slot_ServerCOMSArray = InventorySlots.INVALID;
	protected int slot_ServerCluster = InventorySlots.INVALID;
	protected int slot_SatDish = InventorySlots.INVALID;
	
	protected int TimeOfLastDrain = 0;
	
	void RaidAlarm_Server() {
		slot_ServerBattery = InventorySlots.GetSlotIdFromString("BatteryServer");
		slot_ServerCOMSArray = InventorySlots.GetSlotIdFromString("ServerCOMSArray");
		slot_ServerCluster = InventorySlots.GetSlotIdFromString("ServerCluster");
		slot_SatDish = InventorySlots.GetSlotIdFromString("DishAttachment");
	}
	
	override bool NameOverride(out string output) {
        if(GetGame().IsClient() && CanSetOffAlarm()) {
            output = ConfigGetString("displayName") + " (ONLINE)";
            return true;
        } else if (GetGame().IsClient()) {
            output = ConfigGetString("displayName") + " (OFFLINE)";
            return true;
		}
        return super.NameOverride(output);
    }
	
	override bool CanReleaseAttachment(EntityAI attachment) {
		if (IsFullServer() && ( attachment.IsInherited(RaidAlarm_ServerCluster) ||  attachment.IsInherited(RaidAlarm_CommunicationsArray) )) {
			return false;
		}
		return super.CanReleaseAttachment(attachment);
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

	override void EEItemDetached(EntityAI item, string slot_name) {
		super.EEItemDetached(item, slot_name);
		
		
		if (slot_name == "BatteryServer" && item){
			item.GetCompEM().SwitchOff();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.GetCompEM().SwitchOff);
			SetSynchDirty();
		}
	}
		
	override void EEItemAttached(EntityAI item, string slot_name) {
		super.EEItemAttached(item, slot_name);
				
		if (slot_name == "BatteryServer" && item){
			item.GetCompEM().SwitchOn();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.GetCompEM().SwitchOn);
			SetSynchDirty();
		}
	}
	
	
	override bool CanPutInCargo( EntityAI parent ) {
		if (IsFullServer()){
			return false;
		}		
		return super.CanPutInCargo(parent));
	}
	
	override bool CanPutIntoHands( EntityAI parent ) {
		if (IsFullServer()){
			return false;
		}
		return super.CanPutIntoHands(parent));
	}
	
	
	RaidAlarm_ServerBattery GetServerBattery() {
		return RaidAlarm_ServerBattery.Cast(GetInventory().FindAttachment(slot_ServerBattery));
	}
	
	bool IsFullServer(){
		return true;
	}
	
	override bool CanDisplayAttachmentCategory(string category_name) {
        if (category_name  == "Servers" && GetGame().IsClient() && IsFullServer()) {
            return false;
		}
        if (category_name  == "Communcations" && GetGame().IsClient() && !IsFullServer()) {
            return false;
		}
        return super.CanDisplayAttachmentCategory(category_name);
    }
	
	
	void CreateAndTransferToPowerSuppy() {
		RaidAlarm_PowerSupply server = RaidAlarm_PowerSupply.Cast( GetGame().CreateObject("RaidAlarm_PowerSupply", GetPosition() ) );
		server.SetOrientation(GetOrientation());
		
		RaidAlarm_CommunicationsArray comarray = RaidAlarm_CommunicationsArray.Cast(GetInventory().FindAttachment(slot_ServerCOMSArray));
		server.ServerTakeEntityAsAttachmentEx(comarray, slot_ServerCOMSArray);
		if (GetInventory().FindAttachment(slot_SatDish)){
			comarray.ServerTakeEntityAsAttachmentEx(GetInventory().FindAttachment(slot_SatDish), slot_SatDish);
		}
		if (GetInventory().FindAttachment(slot_ServerBattery)){
			server.ServerTakeEntityAsAttachmentEx(GetInventory().FindAttachment(slot_ServerBattery), slot_ServerBattery);
		}
		server.ServerTakeEntityAsAttachmentEx(GetInventory().FindAttachment(slot_ServerCluster),slot_ServerCluster);
		server.OverrideAlarmPlayers(m_RaidAlarmPlayers);
		server.SetHealth("","",GetHealth("",""));
		server.SetIsDeployed(true);
		server.RAFindAndLinkBaseItemsThread();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, this);
	}
	
}