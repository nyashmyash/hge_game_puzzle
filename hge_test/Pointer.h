#pragma once
#include "Position.h"
#include "Size.h"
#include "hgesprite.h"
#include <memory>
#include <vector>
class Pointer
{
private:
	Position pos_for_goal;
	Position pos;
	Size size;
	Size size_for_goal;
	int numb;
	int numb_current;
	int goal_numb;
	int color;
	int goal_color;
	std::shared_ptr<hgeSprite>	sprite;
	std::shared_ptr<hgeSprite>	sprite_goal;
	int marked_color;
	bool ismarked;
	bool isbusy;
	int find_route;
	std::vector< std::weak_ptr<Pointer> > ptrs;
	void recursiveMarked(Pointer *p);
	void recursiveAddToVect(Pointer * p, std::vector<Position> &res, int numb, int findel);
	void recursiveFind(Pointer *p, int numb, int &find_route, int &findel);
public:
	Pointer()
	{
		sprite = std::make_shared<hgeSprite>(hgeSprite(NULL, 0, 0, 0, 0));
		sprite_goal = std::make_shared<hgeSprite>(hgeSprite(NULL, 0, 0, 0, 0));
		ismarked = false;
		isbusy = false;
		pos = Position(0,0);
		size = Size(0, 0);
		numb = 0;
		goal_numb = 0;
		color = 0;
		marked_color = 0;
		find_route = -1;

	}
	Pointer(const Pointer&) = delete;
	Pointer& operator =(const Pointer&) = delete;
	Pointer(Position pos, Size size, int numb, int color,int marked_color, bool ismarked = false) : pos(pos), size(size), numb(numb), color(color), marked_color(marked_color),ismarked(ismarked)
	{
		sprite = std::make_shared<hgeSprite>(hgeSprite(NULL, 0, 0, size.width, size.height));
		sprite->SetColor(color);
	}
	void addToPosition(float x, float y);
	void setMarkedPath();
	std::vector<Position> findPath(int numb);
	void addPtr(std::weak_ptr<Pointer>);
	void setMarked(bool mark);
	bool isMarked();
	int getNumb();
	void setNumb(int numb);
	int getCurNumb();
	void setCurNumb(int numb);
	int getGoal();
	void setGoal(int numb);
	int getGoalColor();
	void setGoalColor(int goalcol);
	void setSpriteColor(int color);
	int getSpriteColor();	
	void setMrkColor(int color);
	int getMrkColor();
	void setColor(int color);
	int getColor();
	Position getPosition();
	Size getSize();
	void setPosition(Position pos);
	void setSize(Size size);
	Size getSizeForGoal();
	void setSizeForGoal(Size size);
	Position getPosGoal();
	void setPosGoal(Position pos);

	void setBusy(bool isbusy);
	void resetRoute();
	bool isBusy();
	virtual void update();
	virtual void draw();
	virtual ~Pointer() {};
};