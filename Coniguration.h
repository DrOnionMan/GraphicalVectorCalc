#pragma once
#include<windows.h>



namespace ArgandConfig {
	enum UserInput {
		ARGAND_CONVERTER = 4,
		ARGAND_DRAWER,
		NULLCASE = -1
	};

	enum UserInputType {
		REI = 1,
		ABI,
		MOD_ARG
	};
}


struct ConfigSetup {
	ArgandConfig::UserInputType ArgandInputType;


	void (*fpDefault)(ConfigSetup* config);
};


void SetConfigDefault(ConfigSetup* config);

