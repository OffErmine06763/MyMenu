#pragma once
#include <vector>
#include <string>
#include <windows.h>

class Selector {
public:
	using options_t = std::vector<std::pair<std::string, void(*)(Selector&)>>;

public:
	Selector(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string message);

	void execute();
	void updateOptions(std::vector<std::pair<std::string, void(*)()>>& options, std::string message);
	void updateOptions(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string message);

private:
	void moveDown(bool force = false);
	void moveUp(bool force = false);
	void move(int count);

	void executeSelection();
	
	void printSets();
	void printSelectedSet(uint8_t color);

private:
	HANDLE hConsole;
	static constexpr uint8_t RED = 12, GREEN = 10, WHITE = 7;
	static constexpr int DOWN = 80, UP = 72, ARROW = 224;
	int pos = 0;
	bool running = true, nextIsMenu = true;
	std::string lineEnd, message;
	std::vector<std::string> options;
	std::vector<void(*)(Selector&)> optionsMenus;
	std::vector<void(*)()> optionsFns;
};