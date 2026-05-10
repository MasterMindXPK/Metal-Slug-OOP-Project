#pragma once

struct DeveloperMode {
	static bool devModeActive;
	DeveloperMode() {
		devModeActive = false;
	}
	static void toggle() {
		devModeActive = !devModeActive;
	}
	static bool isActive() {
		return devModeActive;
	}
	

};

bool DeveloperMode::devModeActive = true;