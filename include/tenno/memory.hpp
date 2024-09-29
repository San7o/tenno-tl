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

namespace tenno
{

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args... args) noexcept
{
    return tenno::shared_ptr<T>(T(args...));
}

/*
// TODO: array initialization
template< class T >
shared_ptr<T> make_shared(tenno::size N) noexcept
{
    auto t = T[N]();
    return tenno::shared_ptr<T>(&t);
}
*/

template <class T> shared_ptr<T> make_shared() noexcept
{
    return tenno::shared_ptr<T>(T());
}

} // namespace tenno

// TODO: allocate shared
