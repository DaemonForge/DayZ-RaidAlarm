class RaidAlarmPlayers extends Managed {
	
	protected autoptr TStringMap m_Players;
	protected string m_Name;
	
	void SetName(string name){
		m_Name = name;
	}
	
	string GetName(){
		return m_Name;
	}
	
	bool AddPlayer(string guid, string name){
		if (!m_Players){
			m_Players  = new TStringMap;
		}
		if (m_Players.Count() >= 15){ //Prevent rate limiting by discord
			return false;
		}
		m_Players.Insert(guid, name);
		return true;
	}
	
	TStringMap GetPlayers(){
		if (!m_Players){
			m_Players  = new TStringMap;
		}
		return m_Players;
	}
	
	void ClearPlayers(){
		if (!m_Players){
			m_Players  = new TStringMap;
		}
		m_Players.Clear();
	}
	
	void RemovePlayer(string guid){
		if (!m_Players){
			m_Players  = new TStringMap;
		}
		if (m_Players.Contains(guid)){
			m_Players.Remove(guid);
		}
	}
	
}