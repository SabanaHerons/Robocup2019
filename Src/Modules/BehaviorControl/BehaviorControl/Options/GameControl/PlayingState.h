option(PlayingState)
{
  initial_state(demo)
  {
	transition
	{
	if(state_time > 10000)
	goto A;
	}
    action
    {
      LookAround();
    }
  }
  state(A)
  {
  action
  {
    Striker();
  }
  }
}
