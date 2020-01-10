/* Search for the ball if the teamball is not valid
*  We start by moving the head to the far right, then the far left (We only do this once)
*  If the ball gets found, we enter target_state
*  If the ball doesn't get found, we enter aborted_state
*/
option(SearchBall)
{
  static bool girarIzquierda;

  common_transition
  {
    if(theTeamBallModel.isValid || theBallModel.seenPercentage>=30)
        goto ballFound;
  }

  /** Starts moving to the right */
  initial_state(StartSearch)
  {
    transition
    {

      if(!theTeamBallModel.isValid){
        goto movingHead;
      }
    }
  }

  target_state(ballFound)
  {
    action
    {
      // When we are in this state, it means the ball has been found
    }
  }

  /** Moves the head to the far right, then change the state to moveSwitchLeft */
  state(movingHead)
  {
    float tiempo = 700.f;
     transition
    {
      if(state_time>tiempo*5)
        goto ballNotFound;
    }
    action
    {
      LookAround(tiempo);
    }
  }

  // If we are here, the ball hasn't been found, so the position of the NAO should be changed
  aborted_state(ballNotFound)
  {
    transition
    {
      if(action_done){
          goto movingHead;
      }
    }
    action
    {
      SetHeadPanTilt(0, 10_deg,(pi/2));
      MakeTurn((2*girarIzquierda-1) * pi/2);
    }
  }
}
