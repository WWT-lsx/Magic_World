#pragma once

/*
类名:MagicWindows
描述:创建一个给定宽度和高度的Windows窗口
具体功能:
1.快速创建多功能的窗口
*/

//头文件声明:
#include "windows.h"
/*#include "MagicAlphaWindow.h"*/
/*#include "MagicGratingAPI.h"*/
#include "MagicUIMacro.h"
#include "UserScene.h"
#include "MenuScene.h"
#include "DebugScene.h"
#include "SystemScene.h"

/*#pragma comment(lib,"MagicGrating.lib")*/

class MagicWindows
{
public:
	/*
	功能:构造函数
	参数:窗口名字,窗口宽度,窗口高度
	*/
	MagicWindows();
	~MagicWindows();

	static MagicWindows* Instance(){ return pMagicWindows; }

	bool Initialize(const wchar_t* _name, int _x, int _y, int _w, int _h);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //消息处理函数

	static void Updata(EntityCommon _entity);

	inline HWND GetHWND(){ return m_hwnd; }
	inline HINSTANCE GetHINSTANCE(){ return m_hinstance; }

	void SetCallbackMessage_WIN32(Magic::CallbackMessage_WIN32 _CallbackMessage_WIN32);
	void RequestOuitSystem();
private:
	bool CreateWindows(const wchar_t* _Name, int _x, int _y, int _width, int _height);
	void Shutdown();

private:
	Magic::UI::UserScene m_UserScene;
	Magic::UI::MenuScene m_MenuScene;
	Magic::UI::DebugScene m_DebugScene;
	Magic::UI::SystemScene m_SystemScene;
	Magic::CallbackMessage_WIN32 m_CallbackMessage_WIN32;
	//	MagicSceneCircle m_MagicSceneCircle;

	/*
		MGContext m_MGContext;
		MGShader m_Shader;
		MGVertexArrays m_VertexArrays;
		MGBuffer m_VertexBuffer;
		MGFrameBuffer m_MGFrameBuffer;
		unsigned int m_ShaderCamera;
		unsigned int m_Shaderprojection;
		unsigned int m_Shaderworld;*/

	/*	SGDI m_SGDI;*/
	wchar_t m_applicationName[256];
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	static MagicWindows* pMagicWindows;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);