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
	int MinTimeBetweenTiggers = 900; //Seconds 
	
	bool ManagedTypesFile = true;
		
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
			if (ManagedTypesFile){
				string ModFile = "RaidAlarm_types.xml";
				string Path = "RaidAlarm\\xmls\\";
				CopyXmlFile(Path + ModFile, ModFile);
			}
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
	
	
	
	/*
		Copy Xml Files to Mission director and add entry to cfg econmoy core
	
		PathToFile - Is the path to the file inside your mod structure 
	                 remember to add *.xml to the copy directly in addon builder
		
		FileName - This is the file name to save the xml as in the folder, recommended to use modname_type.xml
	    
		Type - The type of xml this is ("types","spawnabletypes","events")
		
	*/
	protected void CopyXmlFile(string PathToFile, string FileName, string type = "types"){
		if (!MakeDirectory("$mission:\\ModTypes")) {
			Error2("", "Couldn't Make mod types folder");
			return;
		}		
		int i = 0;
		string cfgecPath =  "$mission:\\cfgeconomycore.xml";
		if (!FileExist("$mission:\\ModTypes\\" + FileName) && FileExist(PathToFile)){
			CopyFile(PathToFile,"$mission:\\ModTypes\\" + FileName);
		}
		string xmldata;
		if (!FileExist(cfgecPath)) {
			Error2("", "File At cfgeconomycore.xml could not be found");
			return;
		}
		FileHandle fhr = OpenFile(cfgecPath, FileMode.READ);
		string error;
		if (fhr) {
			string line;
			while (FGets(fhr, line) > 0) {
				xmldata = xmldata + "\n" + line;
			}
			CloseFile(fhr);
		} else {
			Error2("", "File At cfgeconomycore.xml could not be opened");
		}
		if (!xmldata.Contains(FileName )){
			TStringArray newXmlLines = new TStringArray;
			TStringArray xmlLines = new TStringArray;
			xmldata.Split("\n", xmlLines);
			bool Found = false;
			bool ChangeMade = false;
			for (i = 0; i < xmlLines.Count(); i++){
				if (xmlLines.Get(i).Contains("<ce folder=\"ModTypes\">")){
					Found = true;
					ChangeMade = true;
					newXmlLines.Insert(xmlLines.Get(i));
					newXmlLines.Insert("        <file name=\"" + FileName + "\" type=\"" + type + "\" />");
				} else if ((xmlLines.Get(i).Contains("</economycore>") || xmlLines.Get(i) == "</economycore>") && !Found){
					ChangeMade = true;
					newXmlLines.Insert(" ");
					newXmlLines.Insert(" ");
					newXmlLines.Insert("    <!-- Created by script its advised not to edit this manually -->");
					newXmlLines.Insert("    <ce folder=\"ModTypes\">");
					newXmlLines.Insert("        <file name=\"" + FileName + "\" type=\"" + type + "\" />");
					newXmlLines.Insert("    </ce>");
					newXmlLines.Insert(" ");
					newXmlLines.Insert(xmlLines.Get(i));
				} else {
					newXmlLines.Insert(xmlLines.Get(i));
				}
			}
			if (ChangeMade){
				FileHandle fhw = OpenFile(cfgecPath, FileMode.WRITE);
				if (!fhw) {
					Print("File At" + cfgecPath + " could not be updated");
					return;
				} 
				if (newXmlLines && newXmlLines.Count() > 0) {
					for (i = 0; i < newXmlLines.Count(); i++){
						FPrintln(fhw, newXmlLines.Get(i));
					}
				}
				CloseFile(fhw);
			}
		}
	}
	
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
