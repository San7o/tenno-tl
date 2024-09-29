/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include <memory> // std::allocator and std::default_delete
#include <tenno/shared_ptr.hpp>
#include <tenno/unique_ptr.hpp>

namespace tenno
{

/**
 * @brief Create a shared pointer with the given arguments
 *
 * @tparam T The type of the object to create
 * @tparam Args The type of the arguments to pass to the constructor
 * @param args The arguments to pass to the constructor
 * @return shared_ptr<T> The shared pointer to the object
 *
 * @note Make shared takes care of the allocation of the object, while the
 * shared pointer takes care of the allocation and deallocation of the
 * internal control block, as well as the deallocation of the object.
 * @note The object must accept variadic arguments in the constructor
 */
template <class T, class... Args>
std::enable_if<!std::is_array<T>::value, shared_ptr<T>>::type
make_shared(Args &&...args) noexcept
{
    return tenno::shared_ptr<T>(new T(args...));
}

/*
template <class T>
std::enable_if<std::is_array<T>::value, shared_ptr<T>>::type
make_shared(tenno::size n) noexcept
{
    return tenno::shared_ptr<T>(new T[n]);
}
*/

template <class T> shared_ptr<T> make_shared() noexcept
{
    return tenno::shared_ptr<T>(new T());
}

template <class T, class Alloc, class... Args>
shared_ptr<T> allocate_shared(Alloc &alloc, Args &&...args) noexcept
{
    T *t = alloc.allocate(1);
    *t = T(args...);
    return tenno::shared_ptr<T>(t);
}

template <class T, class Alloc>
shared_ptr<T> allocate_shared(Alloc &alloc) noexcept
{
    T *t = alloc.allocate(1);
    *t = T();
    return tenno::shared_ptr<T>(t);
}

template <class T, class... Args>
constexpr unique_ptr<T> make_unique(Args &&...args)
{
    T *t = new T(args...);
    auto ptr = tenno::unique_ptr<T>(args...);
    ASSERT(ptr.get() != nullptr);
}

} // namespace tenno
