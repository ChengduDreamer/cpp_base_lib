#pragma once

#ifdef WIN32
#include <map>
#include <memory>
#include "d3d_helper.h"

namespace yk {

class D3DRender
{
public:
	static std::shared_ptr<D3DRender> BuildD3DRenderFromTexture(ID3D11Texture2D* tex);
	static std::shared_ptr<D3DRender> Create(HANDLE handle,ID3D11Texture2D** outSharedTexture);
	static std::shared_ptr<D3DRender> Create();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> OpenSharedTexture(HANDLE handle);

	ID3D11Device *GetDevice() { return mD3D11Device; }
	ID3D11DeviceContext *GetContext() { return mD3D11DeviceContext; }

	CComPtr<ID3D11Device> mD3D11Device;
	CComPtr<ID3D11DeviceContext> mD3D11DeviceContext;
};

} // namespace yk

#endif