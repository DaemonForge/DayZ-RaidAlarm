
class RaidAlarm_ServerBattery extends TruckBattery{

	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		Print("[RAIDALARM] Saving Battery Debug Ammount Left + " + GetCompEM().GetEnergy());
	}
}
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
		if (RaidAlarm_ServerCluster.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
	
	override int GetMinTimeBetweenTiggers() {
		return 90 * 1000;
	}
	
	bool CanSetOffAlarm(){
		return HasDish();
	}
	
	
	override int GetRARadius(){
		return 60;
	}
}
class RaidAlarm_PowerSuply extends RaidAlarm_Base{
	
	
	protected int slot_ServerCOMSArray = InventorySlots.INVALID;
	protected int slot_ServerCluster = InventorySlots.INVALID;
	
	void RaidAlarm_PowerSuply(){
	
		slot_ServerCOMSArray = InventorySlots.GetSlotIdFromString("ServerCOMSArray");
		slot_ServerCluster = InventorySlots.GetSlotIdFromString("ServerCluster");
	}
	
	
	bool HasProxyParts()
	{
		return true;
	}
	
	bool HasAllRequiredParts(){
		return (HasDish() && HasServerCluster());
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if (slot_name == "ServerBattery" && item){
			item.GetCompEM().SwitchOff();
			this.GetCompEM().SwitchOff();
			SetSynchDirty();
		}
	}
		
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if (slot_name == "BatteryServer" && item){
			
			item.GetCompEM().SwitchOn();
			this.GetCompEM().SwitchOn();
			SetSynchDirty();
		}
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
		return 90 * 1000;
	}
	
	bool CanSetOffAlarm(){	
		return (HasServerCluster() && HasCommunicationArray() && HasDish() && IsWorking());
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
		return 60;
	}
}