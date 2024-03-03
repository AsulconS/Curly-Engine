/********************************************************************************
 *                                                                              *
 * Curly Engine                                                                 *
 * Copyright (c) 2021-2024 Adrian Bedregal                                      *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#pragma once

#include <engine/core/config.hpp>
#include <engine/core/common.hpp>

#include <engine/system/utility.hpp>

#define CURLY_STACK_DEFAULT_CAPACITY 16

namespace sys
{
template <typename T>
class Stack
{
public:
    /**
     * @brief Construct a new Stack object
     * 
     * @param t_capacity 
     */
    Stack(cfg::uint64 t_capacity = CURLY_STACK_DEFAULT_CAPACITY);

    /**
     * @brief Construct a new Stack object
     * 
     * @param o 
     */
    Stack(const Stack<T>& o);
    /**
     * @brief Construct a new Stack object
     * 
     * @param o 
     */
    Stack(Stack<T>&& o);

    /**
     * @brief Destroy the Stack object
     * 
     */
    virtual ~Stack();

    /**
     * @brief C-Assigns a Stack to another
     * 
     * @param o 
     * @return Stack<T>& 
     */
    Stack<T>& operator=(const Stack<T>& o);
    /**
     * @brief M-Assigns a Stack to another
     * 
     * @param o 
     * @return Stack<T>& 
     */
    Stack<T>& operator=(Stack<T>&& o);

    /**
     * @brief Gets the size of the Stack
     * 
     * @return uint64 
     */
    cfg::uint64 size() const noexcept;
    /**
     * @brief Returns a boolean indicating if Stack is empty or not
     * 
     * @return true 
     * @return false 
     */
    bool empty() const noexcept;

    /**
     * @brief Returns a reference to access last element
     * 
     * @return T& 
     */
    T& top();
    /**
     * @brief Returns a constant reference to access last element
     * 
     * @return const T& 
     */
    const T& top() const;

    /**
     * @brief C-Pushes a new element to the Stack
     * 
     * @param val 
     */
    void push(const T& val);
    /**
     * @brief M-Pushes a new element to the Stack
     * 
     * @param val 
     */
    void push(T&& val);
    /**
     * @brief Drops the last element of the Stack
     * 
     */
    void pop();

private:
    T* m_data;
    cfg::uint64 m_size;
    cfg::uint64 m_capacity;
};

} // namespace sys

#include <engine/system/dstr/stack.inl>

#undef CURLY_STACK_DEFAULT_CAPACITY
