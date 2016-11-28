#pragma once
#include <vector>
#include<map>
#include "Pointer.h"
class PointersManager
{
private:
	std::vector<std::shared_ptr<Pointer>> crosspointers;
	std::vector<std::shared_ptr<Pointer>> movedpointers;
	bool checkMouseInPointer(std::shared_ptr<Pointer> ptr, Position pos);
	bool ismoved;
	float delta;
	float delta_cur;
	float lenght_path;
	float lenght_speedup;
	float speedup;
	int moved_elem_index;
	std::vector<Position> moved_vect;
	float delim;
	int cur_line;
	bool const_speed;
	Position pospreview;
	void recalcDelta();
	int setMarkMovedPointer(Position mousepos);
public:
	PointersManager() { init(); };
	void init() { const_speed = false; cur_line = 0; moved_elem_index = -1; ismoved = false; lenght_path = 0; speedup = 0.0005;  delta = 0.3; delta_cur = 0; delim = 4; pospreview = Position(400, 0); };
	void drawPointers();
	void updatePointers(Position mouse, bool isdown, bool & isclick);
	void getPointersFromFile(std::string file);
	bool checkGoal();
};