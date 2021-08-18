
class RaidAlarm_ServerCluster extends ItemBase{}

class RaidAlarm_CommunicationsArray extends ItemBase{
	bool HasDish(){
		if (RaidAlarm_Dish.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
	
	RaidAlarm_Dish GetDish(){
		return RaidAlarm_Dish.Cast(FindAttachmentBySlotName("DishAttachment"));
	}
	
	override bool CanDisplayAttachmentCategory(string category_name) {
		EntityAI parent;
        if (Class.CastTo(parent, GetParent()) && parent.GetType() == "RaidAlarm_Server" ) {
            return false;
		}
        return super.CanDisplayAttachmentCategory(category_name);
    }
	
}

class RaidAlarm_PowerSupply extends RaidAlarm_Server {
		
	void ~RaidAlarm_PowerSupply(){
		RemoveProxyPhysics("server_cluster");
		RemoveProxyPhysics("coms_array");
	}
	
	override string GetAlarmSoundSet(){
		//return "RaidAlarmBellShortRange_SoundShader";
		return "RaidAlarmBellLongRange_SoundSet";
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		RefreshPhysics();
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if (slot_name == "ServerCOMSArray" && GetGame().IsServer()){
			CreateAndTransferToServer();
			return;
		}
		
		RefreshPhysics();
	}
	
	void CreateAndTransferToServer(){
		RaidAlarm_Server server = RaidAlarm_Server.Cast( GetGame().CreateObject("RaidAlarm_Server", GetPosition() ) );
		server.SetOrientation(GetOrientation());
		RaidAlarm_CommunicationsArray comarray = RaidAlarm_CommunicationsArray.Cast(GetInventory().FindAttachment(slot_ServerCOMSArray));
		if (comarray && comarray.HasDish()){
			server.ServerTakeEntityAsAttachmentEx(comarray.GetDish(),slot_SatDish);
			server.ServerTakeEntityAsAttachmentEx(comarray,slot_ServerCOMSArray);
		}
		if (GetInventory().FindAttachment(slot_ServerBattery)){
			server.ServerTakeEntityAsAttachmentEx(GetInventory().FindAttachment(slot_ServerBattery),slot_ServerBattery);
		}
		server.ServerTakeEntityAsAttachmentEx(GetInventory().FindAttachment(slot_ServerCluster),slot_ServerCluster);
		
		server.OverrideAlarmPlayers(m_RaidAlarmPlayers);
		server.SetHealth("","",GetHealth("",""));
		
		server.SetIsDeployed(true);
		
		server.RAFindAndLinkBaseItemsThread();
		
		GetGame().ObjectDelete(this);
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