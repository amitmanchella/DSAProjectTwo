#pragma once
#include "tree.hpp"
#include "file_size_max_heap.hpp"
namespace cs251
{
	enum class node_type
	{
		Directory,
		File,
		Link
	};
	struct filesystem_node_data
	{
		/**
		 * The type of the node.
		 */
		node_type m_type = node_type::Directory;
		/**
		 * The handle of linked target.
		 */
		handle m_linkedHandle = -1;
		/**
		 * The name of the node.
		 */
		std::string m_name = {};
		/**
		 * The size of the node, only useful when the node is a file.
		 */
		size_t m_fileSize = 0;
	};

	// Custom exceptions - throw these where appropriate
	class invalid_path : public std::runtime_error {
		public: invalid_path() : std::runtime_error("Invalid path!") {} };
	class invalid_name : public std::runtime_error {
		public: invalid_name() : std::runtime_error("Invalid name!") {} };
	class exceeds_size : public std::runtime_error {
		public: exceeds_size() : std::runtime_error("Exceeds file system size limit!") {} };
	class file_exists : public std::runtime_error {
		public: file_exists() : std::runtime_error("File already exists!") {} };
	class directory_exists : public std::runtime_error {
		public: directory_exists() : std::runtime_error("Directory already exists!") {} };
	class link_exists : public std::runtime_error {
		public: link_exists() : std::runtime_error("Link already exists!") {} };
	class name_exists : public std::runtime_error {
		public: name_exists() : std::runtime_error("Name already used!") {} };


	class filesystem {
	public:


        explicit filesystem(size_t sizeLimit);


            /**
             * \brief Create a new file under the root directory.
             * \param fileSize The size of the file.
             * \param fileName The name of the new file.
             * \return The handle of the file.
             */
		handle create_file(size_t fileSize, const std::string& fileName);

		/**
		 * \brief Create a new directory under the root directory.
		 * \param directoryName The name of the new directory.
		 * \return The handle of the directory.
		 */
		handle create_directory(const std::string& directoryName);

		/**
		 * \brief Create a new link under the root directory.
		 * \param targetHandle The handle of the target, can be a file, a directory, or a link.
		 * \param linkName The name of the new link.
		 * \return The handle of the link.
		 */
		handle create_link(handle targetHandle, const std::string& linkName);

		/**
		 * \brief Delete the file, directory, or link. If the target is a directory, remove only if it's empty. Links are not followed.
		 * \param targetHandle The handle of the target, can be a file, a directory, or a link.
		 * \return If the target is removed.
		 */
		bool remove(handle targetHandle);

		/**
		 * \brief Create a new file under the target directory.
		 * \param fileSize The size of the file.
		 * \param fileName The name of the new file.
		 * \param parentHandle The parent directory handle. The handle may also be a link to a directory.
		 * \return The handle of the file.
		 */
		handle create_file(size_t fileSize, const std::string& fileName, handle parentHandle);

		/**
		 * \brief Create a new directory under the target directory.
		 * \param directoryName The name of the new directory.
		 * \param parentHandle The parent directory handle. The handle may also be a link to a directory.
		 * \return The handle of the directory.
		 */
		handle create_directory(const std::string& directoryName, handle parentHandle);

		/**
		 * \brief Create a new link under the root directory.
		 * \param targetHandle The handle of the target, can be a file, a directory, or a link.
		 * \param linkName The name of the new link.
		 * \param parentHandle The parent directory handle. The handle may also be a link to a directory.
		 * \return The handle of the directory.
		 */
		handle create_link(handle targetHandle, const std::string& linkName, handle parentHandle);

		/**
		 * \brief Get the absolute path of the target. Links are not followed.
		 * \param targetHandle The handle of the target, can be a file, a directory, or a link.
		 * \return The absolute path as string.
		 */
		std::string get_absolute_path(handle targetHandle);

		/**
		 * \brief Get the name of the target by handle.
		 * \param targetHandle The handle of the target, can be a file, a link, or a directory.
		 * \return The name of the target.
		 */
		std::string get_name(handle targetHandle);

		/**
		 * \brief Get the size of the file by handle.
		 * \param targetHandle The handle of the target, can be a file, or a link to the file.
		 * \return The size of the target file.
		 */
		size_t get_file_size(handle targetHandle);
		/**
		 * \brief Get the size of the file by absolute path.
		 * \param absolutePath The absolute path to the file, or a link to the file.
		 * \return The size of the target file.
		 */
		size_t get_file_size(const std::string& absolutePath);

		/**
		 * \brief Change the name of a target.
		 * \param targetHandle The handle of the target to be renamed, can be a file, a directory, or a link.
		 * \param newName The new name of the target.
		 */
		void rename(handle targetHandle, const std::string& newName);

		/**
		 * \brief Check if the target exists. (If it's allocated and not yet deleted)
		 * \param targetHandle The handle of the target, can be a file, a directory, or a link.
		 * \return If the target exists.
		 */
		bool exist(handle targetHandle);

		/**
		 * \brief Get the file handle by its absolute path.
		 * \param absolutePath The absolute path of the target, can be a file, a directory, or a link.
		 * \return The handle to the target.
		 */
		handle get_handle(const std::string& absolutePath);

		/**
		 * \brief Get the handle of the real directory or file, following the links.
		 * \param targetHandle The handle of the target, can be a link to a directory, or a file, or just a file, or just a directory.
		 * \return The handle of the real directory or file.
		 */
		handle follow(handle targetHandle);

		/**
		 * \brief Get the layout of the file hierarchies.
		 * \return The layout as string.
		 */
		std::string print_layout();

		/**
		 * \brief Get the handle of the largest file.
		 * \return The handle to the largest file.
		 */
		handle get_largest_file_handle() const;

		/**
		 * \brief Get the available size of the file system.
		 * \return The available size of the file.
		 */
		size_t get_available_size() const;

        //bool containsSlash(const std::string& fileName);
        //std::string build_path(handle targetHandle) const;
        std::vector<std::string> split_path(const std::string& path);



            private:
		//TODO: You may add private members/methods here.

        size_t theSize;
        size_t totalUsedSize = 0;
        void print_traverse(size_t level, std::stringstream& ss, handle targetHandle);
		/**
		 * The tree instance that hold the filesystem's data.
		 */
		tree<filesystem_node_data> m_fileSystemNodes{};
		/**
		 * The maxheap that keep track of the largest file.
		 */
		file_size_max_heap m_fileSizeMaxHeap{};
	};
}
