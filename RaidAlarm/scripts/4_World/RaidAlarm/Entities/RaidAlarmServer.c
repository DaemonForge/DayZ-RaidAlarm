
class RaidAlarm_ServerBattery extends TruckBattery{}
class RaidAlarm_ServerCluster extends ItemBase{}
class RaidAlarm_Dish extends ItemBase{}
class RaidAlarm_CommunicationsArray extends ItemBase{
	bool HasDish(){
		if (RaidAlarm_Dish.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
}

class RaidAlarm_Server extends RaidAlarm_Base{
	
	bool HasDish(){
		if (RaidAlarm_Dish.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
	
	override int GetMinTimeBetweenTiggers() {
		return 300 * 1000;
	}
	
	bool CanSetOffAlarm(){
		return HasDish();
	}
	
	override string GetAlarmSoundSet(){
		//return "RaidAlarmBellShortRange_SoundShader";
		return "RaidAlarmBellLongRange_SoundSet";
	}
	
	override int GetRARadius(){
		return GameConstants.REFRESHER_RADIUS;
	}
}
class RaidAlarm_PowerSuply extends RaidAlarm_Base{
	
	
	protected int TimeOfLastDrain = 0;
	
	protected int slot_ServerCOMSArray = InventorySlots.INVALID;
	protected int slot_ServerCluster = InventorySlots.INVALID;
	protected int slot_ServerBattery = InventorySlots.INVALID;
	
	void RaidAlarm_PowerSuply(){
		slot_ServerCOMSArray = InventorySlots.GetSlotIdFromString("ServerCOMSArray");
		slot_ServerCluster = InventorySlots.GetSlotIdFromString("ServerCluster");
		slot_ServerBattery = InventorySlots.GetSlotIdFromString("BatteryServer");
		RefreshRAPhysics();
	}
	
	override string GetAlarmSoundSet(){
		//return "RaidAlarmBellShortRange_SoundShader";
		return "RaidAlarmBellLongRange_SoundSet";
	}
	
	bool HasAllRequiredParts(){
		return (HasDish() && HasServerCluster());
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		
		if (slot_name == "BatteryServer" && item){
			item.GetCompEM().SwitchOff();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.GetCompEM().SwitchOff);
			SetSynchDirty();
		}
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.RefreshRAPhysics);
	}
		
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
				
		if (slot_name == "BatteryServer" && item){
			item.GetCompEM().SwitchOn();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.GetCompEM().SwitchOn);
			SetSynchDirty();
		}
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.RefreshRAPhysics);
	}
	
	bool HasDish(){
		RaidAlarm_CommunicationsArray comArray;
		if (GetInventory() && Class.CastTo(comArray, GetInventory().FindAttachment(slot_ServerCOMSArray)) && comArray.HasDish()){
			return true;
		}
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId ) {
		if( !HasServerCluster() && slotId == slot_ServerCOMSArray ){
			return false;
		}
		return super.CanReceiveAttachment( attachment, slotId );
	}
	
	bool HasServerCluster(){
		RaidAlarm_ServerCluster cluster;
		if (slot_ServerCluster != InventorySlots.INVALID && GetInventory() && Class.CastTo(cluster, GetInventory().FindAttachment(slot_ServerCluster))){
			return true;
		}
		return false;
	}
	
	bool HasCommunicationArray(){
		RaidAlarm_CommunicationsArray cluster;
		if (slot_ServerCOMSArray != InventorySlots.INVALID && GetInventory() && Class.CastTo(cluster, GetInventory().FindAttachment(slot_ServerCOMSArray))){
			return true;
		}
		return false;
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
	
	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if( HasCommunicationArray() && attachment.IsInherited(RaidAlarm_ServerCluster)) {
			return false;
		}
		return super.CanReleaseAttachment(attachment);
	}
	
	override int GetMinTimeBetweenTiggers() {
		return 300 * 1000;
	}
	
	bool CanSetOffAlarm(){	
		return (HasServerCluster() && HasCommunicationArray() && HasDish() && GetCompEM().CanWork(1));
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return (super.CanPutInCargo( parent ) && !HasServerCluster() && !HasCommunicationArray());
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return (super.CanPutIntoHands( parent ) && !HasServerCluster() && !HasCommunicationArray());
	}
	
	override int GetRARadius(){
		return GameConstants.REFRESHER_RADIUS;
	}
	
	
	override void AfterStoreLoad()
	{	
		super.AfterStoreLoad();	
		TimeOfLastDrain = GetGame().GetTime();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.RefreshRAPhysics);
	}
	
	RaidAlarm_ServerBattery GetServerBattery(){
		return RaidAlarm_ServerBattery.Cast(GetInventory().FindAttachment(slot_ServerBattery));
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
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
		super.OnStoreSave( ctx );
	}
	
	void RefreshRAPhysics(){
		
		//HideRASimpleSelection("Deployed", !HasServerCluster());
		
		RemoveProxyPhysics("coms_array");
		RemoveProxyPhysics("server_cluster");
		
		if (HasServerCluster()) {
			AddProxyPhysics("server_cluster");
		}
		if (HasCommunicationArray()) {
			AddProxyPhysics("coms_array");
		}
	}
	
	protected void HideRASimpleSelection(string selectionName, bool hide = true)
    {
        TStringArray selectionNames = new TStringArray;
        ConfigGetTextArray("simpleHiddenSelections",selectionNames);
        int selectionId = selectionNames.Find(selectionName);
        SetSimpleHiddenSelectionState(selectionId, hide);
    };
}