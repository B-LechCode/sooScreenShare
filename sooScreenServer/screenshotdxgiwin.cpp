#include "screenshotdxgiwin.h"

#include <winnt.h>

//DUPL_RETURN screenShotDxgiWin::GetFrame(_Out_ FRAME_DATA* Data, _Out_ bool* Timeout, _Out_ IDXGIResource** DesktopResourceI)
//{
//	////Variable Declaration
//	//IDXGIOutputDuplication* IDeskDupl;
//	//IDXGIResource*          IDesktopResource = nullptr;
//	//DXGI_OUTDUPL_FRAME_INFO IFrameInfo;
//	//ID3D11Texture2D*        IAcquiredDesktopImage;
//	//ID3D11Texture2D*        IDestImage = nullptr;
//	//ID3D11DeviceContext*    IImmediateContext;
//	//UCHAR*                  g_iMageBuffer = nullptr;
//
//	////Screen capture start here
//	//auto hr = m_DeskDupl->AcquireNextFrame(20, &IFrameInfo, &IDesktopResource);
//
//	//// >QueryInterface for ID3D11Texture2D
//	//hr = IDesktopResource->QueryInterface(IID_PPV_ARGS(&m_AcquiredDesktopImage));
//	//
//	//IDesktopResource->Release();
//
//	//// Copy image into GDI drawing texture
//	//m_Resources.Context->CopyResource(lDestImage, m_AcquiredDesktopImage);
//	//m_AcquiredDesktopImage->Release();
//	//IDeskDupl->ReleaseFrame();
//
//	//// Copy GPU Resource to CPU
//	//D3D11_TEXTURE2D_DESC desc;
//	//lDestImage->GetDesc(&desc);
//	//D3D11_MAPPED_SUBRESOURCE resource;
//	//UINT subresource = D3D11CalcSubresource(0, 0, 0);
//	//m_Resources.Context->Map(lDestImage, subresource, D3D11_MAP_READ_WRITE, 0, &resource);
//
//	////std::unique_ptr<BYTE> pBuf(new BYTE[resource.RowPitch*desc.Height]);
//	//UINT lBmpRowPitch = lOutputDuplDesc.ModeDesc.Width * 4;
//	//BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);
//	//BYTE* dptr = pBuf.get() + resource.RowPitch*desc.Height - lBmpRowPitch;
//	//UINT lRowPitch = std::min<UINT>(lBmpRowPitch, resource.RowPitch);
//
//	//for (size_t h = 0; h < lOutputDuplDesc.ModeDesc.Height; ++h)
//	//{
//	//	memcpy_s(dptr, lBmpRowPitch, sptr, lRowPitch);
//	//	sptr += resource.RowPitch;
//	//	dptr -= lBmpRowPitch;
//	//}
//
//	//lImmediateContext->Unmap(lDestImage, subresource);
//	//long g_captureSize = lRowPitch * desc.Height;
//	//g_iMageBuffer = new UCHAR[g_captureSize];
//	//g_iMageBuffer = (UCHAR*)malloc(g_captureSize);
//
//	////Copying to UCHAR buffer 
//	//memcpy(g_iMageBuffer, pBuf, g_captureSize);
//
//	IDXGIResource*& DesktopResource = *DesktopResourceI;
//
//
//	DXGI_OUTDUPL_FRAME_INFO FrameInfo;
//	HRESULT hr;
//	while(1)
//	{
//	// Get new frame
//	 hr = m_DeskDupl->AcquireNextFrame(500, &FrameInfo, &DesktopResource);
//	if (hr == DXGI_ERROR_WAIT_TIMEOUT)
//	{
//		*Timeout = true;
//		return DUPL_RETURN_SUCCESS;
//	}
//	*Timeout = false;
//
//	if (FAILED(hr))
//	{
//		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(m_Device, L"Failed to acquire next frame in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
//	}
//
//	// If still holding old frame, destroy it
//	if (m_AcquiredDesktopImage)
//	{
//		m_AcquiredDesktopImage->Release();
//		m_AcquiredDesktopImage = nullptr;
//	}
//
//	// QI for IDXGIResource
//	hr = DesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&m_AcquiredDesktopImage));
//	//DesktopResource->Release();
//	//DesktopResource = nullptr;
//	if (FAILED(hr))
//	{
//		return  DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(nullptr, L"Failed to QI for ID3D11Texture2D from acquired IDXGIResource in DUPLICATIONMANAGER", L"Error", hr);
//	}
//
//	//create texture
//	
//	D3D11_TEXTURE2D_DESC DeskTexD;
//	RtlZeroMemory(&DeskTexD, sizeof(D3D11_TEXTURE2D_DESC));
//	DeskTexD.Width = m_OutputDesc.DesktopCoordinates.right - m_OutputDesc.DesktopCoordinates.left;
//	DeskTexD.Height = m_OutputDesc.DesktopCoordinates.bottom - m_OutputDesc.DesktopCoordinates.top;
//	/*DeskTexD.MipLevels = 1;
//	DeskTexD.ArraySize = 1;
//	DeskTexD.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	DeskTexD.SampleDesc.Count = 1;
//	DeskTexD.Usage = D3D11_USAGE_DEFAULT;
//	DeskTexD.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//	DeskTexD.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//	DeskTexD.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;*/
//	DeskTexD.ArraySize = 1;
//	DeskTexD.BindFlags = 0;
//	DeskTexD.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//	DeskTexD.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	
//	DeskTexD.MipLevels = 1;
//	DeskTexD.MiscFlags = 0;
//	DeskTexD.SampleDesc.Count = 1;
//	DeskTexD.SampleDesc.Quality = 0;
//	DeskTexD.Usage = D3D11_USAGE_STAGING;
//
//
//	hr = m_Device->CreateTexture2D(&DeskTexD, nullptr, &lDestImage);
//
//	// Copy image into GDI drawing texture
//	m_Resources.Context->CopyResource(lDestImage, m_AcquiredDesktopImage);
//
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	hr = m_Resources.Context->Map(lDestImage, 0, D3D11_MAP_READ, 0, &mappedResource);
//		//std::cout << "Error: [CAM 2] could not Map Rendered Camera ShaderResource for Undistortion" << std::endl;
//
//	cv::Mat imgL = cv::Mat(DeskTexD.Height, DeskTexD.Width, CV_8UC4, mappedResource.pData,mappedResource.RowPitch);
//	//memcpy(imgL.ptr(), DeskTexD.Width*DeskTexD.Height * 4);
//
//	cv::imwrite("C:/temp/asdf.png", imgL);
//	}
//
//	//lAcquiredDesktopImage.Release();
//	//lDeskDupl->ReleaseFrame();
//
//	// Get metadata
//	if (FrameInfo.TotalMetadataBufferSize)
//	{
//		// Old buffer too small
//		if (FrameInfo.TotalMetadataBufferSize > m_MetaDataSize)
//		{
//			if (m_MetaDataBuffer)
//			{
//				delete[] m_MetaDataBuffer;
//				m_MetaDataBuffer = nullptr;
//			}
//			m_MetaDataBuffer = new (std::nothrow) BYTE[FrameInfo.TotalMetadataBufferSize];
//			if (!m_MetaDataBuffer)
//			{
//				m_MetaDataSize = 0;
//				Data->MoveCount = 0;
//				Data->DirtyCount = 0;
//				return  DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(nullptr, L"Failed to allocate memory for metadata in DUPLICATIONMANAGER", L"Error", E_OUTOFMEMORY);
//			}
//			m_MetaDataSize = FrameInfo.TotalMetadataBufferSize;
//		}
//
//		UINT BufSize = FrameInfo.TotalMetadataBufferSize;
//
//		// Get move rectangles
//		hr = m_DeskDupl->GetFrameMoveRects(BufSize, reinterpret_cast<DXGI_OUTDUPL_MOVE_RECT*>(m_MetaDataBuffer), &BufSize);
//		if (FAILED(hr))
//		{
//			Data->MoveCount = 0;
//			Data->DirtyCount = 0;
//			return  DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED;//ProcessFailure(nullptr, L"Failed to get frame move rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
//		}
//		Data->MoveCount = BufSize / sizeof(DXGI_OUTDUPL_MOVE_RECT);
//
//		BYTE* DirtyRects = m_MetaDataBuffer + BufSize;
//		BufSize = FrameInfo.TotalMetadataBufferSize - BufSize;
//
//		// Get dirty rectangles
//		hr = m_DeskDupl->GetFrameDirtyRects(BufSize, reinterpret_cast<RECT*>(DirtyRects), &BufSize);
//		if (FAILED(hr))
//		{
//			Data->MoveCount = 0;
//			Data->DirtyCount = 0;
//			return  DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(nullptr, L"Failed to get frame dirty rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
//		}
//		Data->DirtyCount = BufSize / sizeof(RECT);
//
//		Data->MetaData = m_MetaDataBuffer;
//	}
//
//	Data->Frame = m_AcquiredDesktopImage;
//	Data->FrameInfo = FrameInfo;
//
//	return DUPL_RETURN_SUCCESS;
//}

//
// Initialize duplication interfaces
//
//DUPL_RETURN screenShotDxgiWin::InitDupl(_In_ ID3D11Device* Device, UINT Output)
//{
//	m_OutputNumber = Output;
//
//	 Take a reference on the device
//	m_Device = Device;
//	m_Device->AddRef();
//
//	 Get DXGI device
//	IDXGIDevice* DxgiDevice = nullptr;
//	HRESULT hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DxgiDevice));
//	if (FAILED(hr))
//	{
//		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(nullptr, L"Failed to QI for DXGI Device", L"Error", hr);
//	}
//
//	 Get DXGI adapter
//	IDXGIAdapter* DxgiAdapter = nullptr;
//	hr = DxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DxgiAdapter));
//	DxgiDevice->Release();
//	DxgiDevice = nullptr;
//	if (FAILED(hr))
//	{
//		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(m_Device, L"Failed to get parent DXGI Adapter", L"Error", hr, SystemTransitionsExpectedErrors);
//	}
//
//	 Get output
//	IDXGIOutput* DxgiOutput = nullptr;
//	hr = DxgiAdapter->EnumOutputs(Output, &DxgiOutput);
//	DxgiAdapter->Release();
//	DxgiAdapter = nullptr;
//	if (FAILED(hr))
//	{
//		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(m_Device, L"Failed to get specified output in DUPLICATIONMANAGER", L"Error", hr, EnumOutputsExpectedErrors);
//	}	
//
//	DxgiOutput->GetDesc(&m_OutputDesc);
//
//	 QI for Output 1
//	IDXGIOutput1* DxgiOutput1 = nullptr;
//	hr = DxgiOutput->QueryInterface(__uuidof(DxgiOutput1), reinterpret_cast<void**>(&DxgiOutput1));
//	DxgiOutput->Release();
//	DxgiOutput = nullptr;
//	if (FAILED(hr))
//	{
//		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(nullptr, L"Failed to QI for DxgiOutput1 in DUPLICATIONMANAGER", L"Error", hr);
//	}
//
//	 Create desktop duplication
//	hr = DxgiOutput1->DuplicateOutput(m_Device, &m_DeskDupl);
//	DxgiOutput1->Release();
//	DxgiOutput1 = nullptr;
//	if (FAILED(hr))
//	{
//		if (hr == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
//		{
//			MessageBoxW(nullptr, L"There is already the maximum number of applications using the Desktop Duplication API running, please close one of those applications and then try again.", L"Error", MB_OK);
//			return DUPL_RETURN_ERROR_UNEXPECTED;
//		}
//		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED; //ProcessFailure(m_Device, L"Failed to get duplicate output in DUPLICATIONMANAGER", L"Error", hr, CreateDuplicationExpectedErrors);
//	}
//
//	return DUPL_RETURN_SUCCESS;
//}

//
// Get DX_RESOURCES
//
DUPL_RETURN screenShotDxgiWin::InitializeDx(_Out_ DX_RESOURCES* Data)
{
	HRESULT hr = S_OK;

	// Driver types supported
	D3D_DRIVER_TYPE DriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

	// Feature levels supported
	D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

	D3D_FEATURE_LEVEL FeatureLevel;

	// Create device
	for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
	{
		hr = D3D11CreateDevice(nullptr, DriverTypes[DriverTypeIndex], nullptr, /*D3D11_CREATE_DEVICE_DEBUG*/0, FeatureLevels, NumFeatureLevels,
			D3D11_SDK_VERSION, &Data->Device, &FeatureLevel, &Data->Context);
		if (SUCCEEDED(hr))
		{
			// Device creation success, no need to loop anymore
			break;
		}
	}
	if (FAILED(hr))
	{
		return DUPL_RETURN::DUPL_RETURN_ERROR_UNEXPECTED;// ProcessFailure(nullptr, L"Failed to create device in InitializeDx", L"Error", hr);
	}

	return DUPL_RETURN_SUCCESS;
}