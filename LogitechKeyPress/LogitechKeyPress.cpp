#include "pch.h"
#include "LogitechLEDLib.h"

using namespace std;

struct FullColorCfg {
	unsigned int Key;
	unsigned int Red;
	unsigned int Green;
	unsigned int Blue;
};
 
static vector<FullColorCfg>& GetFullColorCfgVector() {
	static vector<FullColorCfg> vec;
	return vec;
}

void ReadSettingsFile() {
	ifstream stream("LEDColorKeyPress.dat");
	for (string line; getline(stream, line); ) {
		if (line[0] != ';' && line[0] != '#') {

			if (!line.compare("keys_full_color")) {
				while (getline(stream, line) && line.compare("end")) {
					if (line[0] != ';' && line[0] != '#') {
						FullColorCfg entry;
						if (sscanf_s(line.c_str(), "%d %d %d %d", &entry.Key, &entry.Red, &entry.Green, &entry.Blue) == 4)
						{
							std::cout << "Added key " << entry.Key << " RGB " << entry.Red << " " << entry.Green << " " << entry.Blue << "\n";
							GetFullColorCfgVector().push_back(entry);
						}
					}
				}
			}

		}
	}
}

int main()
{
	LogiLedInitWithName("LEDColorKeyPress");
	ReadSettingsFile();

	while (true)
	{
		bool pressed = false;
		for (unsigned int i = 0; i < GetFullColorCfgVector().size(); i++) {
			if (GetFullColorCfgVector()[i].Key > 0 && GetKeyState(GetFullColorCfgVector()[i].Key) & 0x8000) {
				LogiLedSetLighting(GetFullColorCfgVector()[i].Red, GetFullColorCfgVector()[i].Green, GetFullColorCfgVector()[i].Blue);
				pressed = true;
				break;
			}
		}
		if (!pressed) {
			if (GetFullColorCfgVector()[0].Key == 0)
			{
				LogiLedSetLighting(GetFullColorCfgVector()[0].Red, GetFullColorCfgVector()[0].Green, GetFullColorCfgVector()[0].Blue);
			}
			else
			{
				LogiLedSetLighting(0, 0, 0); //off
			}
		}
	}
}