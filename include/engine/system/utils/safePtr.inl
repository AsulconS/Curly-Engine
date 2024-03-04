/********************************************************************************
 *                                                                              *
 * Curly Engine                                                                 *
 * Copyright (c) 2019-2024 Adrian Bedregal                                      *
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

namespace sys
{
template <typename T>
template <typename... TArgs>
inline SafePtr<T>::SafePtr(TArgs... args)
{
    m_data = new T(args...);
}

template <typename T>
inline SafePtr<T>::SafePtr(SafePtr<T>&& o)
    : m_data {o.m_data}
{
    o.m_data = nullptr;
}

template <typename T>
inline SafePtr<T>::~SafePtr()
{
    if(m_data != nullptr)
    {
        delete m_data;
    }
}

template <typename T>
inline T& SafePtr<T>::operator*()
{
    return *m_data;
}

template <typename T>
inline T* SafePtr<T>::operator->()
{
    return m_data;
}

template <typename T>
inline bool SafePtr<T>::operator==(const SafePtr<T>& o)
{
    return this->m_data == o.m_data;
}

template <typename T>
inline bool SafePtr<T>::operator!=(const SafePtr<T>& o)
{
    return this->m_data != o.m_data;
}

template <typename T>
inline bool SafePtr<T>::operator==(const std::nullptr_t nullPtr)
{
    return this->m_data == nullPtr;
}

template <typename T>
inline bool SafePtr<T>::operator!=(const std::nullptr_t nullPtr)
{
    return this->m_data != nullPtr;
}

template <typename T>
inline SafePtr<T>::operator T*()
{
    return m_data;
}

} // namespace wnd
