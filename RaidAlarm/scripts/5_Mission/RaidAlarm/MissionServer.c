modded class MissionServer extends MissionBase
{
	override void UniversalApiReady(){
		super.UniversalApiReady();
		GetRaidAlarmConfig();
	}
}