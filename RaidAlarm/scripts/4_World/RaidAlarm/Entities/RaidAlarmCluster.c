
class RaidAlarm_ServerCluster extends ItemBase{}

class RaidAlarm_CommunicationsArray extends ItemBase{
	bool HasDish(){
		if (RaidAlarm_Dish.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
}

class RaidAlarm_PowerSupply extends RaidAlarm_Server {
	
	protected int slot_ServerCOMSArray = InventorySlots.INVALID;
	protected int slot_ServerCluster = InventorySlots.INVALID;
	
	void RaidAlarm_PowerSupply(){
		slot_ServerCOMSArray = InventorySlots.GetSlotIdFromString("ServerCOMSArray");
		slot_ServerCluster = InventorySlots.GetSlotIdFromString("ServerCluster");
	}
	
	void ~RaidAlarm_PowerSupply(){
		RemoveProxyPhysics("server_cluster");
		RemoveProxyPhysics("coms_array");
	}
	
	override string GetAlarmSoundSet(){
		//return "RaidAlarmBellShortRange_SoundShader";
		return "RaidAlarmBellLongRange_SoundSet";
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);
		
		if (GetGame().IsServer()){
			m_IsDeployed = false;
			SetSynchDirty();
		}
	}
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		super.OnPlacementComplete( player, position, orientation );
		
		if ( GetGame().IsServer() ) {
			m_IsDeployed = true;
			SetSynchDirty();
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		RefreshPhysics();
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
				
		RefreshPhysics();
	}
	
	override bool HasDish(){
		RaidAlarm_CommunicationsArray comArray;
		if (GetInventory() && Class.CastTo(comArray, GetInventory().FindAttachment(slot_ServerCOMSArray)) && comArray.HasDish()){
			return true;
		}
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId ) {
		if (!m_IsDeployed){return false;}
		if( !HasServerCluster() && slotId == slot_ServerCOMSArray ){
			return false;
		}
		return super.CanReceiveAttachment( attachment, slotId );
	}
	
	bool HasServerCluster(){
		if (slot_ServerCluster != InventorySlots.INVALID && GetInventory() && GetInventory().FindAttachment(slot_ServerCluster) != NULL){
			return true;
		}
		return false;
	}
	
	bool HasCommunicationArray(){
		if (slot_ServerCOMSArray != InventorySlots.INVALID && GetInventory() && GetInventory().FindAttachment(slot_ServerCOMSArray) != NULL){
			return true;
		}
		return false;
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
	
	override bool CanSetOffAlarm(){	
		return (HasServerCluster() && HasCommunicationArray() && HasDish() && GetCompEM().CanWork(1) && m_IsDeployed);
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return (super.CanPutInCargo( parent ) && !HasServerCluster() && !HasCommunicationArray());
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return (super.CanPutIntoHands( parent ) && !HasServerCluster() && !HasCommunicationArray());
	}
		
	override void RefreshPhysics()
	{
		super.RefreshPhysics();
		Print("RefreshPhysics");
		
		if ( this  &&  !ToDelete() )
		{
			Print("RefreshPhysics");
			RemoveProxyPhysics("server_cluster");
			RemoveProxyPhysics("coms_array");
			if (HasServerCluster()) {
				Print("RefreshRAPhysics AddProxyPhysics server_cluster");
				AddProxyPhysics("server_cluster");
			}
			if (HasCommunicationArray()) {
				Print("RefreshRAPhysics AddProxyPhysics coms_array");
				AddProxyPhysics("coms_array");
			}
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