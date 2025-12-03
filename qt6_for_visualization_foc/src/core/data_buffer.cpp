#include "data_buffer.h"
#include "types.h"

template<typename T>
void data_buffer<T>::push(const T& value) {
    if (m_data.size() < m_capacity) {
        m_data.push_back(value);
    } else {
        m_data[m_head] = value;
        m_head = (m_head + 1) % m_capacity;
    }
}

template<typename T>
T data_buffer<T>::get(size_t index) const {
    size_t actual = (m_head + index) % m_data.size();
    return m_data[actual];
}

// 显式实例化
template class data_buffer<double>;
template class data_buffer<motor_state_t>;
