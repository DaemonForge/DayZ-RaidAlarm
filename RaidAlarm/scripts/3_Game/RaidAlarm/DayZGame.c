modded class DayZGame extends CGame
{
	override void	OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == RAIDALARMRPCs.TIGGERALARMSOUND && GetGame().IsClient()) {
			Param2<string, vector> data;
			if (ctx.Read(data)) {
				string soundset = data.param1;
				vector pos = data.param2;
				EffectSound sound =	SEffectManager.PlaySound( soundset, pos);
				sound.SetSoundAutodestroy( true );
			}
			return;
		}
		super.OnRPC(sender, target, rpc_type, ctx);
	}
	
}