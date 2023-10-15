#include"Coniguration.h"




void SetConfigDefault(ConfigSetup* config) {
	if (!config) { return; }

	config->ArgandInputType = ArgandConfig::ABI;

	return;
}