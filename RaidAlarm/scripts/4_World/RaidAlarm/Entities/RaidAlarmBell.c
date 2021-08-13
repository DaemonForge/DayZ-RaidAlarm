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