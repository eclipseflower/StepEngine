#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <string>
#include <comdef.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
	_com_error err(hr__);												  \
	LPCTSTR errMsg = err.ErrorMessage();							  \
    if(FAILED(hr__)) { throw DxException(hr__, errMsg, __FILE__, __LINE__); } \
}
#endif

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::string& functionName, const std::string& filename, int lineNumber) :
		ErrorCode(hr),
		FunctionName(functionName),
		Filename(filename),
		LineNumber(lineNumber)
	{

	}

	std::string ToString()const
	{
		// Get the string description of the error code.
		_com_error err(ErrorCode);
		std::string msg = err.ErrorMessage();

		return FunctionName + " failed in " + Filename + "; line " + std::to_string(LineNumber) + "; error: " + msg;
	}

	HRESULT ErrorCode = S_OK;
	std::string FunctionName;
	std::string Filename;
	int LineNumber = -1;
};

HWND mhMainWnd;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{

		}
	}

	return (int)msg.wParam;
}

bool InitMainWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, 800, 600 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow("MainWnd", "InitDX12",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!mhMainWnd)
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}

bool InitDevice()
{
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAlloc = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandList> commandList = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;

	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();

	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));
	ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAlloc)));
	ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAlloc.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	DXGI_SWAP_CHAIN_DESC chainDesc;
	chainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferDesc.Height = 600;
	chainDesc.BufferDesc.Width = 800;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	chainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	chainDesc.BufferDesc.RefreshRate.Denominator = 1;
	chainDesc.BufferDesc.RefreshRate.Numerator = 60;

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
	qualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//https://stackoverflow.com/questions/40110699/creating-a-swap-chain-with-msaa-fails
	/*
		Direct3D 12 don't support creating MSAA swap chains
		--attempts to create a swap chain with SampleDesc.Count > 1 will fail. 
		Instead, you create your own MSAA render target and 
		explicitly resolve to the DXGI back-buffer for presentation as shown here.
	*/
	qualityLevels.SampleCount = 1;
	qualityLevels.NumQualityLevels = 0;
	qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	ThrowIfFailed(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof(qualityLevels)));

	chainDesc.SampleDesc.Count = 4;
	chainDesc.SampleDesc.Quality = 0;

	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = 2;
	chainDesc.OutputWindow = mhMainWnd;
	chainDesc.Windowed = true;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(dxgiFactory->CreateSwapChain(commandQueue.Get(), &chainDesc, &dxgiSwapChain));

	UINT rtvIncSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	UINT dsvIncSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;

	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	try
	{
		if (!InitMainWindow(hInstance))
		{
			return 0;
		}

		if (!InitDevice())
		{
			return 0;
		}

		return Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), "HR Failed", MB_OK);
		return 0;
	}
}