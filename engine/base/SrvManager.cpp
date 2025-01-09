#include "SrvManager.h"

const uint32_t SrvManager::kMaxSRVCount = 512;
void SrvManager::Initialize(DirectXCommon* dxcommon)
{
	directXCommon = dxcommon;
	//デスクリプタヒープの生成
	descriptorHeap = directXCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	//デスクリプタ1個分のサイズを取得して記録
	descriptorSize = directXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	useIndex = 0;

}


uint32_t SrvManager::Allocate()
{
	assert(CheckTexturesNumber());

	//reurnする番号をいったん記録しておく
	int index = useIndex;
	//次回のために番号を１進める
	useIndex++;
	//上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDrscriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDrscriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT foemat, UINT MipLevels)
{

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{  };
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);
	srvDesc.Format = foemat;
	directXCommon->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDrscriptorHandle(srvIndex));

}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResourece, UINT numElements, UINT structureByteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{  };
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(structureByteStride);

	directXCommon->GetDevice()->CreateShaderResourceView(pResourece, &srvDesc, GetCPUDrscriptorHandle(srvIndex));

}

void SrvManager::PreDraw()
{
	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
	directXCommon->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

}

void SrvManager::SetGraficsRootDescriptorTable(UINT RootprameterIndex, uint32_t srvIndex)
{
	directXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(RootprameterIndex, GetGPUDrscriptorHandle(srvIndex));
}

bool SrvManager::CheckTexturesNumber()
{
	if (kMaxSRVCount <= useIndex) {
		return false;
	};
	return true;

}
