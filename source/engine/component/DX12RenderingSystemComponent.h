#pragma once
#include "../common/InnoType.h"
#include "../component/DX12RenderPassComponent.h"
#include "../component/DX12ShaderProgramComponent.h"
#include "../component/DX12MeshDataComponent.h"
#include "../component/TextureDataComponent.h"
#include "../component/DX12TextureDataComponent.h"

class DX12RenderingSystemComponent
{
public:
	~DX12RenderingSystemComponent() {};

	static DX12RenderingSystemComponent& get()
	{
		static DX12RenderingSystemComponent instance;
		return instance;
	}

	ObjectStatus m_objectStatus = ObjectStatus::SHUTDOWN;
	EntityID m_parentEntity;

	TVec2<unsigned int> m_refreshRate = TVec2<unsigned int>(0, 1);

	int m_videoCardMemory;
	char m_videoCardDescription[128];

	ID3D12Debug1* m_debugInterface;

	IDXGIFactory4* m_factory;

	DXGI_ADAPTER_DESC m_adapterDesc;
	IDXGIAdapter4* m_adapter;
	IDXGIOutput* m_adapterOutput;

	ID3D12Device2* m_device;

	ID3D12CommandAllocator* m_commandAllocator;

	D3D12_COMMAND_QUEUE_DESC m_commandQueueDesc;
	ID3D12CommandQueue* m_commandQueue;

	DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc;
	IDXGISwapChain4* m_swapChain;

	DX12ShaderProgramComponent* m_swapChainDXSPC;
	DX12RenderPassComponent* m_swapChainDXRPC;

	D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;

	D3D12_DEPTH_STENCIL_VIEW_DESC m_depthStencilViewDesc;

	D3D12_RASTERIZER_DESC m_rasterDescForward;

	D3D12_RASTERIZER_DESC m_rasterDescDeferred;

	D3D12_VIEWPORT m_viewport;

	RenderPassDesc m_deferredRenderPassDesc = RenderPassDesc();

	ID3D12DescriptorHeap* m_CSUHeap;
	D3D12_DESCRIPTOR_HEAP_DESC m_CSUHeapDesc = {};
	D3D12_CPU_DESCRIPTOR_HANDLE m_initialCSUCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_initialCSUGPUHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentCSUCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_currentCSUGPUHandle;

	ID3D12DescriptorHeap* m_samplerHeap;
	D3D12_DESCRIPTOR_HEAP_DESC m_samplerHeapDesc = {};
	D3D12_CPU_DESCRIPTOR_HANDLE m_initialSamplerCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_initialSamplerGPUHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentSamplerCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_currentSamplerGPUHandle;

	DX12ConstantBuffer m_cameraConstantBuffer;
	DX12ConstantBuffer m_meshConstantBuffer;
	DX12ConstantBuffer m_materialConstantBuffer;

private:
	DX12RenderingSystemComponent() {};
};
