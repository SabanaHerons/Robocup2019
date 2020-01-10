option(LookAround, (float) (700) tiempo)
{

  static bool direccion;
  state(lookFront)
  {
    transition
    {
      if(state_time > tiempo){
        if(direccion)
          goto lookDiagonalRight;
        else
          goto lookDiagonalLeft;
       }
    }
    action
    {
      SetHeadPanTilt(0_deg, 10_deg, 10000);
    }
  }

  state(lookDiagonalLeft)
  {
    transition
    {
      if(state_time > tiempo){
        if(direccion)
          goto lookFront;
        else
          goto lookLeft;
      }
    }
    action
    {
      SetHeadPanTilt(-30_deg, 15_deg, 10000);
    }
  }

  state(lookDiagonalRight)
  {
    transition
    {
      if(state_time > tiempo){
        if(!direccion)
          goto lookFront;
        else
          goto lookRight;
      }
    }
    action
    {
      SetHeadPanTilt(30_deg, 15_deg, 10000);
    }
  }

  state(lookLeft)
  {
    transition
    {
      if(state_time > tiempo){
        direccion = true;
        goto lookDiagonalLeft;
      }
    }
    action
    {
      SetHeadPanTilt(-60_deg, 20_deg, 10000);
    }
  }

  initial_state(lookRight)
  {
    transition
    {
      if(state_time > tiempo){
        direccion = false;
        goto lookDiagonalRight;
      }
    }
    action
    {
      SetHeadPanTilt(60_deg, 20_deg, 10000);
    }
  }
}
