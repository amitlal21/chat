#include <iostream>
#include <string>

void process_keyboard_input_loop() {
	while(true) {
		std::cout << "> ";
		std::string message;
		std::getline(std::cin, message);
		std::cout << message << std::endl;
	}
}

int main(int, char**) {
	
	process_keyboard_input_loop();
	return 0;
}