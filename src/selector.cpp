#include <iostream>
#include <chrono>
#include <conio.h>
#include <thread>
#include "selector.h"

Selector::Selector(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string title) {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, WHITE);

	this->options.reserve(options.size());
	this->optionsMenus.reserve(options.size());
	for (auto& op : options) {
		this->options.push_back(op.first);
		this->optionsMenus.push_back(op.second);
	}
	this->title = title;
	nextIsMenu = true;

	int maxLen = 0;
	for (auto& s : options) {
		if (s.first.length() > maxLen)
			maxLen = s.first.length();
	}
	lineEnd = std::string(maxLen, ' ');
	lineEnd.push_back('\r');
}
Selector::Selector(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string title) {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, WHITE);

	this->options.reserve(options.size());
	this->optionsMenus.reserve(options.size());
	for (auto& op : options) {
		this->options.push_back(op.first);
		this->optionsMenus.push_back(op.second);
	}
	this->title = title;
	nextIsMenu = false;

	int maxLen = 0;
	for (auto& s : options) {
		if (s.first.length() > maxLen)
			maxLen = s.first.length();
	}
	lineEnd = std::string(maxLen, ' ');
	lineEnd.push_back('\r');
}

void Selector::updateOptions(std::vector<std::pair<std::string, void(*)()>>& options, std::string title) {
	this->options.clear();
	this->optionsFns.clear();
	this->optionsMenus.clear();
	this->options.reserve(options.size());
	this->optionsFns.reserve(options.size());
	for (auto& op : options) {
		this->options.push_back(op.first);
		this->optionsFns.push_back(op.second);
	}
	this->title = title;
	nextIsMenu = false;

	int maxLen = 0;
	for (auto& s : options) {
		if (s.first.length() > maxLen)
			maxLen = s.first.length();
	}
	lineEnd = std::string(maxLen, ' ');
	lineEnd.push_back('\r');
	running = true;
	std::cout << title << '\n';
	printSets();
}
void Selector::updateOptions(std::vector<std::pair<std::string, void(*)(Selector&)>>& options, std::string title) {
	this->options.clear();
	this->optionsFns.clear();
	this->optionsMenus.clear();
	this->options.reserve(options.size());
	this->optionsMenus.reserve(options.size());
	for (auto& op : options) {
		this->options.push_back(op.first);
		this->optionsMenus.push_back(op.second);
	}
	this->title = title;
	nextIsMenu = true;

	int maxLen = 0;
	for (auto& s : options) {
		if (s.first.length() > maxLen)
			maxLen = s.first.length();
	}
	lineEnd = std::string(maxLen, ' ');
	lineEnd.push_back('\r');
	running = true;
	std::cout << title << '\n';
	printSets();
}


void Selector::execute() {
	std::cout << title << '\n';
	printSets();

	while (running) {
		if (_kbhit()) {
			int input = _getch();

			if (input == ARROW) {
				input = _getch();
				if (input == DOWN)
					moveDown();
				else if (input == UP)
					moveUp();
			}
			else if (input == 'W' || input == 'w') {
				moveUp();
			}
			else if (input == 'S' || input == 's') {
				moveDown();
			}
			else if (input == 13) {
				SetConsoleTextAttribute(hConsole, WHITE);
				executeSelection();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void Selector::moveDown(bool force /*= false*/) {
	if (!force && pos == options.size() - 1) return;

	printSelectedSet(WHITE);
	pos++;
	std::cout << '\n';
	printSelectedSet(GREEN);
}
void Selector::moveUp(bool force /*= false*/) {
	if (!force && pos == 0) return;

	printSelectedSet(WHITE);
	pos--;
	std::cout << "\x1b[A";
	printSelectedSet(GREEN);
}

void Selector::move(int count) {
	if (count == 0) return;

	if (count < 0) {
		count = -std::min<int>(pos, -count);
		if (count == 0) return;
		printSelectedSet(WHITE);
		std::cout << "\x1b[" << -count << "A"; // up
	}
	else {
		count = (count + pos >= options.size() ? options.size() - pos - 1 : count);
		printSelectedSet(WHITE);
		for (int i = 0; i < count; i++)
			std::cout << '\n'; // down
	}
	pos += count;
	printSelectedSet(GREEN);
}

void Selector::executeSelection() {
	if (pos < 0 || pos >= options.size()) {
		moveDown(true);
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << "Selection out of bounds\n";
		SetConsoleTextAttribute(hConsole, WHITE);
		return;
	}

	running = false;
	system("cls");
	if (nextIsMenu)
		optionsMenus[pos](*this);
	else
		optionsFns[pos]();
}

void Selector::printSets() {
	for (auto& s : options)
		std::cout << '>' << s << '\n';
	pos = options.size();
	move(-static_cast<int>(options.size()));
}

void Selector::printSelectedSet(uint8_t color) {
	if (pos >= options.size() || pos < 0) return;

	SetConsoleTextAttribute(hConsole, color);
	std::cout << '>' << options[pos] << lineEnd;
}