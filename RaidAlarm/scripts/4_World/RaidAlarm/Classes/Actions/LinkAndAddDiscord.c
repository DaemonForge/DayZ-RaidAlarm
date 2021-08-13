class LinkAndAddDiscord extends ActionInteractBase
{
	
	bool IsMember = false;
	int m_NextDiscordCheck = 0;

	void LinkAndAddDiscord(){
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText(){
		if (GetGame().IsClient() && !GetDayZGame().DiscordUser()){
			return "Link/Check Discord";
		}
		if (IsMember){
			return "Remove Self";
		}
		return "Add Self";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){
		RaidAlarm_Base raidalarm;
		PlayerBase thePlayer = PlayerBase.Cast(player);
		if ((Class.CastTo(raidalarm, target.GetObject()) || Class.CastTo(raidalarm, target.GetParent()))  && thePlayer && thePlayer.GetIdentity()){
			IsMember = raidalarm.RACheckPlayer(thePlayer.GetIdentity().GetId());
			int curTime = GetGame().GetTime();
			if (curTime > m_NextDiscordCheck && GetGame().IsClient() && !GetDayZGame().DiscordUser()){
				m_NextDiscordCheck = curTime + 5000;
				UApi().ds().GetUser(GetDayZGame().GetSteamId(), GetDayZGame(), "CBCacheDiscordInfo");
			}
			return true;
		}
		return false;
	}
	
	override void OnExecuteClient( ActionData action_data ){
		if (!GetDayZGame().DiscordUser()){
			NotificationSystem.CreateNotification(new StringLocaliser("Link Opened"), new StringLocaliser("A link has opened for you to link your discord account"), "RaidAlarm\\data\\Images\\discord.paa", ARGB(240, 153, 101, 244));
			GetGame().OpenURL( UApi().ds().Link() );
		}
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		if (action_data && action_data.m_Target && action_data.m_Player){
			RaidAlarm_Base raidalarm = RaidAlarm_Base.Cast(action_data.m_Target.GetObject());
			PlayerBase thePlayer = PlayerBase.Cast(action_data.m_Player);
			if (raidalarm && thePlayer && thePlayer.GetIdentity()){
				if (!raidalarm.RACheckPlayer(thePlayer.GetIdentity().GetId())){
					raidalarm.AddRAPlayer(thePlayer.GetIdentity().GetId(),thePlayer.GetIdentity().GetName());
				} else {
					raidalarm.RemoveRAPlayer(thePlayer.GetIdentity().GetId());
				}
			}
		}
	}
};