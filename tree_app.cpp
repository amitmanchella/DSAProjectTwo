#include <iostream>
#include "tree.hpp"
using namespace cs251;

/*
* The code is provided to be build as an executable for grading.
* You can modify the code based on your needs, but the original copy of this file will be used for testing.
*/
template <typename T> void run_test();
template <typename T> T parse_item(std::string itemstr);

int main() {
	try {
		std::string typestr;
		std::getline(std::cin, typestr);
		if (typestr == "int")
			run_test<int>();
		else if (typestr == "float")
			run_test<float>();
		else if (typestr == "string")
			run_test<std::string>();
	} catch (const std::exception& e) {
		std::cerr << "Unhandled exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

template <typename T> void run_test() {
	tree<T> t;
	
	while (true) {
		std::string command;
		std::getline(std::cin, command);
		std::cout << command << std::endl;
		try {
			if (command == "allocate") {
				std::string data_str;
				std::getline(std::cin, data_str);
				T data = parse_item<T>(data_str);

				std::string parent_str;
				std::getline(std::cin, parent_str);
				size_t parent_id = std::atoi(parent_str.c_str());

				handle id = t.allocate(parent_id);
				std::cout << id << std::endl;

				auto& node = t.ref_node(id);
				node.ref_data() = data;

			} else if (command == "remove") {
				std::string node_str;
				std::getline(std::cin, node_str);
				size_t node_id = std::atoi(node_str.c_str());

				t.remove(node_id);

			} else if (command == "set_parent") {
				std::string node_str;
				std::getline(std::cin, node_str);
				size_t node_id = std::atoi(node_str.c_str());

				std::string parent_str;
				std::getline(std::cin, parent_str);
				size_t parent_id = std::atoi(parent_str.c_str());

				t.set_parent(node_id, parent_id);

			} else if (command == "peek_nodes") {
				const auto& nodes = t.peek_nodes();
				for (const auto& node : nodes) {
					std::cout << node.get_handle() << ": ";
					if (node.is_recycled()) {
						std::cout << "[recycled]";
					} else {
						// HORRIBLE DANGEROUS CAST TO NON-CONST REF BELOW!!
						// Never actually do this please
						std::cout << ((tree_node<T>&)node).ref_data();
					}
					std::cout << std::endl;
				}

			} else if (command == "ref_node") {
				std::string node_str;
				std::getline(std::cin, node_str);
				size_t node_id = std::atoi(node_str.c_str());

				auto& node = t.ref_node(node_id);

				std::cout << node.ref_data() << std::endl;
				std::cout << "parent: " << node.get_parent_handle() << std::endl;
				std::cout << "children: ";
				for (auto child_id : node.peek_children_handles()) {
					std::cout << child_id << " ";
				}
				std::cout << std::endl;

			} else if (command == "quit") {
				break;
			}
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

template<> int parse_item<int>(std::string itemstr) {
	return std::atoi(itemstr.c_str());
}
template<> float parse_item<float>(std::string itemstr) {
	return std::atof(itemstr.c_str());
}
template<> std::string parse_item<std::string>(std::string itemstr) {
	return itemstr;
}
