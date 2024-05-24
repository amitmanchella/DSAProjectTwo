#include "filesystem.hpp"

#include <iostream>

using namespace cs251;

filesystem::filesystem(const size_t sizeLimit)
{
    theSize = sizeLimit;
}

handle filesystem::create_file(const size_t fileSize, const std::string& fileName)
{
    return create_file(fileSize, fileName, 0); // Pass 0 as parentHandle, indicating the root
}

handle filesystem::create_directory(const std::string& directoryName)
{
    return create_directory(directoryName, 0);
}

handle filesystem::create_link(const handle targetHandle, const std::string& linkName)
{
    return create_link(targetHandle, linkName, 0);
}

bool filesystem::remove(const handle targetHandle) {
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(targetHandle).ref_data().m_type == node_type::File) {
        m_fileSizeMaxHeap.remove(targetHandle);
    }

    if ((m_fileSystemNodes.ref_node(targetHandle).ref_data().m_type == node_type::Directory) &&
        !(m_fileSystemNodes.ref_node(targetHandle).peek_children_handles().empty())) {
        return false;
    }
    if ((m_fileSystemNodes.ref_node(targetHandle).ref_data().m_type == node_type::Directory) &&
        (m_fileSystemNodes.ref_node(targetHandle).peek_children_handles().empty())) {
        totalUsedSize = totalUsedSize - m_fileSystemNodes.ref_node(targetHandle).ref_data().m_fileSize;
        m_fileSystemNodes.remove(targetHandle);
        return true;
    }
    totalUsedSize = totalUsedSize - m_fileSystemNodes.ref_node(targetHandle).ref_data().m_fileSize;

    m_fileSystemNodes.remove(targetHandle);

    return true;
}

handle filesystem::create_file(const size_t fileSize, const std::string& fileName, const handle parentHandle)
{
    if (!exist(parentHandle)) {
        throw invalid_handle();
    }
    for (char ch : fileName) {
        if (ch == '/') {
            throw invalid_name();
        }
    }
    if (fileSize > get_available_size() || totalUsedSize + fileSize > theSize) {
        throw exceeds_size();
    }

    auto& currentNode = m_fileSystemNodes.ref_node(parentHandle);
    for (auto childHandle : currentNode.peek_children_handles()) {
        auto &childNode = m_fileSystemNodes.ref_node(childHandle);
        if (childNode.ref_data().m_name == fileName) {
            throw file_exists();
        }
    }

    if (m_fileSystemNodes.ref_node(parentHandle).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    totalUsedSize = totalUsedSize + fileSize; // is this how you increment the global variable?
    handle newFileHandle = m_fileSystemNodes.allocate(parentHandle);
    auto& newNode = m_fileSystemNodes.ref_node(newFileHandle);
    newNode.ref_data().m_type = node_type::File;
    newNode.ref_data().m_name = fileName;
    newNode.ref_data().m_fileSize = fileSize;
    m_fileSizeMaxHeap.push(fileSize, newFileHandle);

    return newFileHandle;
}

handle filesystem::create_directory(const std::string& directoryName, const handle parentHandle) 
{
    if (!exist(parentHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(parentHandle).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    for (char ch : directoryName) {
        if (ch == '/') {
            throw invalid_name();
        }
    }

    auto& currentNode = m_fileSystemNodes.ref_node(parentHandle);
    for (auto childHandle : currentNode.peek_children_handles()) {
        auto &childNode = m_fileSystemNodes.ref_node(childHandle);
        if (childNode.ref_data().m_name == directoryName) {
            throw directory_exists();
        }
    }

    handle newDirectoryHandle = m_fileSystemNodes.allocate(parentHandle);
    auto& newDirectoryNode = m_fileSystemNodes.ref_node(newDirectoryHandle);
    newDirectoryNode.ref_data().m_type = node_type::Directory;
    newDirectoryNode.ref_data().m_name = directoryName;
    newDirectoryNode.ref_data().m_fileSize = 0;
    
    return newDirectoryHandle;
}

handle filesystem::create_link(const handle targetHandle, const std::string& linkName, const handle parentHandle)
{
    if (!exist(parentHandle)) {
        throw invalid_handle();
    }

    if (!exist(targetHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(parentHandle).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    for (char ch : linkName) {
        if (ch == '/') {
            throw invalid_name();
        }
    }

    auto& currentNode = m_fileSystemNodes.ref_node(parentHandle);
    for (auto childHandle : currentNode.peek_children_handles()) {
        auto &childNode = m_fileSystemNodes.ref_node(childHandle);
        if (childNode.ref_data().m_name == linkName) {
            throw link_exists();
        }
    }

    handle newLinkHandle = m_fileSystemNodes.allocate(parentHandle);

    auto &newLinkNode = m_fileSystemNodes.ref_node(newLinkHandle);
    newLinkNode.ref_data().m_type = node_type::Link;
    newLinkNode.ref_data().m_name = linkName;
    newLinkNode.ref_data().m_linkedHandle = targetHandle;
    newLinkNode.ref_data().m_fileSize = 0;

    return newLinkHandle;
}


std::string filesystem::get_absolute_path(const handle targetHandle) {
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }
    std::vector<handle> indexes;
    std::string path = "";
    handle parent = targetHandle;
    while (parent != -1) {
        indexes.push_back(parent) ;
        parent = m_fileSystemNodes.ref_node(parent).get_parent_handle();
    }
    if (m_fileSystemNodes.ref_node(indexes.back()).ref_data().m_name == "") {
        indexes.pop_back();
    }
    for (int i = indexes.size() - 1; i >= 0; --i) {
        path += "/" + m_fileSystemNodes.ref_node(indexes[i]).ref_data().m_name;
    }
    return path;
}


std::string filesystem::get_name(const handle targetHandle)
{
    if (targetHandle < 0 || targetHandle >= m_fileSystemNodes.peek_nodes().size() ||
    m_fileSystemNodes.ref_node(targetHandle).is_recycled()) {
        throw invalid_handle();
    }
    return m_fileSystemNodes.ref_node(targetHandle).ref_data().m_name;
}

void filesystem::rename(const handle targetHandle, const std::string& newName)
{
    if (targetHandle < 0 || targetHandle >= m_fileSystemNodes.peek_nodes().size() ||
        m_fileSystemNodes.ref_node(targetHandle).is_recycled()) {
        throw invalid_handle();
    }
    for (char ch : newName) {
        if (ch == '/') {
            throw invalid_name();
        }
    }

    auto& currentNode = m_fileSystemNodes.ref_node(targetHandle);
    for (auto childHandle : currentNode.peek_children_handles()) {
        auto &childNode = m_fileSystemNodes.ref_node(childHandle);
        if (childNode.ref_data().m_name == newName) {
            throw name_exists();
        }
    }
    m_fileSystemNodes.ref_node(targetHandle).ref_data().m_name = newName;
}


bool filesystem::exist(const handle targetHandle) {
    if ((targetHandle < 0) || (targetHandle >= m_fileSystemNodes.peek_nodes().size())) {
        return false;
    }
    if (m_fileSystemNodes.ref_node(targetHandle).is_recycled()) {
        return false;
    }
    return true;
}


handle filesystem::get_handle(const std::string& absolutePath)
{
    std::istringstream pathStream(absolutePath);
    std::string component;
    handle currentHandle = 0;

    if (absolutePath == "/") {
        return currentHandle;
    }

    while (getline(pathStream, component, '/')) {
        if (component.empty()) {
            continue;
        }

        auto& currentNode = m_fileSystemNodes.ref_node(currentHandle);
        bool found = false;

        for (auto childHandle : currentNode.peek_children_handles()) {
            auto& childNode = m_fileSystemNodes.ref_node(childHandle);
            if (childNode.ref_data().m_name == component) {
                if (pathStream.peek() != EOF && childNode.ref_data().m_type == node_type::Link) {
                    currentHandle = follow(childHandle);
                } else {
                    currentHandle = childHandle;
                }
                found = true;
                break;
            }
        }

        if (!found) {
            throw invalid_path();
        }
    }

    return currentHandle;
}

handle filesystem::follow(const handle targetHandle)
{
    if (targetHandle < 0 || targetHandle >= m_fileSystemNodes.peek_nodes().size()) {
        throw invalid_handle();
    }

    auto& node = m_fileSystemNodes.ref_node(targetHandle);
    if (node.is_recycled()) {
        throw invalid_handle();
    }

    if (node.ref_data().m_type == node_type::Link) {
        return follow(node.ref_data().m_linkedHandle);
    }

    return targetHandle;
}

handle filesystem::get_largest_file_handle() const
{
    return m_fileSizeMaxHeap.top();
}

size_t filesystem::get_available_size() const
{
    return theSize - totalUsedSize;
}

size_t filesystem::get_file_size(const handle targetHandle)
{
    if (targetHandle < 0 || targetHandle >= m_fileSystemNodes.peek_nodes().size()) {
        throw invalid_handle();
    }

    auto& node = m_fileSystemNodes.ref_node(targetHandle);

    if ((node.is_recycled()) || (node.ref_data().m_type != node_type::File && node.ref_data().m_type != node_type::Link)) {
        throw invalid_handle();
    }

    if (node.ref_data().m_type == node_type::File) {
        return node.ref_data().m_fileSize;
    }

    if (node.ref_data().m_type == node_type::Link) {
        return get_file_size(follow(targetHandle));
    }

    throw invalid_handle();
}

size_t filesystem::get_file_size(const std::string& absolutePath)
{
    handle targetHandle = get_handle(absolutePath);

    if (targetHandle == -1) {
        throw invalid_path();
    }

    auto node = m_fileSystemNodes.ref_node(targetHandle);
    if (node.is_recycled()) {
        throw invalid_path();
    }

    if (node.ref_data().m_type == node_type::File) {
        return node.ref_data().m_fileSize;
    }

    if (node.ref_data().m_type == node_type::Link) {
        return get_file_size(follow(targetHandle));
    }
    throw invalid_handle();
}

std::string filesystem::print_layout()
{
    std::stringstream ss{};
    const auto& node = m_fileSystemNodes.ref_node(0);
    for (const auto& childHandle : node.peek_children_handles()) {
        print_traverse(0, ss, childHandle);
    }
    return ss.str();
}

void filesystem::print_traverse(const size_t level, std::stringstream& ss, const handle targetHandle)
{
    auto& node = m_fileSystemNodes.ref_node(targetHandle);
    std::stringstream indentation{};
    for (auto i = level; i > 0; i--)
    {
        indentation << "\t";
    }
    std::string type{};
    switch (node.ref_data().m_type)
    {
        case node_type::Directory: type = "[D]"; break;
        case node_type::Link: type = "[L]"; break;
        case node_type::File: type = "[F]"; break;
    }
    ss << indentation.str() << type << node.ref_data().m_name;
    if (node.ref_data().m_type == node_type::Link)
    {
        try {
            const auto path = get_absolute_path(follow(node.get_handle()));
            ss << " [->" << path << "]";
        }
        catch (const std::exception& e)
        {
            ss << " [invalid]";
        }
    }
    else if (node.ref_data().m_type == node_type::File)
    {
        ss << " (size = " << std::to_string(node.ref_data().m_fileSize) << ")";
    }
    ss << std::endl;
    for (const auto& childHandle : node.peek_children_handles())
    {
        print_traverse(level + 1, ss, childHandle);
    }
}
