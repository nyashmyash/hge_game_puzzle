// hge_test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "hge_test.h"
#include "PointersManager.h"
#include <string>

using namespace std;

#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#define MAX_LOADSTRING 100

// Pointers to the HGE objects we will use
HGE *hge;
//
bool mouse_down = false;
std::shared_ptr<hgeSprite>		sprcurs;

HTEXTURE			curs;
Position			mousePos;
PointersManager     pm;
bool isclick = 0;
int indxlvl = 1;

bool FrameFunc()
{
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	mouse_down = hge->Input_GetKeyState(HGEK_LBUTTON);
	
	hge->Input_GetMousePos(&mousePos.x, &mousePos.y);
	pm.updatePointers(mousePos, mouse_down, isclick);
	if (pm.checkGoal()) 
	{
		indxlvl++;
		char buf[10];
		char * a = _itoa(indxlvl, buf, 9);
		strcat(a, ".txt");
		pm.getPointersFromFile(a);
	}
	return false;
}

bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	pm.drawPointers();
	if(hge->Input_IsMouseOver() && sprcurs) sprcurs->Render(mousePos.x,mousePos.y);
	hge->Gfx_EndScene();

	return false;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{


	hge = hgeCreate(HGE_VERSION);
	
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, false);
	hge->System_SetState(HGE_TITLE, "HGE");
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate()) {
		curs = hge->Texture_Load("cursor.png");
		sprcurs = std::make_shared<hgeSprite>(hgeSprite(curs, 0, 0, 32, 32));
		pm.getPointersFromFile("1.txt");
		hge->System_Start();
	}
	hge->System_Shutdown();
	hge->Release();

	return 0;

}
