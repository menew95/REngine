﻿#pragma once

#include <common\common.h>

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x, msg)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, TEXT(msg), MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x, msg) (x)
#endif
#endif 