#pragma once
#include "cstring"
#include "sstream"
#include "string"
#include "exception"
#include "vector"
#include "queue"
#include "tree.hpp"

namespace cs251
{
	class heap_empty : public std::runtime_error {
		public: heap_empty() : std::runtime_error("Heap is empty!") {} };

	struct file_size_max_heap_node
	{
		/**
		 * The handle of the file being tracked.
		 */
		handle m_handle = -1;
		/**
		 * The size of the target file, used for sorting.
		 */
		size_t m_value = 0;
	};

	class file_size_max_heap {
	public:
		/**
		 * \brief Register a new file.
		 * \param fileSize The size of the file.
		 * \param handle The handle of the file.
		 */
		void push(size_t fileSize, handle handle);

		/**
		 * \brief Get the handle of the file with maximum size.
		 * \return The handle of the file.
		 */
		handle top() const;

		/**
		 * \brief Unregister the file by its handle.
		 * \param handle The handle of the file to be removed.
		 */
		void remove(handle handle);


            private:

        //TODO: You may add private members/methods here.

		/**
		 * The amount of nodes of the heap.
		 */
		size_t m_nodeSize = 0;

		/**
		 * List to store all the nodes.
		 */
		std::vector<file_size_max_heap_node> m_nodes{};

        void heapify_down(const handle index);

        };

	
}
