modded class PlayerBase extends ManBase{
	

}


modded class Hologram {
	
	
	override void EvaluateCollision(ItemBase action_item = null)
	{
		if (m_Projection && m_Projection.IsInherited(RaidAlarm_Bell) && !IsUnderwater()){
			SetIsColliding( false );
			return;
		} 
		super.EvaluateCollision(action_item);
	}
}