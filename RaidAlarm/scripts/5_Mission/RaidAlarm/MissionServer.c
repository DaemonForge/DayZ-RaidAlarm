modded class MissionServer extends MissionBase
{
	override void UniversalApiReady(){
		super.UniversalApiReady();
		GetRaidAlarmConfig();
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
	}
	
}