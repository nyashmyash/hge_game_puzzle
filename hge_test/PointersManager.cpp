#include "PointersManager.h"
bool PointersManager::checkMouseInPointer(std::shared_ptr<Pointer> ptr, Position pos)
{
	float x = ptr->getPosition().x - ptr->getSize().width/2;
	float y = ptr->getPosition().y - ptr->getSize().height / 2;
	if (x < pos.x && pos.x < x + ptr->getSize().width && y < pos.y && pos.y < y + ptr->getSize().height)
		return true;
	return false;
}


void PointersManager::recalcDelta()//изменение скорости движения фишки
{
	
	float summ = 0;
	
	for (int i = 0; i < moved_vect.size()-1; i++)
	{
		float y1 = moved_vect[i].y;
		float y2 = moved_vect[i + 1].y;
		float x1 = moved_vect[i].x;
		float x2 = moved_vect[i + 1].x;
		summ += sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	if (!const_speed)
		delta_cur += speedup;
	if (delta_cur > delta && speedup > 0)
	{
		lenght_speedup = lenght_path;
		const_speed = true;
	}
	if (abs(lenght_path - summ)<0.5)
	{
		delta_cur = 0;
		lenght_path = 0;
		const_speed = false;
		lenght_speedup = 0;
	}

}

int PointersManager::setMarkMovedPointer(Position mousepos)
{
	for (int i = 0; i < movedpointers.size(); i++)
	{
		if(checkMouseInPointer(movedpointers[i], mousepos))
		{
			movedpointers[i]->setMarked(true);
			return i;
		}
	}
	return -1;
}

void PointersManager::drawPointers()
{
	for (int i = 0; i < crosspointers.size(); i++)
		crosspointers[i]->draw();	
	for (int i = 0; i < movedpointers.size(); i++)
		movedpointers[i]->draw();
}

void PointersManager::updatePointers(Position mouse, bool isdown, bool &isclick)
{
	if (!isdown)
		isclick = false;
	if (ismoved)
	{
		int indx = -1;
		
		if (cur_line<moved_vect.size()-1)
		{
			float y1 = moved_vect[cur_line].y;
			float y2 = moved_vect[cur_line + 1].y;
			float x1 = moved_vect[cur_line].x;
			float x2 = moved_vect[cur_line + 1].x;

			float dy = y2 - y1;
			float dx = x2 - x1;

			float a = sqrt(dy*dy + dx*dx);
			lenght_path+=delta_cur;
			movedpointers[moved_elem_index]->addToPosition(delta_cur*dx/a, delta_cur*dy/a);
			recalcDelta();
			ismoved = true;
			Position pos = movedpointers[moved_elem_index]->getPosition();
			if (abs(y2 - pos.y) < 0.2 && abs(x2 - pos.x) < 0.2)
			{
				movedpointers[moved_elem_index]->setPosition(Position(x2, y2));
				cur_line++;
			}
		}
		else
		{
			cur_line = 0;
			lenght_path = 0;
			ismoved = false; //остановка движения
			moved_vect.erase(moved_vect.begin(), moved_vect.end()); //очистка вектора движения

		}
	}
	else
	{
		for (int i = 0; i < movedpointers.size(); i++)
			if (checkMouseInPointer(movedpointers[i], mouse) && isdown && !isclick) //клик на фишке
			{
				movedpointers[i]->setMarked(true);//делаем ее выделенной
				movedpointers[i]->update();
				isclick = true;
				for (int j = 0; j < movedpointers.size(); j++)
				{
					if (i != j && movedpointers[j]->isMarked()) // убираем выделение выделенным другим
					{
						movedpointers[j]->setMarked(false);
						movedpointers[j]->update();
					}
				}
				for (int j = 0; j < crosspointers.size(); j++) // убираем выделения пересечений
				{
					crosspointers[j]->setMarked(false);
				}
				for (int j = 0; j < crosspointers.size(); j++) //выставляем путь для прохождения фишки
				{
					if (crosspointers[j]->getNumb() == movedpointers[i]->getCurNumb())
					{
						crosspointers[j]->setMarkedPath();
						break;
					}
				}
				break;
			}
		for (int i = 0; i < crosspointers.size(); i++)
			if (checkMouseInPointer(crosspointers[i], mouse) && isdown && !isclick && crosspointers[i]->isMarked())//поинтер на который идем
			{
				for (int j = 0; j < movedpointers.size(); j++)//просматриваем все маркеры
					if (movedpointers[j]->isMarked()) //если маркер сейчас активен
					{
						for (int k = 0; k < crosspointers.size(); k++)//смотрим все поинтеры с которых маркер может идти
							if (crosspointers[k]->getNumb() == movedpointers[j]->getCurNumb())//тот что сейчас маркер на поинтере
							{
								std::vector<Position> moved_vect_t = crosspointers[k]->findPath(crosspointers[i]->getNumb());//ищем путь от поинтера где сейчас маркер до того поинтера куда кликнули
								if (!moved_vect_t.empty())
								{
									moved_vect.push_back(movedpointers[j]->getPosition());//добавили начальную координату для движения
									moved_vect.insert(moved_vect.end(), moved_vect_t.begin(), moved_vect_t.end()); // вставили все остальные координаты и получили полный путь
									crosspointers[k]->setMarked(true);//поставили выделение поинтера начального
									crosspointers[i]->setMarked(false);//убрали выделение текущего поинтера
									crosspointers[k]->setBusy(false); //убрал значение занятости у поинтера старого
									crosspointers[i]->setBusy(true); // установил текущую занятость
									movedpointers[j]->setCurNumb(crosspointers[i]->getNumb()); //установил у маркера ид поинтера
									for (int i = 0; i < crosspointers.size(); i++)//очистить пути
										crosspointers[i]->resetRoute();
								}
								break;
							}
						if (!moved_vect.empty())
						{
							moved_elem_index = j;
							ismoved = true;
						}
						break;
					}
				isclick = true;
			}
	}
}
//индексы для чтения данных выгруженных из файла
#define CNT_MOVED 0 // запись количества фишек
#define CNT_ALL_CROSS 1 // кол-во пересечений
#define START_POSITIONS 2 // начало позиций пересечений
#define LIST_MOVED_POINT 0 // началальное расположение фишек
#define LIST_MOVED_GOAL 1 // нужное расположение
#define CNT_ROUTES 2 // количество связей
#define START_ROUTES 3 //начало перечисления связей

void PointersManager::getPointersFromFile(std::string file)
{
	char buf[100];
	std::vector<std::string> data;
	FILE* f = NULL;
	f = fopen(file.c_str(), "r");
	if (f == nullptr)
		return;
	while (1)
	{
		char * result = fgets(buf, 100, f);
		if (!result) break;
		int strln = strlen(result);
		if (result[strln -1] == '\n') result[strln-1] = 0;
		data.push_back(result);
	}
	fclose(f);
	int cntmoved = atoi(data[CNT_MOVED].c_str());
	int cntcross = atoi(data[CNT_ALL_CROSS].c_str());
	int i = 0;
	
	if (!crosspointers.empty())
	{
		crosspointers.erase(crosspointers.begin(), crosspointers.end());
		movedpointers.erase(movedpointers.begin(), movedpointers.end());
		init();
	}
	//создание точек пересечения
	while (i < cntcross)
	{
		crosspointers.push_back(std::make_shared<Pointer>());
		crosspointers[i]->setNumb(i + 1);
		crosspointers[i]->setColor(0xff0066ff);
		crosspointers[i]->setSpriteColor(0xff0066ff);
		crosspointers[i]->setMrkColor(0x800066ff);//цвет при выделении
		crosspointers[i]->setSize(Size(20, 20));
		crosspointers[i]->setSizeForGoal(Size(20/delim, 20/delim));

		i++;
	}
	//создание маркеров
	i = 0;
	while (i < cntmoved)
	{
		movedpointers.push_back(std::make_shared<Pointer>());
		int color = RGB(rand() % 255, rand() % 255, rand() % 255);
		movedpointers[i]->setColor(color + 0xff000000);
		movedpointers[i]->setSpriteColor(color + 0xff000000);
		movedpointers[i]->setMrkColor(color + 0x8f000000);
		movedpointers[i]->setSize(Size(30, 30));
		movedpointers[i]->setSizeForGoal(Size(30/delim, 30/delim));
		i++;
	}
	i = 0;
	//позиции для пересечений
	while (i < cntcross)
	{
		int ch = 0;
		std::string posX;
		std::string posY;

		while (data[START_POSITIONS + i][ch] != ',')
		{
			posX += data[START_POSITIONS + i][ch];
			ch++;
		}
		while (ch < data[START_POSITIONS + i].length())
			posY += data[START_POSITIONS + i][++ch];
		crosspointers[i]->setPosition(Position(atof(posX.c_str()), atof(posY.c_str())));
		crosspointers[i]->setPosGoal(Position(atof(posX.c_str())/delim+pospreview.x, atof(posY.c_str())/delim + pospreview.y));
		i++;
	}
	i = 0;
	int ch1 = 0;
	int ch2 = 0;
	while (i < cntmoved)
	{
		std::string id;
		std::string idgoal;
		// начальная нумерация маркера
		while (data[START_POSITIONS + cntcross + LIST_MOVED_POINT][ch1] != ','&& ch1 < data[START_POSITIONS + cntcross + LIST_MOVED_POINT].length())
		{
			id += data[START_POSITIONS + cntcross + LIST_MOVED_POINT][ch1];
			ch1++;
		}
		// нужный номер
		while (data[START_POSITIONS + cntcross+ LIST_MOVED_GOAL][ch2] != ','&& ch2 < data[START_POSITIONS + cntcross+ LIST_MOVED_GOAL].length())
		{
			idgoal += data[START_POSITIONS + cntcross + LIST_MOVED_GOAL][ch2];
			ch2++;
		}
		int numb = atoi(id.c_str());
		int goaln = atoi(idgoal.c_str());
		movedpointers[i]->setNumb(numb);
		movedpointers[i]->setCurNumb(numb);
		movedpointers[i]->setGoal(goaln);
		
		movedpointers[i]->setPosition(crosspointers[numb - 1]->getPosition());
		movedpointers[i]->setPosGoal(crosspointers[numb - 1]->getPosGoal());

		crosspointers[numb-1]->setBusy(true);
		i++; ch1++;
		ch2++;
	}
	for (int i = 0; i < cntmoved; i++)
		for (int j = 0; j < cntmoved; j++)
		if (movedpointers[i]->getGoal()== movedpointers[j]->getNumb())
		{
			movedpointers[i]->setGoalColor(movedpointers[j]->getColor());
			break;
		}
	//получение и заполнение возможных путей для движения фишки	
	int cnt_cross = atoi(data[START_POSITIONS + cntcross + CNT_ROUTES].c_str());
	i = 0;
	while (i < cnt_cross)
	{
		int ch = 0;
		std::string idfrom;
		std::string idto;
		while (data[START_POSITIONS + cntcross + START_ROUTES + i][ch] != ',')
		{
			idfrom += data[START_POSITIONS + cntcross + START_ROUTES + i][ch];
			ch++;
		}
		while (ch < data[START_POSITIONS + cntcross + START_ROUTES + i].length())
			idto += data[START_POSITIONS + cntcross + START_ROUTES + i][++ch];
		int idfromi = atoi(idfrom.c_str());
		int idtoi = atoi(idto.c_str());

		if (idfromi > 0 && idtoi > 0 && idfromi <= crosspointers.size() && idtoi <= crosspointers.size())
		{
			crosspointers[idfromi - 1]->addPtr(crosspointers[idtoi - 1]);
			crosspointers[idtoi - 1]->addPtr(crosspointers[idfromi - 1]);
		}
		
		i++;
	}
}

bool PointersManager::checkGoal()
{
	if (!ismoved)
	{
		for (int i = 0; i < movedpointers.size(); i++)
		{
			if (movedpointers[i]->getCurNumb() != movedpointers[i]->getGoal())
				return false;
		}
		return true;
	}
	return false;
}
