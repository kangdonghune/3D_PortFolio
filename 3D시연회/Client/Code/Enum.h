namespace MonsterState
{
	enum GoblinBlakcSmith { IDLE, ROAR, WALK,ATTACK,DOWN, GETUP, DEAD, DAMAGED, STOP, Enum_END};
}

namespace PlayerState
{
	enum STATE { IDLE, WALK, RUN, AIMStand, AIMWALK, FIRE ,RELOAD, BASH};
}

namespace Player3State
{
	enum State { MOVE, AIM, IDLE, NONE};
}

namespace Direction
{
	enum Dir { F,B,L,R,FL,FR,BL,BR, NONE};
}