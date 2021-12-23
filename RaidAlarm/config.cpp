class CfgPatches
{
	class RaidAlarm
	{
		units[]={"RaidAlarm_ServerBattery", "RaidAlarm_Bell","RaidAlarm_Server","RaidAlarm_PowerSuply","RaidAlarm_ServerCluster","RaidAlarm_CommunicationsArray","RaidAlarm_Dish"};
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
		dependencies[] = { "Game", "World", "Mission"};
		class defs
		{
			
			class imageSets
			{
				files[] =
				{
					"RaidAlarm/data/Images/raidalarm_ghost.imageset"
				};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {
					"RaidAlarm/scripts/3_Game"
					};
			};
			
			class worldScriptModule
			{
				value = "";
				files[] = {
					"RaidAlarm/scripts/4_World"
					};
			};

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
	class Battery9V;
	class TruckBattery: Inventory_Base
	{
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"DZ\vehicles\parts\data\truck_bat_co.paa"
		};
	};
	class RaidBellBattery: Battery9V
	{
		itemSize[]={1,1};
		stackedUnit="w";
		quantityBar=1;
		varQuantityInit=50;
		varQuantityMin=0;
		varQuantityMax=50;
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"RaidAlarm\data\Bell\Bell Alarm Battery_co.paa"
		};
		class EnergyManager
		{
			hasIcon=1;
			switchOnAtSpawn=1;
			isPassiveDevice=1;
			energyStorageMax=50;
			energyAtSpawn=50;
			convertEnergyToQuantity=1;
			reduceMaxEnergyByDamageCoef=1;
			powerSocketsCount=1;
			compatiblePlugTypes[]={1};
		};
	};
	class RaidAlarm_ServerBattery: TruckBattery
	{
		displayName="Server Battery";
		inventorySlot[] = {"BatteryServer","LargeBattery"};
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
			energyStorageMax=8000;
			energyAtSpawn=6048;
			reduceMaxEnergyByDamageCoef=0.5;
			powerSocketsCount=1;
			plugType=1;
			compatiblePlugTypes[]={1};
			attachmentAction=2;
		};
	};
	
	class RaidAlarm_Base : Inventory_Base
	{
		scope=0;
		displayName="Raid Alarm Base";
		descriptionShort="RaidAlarm";
		model="RaidAlarm\data\Bell\AlarmBell.p3d";
		itemBehaviour = 1;
		weight=2000;
		itemSize[]={2,3};
		hitpoints=200;
	};
	class ServerBoxKitTest : Inventory_Base
	{
		scope=2;
		displayName="ServerBoxTest";
		descriptionShort="";
		model="\RaidAlarm\data\Box\ServerBox.p3d";
		weight=1100;
		itemSize[]={4,3};
		hiddenSelections[]=
		{
			"box_1"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								"RaidAlarm\data\Box\Textures\ServerBox.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"RaidAlarm\data\Box\Textures\ServerBox.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"RaidAlarm\data\Box\Textures\Dmg\DmgServerBox.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\Box\Textures\Dmg\DmgServerBox.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\Box\Textures\Dmg\DmgServerBox Ruined.rvmat"
							}
						}
					};
				};
			};
		};
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
		itemSize[]={2,3};
		rotationFlags=16;
		hiddenSelections[]=
		{
			"Base",
			"Bell",
			"Hammer"
		};
		hiddenSelectionsTextures[]=
		{
			"RaidAlarm\data\Bell\Textures\Housing_co.paa",
			"RaidAlarm\data\Bell\Textures\Bell_co.paa",
			"RaidAlarm\data\Bell\Textures\Hammer_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"RaidAlarm\data\Bell\Textures\Housing.rvmat",
			"RaidAlarm\data\Bell\Textures\Bell.rvmat",
			"RaidAlarm\data\Bell\Textures\Hammer.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								"RaidAlarm\data\Bell\Textures\Housing.rvmat",
								"RaidAlarm\data\Bell\Textures\Bell.rvmat",
								"RaidAlarm\data\Bell\Textures\Hammer.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"RaidAlarm\data\Bell\Textures\Housing.rvmat",
								"RaidAlarm\data\Bell\Textures\Bell.rvmat",
								"RaidAlarm\data\Bell\Textures\Hammer.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"RaidAlarm\data\Bell\Textures\dmg\Dmg\HousingDmg.rvmat",
								"RaidAlarm\data\Bell\Textures\dmg\BellDmg.rvmat",
								"RaidAlarm\data\Bell\Textures\Hammer.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\Bell\Textures\dmg\Dmg\HousingDmg.rvmat",
								"RaidAlarm\data\Bell\Textures\dmg\BellDmg.rvmat",
								"RaidAlarm\data\Bell\Textures\Hammer.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\Bell\Textures\dmg\Dmg\HousingDmg Ruined.rvmat",
								"RaidAlarm\data\Bell\Textures\dmg\Bell Ruined.rvmat",
								"RaidAlarm\data\Bell\Textures\Hammer.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class RaidAlarm_Server : RaidAlarm_Base
	{
		scope=2;
		displayName="Raid Alarm Server";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\ServerRack\FullServerRack.p3d";
		weight=25000;
		hitpoints=8000;
		itemSize[]={10,14};
		physLayer = "item_large";
		attachments[] = {"BatteryServer","ServerCluster","ServerCOMSArray","DishAttachment"};
		carveNavmesh=1;
		itemBehaviour=0;
		heavyItem=1;
		energyResources[] = {{"power",8.0}};
		class EnergyManager
		{
			hasIcon = 1;
			switchOnAtSpawn=1;
			energyUsagePerSecond = 0.01;
			plugType = 1;
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
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1.0,
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						{
							0.69999999,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						{
							0.5,
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Ruined.rvmat"
							}
						}
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Servers
			{
				name = "Servers";
				description = "";
				attachmentSlots[] = {"ServerCluster","ServerCOMSArray"};
				icon = "set:raidalarm_ghost image:ServerPower";
				view_index = 1;
			};
			class Power
			{
				name = "Power";
				description = "";
				attachmentSlots[] = {"BatteryServer"};
				icon = "set:dayz_inventory image:carbattery";
				view_index = 1;
			};
			class Communcations
			{
				name = "Communcations";
				description = "";
				attachmentSlots[] = {"DishAttachment"};
				icon = "set:raidalarm_ghost image:ServerCOMSArray";
				view_index = 1;
			};
		};
	};
	class RaidAlarm_PowerSupply : RaidAlarm_Server
	{
		scope=2;
		displayName="Raid Alarm Power Supply";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\ServerRack\ServerPowerSupply.p3d";
		weight=8000;
		hitpoints=6000;
		slopeTolerance = 0.4;
		itemSize[]={9,5};
		physLayer = "item_large";
		attachments[] = {"BatteryServer","ServerCluster","ServerCOMSArray"};
		carveNavmesh = 1;
		itemBehaviour=0;
		heavyItem = 1;
		energyResources[] = {{"power",8.0}};
		simpleHiddenSelections[] = {
			"coms_array",
			"server_cluster"
		};
		hiddenSelections[]= {
			"coms_array",
			"server_cluster"
		};
		class EnergyManager
		{
			hasIcon = 1;
			switchOnAtSpawn=1;
			energyUsagePerSecond = 0.01;
			plugType = 1;
			attachmentAction = 1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Ruined.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class RaidAlarm_ServerCluster : Inventory_Base
	{
		scope=2;
		displayName="Raid Alarm Server Cluster";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\ServerRack\ServerCluster.p3d";
		weight=7000;
		hitpoints=6000;
		itemSize[]={9,4};
		physLayer = "item_large";
		inventorySlot[] = {"ServerCluster"};
		carveNavmesh=1;
		itemBehaviour=0;
		heavyItem=1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Ruined.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class RaidAlarm_CommunicationsArray : Inventory_Base
	{
		scope=2;
		displayName="Raid Alarm Communcation Array";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\ServerRack\ServerCOMSArray.p3d";
		weight=8000;
		hitpoints=6000;
		itemSize[]={9,5};
		physLayer="item_large";
		inventorySlot[]={"ServerCOMSArray"};
		attachments[] ={"DishAttachment"};
		carveNavmesh =1;
		itemBehaviour=0;
		heavyItem=1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Radio Server Rack.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Dmg.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\ServerRack\Textures\Dmg\Radio Server Rack Ruined.rvmat"
							}
						}
					};
				};
			};
		};
		
	};
	class RaidAlarm_Dish : Inventory_Base
	{
		scope=2;
		displayName="Raid Alarm Satillite Dish";
		descriptionShort="Raid Alarm";
		model="RaidAlarm\data\Dish Attachment\DishAttachment.p3d";
		weight=2000;
		absorbency=1;
		hitpoints=150;
		itemSize[]={4,8};
		inventorySlot[] = {"DishAttachment"};
		hiddenSelectionsTextures[]=
		{
			"RaidAlarm\data\Dish Attachment\Textures\Antenna_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"RaidAlarm\data\Dish Attachment\Textures\Antenna.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								"RaidAlarm\data\Dish Attachment\Textures\Antenna.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"RaidAlarm\data\Dish Attachment\Textures\Antenna.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"RaidAlarm\data\Dish Attachment\Textures\Dmg\Antenna Dmg.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"RaidAlarm\data\Dish Attachment\Textures\Dmg\Antenna Dmg.rvmat"
							}
						},
						
						{
							0.0,
							
							{
								"RaidAlarm\data\Dish Attachment\Textures\Dmg\Antenna Dmg Ruined.rvmat"
							}
						}
					};
				};
			};
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
		volume=1.4;
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
		volume=1.0;
		range=500;
	};
};
class CfgSlots
{
	class Slot_DishAttachment
	{
		name = "DishAttachment";
		displayName = "Sat Dish";
		ghostIcon = "set:raidalarm_ghost image:DishAttachment";
	};
	class Slot_ServerCluster
	{
		name = "ServerCluster";
		displayName = "Server Cluster";
		ghostIcon = "set:raidalarm_ghost image:ServerCluster";
	};
	class Slot_ServerCOMSArray
	{
		name = "ServerCOMSArray";
		displayName = "Communcation Array";
		ghostIcon = "set:raidalarm_ghost image:ServerCOMSArray";
	};
	class Slot_BatteryServer
	{
		name = "BatteryServer";
		displayName = "Server Battery";
		selection = "battery";
		ghostIcon = "set:dayz_inventory image:carbattery";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyDishAttachment: ProxyAttachment
	{
		scope=2;
		model = "\RaidAlarm\data\Dish Attachment\DishAttachment.p3d";
		inventorySlot[] = {"DishAttachment"};
	};
	class ProxyServerCluster: ProxyAttachment
	{
		scope=2;
		model = "\RaidAlarm\data\ServerRack\ServerCluster.p3d";
		inventorySlot[] = {"ServerCluster"};
	};
	class ProxyServerCOMSArray: ProxyAttachment
	{
		scope=2;
		model = "\RaidAlarm\data\ServerRack\ServerCOMSArray.p3d";
		inventorySlot[] = {"ServerCOMSArray"};
	};
};
