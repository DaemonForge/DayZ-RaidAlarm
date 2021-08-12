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
	class TruckBattery: Inventory_Base
	{
		hiddenSelections[]=
		{
			"zbytek"
		};
	}
	
	class ServerBattery: TruckBattery
	{
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"RaidAlarm\data\ServerRack\Textures\Battery\ServerBattery_co.paa"
		};
		class EnergyManager
		{
			hasIcon=1;
			switchOnAtSpawn=1;
			isPassiveDevice=1;
			convertEnergyToQuantity=1;
			energyStorageMax=1500;
			energyAtSpawn=1500;
			reduceMaxEnergyByDamageCoef=0.5;
			powerSocketsCount=1;
			plugType=4;
			compatiblePlugTypes[]={8};
			attachmentAction=2;
		};
	}
	
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
		hitpoints=150;
	};
	class RaidAlarm_Bell : RaidAlarm_Base
	{
		scope=2;
		displayName="Raid Alarm";
		descriptionShort="RaidAlarm";
		model="RaidAlarm\data\Bell\AlarmBell.p3d";
		isMeleeWeapon=1;
		itemBehaviour = 1;
		weight=1800;
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
		hitpoints=6000;
		itemSize[]={10,16};
		physLayer = "item_large";
		attachments[] = {"TruckBattery","DishAttachment"};
		carveNavmesh = 1;
		heavyItem = 1;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 0;
			energyUsagePerSecond = 0.01;
			plugType = 5;
			attachmentAction = 1;
		};
		hiddenSelectionsTextures[]=
		{
			"RaidAlarm\data\ServerRack\Textures\Radio_server_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
		};
	};
	class RaidAlarm_Dish : Inventory_Base
	{
		scope=2;
		displayName="Raid Alarm Stat Dish";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\Dish Attachment\DishAttachment.p3d";
		isMeleeWeapon=1;
		itemBehaviour = 1;
		weight=2000;
		absorbency=1;
		hitpoints=150;
		itemSize[]={3,6};
		inventorySlot[] = {"DishAttachment"};
		hiddenSelectionsTextures[]=
		{
			"RaidAlarm\data\Dish Attachment\Textures\Antenna_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"RaidAlarm\data\Dish Attachment\Textures\Antenna.rvmat"
		};
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
				"RaidAlarm\data\Sounds\Bell",
				1
			}
		};
		volume=1.9;
		range=1100;
	};
	class RaidAlarmBellShortRange_SoundShader: baseCharacter_SoundShader
	{
		samples[]={
			{
				"RaidAlarm\data\Sounds\Bell",
				1
			}
		};
		volume=1.6;
		range=500;
	};
};
class CfgSlots
{
	class Slot_DishAttachment
	{
		name = "DishAttachment";
		displayName = "Sat Dish";
		ghostIcon = "missing";
	};
};
class CfgNonAIVehicles
{
	class ProxyBaseBuilding;
	class ProxyDishAttachment: ProxyBaseBuilding
	{
		model = "\RaidAlarm\data\Dish Attachment\DishAttachment.p3d";
		inventorySlot[] = {"DishAttachment"};
	};
};
