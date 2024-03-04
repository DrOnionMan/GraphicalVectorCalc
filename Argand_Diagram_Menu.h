#pragma once

#include"Complex.h"
#include"children.h"
#include"MenuMacroDef.h"
#include<string>
#include<vector>



namespace Argand {

	void SetupDraw(HWND Parent, std::vector<children>*__restrict children);

	char* GetStringFromEdit(children* Children);

	void SetupConverter(HWND Parent,std::vector<children>* __restrict children);

	
	void DisplayConverterResult(complex& complex, std::vector<children>* __restrict Children, HWND Parent);
	
	RECT EmplaceEdit(std::vector<children>* __restrict Children);


	
	void AddGeometryBox(HWND Parent, std::vector<children>* __restrict children);
	
}

namespace Mats {
	void SetupMats(HWND Parent, std::vector<children> * __restrict children);
}


