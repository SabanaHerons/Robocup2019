option(Supporter)
{
	initial_state(start)
	{
		transition
		{
		  if (state_time > 1000)
			goto turnToBall;
		}
			action
		{
		  HeadControlMode(HeadControl::lookForward);
		  Stand();
		}
	}

	state(turnToBall)
	{
		transition
		{
		}
		action
		{
		}
	}

	state(walkToBall)
	{
		transition
		{
		}
		action
		{
		}
	}

	state(alignToGoal)
	{
		transition
		{
		}
		action
		{
		}
	}

	state(alignBehindBall)
	{
		transition
		{
		}
		action
		{
		}
	}

	state(kick)
	{
		transition
		{
		}
		action
		{
		}
	}

	state(searchForBall)
	{
		transition
		{
		}
		action
		{
		}
	}
}