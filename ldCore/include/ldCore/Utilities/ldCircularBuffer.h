/**
    libLaserdockCore
    Copyright(c) 2018 Wicked Lasers

    This file is part of libLaserdockCore.

    libLaserdockCore is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libLaserdockCore is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libLaserdockCore.  If not, see <https://www.gnu.org/licenses/>.
**/

#ifndef LDCIRCULARBUFFER_H
#define LDCIRCULARBUFFER_H

#include <ldCore/Utilities/ldVertex.h>
#include <ldCore/Utilities/ldBuffer.h>

/*
 circular buffer does not have a tail.
 data is continously fed into the head and push head forward.
*/
template<class T>
class LDCORESHARED_EXPORT ldCircularBuffer: private ldBuffer<T>
{
public:
    ldCircularBuffer(unsigned size)
        : ldBuffer<T>(size)
        , m_head(0)
    {}
    virtual ~ldCircularBuffer() {}

    /*--------------------------------------------------//
     Push data to the buffer
     [parameters]
     data       - pointer to the data array
     size       - size of data array (in items)
     //--------------------------------------------------*/
    void Push(const T* data, unsigned size)
    {
        T* ptr = ldBuffer<T>::GetData() + m_head;
        unsigned remained = ldBuffer<T>::GetSize() - m_head;

        if (remained >= size)
        {
            memcpy(ptr, data, size * sizeof(T));

            if (remained > size)
            {
                m_head += size;
            }
            else
            {
                m_head = 0;
                m_isFull = true;
            }
        }
        else
        {
            memcpy(ptr, data, remained * sizeof(T));

            ptr = ldBuffer<T>::GetData();
            data += remained;
            memcpy(ptr, data, (size - remained) * sizeof(T));

            m_head = size - remained;

            m_isFull = true;
        }
    }
    /*--------------------------------------------------//
     Push value to the buffer
     [parameters]
     val        - value
     //--------------------------------------------------*/
    void Push(const T& val)
    {
        ldBuffer<T>::GetData()[m_head] = val;
        ++m_head;
        if (m_head == ldBuffer<T>::GetSize())
        {
            m_head = 0;
            m_isFull = true;
        }
    }
    /*--------------------------------------------------//
     Get last data from the buffer
     [parameters]
     data       - pointer to the data array
     size       - size of data array (in items)
     //--------------------------------------------------*/
    void Get(T* data, unsigned size)
    {
        if(size >= ldBuffer<T>::GetSize())
            size == ldBuffer<T>::GetSize();

        if (size <= m_head)
        {
            T* ptr = ldBuffer<T>::GetData() + m_head - size;
            memcpy(data, ptr, size * sizeof(T));
        }
        else
        {
            T* ptr = ldBuffer<T>::GetData() + ldBuffer<T>::GetSize() - (size - m_head);
            if(m_isFull)
                memcpy(data, ptr, (size - m_head) * sizeof(T));

            ptr = ldBuffer<T>::GetData();
            if(m_isFull)
                data += size - m_head;

            memcpy(data, ptr, m_head * sizeof(T));
        }
    }

    /*--------------------------------------------------//
     Get first data from the buffer
     [parameters]
     data       - pointer to the data array
     size       - size of data array (in items)
     //--------------------------------------------------*/
    void GetFirst(T* data, unsigned size)
    {
        if(size >= ldBuffer<T>::GetSize())
            size == ldBuffer<T>::GetSize();

        if(m_isFull) {
            int remainingSpace = ldBuffer<T>::GetSize() - m_head;
            if (size <= remainingSpace)
            {
                T* ptr = ldBuffer<T>::GetData() + m_head;
                memcpy(data, ptr, size * sizeof(T));
            }
            else
            {
                T* ptr = ldBuffer<T>::GetData() + m_head;
                memcpy(data, ptr, remainingSpace * sizeof(T));

                ptr = ldBuffer<T>::GetData();
                data += remainingSpace;
                memcpy(data, ptr, (size - remainingSpace) * sizeof(T));
            }
        } else {
            if (size <= m_head)
            {
                T* ptr = ldBuffer<T>::GetData() ;
                memcpy(data, ptr, size * sizeof(T));
            }
            else
            {
                T* ptr = ldBuffer<T>::GetData();
                memcpy(data, ptr, m_head * sizeof(T));
            }
        }
    }

    unsigned GetLevel()
    {
        return m_isFull ? ldBuffer<T>::GetSize() : m_head;
    }

    void Reset()
    {
        memset(ldBuffer<T>::GetData(), 0, ldBuffer<T>::GetSize() * sizeof(T));
        m_head = 0;
        m_isFull = false;
    }

private:
    unsigned    m_head;
    bool    m_isFull{false};
};

typedef ldCircularBuffer<ldVertex> ldVertexCircularBuffer;

#endif // LDCIRCULARBUFFER_H
