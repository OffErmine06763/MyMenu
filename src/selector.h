#pragma once
#include <vector>
#include <string>
#include <windows.h>

class Selector {
public:
	/// <summary>
	/// Create menu that expects submenues
	/// </summary>
	/// <param name="options">Pairs of option title and a function that expects the current menu passed as parameter to update it</param>
	/// <param name="title">Menu title</param>
	Selector(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string title);
	/// <summary>
	/// Create menu that runs function on selection
	/// </summary>
	/// <param name="options">Pairs of option title and a function to execute on ENTER pressed</param>
	/// <param name="title">Menu title</param>
	Selector(std::vector<std::pair<std::string, void(*)()>>& options, std::string title);

	void execute();
	void updateOptions(std::vector<std::pair<std::string, void(*)()>>& options, std::string title);
	void updateOptions(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string title);

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
	static constexpr int DOWN = 80, UP = 72, ARROW = 224, ENTER = 13;
	int pos = 0;
	bool running = true, nextIsMenu = true;
	std::string lineEnd, title;
	std::vector<std::string> options;
	std::vector<void(*)(Selector&)> optionsMenus;
	std::vector<void(*)()> optionsFns;
};