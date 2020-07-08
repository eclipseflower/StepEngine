#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <string>
#include <comdef.h>
#include <wrl.h>
#include "d3dx12.h"

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

HWND ghMainWnd;

Microsoft::WRL::ComPtr<ID3D12Fence> gFence = nullptr;
Microsoft::WRL::ComPtr<ID3D12CommandQueue> gCommandQueue = nullptr;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> gCommandList = nullptr;
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> gCommandAlloc = nullptr;
Microsoft::WRL::ComPtr<IDXGISwapChain> gSwapChain = nullptr;
Microsoft::WRL::ComPtr<ID3D12Resource> gBackBuffer[2];
Microsoft::WRL::ComPtr<ID3D12Device> gDevice = nullptr;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> gRtvHeap = nullptr;
Microsoft::WRL::ComPtr<ID3D12Resource> gDepthStencilBuffer = nullptr;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> gDsvHeap = nullptr;

UINT gRtvIncSize = 0;
UINT fenceCount = 0;

D3D12_VIEWPORT gViewport;
D3D12_RECT gScissorRect;

void OnResize();

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

	ghMainWnd = CreateWindow("MainWnd", "InitDX12",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!ghMainWnd)
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);

	return true;
}

bool InitDevice()
{
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory = nullptr;

	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();

	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&gDevice)));
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

	ThrowIfFailed(gDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&gFence)));

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	ThrowIfFailed(gDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&gCommandQueue)));
	ThrowIfFailed(gDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&gCommandAlloc)));
	ThrowIfFailed(gDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, gCommandAlloc.Get(), nullptr, IID_PPV_ARGS(&gCommandList)));
	gCommandList->Close();

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
	ThrowIfFailed(gDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof(qualityLevels)));

	chainDesc.SampleDesc.Count = 1;
	chainDesc.SampleDesc.Quality = 0;

	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = 2;
	chainDesc.OutputWindow = ghMainWnd;
	chainDesc.Windowed = true;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(dxgiFactory->CreateSwapChain(gCommandQueue.Get(), &chainDesc, &gSwapChain));

	gRtvIncSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	UINT dsvIncSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 2;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(gDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&gRtvHeap)));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(gDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&gDsvHeap)));

	OnResize();

	return true;
}

void FlushCommandQueue()
{
	fenceCount++;
	ThrowIfFailed(gCommandQueue->Signal(gFence.Get(), fenceCount));
	if (gFence->GetCompletedValue() < fenceCount)
	{
		HANDLE handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(gFence->SetEventOnCompletion(fenceCount, handle));

		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
	}
}

void OnResize()
{
	assert(gDevice);
	assert(gSwapChain);
	assert(gCommandAlloc);

	FlushCommandQueue();

	ThrowIfFailed(gSwapChain->ResizeBuffers(2, 800, 600, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(gRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < 2; i++)
	{
		ThrowIfFailed(gSwapChain->GetBuffer(i, IID_PPV_ARGS(&gBackBuffer[i])));
		gDevice->CreateRenderTargetView(gBackBuffer[i].Get(), nullptr, handle);
		handle.Offset(1, gRtvIncSize);
	}

	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = 800;
	desc.Height = 600;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;
	ThrowIfFailed(gDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, &clearValue, IID_PPV_ARGS(&gDepthStencilBuffer)));
	
	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc;
	viewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Flags = D3D12_DSV_FLAG_NONE;
	viewDesc.Texture2D.MipSlice = 0;

	handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(gDsvHeap->GetCPUDescriptorHandleForHeapStart());
	gDevice->CreateDepthStencilView(gDepthStencilBuffer.Get(), &viewDesc, handle);
	
	ThrowIfFailed(gCommandAlloc->Reset());
	ThrowIfFailed(gCommandList->Reset(gCommandAlloc.Get(), nullptr));
	
	gCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(gDepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	ThrowIfFailed(gCommandList->Close());

	ID3D12CommandList *cmdList[] = { gCommandList.Get() };
	gCommandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);

	FlushCommandQueue();

	gViewport.TopLeftX = 0;
	gViewport.TopLeftY = 0;
	gViewport.Width = 800;
	gViewport.Height = 600;
	gViewport.MinDepth = 0.0f;
	gViewport.MaxDepth = 1.0f;

	gScissorRect.left = 0;
	gScissorRect.right = 800;
	gScissorRect.top = 0;
	gScissorRect.bottom = 600;
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