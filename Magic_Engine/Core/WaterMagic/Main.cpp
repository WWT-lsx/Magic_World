#include "MagicUIAPI.h"
#include "MainScene.h"
#include "Render/MagicEngineAPI.h"
#include <tchar.h>


int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevainstance, LPWSTR lpcmdline, int iCmdshow)
{
	bool _result = Magic::CreateSystemUI(L"WaterMagic", 0, 0, 1024, 768);
	if (!_result)
	{
		MessageBoxA(NULL, "����", Magic::GetEngineErrorMessage(), MB_OK);
		return false;
	}

	HICON _ico = (HICON)::LoadImageA(NULL, 0, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	Magic::SetWindowICO(_ico);

	MainScene* _pMainScene = new MainScene;

	_result = Magic::CreateScene(Magic::GetSceneCommon("UserScene")->GetEntity(), &_pMainScene);
	if (!_result)
	{
		MessageBoxA(NULL, "����", Magic::GetEngineErrorMessage(), MB_OK);
		return false;
	}

	Magic::RunEngine();

	Magic::ShutdownSystemUI();

	if (_pMainScene)
	{
		delete _pMainScene;
		_pMainScene = 0;
	}

	return 0;
}