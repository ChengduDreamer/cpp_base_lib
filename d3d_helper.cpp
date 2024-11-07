#include "d3d_helper.h"
#ifdef WIN32
#include <DirectXTex.h>
#include <chrono>
#include <sstream>
#include "d3d_render.h"
#include "str_util.h"
#include "win32_helper.h"

namespace yk {

void PrintD3DTexture2DDesc(const std::string& name, D3D11_TEXTURE2D_DESC* desc)
{
	printf("%s:Width:%d Height:%d MipLevels:%d ArraySize:%d Format:%d SampleDesc.Count:%d SampleDesc.Quality:%d Usage:%d BindFlags:%d CPUAccessFlags:%d MiscFlags:%d",
		name.c_str(),
		desc->Width,
		desc->Height,
		desc->MipLevels,
		desc->ArraySize,
		desc->Format,
		desc->SampleDesc.Count,
		desc->SampleDesc.Quality,
		desc->Usage,
		desc->BindFlags,
		desc->CPUAccessFlags,
		desc->MiscFlags
	);
}

void PrintD3DTexture2DDesc(const std::string& name,ID3D11Texture2D* tex)
{
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	PrintD3DTexture2DDesc(name,&desc);
}

bool DebugOutDDS(ID3D11Texture2D* pResource, const std::string& name)
{
	auto rhi = yk::D3DRender::BuildD3DRenderFromTexture(pResource);
	DirectX::ScratchImage image;
	static int i = 0;
	auto hr = DirectX::CaptureTexture(rhi->GetDevice(), rhi->GetContext(), pResource, image);
	std::stringstream oss;
	oss << "debug-" << ++i << "-" << name << ".dds";
	
	if (SUCCEEDED(hr)) {
		hr = DirectX::SaveToDDSFile(image.GetImages(),
			image.GetImageCount(),
			image.GetMetadata(),
			DirectX::DDS_FLAGS_NONE,
			yk::ToWString(oss.str()).c_str());
		return true;
	}
	else {
		printf("failed save image:%p", hr);
		return false;
	}
	
	return true;
}

void RecvD3D11Texture2DShareHandle(CComPtr<ID3D11Device> d3d11_device, HANDLE shared_handle, int monitor_index) {
    auto texture2d = OpenSharedTexture(d3d11_device, shared_handle);

    CopyID3D11Texture2D(texture2d);
}

ComPtr<ID3D11Texture2D> OpenSharedTexture(CComPtr<ID3D11Device> d3d11_device, HANDLE handle) {
    ComPtr<ID3D11Texture2D> sharedTexture;
    HRESULT hRes;
    if (FAILED(hRes = d3d11_device->OpenSharedResource(handle, IID_PPV_ARGS(sharedTexture.GetAddressOf())))) {
        return nullptr;
    }
    return sharedTexture;
}

bool D3D11Texture2DLockMutex(ComPtr<ID3D11Texture2D> texture2d) {
    HRESULT hRes;
    ComPtr<IDXGIKeyedMutex> key_mutex;
    if (FAILED(hRes = texture2d.As<IDXGIKeyedMutex>(&key_mutex)))
    {
        printf("D3D11Texture2DReleaseMutex IDXGIKeyedMutex. error\n");
        return false;
    }
    if (FAILED(hRes = key_mutex->AcquireSync(0,INFINITE)))
    {
        printf("D3D11Texture2DReleaseMutex AcquireSync failed.\n");
        return false;
    }
    return  true;
}

bool D3D11Texture2DReleaseMutex(ComPtr<ID3D11Texture2D> texture2d) {



    HRESULT hRes;
    ComPtr<IDXGIKeyedMutex> key_mutex;
    if(FAILED(hRes = texture2d.As<IDXGIKeyedMutex>(&key_mutex)))
    {
        printf("D3D11Texture2DReleaseMutex IDXGIKeyedMutex. error\n");
        return false;
    }
    if(FAILED(hRes = key_mutex->ReleaseSync(0)))
    {
        printf("D3D11Texture2DReleaseMutex ReleaseSync failed.\n");
        return false;
    }
}

bool CopyID3D11Texture2D(ComPtr<ID3D11Texture2D> shared_texture) {

    ComPtr<ID3D11Texture2D> texture2d;

    if(!D3D11Texture2DLockMutex(shared_texture)) {
        printf("D3D11Texture2DLockMutex error\n");
        return false;
    }
    std::shared_ptr<void> auto_realse_texture2D_mutex((void*)nullptr, [=](void* temp){
        D3D11Texture2DReleaseMutex(shared_texture);
    });

    HRESULT hRes;
    D3D11_TEXTURE2D_DESC desc;
    shared_texture->GetDesc(&desc);

    ComPtr<ID3D11Device> curDevice;
    shared_texture->GetDevice(&curDevice);

    if (texture2d)
    {
        ComPtr<ID3D11Device> sharedTextureDevice;
        texture2d->GetDevice(&sharedTextureDevice);
        if (sharedTextureDevice != curDevice) {
            texture2d = nullptr;
        }
        if (texture2d)
        {
            D3D11_TEXTURE2D_DESC sharedTextureDesc;
            texture2d->GetDesc(&sharedTextureDesc);
            if (desc.Width != sharedTextureDesc.Width ||
                desc.Height != sharedTextureDesc.Height ||
                desc.Format != sharedTextureDesc.Format)
            {
                texture2d = nullptr;
            }
        }
    }

    if (!texture2d)
    {
        D3D11_TEXTURE2D_DESC createDesc;
        ZeroMemory(&createDesc, sizeof(createDesc));
        createDesc.Format = desc.Format;
        createDesc.Width = desc.Width;
        createDesc.Height = desc.Height;
        createDesc.MipLevels = 1;
        createDesc.ArraySize = 1;
        createDesc.SampleDesc.Count = 1;
        createDesc.Usage = D3D11_USAGE_DEFAULT;
        createDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (FAILED(hRes = curDevice->CreateTexture2D(&createDesc, NULL, texture2d.GetAddressOf())))
        {
            printf("desktop capture create texture failed with:%s", yk::GetErrorStr(hRes).c_str());
            return false;
        }
    }
    ComPtr<ID3D11DeviceContext> ctx;
    curDevice->GetImmediateContext(&ctx);
    ctx->CopyResource(texture2d.Get(), shared_texture.Get());


    DebugOutDDS(texture2d.Get(), "debug");
    return true;
}



} // namespace yk

#endif