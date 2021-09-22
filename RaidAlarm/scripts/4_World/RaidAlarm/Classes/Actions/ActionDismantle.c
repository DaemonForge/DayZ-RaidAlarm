class ActionDismantleServer: ActionContinuousBase
{
	
	override string GetText(){
		return "#dismantle";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return super.ActionCondition(player, target, item);
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		return super.ActionConditionContinue( action_data );
	}
	
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer( action_data );
		RaidAlarm_Server server;
		if (Class.CastTo(server, action_data.m_Target.GetObject()) || Class.CastTo(server, action_data.m_Target.GetParent())){
			
			server.CreateAndTransferToPowerSuppy();
		}
	}
}