#include "d3d_render.h"

#ifdef WIN32

#include <d3d11_4.h>
#include "win32_helper.h"
#include "str_util.h"


namespace yk {

std::shared_ptr<D3DRender> D3DRender::BuildD3DRenderFromTexture(ID3D11Texture2D* tex)
{
	if(!tex)
		return nullptr;
	auto ret = std::make_shared<D3DRender>();
	tex->GetDevice(&ret->mD3D11Device);
	ret->mD3D11Device->GetImmediateContext(&ret->mD3D11DeviceContext);
	return ret;
}

std::shared_ptr<D3DRender> D3DRender::Create(HANDLE handle,ID3D11Texture2D** outSharedTexture)
{
 	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	HRESULT hRes;
	if (FAILED(hRes = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&factory)))) {
		printf("CreateDXGIFactory1 failed with %s", yk::GetErrorStr(hRes).c_str());
		return nullptr;
	}

	bool createDeviceSuccess = false;
	int adapterNum = 0;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> retTexture;
	while (1)
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapterPtr;
		if (FAILED(hRes = factory->EnumAdapters(adapterNum++, &adapterPtr)))
		{
			if (hRes == DXGI_ERROR_NOT_FOUND)
				break;
			printf("EnumAdapters failed with %s", yk::GetErrorStr(hRes).c_str());
			break;
		}

		DXGI_ADAPTER_DESC desc;
		if (SUCCEEDED(adapterPtr->GetDesc(&desc)))
		{
			printf("Enum adapter name:%s index:%d", yk::ToUTF8(desc.Description).c_str(), adapterNum);
		}

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx;
		if (FAILED(hRes = D3D11CreateDevice(adapterPtr.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0,
			D3D11_SDK_VERSION, &device, NULL, &ctx))) {
			printf("enum adapters D3D11CreateDevice failed err:%s adapterNum:%d", yk::GetErrorStr(hRes).c_str(), adapterNum);
			continue;
		}

		if (FAILED(hRes = device->OpenSharedResource(handle, IID_PPV_ARGS(outSharedTexture))))
		{
			printf("OpenSharedResource failed.  SharedTexHandle:%p err:%p info:%s, adapter index : %d", handle, hRes, yk::GetErrorStr(hRes).c_str(), adapterNum);
			continue;
		}
		else
		{
			printf("OpenSharedResource Success at adapter : %d  texhandle:%p", adapterNum, handle);
			auto ret = std::make_shared<D3DRender>();
			ret->mD3D11Device = device.Get();
			ret->mD3D11DeviceContext = ctx.Get();
			return ret;
		}
	}
	return nullptr;
}

std::shared_ptr<D3DRender> D3DRender::Create()
{
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	HRESULT hRes;
	if (FAILED(hRes = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&factory)))) {
		printf("CreateDXGIFactory1 failed with %s", yk::GetErrorStr(hRes).c_str());
		return nullptr;
	}

	bool createDeviceSuccess = false;
	int adapterNum = 0;
	while (1)
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapterPtr;
		if (FAILED(hRes = factory->EnumAdapters(adapterNum++, &adapterPtr)))
		{
			if (hRes == DXGI_ERROR_NOT_FOUND)
				break;
			printf("EnumAdapters failed with %s", yk::GetErrorStr(hRes).c_str());
			break;
		}

		DXGI_ADAPTER_DESC desc;
		if (SUCCEEDED(adapterPtr->GetDesc(&desc)))
		{
			printf("Enum adapter name:%s index:%d", yk::ToUTF8(desc.Description).c_str(), adapterNum);
		}

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx;
		if (FAILED(hRes = D3D11CreateDevice(adapterPtr.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0,
			D3D11_SDK_VERSION, &device, NULL, &ctx))) {
			printf("enum adapters D3D11CreateDevice failed err:%s adapterNum:%d", yk::GetErrorStr(hRes).c_str(), adapterNum);
			continue;
		}

		auto ret = std::make_shared<D3DRender>();
		ret->mD3D11Device = device.Get();
		ret->mD3D11DeviceContext = ctx.Get();
		return ret;
	}
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DRender::OpenSharedTexture(HANDLE handle)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> sharedTexture;
	HRESULT hRes;
	if (FAILED(hRes = mD3D11Device->OpenSharedResource(handle, IID_PPV_ARGS(&sharedTexture))))
		return nullptr;
	return sharedTexture;
}

} // namespace yk

#endif