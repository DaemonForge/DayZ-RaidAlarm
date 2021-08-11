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

class cfgVehicles
{
	class Inventory_Base;
	
	class RaidAlarm_Base : Inventory_Base
	{
		scope=2;
		displayName="Raid Alarm Base";
		descriptionShort="RaidAlarm";
		model="RaidAlarm\data\Bell\AlarmBell.p3d";
		itemBehaviour = 1;
		weight=2000;
		absorbency=1;
		itemSize[]={2,3};
		rotationFlags=16;
	};
	class RaidAlarm_Bell : RaidAlarm_Base
	{
		scope=2;
		displayName="Raid Alarm";
		descriptionShort="RaidAlarm";
		model="RaidAlarm\data\Bell\AlarmBell.p3d";
		isMeleeWeapon=1;
		itemBehaviour = 1;
		weight=2000;
		absorbency=1;
		itemSize[]={2,3};
		rotationFlags=16;
	};
	class RaidAlarm_Server : RaidAlarm_Base
	{
		scope=2;
		displayName="Raid Alarm Server";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\ServerRack\ServerRack.p3d";
		isMeleeWeapon=1;
		itemBehaviour = 1;
		weight=20000;
		absorbency=1;
		itemSize[]={10,16};
	};
	class RaidAlarm_Dish : Inventory_Base
	{
		scope=2;
		displayName="Raid Alarm Server";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\Dish Attachment\DishAttachment.p3d";
		isMeleeWeapon=1;
		itemBehaviour = 1;
		weight=20000;
		absorbency=1;
		itemSize[]={3,6};
	};
};

class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class RaidAlarmBellLongRange_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"RaidAlarmBellLongRange_SoundShader"
		};
	};
	class RaidAlarmBellShortRange_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"RaidAlarmBellShortRange_SoundShader"
		};
	};
};
class CfgSoundShaders
{
    class baseCharacter_SoundShader;
	class RaidAlarmBellLongRange_SoundShader: baseCharacter_SoundShader
	{
		samples[]={
			{
				"KeyCardDoor\Data\Sounds\bunkerwarning",
				1
			}
		};
		volume=1.6;
		range=1200;
	};
	class RaidAlarmBellShortRange_SoundShader: baseCharacter_SoundShader
	{
		samples[]={
			{
				"KeyCardDoor\Data\Sounds\bunkerwarning",
				1
			}
		};
		volume=1.2;
		range=600;
	};
};