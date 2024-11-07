#pragma once
#ifdef WIN32
#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <atlbase.h>
#include <d3d11.h>
#include <wrl.h>

namespace yk {

using Microsoft::WRL::ComPtr;

void PrintD3DTexture2DDesc(const std::string& name,D3D11_TEXTURE2D_DESC* desc);
void PrintD3DTexture2DDesc(const std::string& name,ID3D11Texture2D* tex);
bool DebugOutDDS(ID3D11Texture2D* pResource, const std::string& name);

void RecvD3D11Texture2DShareHandle(CComPtr<ID3D11Device> d3d11_device, HANDLE shared_handle, int monitor_index);
ComPtr<ID3D11Texture2D> OpenSharedTexture(CComPtr<ID3D11Device> d3d11_device, HANDLE handle);

bool D3D11Texture2DLockMutex(ComPtr<ID3D11Texture2D> texture2d);
bool D3D11Texture2DReleaseMutex(ComPtr<ID3D11Texture2D> texture2d);
bool CopyID3D11Texture2D(ComPtr<ID3D11Texture2D> texture2d);

} // namespace yk
#endif