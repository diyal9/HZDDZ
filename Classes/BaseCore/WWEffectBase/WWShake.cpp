#include "WWShake.h"

WWShake::WWShake() :
	_initial_x(0),
	_initial_y(0),
	_strength_x(0),
	_strength_y(0)
{
}

WWShake::~WWShake()
{ 
}

WWShake* WWShake::create(float d, float strength)
{
	return create(d, strength, strength);
}

WWShake* WWShake::create(float d, float strength_x, float strength_y)
{
	//WWShake *p_action = new WWShake();
	//p_action->initWithDuration(d, strength_x, strength_y);
	//p_action->autorelease();

	//return p_action;

	WWShake* pItem = new WWShake();
	if(pItem && pItem->initWithDuration(d, strength_x, strength_y))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		CC_SAFE_DELETE(pItem);
		return nullptr;
	}
}

bool WWShake::initWithDuration(float duration, float strength_x, float strength_y)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		_strength_x = strength_x;
		_strength_y = strength_y;
		return true;
	}

	return false;
}


float WWShake::fgRangeRand( float min, float max )
{
	float rnd = ((float)rand()/(float)RAND_MAX);
	return rnd*(max-min)+min;
}

void WWShake::update(float time)
{
	float randx = fgRangeRand( -_strength_x, _strength_x );
	float randy = fgRangeRand( -_strength_y, _strength_y );

	_target->setPosition(Vec2(_initial_x + randx,
		_initial_y + randy));
}

WWShake* WWShake::clone(void) const
{
	auto a = new WWShake();
	a->initWithDuration(_duration, _strength_x, _strength_y);
	a->autorelease();
	return a;
}

WWShake* WWShake::reverse() const
{
	return WWShake::create(_duration, -_strength_x, -_strength_y);
}

void WWShake::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);

	_initial_x = target->getPosition().x;
	_initial_y = target->getPosition().y;
}

void WWShake::stop(void)
{
	_target->setPosition(Vec2( _initial_x, _initial_y ) );
	ActionInterval::stop();
}