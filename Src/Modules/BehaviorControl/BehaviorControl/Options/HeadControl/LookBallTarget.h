option(LookBallTarget,(Vector2f) relTarget)
{
  /** Simply sets the necessary angles */
  initial_state(lookBall)
  {
  	transition{
  		if(state_time > 3500)
        	goto lookTarget;
  	}
    action
    {
      TrackBall();
    }
  }
  state(lookTarget)
  {
  	transition{
  		if(state_time > 1500)
        	goto lookBall;
  	}
  	action{
  		SetHeadTargetOnGround(Vector3f(relTarget.x(),relTarget.y(),0));
  	}
  }
}
