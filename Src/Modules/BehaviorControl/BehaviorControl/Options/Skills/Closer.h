option(Closer)
{
  initial_state(start)
  {
    transition
    {
      if(state_time > 1000)
        goto A;
    }
    action
    {
      LookForward();
      Stand();
    }
  }

  state(A)
  {
    action
    {
      if(theLibCodeRelease.closerToTheBall)
      {
        SpecialAction(SpecialActionRequest::ArmUp);
      }
      else
      {
        SpecialAction(SpecialActionRequest::standHigh);

    }
  }

}
