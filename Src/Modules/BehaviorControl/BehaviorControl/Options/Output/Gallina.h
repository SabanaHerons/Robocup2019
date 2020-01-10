option(Gallina){

    bool hayObstaculoCerca = false;
    if(!theObstacleModel.obstacles.empty()){      
      for(const auto& obstacle : theObstacleModel.obstacles){
        //See if the obstacle is first than the target
        if (obstacle.center.norm()<400.f)
            hayObstaculoCerca=true;
      }
    }
  initial_state(piopio){
  	transition
  	{
      if(!hayObstaculoCerca && state_time>7000)
        goto wait;
  	}
    action
    {
      theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = ArmKeyFrameRequest::back;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back; 	
    }
  }

  state(wait){
    transition
    {
      if(hayObstaculoCerca)
        goto piopio;

    }
    action
    {
      
    }
  }
}