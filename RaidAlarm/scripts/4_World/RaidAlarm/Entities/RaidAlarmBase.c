class RaidAlarm_Base extends ItemBase {

	protected bool m_IsDeployed = false;
	
	protected bool m_HasFindAndLinkQueued = false;
	
	protected bool m_HasRASyncedRequested = false;
	
	protected int m_LastAlarmTriggered = 0;
	
	protected autoptr RaidAlarmPlayers m_RaidAlarmPlayers;
	
	protected autoptr Timer m_TriggerDelayTimer;
	
	void RaidAlarm_Base() {
		RegisterNetSyncVariableBool("m_IsDeployed");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.RaidAlarmBaseInit);
	}
	
	override void OnWorkStart() {
		Print("[RAIDALARM] On Work Start");
	}
	
	void OnSwitchOn() {
	
	}
	
	void OnSwitchOff() {
	
	}
	
	void OverrideAlarmPlayers( RaidAlarmPlayers raidAlarmPlayers) {
		if (!raidAlarmPlayers){return;}
		Class.CastTo(m_RaidAlarmPlayers,raidAlarmPlayers);
	}
	
	override void OnWorkStop() {
		Print("[RAIDALARM] On Work Stop");
		
	}
	
	bool IsWorking() {
		if (GetCompEM() && GetCompEM().CanWork())
			return true;
		return false;
	}
	
	
	bool CanSetOffAlarm() {
		return true;
	}
	
	void ~RaidAlarm_Base() {
		if (m_HasFindAndLinkQueued){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.RAFindAndLinkBaseItemsThread);
		}
	}
	
	override void AfterStoreLoad() {	
		super.AfterStoreLoad();		
		SetSynchDirty();
		m_HasFindAndLinkQueued = true;
		if (m_IsDeployed && GetGame().IsServer()){
			SetIsDeployed(true);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RAFindAndLinkBaseItemsThread, Math.RandomInt(3200,4800));
		}
	}
	
	void SetRAName(string name) {
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		m_RaidAlarmPlayers.SetName(name);
	}
	
	bool AddRAPlayer(string guid, string name) {
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		if ( m_RaidAlarmPlayers.AddPlayer(guid, name) ){
			SyncAlarm();
			return true;
		}
		return false;
	}
	
	
	bool RemoveRAPlayer(string guid) {
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		if (m_RaidAlarmPlayers.RemovePlayer(guid)){
			SyncAlarm();
			return true;
		}
		return false;
	}
	
	bool RACheckPlayer(string guid) {
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		return m_RaidAlarmPlayers.CheckPlayer(guid);
	}	
	
	void RAFindAndLinkBaseItemsThread() {
		m_HasFindAndLinkQueued = false;
		if (GetGame().IsServer()){
			thread RAFindAndLinkBaseItems();
		} else {
			SyncAlarm();
		}
	}
	
	protected void RAFindAndLinkBaseItems() {
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
		return GetRaidAlarmConfig().MinTimeBetweenTiggers * 1000;
	}
	
	int TriggerDelay(){
		return GetRaidAlarmConfig().TriggerDelayServer;
	}
	
	bool ShouldTrigger(){
		return true;
	}
	
	bool TriggerAlarm(bool force = false){
		if (!ShouldTrigger() && !force){
			return false;
		}
		if (!m_TriggerDelayTimer){
			m_TriggerDelayTimer = new Timer;
		}
		if (m_TriggerDelayTimer.IsRunning()){
			return false;
		}
		m_TriggerDelayTimer.Run(TriggerDelay(),this,"SetOffAlarm");
		return true;
	}
	
	protected bool SetOffAlarm() {
		int curTime = GetGame().GetTime();
		if (curTime < m_LastAlarmTriggered || !CanSetOffAlarm()){
			return false;
		}
		m_LastAlarmTriggered = curTime + GetMinTimeBetweenTiggers();
		autoptr TStringMap players = m_RaidAlarmPlayers.GetPlayers();
		if (GetRaidAlarmConfig().IngameSoundAlarm){
			TriggerAlarmSound();
		}
		for (int i = 0; i < players.Count(); i++){
			if (GetRaidAlarmConfig().DiscordAlarm){
				UApi().ds().UserSend(players.GetKey(i), GetRaidAlarmConfig().DiscordAlarmMsg);
			}
			PlayerBase player;
			if ( Class.CastTo( player, UApi().FindPlayer(players.GetKey(i)) ) && player.GetIdentity()  ) {
				NotificationSystem.CreateNotification(new StringLocaliser("Raid Alarm"), new StringLocaliser(GetRaidAlarmConfig().DiscordAlarmMsg), "RaidAlarm\\data\\Images\\SoundOn.edds", ARGB(240, 153, 101, 244), 15, player.GetIdentity());
			}
		}
		return true;
	}
	
	
	int GetRARadius() {
		return 3.5;
	}
	
	void RaidAlarmBaseInit() {
		if (GetGame().IsClient() && !m_HasRASyncedRequested) {
			m_HasRASyncedRequested = true;
			SyncAlarm();
		}
		if (GetGame().IsServer()){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RaidAlarmDelayedInit, 30000, false);
		}
	}
	
	void RaidAlarmDelayedInit(){
		SetAllowDamage(GetRaidAlarmConfig().CanDestroy);
	}
	
	void SyncAlarm(PlayerIdentity identity = NULL) {
		if ( GetGame().IsServer() ) {
			SetSynchDirty();
			RPCSingleParam(RAIDALARMRPCs.SEND_DATA, new Param1< RaidAlarmPlayers >(m_RaidAlarmPlayers), true, identity);
		} else if ( GetGame().IsClient() ){
			m_HasRASyncedRequested = true;
			RPCSingleParam(RAIDALARMRPCs.REQUEST_DATA, new Param1<bool>( true ), true, NULL);
		}
	}
	
	string GetAlarmSoundSet() {
		return "RaidAlarmBellShortRange_SoundSet";
		//return "RaidAlarmBellLongRange_SoundSet";
	}
	
	protected void TriggerAlarmSound() {
		if (GetGame().IsServer()){
			GetGame().RPCSingleParam(this, RAIDALARMRPCs.TIGGERALARMSOUND, new Param2<string,vector>( GetAlarmSoundSet(), GetPosition() ), true, NULL);
		} else {
			SoundBellPlay();
		}
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
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
		super.OnRPC(sender, rpc_type, ctx);
	}
	
	override bool IsDeployable() {
		return true;
	}
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" ) {
		super.OnPlacementComplete( player, position, orientation );
		
		if ( GetGame().IsServer() ) {
			SetIsDeployed(true);
			RAFindAndLinkBaseItemsThread();
		}
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner) {
		super.OnItemLocationChanged(old_owner, new_owner);
		
		if (GetGame().IsServer() && GetParent()){
			SetIsDeployed(false);
		}
	}
	
	void SetIsDeployed(bool state = true) {
		m_IsDeployed = state;
		SetSynchDirty();
	}
	
	bool IsDeployed(){
		return m_IsDeployed;
	}
	
	protected void SoundBellPlay() {
		if (GetGame().IsClient()){
			EffectSound sound =	SEffectManager.PlaySound( "RaidAlarmBellLongRange_SoundSet", GetPosition() );
			sound.SetSoundAutodestroy( true );
		}
	}
	
	override void OnStoreSave( ParamsWriteContext ctx ) {   
		super.OnStoreSave( ctx );
		bool triggerAlarm = false;
		if (m_TriggerDelayTimer && m_TriggerDelayTimer.IsRunning()){
			triggerAlarm = true;
		}
		ctx.Write(triggerAlarm);
		ctx.Write(m_RaidAlarmPlayers);
		ctx.Write(m_IsDeployed);
	}
	
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version ) {
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		bool triggerAlarm = false;
		if (!ctx.Read(triggerAlarm)){
			return false;
		}
		
		if (!ctx.Read(m_RaidAlarmPlayers)){
			return false;
		}

		if (!ctx.Read(m_IsDeployed)){
			return false;
		}
		if (triggerAlarm){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.TriggerAlarm, 1000, false, true);
		}
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent ) {
		return (super.CanPutInCargo( parent ) && !m_IsDeployed);
	}
	
	override bool CanPutIntoHands( EntityAI parent ) {
		return (super.CanPutIntoHands( parent ) && !m_IsDeployed);
	}
	
	override void SetActions() {
		super.SetActions();
		AddAction(LinkAndAddDiscord);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
		
	}
}

