#include "Pointer.h"
#include "hge_test.h"
int Pointer::getNumb()
{
	return numb;
}
void Pointer::setNumb(int numb)
{
	this->numb = numb;
}
int Pointer::getCurNumb()
{
	return numb_current;
}
void Pointer::setCurNumb(int numb)
{
	numb_current = numb;
}
int Pointer::getGoal()
{
	return goal_numb;
}
void Pointer::setGoal(int numb)
{
	goal_numb = numb;
}
int Pointer::getGoalColor()
{
	return goal_color;
}
void Pointer::setGoalColor(int goalcol)
{
	goal_color = goalcol;
	sprite_goal->SetColor(goalcol);
}
void Pointer::setSpriteColor(int color)
{
	sprite->SetColor(color);
}
int Pointer::getSpriteColor()
{
	return sprite->GetColor();
}
void Pointer::setColor(int color)
{
	this->color = color;
}
int Pointer::getColor()
{
	return color;
}
void Pointer::setMrkColor(int color)
{
	marked_color = color;
}
int Pointer::getMrkColor()
{
	return marked_color;
}
Position Pointer::getPosition()
{
	return pos;
}

Size Pointer::getSize()
{
	return size;
}

void Pointer::setPosition(Position pos)
{
	this->pos = pos;
}

void Pointer::setSize(Size size)
{
	sprite->SetTextureRect(0, 0, size.width, size.height);
	this->size = size;
}
Size Pointer::getSizeForGoal()
{
	return size_for_goal;
}
void Pointer::setSizeForGoal(Size size)//размер цели
{
	sprite_goal->SetTextureRect(0, 0, size.width, size.height);
	size_for_goal = size;
}
Position Pointer::getPosGoal()
{
	return pos_for_goal;
}
void Pointer::setPosGoal(Position pos)//установить позицию цели
{
	pos_for_goal = pos;
}
void Pointer::setBusy(bool isbusy)//установить персечение как занято маркером
{
	this->isbusy = isbusy;
}
void Pointer::resetRoute()//сбросить пути
{
	find_route = -1;
}
bool Pointer::isBusy()//пересечение занято
{
	return isbusy;
}
void Pointer::update()
{
}
void Pointer::draw()//рисование маркеров, линий и целевых маркеров
{
	for (int k = 0; k < ptrs.size(); k++)
		hge->Gfx_RenderLine(pos.x, pos.y, ptrs[k].lock()->getPosition().x, ptrs[k].lock()->getPosition().y, 0xFF660022, 0.5);

	sprite->Render(pos.x-size.width/2, pos.y-size.height/2);

	sprite_goal->Render(pos_for_goal.x - size_for_goal.width / 2, pos_for_goal.y - size_for_goal.height / 2);
}

void Pointer::recursiveMarked(Pointer * p)
{
	for (int i = 0; i < p->ptrs.size(); i++)
	{
		if (!p->ptrs[i].lock()->isBusy() && !p->ptrs[i].lock()->isMarked())
		{
			p->ptrs[i].lock()->setMarked(true);
			recursiveMarked(p->ptrs[i].lock().get());
		}
	}
}
void Pointer::recursiveAddToVect(Pointer * p, std::vector<Position>& res, int numb, int findel) //добавляем в список путей допустимые
{
	int max = -1;
	int maxi = -1;
	for (int i = 0; i < p->ptrs.size(); i++)
	{
		if (p->ptrs[i].lock()->isMarked() && p->ptrs[i].lock()->find_route<=findel && p->ptrs[i].lock()->find_route>=0)
		{
			maxi = i;
			max = p->ptrs[i].lock()->find_route;
			if (p->ptrs[i].lock()->find_route == findel)
				break;
		}
		
	}
	if (max < 0)
		return;
	res.push_back(p->ptrs[maxi].lock()->pos);
	p->ptrs[maxi].lock()->find_route = -1;
	if (p->ptrs[maxi].lock()->numb != numb)
		recursiveAddToVect(p->ptrs[maxi].lock().get(), res, numb,findel);
}
void Pointer::recursiveFind(Pointer * p, int numb, int &find_r, int& findel) //ищем рекурсивно возможные пути
{
	int isin = false;
	for (int i = 0; i < p->ptrs.size(); i++)
	{
		if (p->ptrs[i].lock()->isMarked() && p->ptrs[i].lock()->find_route <0 && !p->ptrs[i].lock()->isBusy())
		{
			isin = true;
			p->ptrs[i].lock()->find_route = find_r;
			if (p->ptrs[i].lock()->numb == numb)
			{
				findel = find_r;
			}
			recursiveFind(p->ptrs[i].lock().get(), numb, find_r, findel);	
		}		
	}
	if (!isin)
		find_r++;
}

void Pointer::addToPosition(float x, float y)
{
	pos.x += x;
	pos.y += y;
}

void Pointer::setMarkedPath()
{
	for (int i = 0; i < ptrs.size(); i++)
	{
		if (!ptrs[i].lock()->isBusy())
		{
			ptrs[i].lock()->setMarked(true);
			recursiveMarked(ptrs[i].lock().get());
		}
	}
}
std::vector<Position> Pointer::findPath(int numb)
{
	int find_r = 0;
	int findel = -1;
	recursiveFind(this, numb, find_r, findel);
	std::vector<Position> res;
	recursiveAddToVect(this, res, numb, findel);
	return res;
}
void Pointer::addPtr(std::weak_ptr<Pointer> p)
{
	for (int i = 0; i < ptrs.size(); i++)
		if (ptrs[i].lock()->getNumb() == p.lock()->getNumb())
			return;
	ptrs.push_back(p);
}
void Pointer::setMarked(bool mark)
{
	if (mark)
		setSpriteColor(marked_color);
	else
		setSpriteColor(color);
	ismarked = mark;
}

bool Pointer::isMarked()
{
	return ismarked;
}
