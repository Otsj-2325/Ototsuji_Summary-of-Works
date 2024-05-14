

#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "selectCursor.h"
#include <thread>

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "AT13E093_09_‰¹’ÒŠ°“o_AEì•i";


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

POINT g_LastMousePos;
POINT g_CurrentMousePos;
POINT g_MouseDelta;
bool g_isSkip = false;
MOUSESTATE m_MouseState;
MOUSESTATE m_OldMS;
HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}

POINT GetMousePos()
{
	return g_CurrentMousePos;
}

POINT GetMouseDelta()
{
	return g_MouseDelta;
}

const MOUSESTATE GetMouseState()
{
	return m_MouseState;
}

void SetMouseSkip()
{
	g_isSkip = true;
}

bool GetMouseSkip()
{
	return g_isSkip;
}

void MouseProcess()
{
	g_MouseDelta.x = g_CurrentMousePos.x - g_LastMousePos.x;
	g_MouseDelta.y = g_CurrentMousePos.y - g_LastMousePos.y;
	g_LastMousePos = g_CurrentMousePos;

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_LastMousePos.x = 0;
	g_LastMousePos.y = 0;

	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

		SetWindowPos(g_Window, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
	}

	Manager::Init();
	
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	io.Fonts->AddFontFromFileTTF("asset\\font\\GenEiLateMinP_v2.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	ImGui_ImplWin32_Init(g_Window);
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	/*colors[ImGuiCol_Text] = ImVec4(0.4f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.98f, 0.90f, 0.9f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.8f, 0.5f, 0.5f, 1.0f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.9f, 0.3f, 0.3f, 1.0f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.59f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.33f, 0.06f, 1.00f);*/

	MSG msg;
	while(1)
	{		
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				Manager::Update();

				{
					ImGui::Begin("Main Property");

					float cursorX, cursorY;
					cursorX = g_CurrentMousePos.x;
					cursorY = g_CurrentMousePos.y;
					ImGui::Text("Cursor Pos : %g, %g", cursorX, cursorY);

					ImGui::Text("MOUSESTATE L : %d", m_MouseState.LBUTTON);
					ImGui::Text("MOUSESTATE M : %d", m_MouseState.MBUTTON);
					ImGui::Text("MOUSESTATE R : %d", m_MouseState.RBUTTON);

					ImGui::End();

				}

				ImGui::EndFrame();

				Manager::Draw();
			}
		}

		if(m_OldMS.LBUTTON == m_MouseState.LBUTTON)
		{
			if(m_MouseState.LBUTTON == MS_TRIGGER_UP)
			{
				m_MouseState.LBUTTON = MS_PREASS_UP;
			}
			if(m_MouseState.LBUTTON == MS_TRIGGER_DOWN)
			{
				m_MouseState.LBUTTON = MS_PREASS_DOWN;
			}
						
		}
		if(m_OldMS.MBUTTON == m_MouseState.MBUTTON)
		{
			if(m_MouseState.MBUTTON == MS_TRIGGER_UP)
			{
				m_MouseState.MBUTTON = MS_PREASS_UP;
			}
			if(m_MouseState.MBUTTON == MS_TRIGGER_DOWN)
			{
				m_MouseState.MBUTTON = MS_PREASS_DOWN;
			}

		}
		if(m_OldMS.RBUTTON == m_MouseState.RBUTTON)
		{
			if(m_MouseState.RBUTTON == MS_TRIGGER_UP)
			{
				m_MouseState.RBUTTON = MS_PREASS_UP;
			}
			if(m_MouseState.RBUTTON == MS_TRIGGER_DOWN)
			{
				m_MouseState.RBUTTON = MS_PREASS_DOWN;
			}

		}

		/*if ((GetMousePos().x > 0 && GetMousePos().x < SCENE_WIDTH_L) 
			|| (GetMousePos().x > SCENE_WIDTH_R && GetMousePos().x < SCREEN_WIDTH))
		{
			m_MouseState.LBUTTON = MS_SKIP;
			m_MouseState.MBUTTON = MS_SKIP;
			m_MouseState.RBUTTON = MS_SKIP;
		}
		else if(m_MouseState.LBUTTON == MS_SKIP)
		{
			m_MouseState.LBUTTON = MS_PREASS_UP;
			m_MouseState.MBUTTON = MS_PREASS_UP;
			m_MouseState.RBUTTON = MS_PREASS_UP;
		}*/
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	RECT rtScreen;
	GetWindowRect(GetDesktopWindow(), &rtScreen);
	ClipCursor(&rtScreen);

	return (int)msg.wParam;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	g_MouseDelta.x = 0;
	g_MouseDelta.y = 0;
	m_OldMS = m_MouseState;
	g_isSkip = false;

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_CurrentMousePos.x = GET_X_LPARAM(lParam);
		g_CurrentMousePos.y = GET_Y_LPARAM(lParam);
		MouseProcess();
		break;

	case WM_LBUTTONUP:
		m_MouseState.LBUTTON = MS_TRIGGER_UP;
		break;

	case WM_LBUTTONDOWN:
		m_MouseState.LBUTTON = MS_TRIGGER_DOWN;
		break;

	case WM_MBUTTONUP:
		m_MouseState.MBUTTON = MS_TRIGGER_UP;
		break;

	case WM_MBUTTONDOWN:
		m_MouseState.MBUTTON = MS_TRIGGER_DOWN;
		break;

	case WM_RBUTTONUP:
		m_MouseState.RBUTTON = MS_TRIGGER_UP;
		break;

	case WM_RBUTTONDOWN:
		m_MouseState.RBUTTON = MS_TRIGGER_DOWN;
		break;


	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

