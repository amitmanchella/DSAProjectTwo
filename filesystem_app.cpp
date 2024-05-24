#include "filesystem.hpp"

#include "iostream"
#include "cstdlib"
using namespace cs251;
/*
The code is provided to be built as an executable for grading.
You can modify the code based on your needs, but the original copy of this file will be used for testing.
*/
int main()
{
	try {
		std::string args;
		std::getline(std::cin, args);
		const size_t fileSystemSize = std::atoi(args.c_str());
	
		filesystem fs{ fileSystemSize };
		while (true)
		{
			std::string input;
			std::getline(std::cin, input);
			std::cout << input << std::endl;
			try {
				if (input == "quit") {
					break;
				}
				std::string text;
				if (input == "create_file")
				{
					std::getline(std::cin, text);
					const auto fileSize = std::atoi(text.c_str());
					std::getline(std::cin, text);
					const auto fileName = text;
					std::getline(std::cin, text);
					const auto parentHandle = std::atoi(text.c_str());
					std::cout << fs.create_file(fileSize, fileName, parentHandle) << std::endl;
				}
				else if (input == "create_file_root")
				{
					std::getline(std::cin, text);
					const auto fileSize = std::atoi(text.c_str());
					std::getline(std::cin, text);
					const auto fileName = text;
					std::cout << fs.create_file(fileSize, fileName) << std::endl;
				}
				else if (input == "create_directory")
				{
					std::getline(std::cin, text);
					const auto directoryName = text;
					std::getline(std::cin, text);
					const auto parentHandle = std::atoi(text.c_str());
					std::cout << fs.create_directory(directoryName, parentHandle) << std::endl;
				}
				else if (input == "create_directory_root")
				{
					std::getline(std::cin, text);
					std::cout << fs.create_directory(text) << std::endl;
				}
				else if (input == "create_link")
				{
					std::getline(std::cin, text);
					const auto targetHandle = std::atoi(text.c_str());
					std::getline(std::cin, text);
					const auto linkName = text;
					std::getline(std::cin, text);
					const auto parentHandle = std::atoi(text.c_str());
					std::cout << fs.create_link(targetHandle, linkName, parentHandle) << std::endl;
				}
				else if (input == "create_link_root")
				{
					std::getline(std::cin, text);
					const auto targetHandle = std::atoi(text.c_str());
					std::getline(std::cin, text);
					const auto linkName = text;
					std::cout << fs.create_link(targetHandle, linkName) << std::endl;
				}
				else if (input == "remove")
				{
					std::getline(std::cin, text);
					fs.remove(std::atoi(text.c_str()));
				}
				else if (input == "get_absolute_path")
				{
					std::getline(std::cin, text);
					std::cout << fs.get_absolute_path(std::atoi(text.c_str())) << std::endl;
				}
				else if (input == "get_name")
				{
					std::getline(std::cin, text);
					std::cout << fs.get_name(std::atoi(text.c_str())) << std::endl;
				}
				else if (input == "get_file_size")
				{
					std::getline(std::cin, text);
					std::cout << fs.get_file_size(std::atoi(text.c_str())) << std::endl;
				}
				else if (input == "get_file_size_path")
				{
					std::getline(std::cin, text);
					std::cout << fs.get_file_size(text) << std::endl;
				}
				else if (input == "rename")
				{
					std::getline(std::cin, text);
					const auto targetHandle = std::atoi(text.c_str());
					std::getline(std::cin, text);
					const auto newName = text;
					fs.rename(targetHandle, newName);
				}
				else if (input == "exist")
				{
					std::getline(std::cin, text);
					std::cout << fs.exist(std::atoi(text.c_str())) << std::endl;
				}
				else if (input == "get_handle")
				{
					std::getline(std::cin, text);
					std::cout << fs.get_handle(text) << std::endl;
				}
				else if (input == "follow")
				{
					std::getline(std::cin, text);
					std::cout << fs.follow(std::atoi(text.c_str())) << std::endl;
				}
				else if (input == "print_layout")
				{
					std::cout << fs.print_layout() << std::endl;
				}
				else if (input == "get_largest_file_handle")
				{
					std::cout << fs.get_largest_file_handle() << std::endl;
				}
				else if (input == "get_available_size")
				{
					std::cout << fs.get_available_size() << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	} catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << std::endl;
    return 1;
	}
	return 0;
}
