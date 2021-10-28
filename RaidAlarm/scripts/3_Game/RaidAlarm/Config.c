class RaidAlarmConfig extends UApiConfigBase
{
	string ConfigVersion = "1";
	int ChanceToTriggerBell = 100; //0-100
	int ChanceToTriggerServer = 100; //0-100
	int TriggerDelayBell = 0; //Seconds (if dismantled or disabled before this it won't tigger)
	int TriggerDelayServer = 0; //Seconds (if dismantled or disabled before this it won't tigger)
	bool RequireBattery = true;
	bool CanRechargeBattery = true;
	bool CanDestroy = true;
	bool VanillaDamageThresold = -1;
	bool RadioDishNeedsOpenSky = true;
	bool IngameSoundAlarm = true;
	bool DiscordAlarm = true;
	string DiscordAlarmMsg = "Base being Raided";
	
		
	override void Load(){
		SetDataReceived(false);
		SetDefaults();
		UApi().Rest().GlobalsLoad("RaidAlarm", this, this.ToJson());
	}
	
	override void Save(){
		if (GetGame().IsServer()){	
			UApi().Rest().GlobalsSave("RaidAlarm", this.ToJson());
		}
	}
	
	
	override void OnDataReceive(){
		SetDataReceived();
		if (GetGame().IsServer() && ConfigVersion != "1"){
			Save();
		}
	}
	
	
	override string ToJson(){
		string jsonString = JsonFileLoader<RaidAlarmConfig>.JsonMakeData(this);
		return jsonString;
	}
	
	override void SetDefaults(){

	}
	
	override void OnError(int errorCode) {
		Print("[RaidAlarm] CallBack Failed errorCode: " + errorCode);		
		SetDataReceived(true);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Load, 300 * 1000, false);
	};
	
	override void OnTimeout() {
		Print("[RaidAlarm] CallBack Failed errorCode: Timeout");
		SetDataReceived(true);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Load, 300 * 1000, false);
	};
	
	override void OnSuccess(string data, int dataSize) {
		JsonFileLoader<RaidAlarmConfig>.JsonLoadData(data, this);
		if (this){
			OnDataReceive();
		} else {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Load, 300 * 1000, false);
		}
		
	};
	
};

ref RaidAlarmConfig m_RaidAlarmConfig;
RaidAlarmConfig GetRaidAlarmConfig(){
	if (!m_RaidAlarmConfig){
		 m_RaidAlarmConfig = new RaidAlarmConfig;
		Print("[RaidAlarm] Init");
		 m_RaidAlarmConfig.Load();
	}
	return m_RaidAlarmConfig;
}
