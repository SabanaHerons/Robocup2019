/** A test striker option without common decision */
#include <stdlib.h>
#include <time.h>

int num;

option(PenaltyStriker)
{
    initial_state(start)
    {
      transition
      {
        if(state_time > 1000)
          goto searchForBall;
      }
      action
      {
        LookAround();
        Stand();
      }
    }

    state(searchForBall)
    {
      transition
      {
        if(theLibCodeRelease.timeSinceBallWasSeen < 300)
        {
          goto chooseSide;
        }
      }
      action
      {
        TrackBallA();
        WalkAtRelativeSpeed(Pose2f(0.f, 0.1f, 0.f));
      }
    }

    state(chooseSide)
    {
      transition
      {
        if (num==1) {
          goto alignLeft;
        }
        if (num==0) {
          goto alignRight;
        }
      }
      action
      {
        srand((int)time(NULL));
        num=0+rand()%(2-0);
      }
    }
    state(alignRight)
    {
      transition
      {
        if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -10.f)
           && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
           && std::abs(theLibCodeRelease.angleToOppGoal) > 15_deg)
           goto kick;
      }
      action
      {
        WalkToTarget(Pose2f(0.35f, 0.35f, 0.35f), Pose2f(theLibCodeRelease.angleToOppGoal- 0.40f, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() + 50.f));

      }
    }

    state(alignLeft)
    {
      transition
      {
        if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -10.f)
           && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
           && std::abs(theLibCodeRelease.angleToOppGoal) > 15_deg)
           goto kick;
      }
      action
      {
        WalkToTarget(Pose2f(0.35f, 0.35f, 0.35f), Pose2f(theLibCodeRelease.angleToOppGoal+ 0.40f, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() + 50.f));

      }
    }

    state(kick)
    {
      transition
      {
        if(action_done)
          goto stand;
      }
        action
        {
          theMotionRequest.motion = MotionRequest::kick;
          theMotionRequest.kickRequest.kickMotionType = KickRequest::kickForward;
        }
    }

    state(stand)
    {
        action
        {
            Stand();
        }
    }
}
