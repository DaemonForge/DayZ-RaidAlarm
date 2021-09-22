#ifdef BBP
modded class ActionDestroyBBPPart: ActionContinuousBase
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		
		return super.ActionCondition(player, target, item);
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		return super.ActionConditionContinue( action_data );
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer( action_data );
		BBP_BASE base_building = BBP_BASE.Cast( action_data.m_Target.GetObject() );
		if (base_building){
			base_building.SetOffAlarm();
		}
	}
	
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		BBP_BASE base_building = BBP_BASE.Cast( action_data.m_Target.GetObject() );
		if (base_building){
			base_building.SetOffAlarm();
		}
		super.OnFinishProgressServer( action_data );
	}
}
#endif