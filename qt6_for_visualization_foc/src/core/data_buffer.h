#ifndef CORE_DATA_BUFFER_H
#define CORE_DATA_BUFFER_H

#include <vector>
#include <cstddef>

// 环形数据缓冲（占位）
template<typename T>
class data_buffer {
public:
    explicit data_buffer(size_t capacity = 1000) : m_capacity(capacity) { m_data.reserve(capacity); }
    void push(const T& value);
    T get(size_t index) const;
    size_t size() const { return m_data.size(); }
    void clear() { m_data.clear(); m_head = 0; }

private:
    std::vector<T> m_data;
    size_t m_capacity;
    size_t m_head = 0;
};

#endif // CORE_DATA_BUFFER_H
