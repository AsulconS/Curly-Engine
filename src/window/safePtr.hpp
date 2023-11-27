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

#ifndef CURLY_SAFE_PTR_HPP
#define CURLY_SAFE_PTR_HPP

#include <config/config.hpp>
#include <config/common.hpp>

namespace wnd
{
template <typename T>
class CURLY_API SafePtr final
{
public:
    template <typename... TArgs>
    SafePtr(TArgs... args);
    SafePtr(SafePtr<T>&& o);
    ~SafePtr();

    T& operator*();
    T* operator->();
    bool operator==(const SafePtr<T>& o);
    bool operator!=(const SafePtr<T>& o);
    bool operator==(const std::nullptr_t nullPtr);
    bool operator!=(const std::nullptr_t nullPtr);
    operator T*();

private:
    T* m_data;

    SafePtr(const SafePtr<T>& o) = delete;
};

} // namespace wnd

#include "safePtr.inl"

#endif // CURLY_SAFE_PTR_HPP