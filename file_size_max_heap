#include "file_size_max_heap.hpp"
using namespace cs251;

void file_size_max_heap::remove(const handle fileHandle) {
    size_t counter = 0;
    int index = -1;
    while (counter < m_nodes.size()) {
        if (m_nodes[counter].m_handle == fileHandle) {
            index = counter;
            break;
        }
        counter++;
    }
    if (index == -1) {
        throw invalid_handle();
    }
    m_nodes[index] = m_nodes.back();
    m_nodes.pop_back();
    heapify_down(index);
    return;
}

void file_size_max_heap::heapify_down(const handle index) {
    size_t leftChild = 2 * index + 1;
    size_t rightChild = 2 * index + 2;
    size_t parent = index;

    if (leftChild < m_nodes.size()) {
        if (m_nodes[leftChild].m_value > m_nodes[parent].m_value)
            parent = leftChild;
    }

    if (rightChild < m_nodes.size()) {
        if (m_nodes[rightChild].m_value > m_nodes[parent].m_value)
            parent = rightChild;
    }

    if (parent != index) {
        std::swap(m_nodes[index], m_nodes[parent]);
        heapify_down(parent);
    }
}


void file_size_max_heap::push(size_t fileSize, handle fileHandle) {
    file_size_max_heap_node newNode;
    newNode.m_handle = fileHandle;
    newNode.m_value = fileSize;
    m_nodes.push_back(newNode);
    int index = m_nodes.size() - 1;
    size_t parent = (index - 1) / 2;
    while (index > 0 && m_nodes[index].m_value > m_nodes[parent].m_value) {
        std::swap(m_nodes[index], m_nodes[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

handle file_size_max_heap::top() const {
    if (m_nodes.empty()) {
        throw heap_empty();
    }
    return m_nodes[0].m_handle;
}

