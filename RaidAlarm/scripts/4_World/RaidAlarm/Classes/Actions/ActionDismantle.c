class ActionDismantleServer: ActionContinuousBase
{
	
	void ActionDismantleServer()
	{
		m_CallbackClass = ActionDismantlePartCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;	
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override string GetText(){
		return "#dismantle";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		
		RaidAlarm_Server server;
		if (Class.CastTo(server, target.GetObject()) || Class.CastTo(server, target.GetParent())){
			return server.CanDismantle();
		}
		return false;
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