class CfgPatches
{
	class RaidAlarm
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"UniversalApi"
		};
	};
};

class CfgMods
{
	class RaidAlarm
	{
		dir = "RaidAlarm";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Raid Alarm";
		credits = "";
		author = "DaemonForge, Dumpgrah";
		authorID = "0"; 
		version = "1.0.0"; 
		extra = 0;
		type = "mod";
		
		dependencies[] = {"Game", "World", "Mission"};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {
					"RaidAlarm/scripts/3_Game"
					};
			}
			
			class worldScriptModule
			{
				value = "";
				files[] = {
					"RaidAlarm/scripts/4_World"
					};
			}

			class missionScriptModule
			{
				value = "";
				files[] = {
					"RaidAlarm/scripts/5_Mission"
					};
			};
		};
	};
};