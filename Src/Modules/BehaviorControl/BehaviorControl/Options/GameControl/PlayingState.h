option(PlayingState)
{
  int selector = 0;

  common_transition{

    selector = 0;
    //Penales
    if (theGameInfo.gamePhase == GAME_PHASE_PENALTYSHOOT)
        goto penaltyShoot;

    //Juego Normal!
    if (theGameInfo.setPlay == SET_PLAY_NONE)
      goto asignar;

    //Saque de arquero
    if (theGameInfo.setPlay == SET_PLAY_GOAL_FREE_KICK){
      if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber){
        if(theBehaviorStatus.role == Role::keeper)
          goto golpear;

        else
          goto asignar;
      }
      else{
        selector=1;
        goto posicionarse;
      }
    }

    //Tiro libre por pushing
    if (theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK){
      if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber){
        if(theLibCodeRelease.closerToTheBall)
          goto golpear;

        else
          goto asignar;
      }
      else{
        selector=2;
        goto posicionarse;
      }
    }

    //Tiro de esquina
    if (theGameInfo.setPlay == SET_PLAY_CORNER_KICK){
      if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber){
        selector=3;
        goto posicionarse;
      }
      else{
        selector=4;
        goto posicionarse;
      }
    }

    //Saque de banda
    if (theGameInfo.setPlay == SET_PLAY_KICK_IN){
      if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber){
        if(theLibCodeRelease.closerToTheBall)
          goto golpear;
        else
          goto asignar;
      }
      else{
        selector=5;
        goto posicionarse;
      }
    }
  }

  initial_state(initial)
  {
    transition
    {
      if(state_time > 4000)
        goto asignar;
    }
    action
    {
      Stand();
      LookAround();
    }
  }

  state(penaltyShoot)
  {
    action
    {
      if(!(theGameInfo.kickingTeam != Global::getSettings().teamNumber))
        Goalie();
    }
  }

  state(asignar)
  {
    action
    {
      ChooseRole();/*///////////////////////===============================================================
      ===========================================================================*/
      if(theBehaviorStatus.role != Role::keeper)
        Gallina();
    }
  }

  state(posicionarse)
  {
    action
    {
      IdealPosition(selector);
    }
  }

  state(golpear)
  {
    action
    {
      Shoot();
    }
  }
}
