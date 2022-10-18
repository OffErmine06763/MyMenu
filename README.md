# MyMenu

Usage:

TO EXECUTE ACTION:

	std::vector<std::pair<std::string, void (*)()>> options = {
		{"Option A", functionA},
		{"Option B", functionB},
		{"Option C", functionC},
  	};

	(Selector(options, "Title")).execute();

where funcionA, B and C have the following signature:

	void functionA();

that will be called by the menu on ENTER pressed.

TO MOVE TO ANOTHER MENU:
  
	std::vector<std::pair<std::string, void (*)(Selector&)>> options = {
		{"Option A", functionA},
		{"Option B", functionB},
		{"Option C", functionC},
	};

	(Selector(options, "Title")).execute();
  
where funtionA, B and C have the following signature:

	void functionA(Selector& selector);
  
that will be called by the menu on ENTER pressed and pass itself.
See UPDATE MENU section.

TO UPDATE MENU:
	
	selector.updateOptions(newOptions, "New Title");
