//-------------------------------------------------------------------------------------
// DirectXTexP.h
//
// DirectX Texture Library - Private header
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248926
//-------------------------------------------------------------------------------------

#pragma once

#ifdef _MSC_VER
// Off by default warnings
#pragma warning(disable : 4619 4616 4061 4265 4365 4571 4623 4625 4626 4628 4668 4710 4711 4746 4774 4820 4987 5026 5027 5031 5032 5039 5045 5219 5246 5264 26812)
// C4619/4616 #pragma warning warnings
// C4061 enumerator 'X' in switch of enum 'X' is not explicitly handled by a case label
// C4265 class has virtual functions, but destructor is not virtual
// C4365 signed/unsigned mismatch
// C4571 behavior change
// C4623 default constructor was implicitly defined as deleted
// C4625 copy constructor was implicitly defined as deleted
// C4626 assignment operator was implicitly defined as deleted
// C4628 digraphs not supported
// C4668 not defined as a preprocessor macro
// C4710 function not inlined
// C4711 selected for automatic inline expansion
// C4746 volatile access of '<expression>' is subject to /volatile:<iso|ms> setting
// C4774 type string expected in argument 3 is not a string literal
// C4820 padding added after data member
// C4987 nonstandard extension used
// C5026 move constructor was implicitly defined as deleted
// C5027 move assignment operator was implicitly defined as deleted
// C5031/5032 push/pop mismatches in windows headers
// C5039 pointer or reference to potentially throwing function passed to extern C function under - EHc
// C5045 Spectre mitigation warning
// C5219 implicit conversion from 'int' to 'float', possible loss of data
// C5246 the initialization of a subobject should be wrapped in braces
// C5264 'const' variable is not used
// 26812: The enum type 'x' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).

#if defined(_XBOX_ONE) && defined(_TITLE)
// Xbox One XDK related Off by default warnings
#pragma warning(disable : 4471 4643 4917 4986 5029 5038 5040 5043 5204 5246 5256 5262 5267)
// C4471 forward declaration of an unscoped enumeration must have an underlying type
// C4643 Forward declaring in namespace std is not permitted by the C++ Standard
// C4917 a GUID can only be associated with a class, interface or namespace
// C4986 exception specification does not match previous declaration
// C5029 nonstandard extension used
// C5038 data member 'X' will be initialized after data member 'Y'
// C5040 dynamic exception specifications are valid only in C++14 and earlier; treating as noexcept(false)
// C5043 exception specification does not match previous declaration
// C5204 class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
// C5246 'anonymous struct or union': the initialization of a subobject should be wrapped in braces
// C5256 a non-defining declaration of an enumeration with a fixed underlying type is only permitted as a standalone declaration
// C5262 implicit fall-through occurs here; are you missing a break statement?
// C5267 definition of implicit copy constructor for 'X' is deprecated because it has a user-provided assignment operator
#endif // _XBOX_ONE && _TITLE
#endif // _MSC_VER

#ifdef __INTEL_COMPILER
#pragma warning(disable : 161)
// warning #161: unrecognized #pragma
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wc++98-compat-local-type-template-args"
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wtautological-type-limit-compare"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX 1
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#pragma warning(pop)

#include <Windows.h>

#ifdef __MINGW32__
#include <unknwn.h>
#endif

#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10 0x0A00
#endif

#ifdef _GAMING_XBOX_SCARLETT
#pragma warning(push)
#pragma warning(disable: 5204 5249)
#include <d3d12_xs.h>
#pragma warning(pop)
#elif defined(_GAMING_XBOX)
#pragma warning(push)
#pragma warning(disable: 5204)
#include <d3d12_x.h>
#pragma warning(pop)
#elif defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d12_x.h>
#include <d3d11_x.h>
#elif (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
#ifdef USING_DIRECTX_HEADERS
#include <directx/dxgiformat.h>
#include <directx/d3d12.h>
#else
#include <d3d12.h>
#endif
#include <d3d11_4.h>
#else
#include <d3d11_1.h>
#endif
#else // !WIN32
#include <wsl/winadapter.h>
#include <wsl/wrladapter.h>
#include <directx/d3d12.h>
#endif

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iterator>
#include <memory>
#include <new>
#include <tuple>

#ifndef _WIN32
#include <fstream>
#include <filesystem>
#include <thread>
#endif

#define _XM_NO_XMVECTOR_OVERLOADS_

#include <DirectXPackedVector.h>

#if (DIRECTX_MATH_VERSION < 315)
#define XM_ALIGNED_DATA(x) __declspec(align(x))
#endif

#include "DirectXTex.h"

#ifdef _WIN32
#include <malloc.h>

#if defined(NTDDI_WIN10_FE) || defined(__MINGW32__)
#include <ole2.h>
#else
#include <Ole2.h>
#endif
#include <wincodec.h>
#include <wrl\client.h>
#else
using WICPixelFormatGUID = GUID;
#endif

#include "scoped.h"

#define XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT DXGI_FORMAT(116)
#define XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT DXGI_FORMAT(117)
#define XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT DXGI_FORMAT(118)
#define XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS DXGI_FORMAT(119)
#define XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT DXGI_FORMAT(120)

#define WIN10_DXGI_FORMAT_P208 DXGI_FORMAT(130)
#define WIN10_DXGI_FORMAT_V208 DXGI_FORMAT(131)
#define WIN10_DXGI_FORMAT_V408 DXGI_FORMAT(132)

#ifndef XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM
#define XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM DXGI_FORMAT(189)
#endif

#define XBOX_DXGI_FORMAT_R4G4_UNORM DXGI_FORMAT(190)

#define WIN11_DXGI_FORMAT_A4B4G4R4_UNORM DXGI_FORMAT(191)

#if defined(__MINGW32__) && !defined(E_BOUNDS)
#define E_BOUNDS static_cast<HRESULT>(0x8000000BL)
#endif

// HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
#define HRESULT_ERROR_FILE_NOT_FOUND static_cast<HRESULT>(0x80070002)

// HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW)
#define HRESULT_E_ARITHMETIC_OVERFLOW static_cast<HRESULT>(0x80070216L)

// HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED)
#define HRESULT_E_NOT_SUPPORTED static_cast<HRESULT>(0x80070032L)

// HRESULT_FROM_WIN32(ERROR_HANDLE_EOF)
#define HRESULT_E_HANDLE_EOF static_cast<HRESULT>(0x80070026L)

// HRESULT_FROM_WIN32(ERROR_INVALID_DATA)
#define HRESULT_E_INVALID_DATA static_cast<HRESULT>(0x8007000DL)

// HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE)
#define HRESULT_E_FILE_TOO_LARGE static_cast<HRESULT>(0x800700DFL)

// HRESULT_FROM_WIN32(ERROR_CANNOT_MAKE)
#define HRESULT_E_CANNOT_MAKE static_cast<HRESULT>(0x80070052L)

// HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER static_cast<HRESULT>(0x8007007AL)
#endif

//-------------------------------------------------------------------------------------
namespace DirectX
{
    namespace Internal
    {
        //-----------------------------------------------------------------------------
        // WIC helper functions
    #ifdef _WIN32
        DXGI_FORMAT __cdecl WICToDXGI(_In_ const GUID& guid) noexcept;
        bool __cdecl DXGIToWIC(_In_ DXGI_FORMAT format, _Out_ GUID& guid, _In_ bool ignoreRGBvsBGR = false) noexcept;

        TEX_FILTER_FLAGS __cdecl CheckWICColorSpace(_In_ const GUID& sourceGUID, _In_ const GUID& targetGUID) noexcept;

        inline WICBitmapDitherType __cdecl GetWICDither(_In_ TEX_FILTER_FLAGS flags) noexcept
        {
            static_assert(TEX_FILTER_DITHER == 0x10000, "TEX_FILTER_DITHER* flag values don't match mask");

            static_assert(static_cast<int>(TEX_FILTER_DITHER) == static_cast<int>(WIC_FLAGS_DITHER), "TEX_FILTER_DITHER* should match WIC_FLAGS_DITHER*");
            static_assert(static_cast<int>(TEX_FILTER_DITHER_DIFFUSION) == static_cast<int>(WIC_FLAGS_DITHER_DIFFUSION), "TEX_FILTER_DITHER* should match WIC_FLAGS_DITHER*");

            switch (flags & TEX_FILTER_DITHER_MASK)
            {
            case TEX_FILTER_DITHER:
                return WICBitmapDitherTypeOrdered4x4;

            case TEX_FILTER_DITHER_DIFFUSION:
                return WICBitmapDitherTypeErrorDiffusion;

            default:
                return WICBitmapDitherTypeNone;
            }
        }

        inline WICBitmapDitherType __cdecl GetWICDither(_In_ WIC_FLAGS flags) noexcept
        {
            switch (flags & TEX_FILTER_DITHER_MASK)
            {
            case WIC_FLAGS_DITHER:
                return WICBitmapDitherTypeOrdered4x4;

            case WIC_FLAGS_DITHER_DIFFUSION:
                return WICBitmapDitherTypeErrorDiffusion;

            default:
                return WICBitmapDitherTypeNone;
            }
        }

        inline WICBitmapInterpolationMode __cdecl GetWICInterp(_In_ WIC_FLAGS flags) noexcept
        {
            static_assert(TEX_FILTER_POINT == 0x100000, "TEX_FILTER_ flag values don't match TEX_FILTER_MASK");

            static_assert(static_cast<int>(TEX_FILTER_POINT) == static_cast<int>(WIC_FLAGS_FILTER_POINT), "TEX_FILTER_* flags should match WIC_FLAGS_FILTER_*");
            static_assert(static_cast<int>(TEX_FILTER_LINEAR) == static_cast<int>(WIC_FLAGS_FILTER_LINEAR), "TEX_FILTER_* flags should match WIC_FLAGS_FILTER_*");
            static_assert(static_cast<int>(TEX_FILTER_CUBIC) == static_cast<int>(WIC_FLAGS_FILTER_CUBIC), "TEX_FILTER_* flags should match WIC_FLAGS_FILTER_*");
            static_assert(static_cast<int>(TEX_FILTER_FANT) == static_cast<int>(WIC_FLAGS_FILTER_FANT), "TEX_FILTER_* flags should match WIC_FLAGS_FILTER_*");

            switch (flags & TEX_FILTER_MODE_MASK)
            {
            case TEX_FILTER_POINT:
                return WICBitmapInterpolationModeNearestNeighbor;

            case TEX_FILTER_LINEAR:
                return WICBitmapInterpolationModeLinear;

            case TEX_FILTER_CUBIC:
                return WICBitmapInterpolationModeCubic;

            case TEX_FILTER_FANT:
            default:
                return WICBitmapInterpolationModeFant;
            }
        }

        inline WICBitmapInterpolationMode __cdecl GetWICInterp(_In_ TEX_FILTER_FLAGS flags) noexcept
        {
            switch (flags & TEX_FILTER_MODE_MASK)
            {
            case TEX_FILTER_POINT:
                return WICBitmapInterpolationModeNearestNeighbor;

            case TEX_FILTER_LINEAR:
                return WICBitmapInterpolationModeLinear;

            case TEX_FILTER_CUBIC:
                return WICBitmapInterpolationModeCubic;

            case TEX_FILTER_FANT:
            default:
                return WICBitmapInterpolationModeFant;
            }
        }
    #endif // WIN32

        //---------------------------------------------------------------------------------
        // Image helper functions
        HRESULT __cdecl DetermineImageArray(
            _In_ const TexMetadata& metadata, _In_ CP_FLAGS cpFlags,
            _Out_ size_t& nImages, _Out_ size_t& pixelSize) noexcept;

        _Success_(return) bool __cdecl SetupImageArray(
            _In_reads_bytes_(pixelSize) uint8_t* pMemory, _In_ size_t pixelSize,
            _In_ const TexMetadata& metadata, _In_ CP_FLAGS cpFlags,
            _Out_writes_(nImages) Image* images, _In_ size_t nImages) noexcept;

        //---------------------------------------------------------------------------------
        // Conversion helper functions

        enum TEXP_SCANLINE_FLAGS : uint32_t
        {
            TEXP_SCANLINE_NONE = 0,

            TEXP_SCANLINE_SETALPHA = 0x1,
            // Set alpha channel to known opaque value

            TEXP_SCANLINE_LEGACY = 0x2,
            // Enables specific legacy type conversion cases
        };

        enum CONVERT_FLAGS : uint32_t
        {
            CONVF_FLOAT = 0x1,
            CONVF_UNORM = 0x2,
            CONVF_UINT = 0x4,
            CONVF_SNORM = 0x8,
            CONVF_SINT = 0x10,
            CONVF_DEPTH = 0x20,
            CONVF_STENCIL = 0x40,
            CONVF_SHAREDEXP = 0x80,
            CONVF_BGR = 0x100,
            CONVF_XR = 0x200,
            CONVF_PACKED = 0x400,
            CONVF_BC = 0x800,
            CONVF_YUV = 0x1000,
            CONVF_POS_ONLY = 0x2000,
            CONVF_R = 0x10000,
            CONVF_G = 0x20000,
            CONVF_B = 0x40000,
            CONVF_A = 0x80000,
            CONVF_RGB_MASK = 0x70000,
            CONVF_RGBA_MASK = 0xF0000,
        };

        uint32_t __cdecl GetConvertFlags(_In_ DXGI_FORMAT format) noexcept;

        void __cdecl CopyScanline(
            _When_(pDestination == pSource, _Inout_updates_bytes_(outSize))
            _When_(pDestination != pSource, _Out_writes_bytes_(outSize))
            void* pDestination, _In_ size_t outSize,
            _In_reads_bytes_(inSize) const void* pSource, _In_ size_t inSize,
            _In_ DXGI_FORMAT format, _In_ uint32_t tflags) noexcept;

        void __cdecl SwizzleScanline(
            _When_(pDestination == pSource, _In_)
            _When_(pDestination != pSource, _Out_writes_bytes_(outSize))
            void* pDestination, _In_ size_t outSize,
            _In_reads_bytes_(inSize) const void* pSource, _In_ size_t inSize,
            _In_ DXGI_FORMAT format, _In_ uint32_t tflags) noexcept;

        _Success_(return) bool __cdecl ExpandScanline(
            _Out_writes_bytes_(outSize) void* pDestination, _In_ size_t outSize,
            _In_ DXGI_FORMAT outFormat,
            _In_reads_bytes_(inSize) const void* pSource, _In_ size_t inSize,
            _In_ DXGI_FORMAT inFormat, _In_ uint32_t tflags) noexcept;

        _Success_(return) bool __cdecl LoadScanline(
            _Out_writes_(count) XMVECTOR* pDestination, _In_ size_t count,
            _In_reads_bytes_(size) const void* pSource, _In_ size_t size,
            _In_ DXGI_FORMAT format) noexcept;

        _Success_(return) bool __cdecl LoadScanlineLinear(
            _Out_writes_(count) XMVECTOR* pDestination, _In_ size_t count,
            _In_reads_bytes_(size) const void* pSource, _In_ size_t size,
            _In_ DXGI_FORMAT format, _In_ TEX_FILTER_FLAGS flags) noexcept;

        _Success_(return) bool __cdecl StoreScanline(
            _Out_writes_bytes_(size) void* pDestination, _In_ size_t size, _In_ DXGI_FORMAT format,
            _In_reads_(count) const XMVECTOR* pSource, _In_ size_t count, _In_ float threshold = 0) noexcept;

        _Success_(return) bool __cdecl StoreScanlineLinear(
            _Out_writes_bytes_(size) void* pDestination, _In_ size_t size, _In_ DXGI_FORMAT format,
            _Inout_updates_all_(count) XMVECTOR* pSource, _In_ size_t count,
            _In_ TEX_FILTER_FLAGS flags, _In_ float threshold = 0) noexcept;

        _Success_(return) bool __cdecl StoreScanlineDither(
            _Out_writes_bytes_(size) void* pDestination, _In_ size_t size, _In_ DXGI_FORMAT format,
            _Inout_updates_all_(count) XMVECTOR* pSource, _In_ size_t count,
            _In_ float threshold, size_t y, size_t z,
            _Inout_updates_all_opt_(count + 2) XMVECTOR* pDiffusionErrors) noexcept;

        HRESULT __cdecl ConvertToR32G32B32A32(_In_ const Image& srcImage, _Inout_ ScratchImage& image) noexcept;

        HRESULT __cdecl ConvertFromR32G32B32A32(_In_ const Image& srcImage, _In_ const Image& destImage) noexcept;
        HRESULT __cdecl ConvertFromR32G32B32A32(
            _In_ const Image& srcImage, _In_ DXGI_FORMAT format, _Inout_ ScratchImage& image) noexcept;
        HRESULT __cdecl ConvertFromR32G32B32A32(
            _In_reads_(nimages) const Image* srcImages, _In_ size_t nimages, _In_ const TexMetadata& metadata,
            _In_ DXGI_FORMAT format, _Out_ ScratchImage& result) noexcept;

        HRESULT __cdecl ConvertToR16G16B16A16(_In_ const Image& srcImage, _Inout_ ScratchImage& image) noexcept;

        HRESULT __cdecl ConvertFromR16G16B16A16(_In_ const Image& srcImage, _In_ const Image& destImage) noexcept;

        void __cdecl ConvertScanline(
            _Inout_updates_all_(count) XMVECTOR* pBuffer, _In_ size_t count,
            _In_ DXGI_FORMAT outFormat, _In_ DXGI_FORMAT inFormat, _In_ TEX_FILTER_FLAGS flags) noexcept;

        //---------------------------------------------------------------------------------
        // Misc helper functions
        bool __cdecl IsAlphaAllOpaqueBC(_In_ const Image& cImage) noexcept;
        bool __cdecl CalculateMipLevels(_In_ size_t width, _In_ size_t height, _Inout_ size_t& mipLevels) noexcept;
        bool __cdecl CalculateMipLevels3D(_In_ size_t width, _In_ size_t height, _In_ size_t depth,
            _Inout_ size_t& mipLevels) noexcept;

    #ifdef _WIN32
        HRESULT __cdecl ResizeSeparateColorAndAlpha(_In_ IWICImagingFactory* pWIC,
            _In_ bool iswic2,
            _In_ IWICBitmap* original,
            _In_ size_t newWidth, _In_ size_t newHeight, _In_ TEX_FILTER_FLAGS filter,
            _Inout_ const Image* img) noexcept;
    #endif

    } // namespace Internal
} // namespace DirectX
