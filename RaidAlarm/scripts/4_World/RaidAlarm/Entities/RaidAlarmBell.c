class RaidAlarm_Bell extends RaidAlarm_Base{
		
	override int GetMinTimeBetweenTiggers() {
		return 180 * 1000;
	}
	
	bool CanSetOffAlarm(){
		return true;
	}
	
	override int GetRARadius(){
		return 3.5;
	}
	
	override string GetAlarmSoundSet(){
		return "RaidAlarmBellShortRange_SoundSet";
	}
	
	override bool ShouldTrigger(){
		return (Math.QRandomInt(0, 100) <= GetRaidAlarmConfig().ChanceToTriggerBell );
	}
	
	override int TriggerDelay(){
		return GetRaidAlarmConfig().TriggerDelayBell;
	}
}