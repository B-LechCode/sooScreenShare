//sooScreenShare windows dxgi backend by Simon Wezstein (B-LechCode), 2021
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef SCREENSHOTWIN_DXGI_H
#define SCREENSHOTWIN_DXGI_H
#ifdef WITH_WINAPI_DXGI

namespace shot {
#define DISPLAY_NUMBER "Display Number"
#define OVERRIDE_DISPLAY_SELECTION "Override Display Selection"
#define Grab_X "X"
#define Grab_Y "Y"
#define Grab_W "Width"
#define Grab_H "Height"
}

#include "iscreenshot.h"
#include <windows.h>

#include <dxgi1_2.h>
#include <d3d11.h>


#pragma comment(lib, "dxgi.lib")

typedef _Return_type_success_(return == DUPL_RETURN_SUCCESS) enum
{
	DUPL_RETURN_SUCCESS = 0,
	DUPL_RETURN_ERROR_EXPECTED = 1,
	DUPL_RETURN_ERROR_UNEXPECTED = 2
}DUPL_RETURN;

//
// FRAME_DATA holds information about an acquired frame
//
typedef struct _FRAME_DATA
{
	ID3D11Texture2D* Frame;
	DXGI_OUTDUPL_FRAME_INFO FrameInfo;
	_Field_size_bytes_((MoveCount * sizeof(DXGI_OUTDUPL_MOVE_RECT)) + (DirtyCount * sizeof(RECT))) BYTE* MetaData;
	UINT DirtyCount;
	UINT MoveCount;
} FRAME_DATA;

//
// Structure that holds D3D resources not directly tied to any one thread
//
typedef struct _DX_RESOURCES
{
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;
	ID3D11SamplerState* SamplerLinear;
	
} DX_RESOURCES;

class screenShotDxgiWin : public IscreenShot
{
	
	//von Simon
	IDXGIFactory1* pFactory;
	IDXGIOutput1 * m_pOutput;
	std::vector< IDXGIOutput1*> vOutputs;
	ID3D11Texture2D*        lDestImage = nullptr;
	DXGI_OUTPUT_DESC m_OutputDesc;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	cv::Mat m_Out;
	IDXGIOutputDuplication* pDup = nullptr;
	bool m_restart = false;

	

	ID3D11Texture2D* m_AcquiredDesktopImage;	
	UINT m_OutputNumber;
	DX_RESOURCES m_Resources;

	
	

	DUPL_RETURN InitializeDx(_Out_ DX_RESOURCES* Data);
	
	void createTexture()
	{
		if (lDestImage)
		{
			lDestImage->Release();
			lDestImage = nullptr;
		}
		D3D11_TEXTURE2D_DESC texDesc;
		memset(&texDesc, 0, sizeof(texDesc));
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.BindFlags = 0;
		texDesc.Width = m_OutputDesc.DesktopCoordinates.right - m_OutputDesc.DesktopCoordinates.left;
		texDesc.Height = m_OutputDesc.DesktopCoordinates.bottom - m_OutputDesc.DesktopCoordinates.top;
		texDesc.MipLevels = 1;
		texDesc.SampleDesc = { 1, 0 };
		texDesc.Usage = D3D11_USAGE_STAGING;
		texDesc.ArraySize = 1;
		ID3D11Texture2D* destinationTexture = 0;


		auto hr = m_Resources.Device->CreateTexture2D(&texDesc, nullptr, &lDestImage);

		m_Out = cv::Mat(texDesc.Height, texDesc.Width, CV_8UC4);
	}

public:	
	virtual void parameterMapChangedEvent()
	{
		initialize();

	}

	void applyParameters()
	{
		std::string overrideStr = m_parameters[OVERRIDE_DISPLAY_SELECTION].value();
		std::transform(overrideStr.begin(), overrideStr.end(), overrideStr.begin(), [](int c) -> int { return std::tolower(c); });
		bool override = overrideStr == "true";



		if (!override)
		{
			int displayNumber = std::stoi(m_parameters[DISPLAY_NUMBER].value());
			auto displays = getScreens();

			if (displays.size() - 1 < displayNumber)
				;//FAIL!
			else
			{
				auto displayNumberS = static_cast<size_t>(displayNumber);
				m_x = displays[displayNumberS].x;
				m_y = displays[displayNumberS].y;
				m_h = displays[displayNumberS].h;
				m_w = displays[displayNumberS].w;
			}
		}
		else
		{
			m_x = std::stoi(m_parameters[Grab_X].value());
			m_y = std::stoi(m_parameters[Grab_Y].value());
			m_h = static_cast<uint32_t>(std::stoi(m_parameters[Grab_H].value()));
			m_w = static_cast<uint32_t>(std::stoi(m_parameters[Grab_W].value()));
		}

		auto o = w();
		cv::imwrite("C:/temp/lool.png", o);
	}

	void destruct()
	{
		m_init = false;
		if(pFactory)
			pFactory->Release();		
	}

	virtual void parameterChangedEvent(const std::string& key)
	{
		initialize();
	}

	screenShotDxgiWin() : IscreenShot(), m_init(false), m_AcquiredDesktopImage(nullptr), m_pOutput(nullptr)
	{
		m_defaultParameters[OVERRIDE_DISPLAY_SELECTION] = parameter("Override display selection to freely define the grab ROI", "bool", "false");
		m_defaultParameters[DISPLAY_NUMBER] = parameter("The display to grab", "int16", "0");
		m_defaultParameters[Grab_X] = parameter("The X coordinate of the grab ROI", "int32", "0");
		m_defaultParameters[Grab_Y] = parameter("The Y coordinate of the grab ROI", "int32", "0");
		m_defaultParameters[Grab_W] = parameter("The width of the grab ROI", "uint32", "640");
		m_defaultParameters[Grab_H] = parameter("The width of the grab ROI", "uint32", "480");

		auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory));

		if (hr != S_OK)
			throw "CreateDXGIFactory1 failed!";
		auto res = InitializeDx(&m_Resources);
		setParameters(m_defaultParameters);
		mappedResource.pData = nullptr;
	}
	
	virtual void initialize()
	{
		applyParameters();
		if (m_init)
			destruct();	

		m_init = true;
	}

	virtual ~screenShotDxgiWin() {
		destruct();
	}

	

	virtual std::vector<screenDef> getScreens()
	{
		screens.clear();
		vOutputs.clear();
		HRESULT hr;
		//1: Factory
		if (!pFactory)
			return screens;


		//2: Enumerate Adapters
		IDXGIAdapter1* pAdapter = nullptr;
		std::vector< IDXGIAdapter1*> vAdapters;
		std::vector< std::wstring > vAdapterDesc;
		for (UINT i = 0; pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
		{
			vAdapters.push_back(pAdapter);
			DXGI_ADAPTER_DESC1 pDesc;
			hr = pAdapter->GetDesc1(&pDesc);
			if (hr == S_OK)
				vAdapterDesc.push_back(std::wstring(pDesc.Description));
			else
				vAdapterDesc.push_back(std::wstring());			
		}

		//3: Enumerate Outputs
		
		std::vector< std::wstring > vOutputDesc;
		for (size_t j = 0; j < vAdapters.size(); ++j)
		{
			pAdapter = vAdapters[j];
			IDXGIOutput* ptrOutput = nullptr;
			for (UINT i = 0; pAdapter->EnumOutputs(i, &ptrOutput) != DXGI_ERROR_NOT_FOUND; i++)
			{
				IDXGIOutput1* ptrOutput1 = nullptr;
				hr = ptrOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&ptrOutput1));
				vOutputs.push_back(ptrOutput1);
				DXGI_OUTPUT_DESC dOutput;
				hr = ptrOutput->GetDesc(&dOutput);
				if (hr == S_OK)
					vOutputDesc.push_back(std::wstring(dOutput.DeviceName));
				else
					vOutputDesc.push_back(std::wstring());

				//Screen definitions
				auto y = dOutput.DesktopCoordinates.top;
				auto x = dOutput.DesktopCoordinates.left;
				auto w = dOutput.DesktopCoordinates.right - x;
				auto h = dOutput.DesktopCoordinates.bottom - y;
				screenDef d(x, y, w, h);

				screens.push_back(d);
			}
		}
		auto pOutput = vOutputs[0];
		hr = pOutput->GetDesc(&m_OutputDesc);
		if (hr != S_OK)
			throw "Error in GetDesc";
		createTexture();
		
		if (pDup)
		{
			pDup->Release();
			pDup = nullptr;
		}
		hr = pOutput->DuplicateOutput(m_Resources.Device, &pDup);
		return screens;
	}
	UINT m_MetaDataSize = 0;
	BYTE* m_MetaDataBuffer = nullptr;

	cv::Mat w()
	{
		auto pOutput = vOutputs[0];
		auto hr = pOutput->GetDesc(&m_OutputDesc);
		if (hr != S_OK)
			throw "Error in GetDesc";
		
		

		
		DXGI_OUTDUPL_FRAME_INFO fInfo;
		IDXGIResource* pDR = nullptr;
		_FRAME_DATA Data;
		
		while (true)
		{
			hr = pDup->AcquireNextFrame(INFINITE, &fInfo, &pDR);
			
			if (hr != S_OK)
			{
				if (hr == DXGI_ERROR_INVALID_CALL)
				{
					m_restart = true;
				}
				else if (hr == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
				{
					m_restart = true;
				}
				
				return cv::Mat();
			}

			
			

			//	// Get metadata
			if (fInfo.TotalMetadataBufferSize)
			{
				// Old buffer too small
				if (fInfo.TotalMetadataBufferSize > m_MetaDataSize)
				{
					if (m_MetaDataBuffer)
					{
						delete[] m_MetaDataBuffer;
						m_MetaDataBuffer = nullptr;
					}
					m_MetaDataBuffer = new (std::nothrow) BYTE[fInfo.TotalMetadataBufferSize];
					if (!m_MetaDataBuffer)
					{
						m_MetaDataSize = 0;
						Data.MoveCount = 0;
						Data.DirtyCount = 0;
						return  cv::Mat(); //ProcessFailure(nullptr, L"Failed to allocate memory for metadata in DUPLICATIONMANAGER", L"Error", E_OUTOFMEMORY);
					}
					m_MetaDataSize = fInfo.TotalMetadataBufferSize;
				}

				UINT BufSize = fInfo.TotalMetadataBufferSize;
				DXGI_OUTDUPL_MOVE_RECT lool;
				
				// Get move rectangles
				hr = pDup->GetFrameMoveRects(BufSize, reinterpret_cast<DXGI_OUTDUPL_MOVE_RECT*>(m_MetaDataBuffer), &BufSize);
				if (FAILED(hr))
				{
					Data.MoveCount = 0;
					Data.DirtyCount = 0;
					return  cv::Mat();//ProcessFailure(nullptr, L"Failed to get frame move rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
				}
				Data.MoveCount = BufSize / sizeof(DXGI_OUTDUPL_MOVE_RECT);

				RECT* DirtyRects = reinterpret_cast<RECT*>(m_MetaDataBuffer + BufSize);
				BufSize = fInfo.TotalMetadataBufferSize - BufSize;

				// Get dirty rectangles
				hr = pDup->GetFrameDirtyRects(BufSize, reinterpret_cast<RECT*>(DirtyRects), &BufSize);
				if (FAILED(hr))
				{
					Data.MoveCount = 0;
					Data.DirtyCount = 0;
					return  cv::Mat(); //ProcessFailure(nullptr, L"Failed to get frame dirty rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
				}
				Data.DirtyCount = BufSize / sizeof(RECT);

				Data.MetaData = m_MetaDataBuffer;
			}
			if (fInfo.LastPresentTime.QuadPart)
			{
				break;
			}

			pDup->ReleaseFrame(); // TODO: check returned value here as well
		}
		if (m_restart)
			throw "lool";

		ID3D11Texture2D* m_AcquiredDesktopImage=nullptr;
		
		hr = pDR->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_AcquiredDesktopImage));
		
		m_Resources.Context->Unmap(lDestImage, 0);
		m_Resources.Context->CopyResource(lDestImage, m_AcquiredDesktopImage);
		hr = m_Resources.Context->Map(lDestImage, 0, D3D11_MAP_READ, 0, &mappedResource);


		m_AcquiredDesktopImage->Release();
		pDup->ReleaseFrame();
		
		return cv::Mat(m_OutputDesc.DesktopCoordinates.bottom - m_OutputDesc.DesktopCoordinates.top, m_OutputDesc.DesktopCoordinates.right - m_OutputDesc.DesktopCoordinates.left, CV_8UC4, mappedResource.pData, mappedResource.RowPitch);
		
	}

	virtual cv::Mat operator() () 
	{
		return w();
	}

private:
	std::vector<screenDef> screens;

	bool m_init;
};
#endif //WITH_WINAPI
#endif //SCREENSHOTWIN_H
