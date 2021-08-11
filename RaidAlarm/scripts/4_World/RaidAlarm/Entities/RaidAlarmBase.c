class RaidAlarm_Base extends ItemBase {


	protected int m_RASoundSynchRemote = -1;
	protected int m_RASoundSynch = 0;
	
	protected autoptr RaidAlarmPlayers m_RaidAlarmPlayers;
	
	
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		ctx.Write(m_RaidAlarmPlayers);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if (!ctx.Read(m_RaidAlarmPlayers)){
			return false;
		}

		return true;
	}
	
	override void AfterStoreLoad()
	{	
		super.AfterStoreLoad();		
		
		
	}
	
	void SetRAName(string name){
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		m_RaidAlarmPlayers.SetName(name);
	}
	
	bool AddNewRAPlayer(string guid, string name){
		if (!m_RaidAlarmPlayers){
			m_RaidAlarmPlayers = new RaidAlarmPlayers;
		}
		return m_RaidAlarmPlayers.AddPlayer(guid, name);
	}
	
	
	
	void RAFindAndLinkBaseItems(){
		
	}

	
	
	
	
	
	int GetRadius(){
		return 3;
	}
}