modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(LinkAndAddDiscord); 
        actions.Insert(ActionDismantleServer); 
    }
}
