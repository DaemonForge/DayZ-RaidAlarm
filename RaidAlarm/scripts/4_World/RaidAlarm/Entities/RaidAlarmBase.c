class RaidAlarm_Bell extends RaidAlarm_Base{
	
	override int GetMinTimeBetweenTiggers() {
		return 90 * 1000;
	}
	
	bool CanSetOffAlarm(){
		return true;
	}
	
	
	override int GetRARadius(){
		return 3.5;
	}
}

class RaidAlarm_Server extends RaidAlarm_Base{
	
	override int GetMinTimeBetweenTiggers() {
		return 90 * 1000;
	}
	
	bool CanSetOffAlarm(){
		return true;
	}
	
	
	override int GetRARadius(){
		return 60;
	}
}
class RaidAlarm_ServerCluster extends ItemBase{

}
class RaidAlarm_CommunicationsArray extends ItemBase{
	bool HasDish(){
		if (RaidAlarm_ServerCluster.Cast(FindAttachmentBySlotName("DishAttachment"))){
			return true;
		}
		return false;
	}
}
class RaidAlarm_Dish extends ItemBase{
	
}
class RaidAlarm_PowerSuply extends RaidAlarm_Base{
	
	bool HasAllRequiredParts(){
		return (HasDish() && HasServerCluster());
	}
	
	
	bool HasDish(){
		RaidAlarm_CommunicationsArray comarray = RaidAlarm_CommunicationsArray.Cast(FindAttachmentBySlotName("ServerCluster"));
		if (comarray && comarray.HasDish()){
			return true;
		}
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment,int slotId )
	{
		if( !HasServerCluster() && attachment.IsInherited(RaidAlarm_CommunicationsArray))
		{
			return false;
		}
		return super.CanReceiveAttachment( attachment, slotId );
	}
	
	bool HasServerCluster(){
		if (RaidAlarm_ServerCluster.Cast(FindAttachmentBySlotName("ServerCluster"))){
			return true;
		}
		return false;
	}
	
	bool HasCommunicationArray(){
		if (RaidAlarm_CommunicationsArray.Cast(FindAttachmentBySlotName("ServerCOMSArray"))){
			return true;
		}
		return false;
	}
	
	override bool CanReleaseAttachment (EntityAI attachment)
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
		return true;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return (super.CanReceiveItemIntoCargo( item ) && !HasServerCluster() && !HasCommunicationArray());
	}
	
	
	override int GetRARadius(){
		return 60;
	}
}

class RaidAlarm_Base extends ItemBase {

	protected bool m_HasFindAndLinkQueued = false;
	
	protected bool m_HasRASyncedRequested = false;
	
	protected int m_LastAlarmTriggered = 0;

	protected bool m_RASoundSynchRemote = false;
	protected bool m_RASoundSynch = false;
	
	protected autoptr RaidAlarmPlayers m_RaidAlarmPlayers;
	
	void RaidAlarm_Base(){
		RegisterNetSyncVariableBool("m_RASoundSynch");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.DoFirstSync);
	}
	
	
	bool CanSetOffAlarm(){
		return true;
	}
	
	void ~RaidAlarm_Base(){
		if (m_HasFindAndLinkQueued){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.RAFindAndLinkBaseItemsThread);
		}
	}
	
	override void AfterStoreLoad()
	{	
		super.AfterStoreLoad();		
		SetSynchDirty();
		m_HasFindAndLinkQueued = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RAFindAndLinkBaseItemsThread, Math.QRandomInt(3200,4800));
	}
	
	void SetRAName(string name){
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		m_RaidAlarmPlayers.SetName(name);
	}
	
	bool AddRAPlayer(string guid, string name){
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		if ( m_RaidAlarmPlayers.AddPlayer(guid, name) ){
			SyncAlarm();
			return true;
		}
		return false;
	}
	
	
	bool RemoveRAPlayer(string guid){
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		if (m_RaidAlarmPlayers.RemovePlayer(guid)){
			SyncAlarm();
			return true;
		}
		return false;
	}
	
	bool RACheckPlayer(string guid){
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		return m_RaidAlarmPlayers.CheckPlayer(guid);
	}	
	
	void RAFindAndLinkBaseItemsThread(){
		m_HasFindAndLinkQueued = false;
		if (GetGame().IsServer()){
			thread RAFindAndLinkBaseItems();
		} else {
			SyncAlarm();
		}
	}
	
	protected void RAFindAndLinkBaseItems(){
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(GetPosition(), GetRARadius(), objects, proxyCargos);
		BaseBuildingBase baseItem;
		if (objects){
			for (int i = 0; i < objects.Count(); i++ ){
				if (Class.CastTo( baseItem, objects.Get(i) ) ){
					Print("[RAIDALARM] Setting Up Alarm on " + baseItem.GetType());
					baseItem.SetRaidAlarm(this);
				}
			}
		}
	}

	int GetMinTimeBetweenTiggers() {
		return 120 * 1000;
	}
	
	bool SetOffAlarm(){
		int curTime = GetGame().GetTime();
		if (curTime < m_LastAlarmTriggered || !CanSetOffAlarm()){
			return false;
		}
		m_LastAlarmTriggered = curTime + GetMinTimeBetweenTiggers();
		autoptr TStringMap players = m_RaidAlarmPlayers.GetPlayers();
		m_RASoundSynch = !m_RASoundSynch;
		SetSynchDirty();
		for (int i = 0; i < players.Count(); i++){
			UApi().ds().UserSend(players.GetKey(i), "You are being Raided");
			PlayerBase player;
			if ( Class.CastTo( player, UApi().FindPlayer(players.GetKey(i)) ) && player.GetIdentity()  ) {
				NotificationSystem.CreateNotification(new StringLocaliser("Raid Alarm"), new StringLocaliser("You are being raided"), "RaidAlarm\\data\\Images\\SoundOn.edds", ARGB(240, 153, 101, 244), 15, player.GetIdentity());
			}
		}
		return true;
	}
	
	
	int GetRARadius(){
		return 3.5;
	}
	
	void DoFirstSync(){
		if (GetGame().IsClient() && !m_HasRASyncedRequested){
			m_HasRASyncedRequested = true;
			SyncAlarm();
		}
	}
	
	void SyncAlarm(PlayerIdentity identity = NULL)
	{
		if ( GetGame().IsServer() ) {
			SetSynchDirty();
			RPCSingleParam(RAIDALARMRPCs.SEND_DATA, new Param1< RaidAlarmPlayers >(m_RaidAlarmPlayers), true, identity);
		} else if (GetGame().IsClient()){
			m_HasRASyncedRequested = true;
			RPCSingleParam(RAIDALARMRPCs.REQUEST_DATA, new Param1<bool>( true ), true, NULL);
		}
	}
	
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type == RAIDALARMRPCs.SEND_DATA && GetGame().IsClient()) {
			Param1< RaidAlarmPlayers > sdata;
			if (ctx.Read(sdata)){
				m_HasRASyncedRequested = true;
				m_RaidAlarmPlayers = RaidAlarmPlayers.Cast(sdata.param1);
			}
			return;
		}
		if (rpc_type == RAIDALARMRPCs.REQUEST_DATA && GetGame().IsServer()) {
			Param1<bool> rdata;
			if (ctx.Read(rdata) && sender)	{
				SyncAlarm(sender);
			}
			return;
		}
	}
	
	override bool IsDeployable() {
		return true;
	}
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		super.OnPlacementComplete( player, position, orientation );
		
		if ( GetGame().IsServer() ) {
			RAFindAndLinkBaseItemsThread();
		}
	}
		
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
				
		if (m_RASoundSynchRemote != m_RASoundSynch) {
			SoundBellPlay();
		}
		
	}
	
	
	protected void SoundBellPlay() 
	{
		m_RASoundSynchRemote = m_RASoundSynch;
		if (GetGame().IsClient()){
			EffectSound sound =	SEffectManager.PlaySound( "RaidAlarmBellLongRange_SoundSet", GetPosition() );
			sound.SetSoundAutodestroy( true );
		}
	}

	
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		ctx.Write(m_RaidAlarmPlayers);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if (!ctx.Read(m_RaidAlarmPlayers)){
			return false;
		}

		return true;
	}
	
	void SetActions() {
		super.SetActions();
		AddAction(LinkAndAddDiscord);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
		
	}
}