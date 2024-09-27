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

// TODO: remove this
#include <memory> // for std::allocator and atd::default_delete

namespace tenno
{

template <typename T> class shared_ptr
{
  public:
    using element_type = T;

    struct control_block
    {
        T object;
        void (*allocator)();
        void (*default_delete)();
        int num_ptrs;
        int num_weak_ptrs;
    };

    shared_ptr(control_block *cb)
    {
        this->_control_block = cb;
        this->_element = &cb->object;
        cb->num_ptrs++;
    }

    ~shared_ptr()
    {
        this->_control_block->num_ptrs--;
        if (this->_control_block->num_ptrs == 0)
        {
            if (this->_control_block->default_delete)
                this->_control_block->default_delete();
        }
    }

#ifndef TENNO_DEBUG
  private:
#endif
    T *_element;
    control_block* _control_block;
};

/*
// TODO move in memory
template <typename T, typename... Args>
shared_ptr<T> make_shared(Args... args) noexcept
{
    // TODO
    return shared_ptr();
}
*/

} // namespace tenno
