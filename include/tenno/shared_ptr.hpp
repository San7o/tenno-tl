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
#include <tenno/mutex.hpp>
#include <tenno/types.hpp>

namespace tenno
{

/**
 * @brief A shared pointer
 *
 * @tparam T The type of the object to point to
 */
template <typename T> class shared_ptr
{
  public:
    /**
     * @brief The type of the object to point to
     */
    using element_type = T;

    /**
     * @brief The type of the control block
     */
    struct control_block
    {
        T object;
        void (*allocator)();
        void (*default_delete)();
        long num_ptrs;
        long num_weak_ptrs;
        tenno::mutex cb_mutex;
        control_block() = default;
    };

    /**
     * @brief Default constructor
     */
    shared_ptr() noexcept : _element(nullptr), _control_block(nullptr) {}

    /**
     * @brief Constructs a shared pointer that points to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     */
    shared_ptr(control_block *cb)
    {
        tenno::lock_guard<tenno::mutex> lock(cb->cb_mutex);
        this->_control_block = cb;
        this->_element = &cb->object;
        cb->num_ptrs++;
    }

    /**
     * @brief Copy constructor
     *
     * @param other The shared pointer to copy
     */
    shared_ptr(const shared_ptr &other) noexcept : _element(other._element), _control_block(other._control_block)
    {
        tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
        this->_control_block->num_ptrs++;
    }

    /**
     * @brief Move constructor
     *
     * @param other The shared pointer to move
     */
    shared_ptr(shared_ptr &&other) noexcept : _element(other._element), _control_block(other._control_block)
    {
        other._element = nullptr;
        other._control_block = nullptr;
    }

    /**
     * @brief Destructor
     */
    ~shared_ptr()
    {
        if (!this->_control_block) return;

        {
            tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            if (this->_control_block->default_delete)
                this->_control_block->default_delete();
        }
    }

    /**
     * @brief Move assignment operator
     *
     * @param other The shared pointer to move
     * @return shared_ptr& The moved shared pointer
     */
    shared_ptr& operator=(shared_ptr &&other) noexcept
    {
        if (this == &other)
            return *this;

        this->_element = other._element;
        this->_control_block = other._control_block;
        return *this;
    }

    /**
     * @brief Reset the shared pointer
     */
    void reset() noexcept
    {
        {
            tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            if (this->_control_block->default_delete)
                this->_control_block->default_delete();
        }
        this->_element = nullptr;
        this->_control_block = nullptr;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the control
     * block cb
     *
     * @param cb The control block to manage the object
     */
    void reset(control_block *cb) noexcept
    {
        {
            tenno::lock_guard<tenno::mutex> lock(cb->cb_mutex);
            cb->num_ptrs++;
        }
        {
            tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            if (this->_control_block->default_delete)
                this->_control_block->default_delete();
        }
        this->_element = &cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the control
     * block cb
     *
     * @param cb The control block to manage the object
     * @param deleter The deleter to use to delete the object
     */
    void reset(control_block* cb, void (*deleter)()) noexcept
    {
        {
            tenno::lock_guard<tenno::mutex> lock(cb->cb_mutex);
            cb->num_ptrs++;
        }
        {
            tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            if (this->_control_block->default_delete)
                this->_control_block->default_delete();
        }
        this->_element = &cb->object;
        this->_control_block = cb;
        cb->default_delete = deleter;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the control
     * block cb
     *
     * @param cb The control block to manage the object
     * @param deleter The deleter to use to delete the object
     * @param allocator The allocator to use to allocate the object
     */
    void reset(control_block* cb, void (*deleter)(), void (*allocator)()) noexcept
    {
        {
            tenno::lock_guard<tenno::mutex> lock(cb->cb_mutex);
            cb->num_ptrs++;
        }
        {
            tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            if (this->_control_block->default_delete)
                this->_control_block->default_delete();
        }
        this->_element = &cb->object;
        this->_control_block = cb;
        cb->default_delete = deleter;
        cb->allocator = allocator;
    }

    /**
     * @brief Swap the shared pointer with another shared pointer
     *
     * @param other The shared pointer to swap with
     */
    void swap(shared_ptr& other) noexcept
    {
        T* tmp_element = this->_element;
        control_block* tmp_control_block = this->_control_block;

        this->_element = other._element;
        this->_control_block = other._control_block;

        other._element = tmp_element;
        other._control_block = tmp_control_block;
    }

    /**
     * @brief Get the object pointed to by the shared pointer
     *
     * @return T* The object pointed to by the shared pointer
     */
    T* get() const noexcept
    {
        return this->_element;
    }

    /**
     * @brief Dereference operator
     *
     * @return T& The object pointed to by the shared pointer
     */
    T& operator*() const noexcept
    {
        return *this->_element;
    }

    /**
     * @brief Member access operator
     *
     * @return T* The object pointed to by the shared pointer
     */
    T* operator->() const noexcept
    {
        return this->_element;
    }

    /**
     * @brief Array access operator
     *
     * @param index The index of the element to access
     * @return T& The element at the given index
     */
    auto& operator[](tenno::size index) const noexcept
    {
        return (*this->_element)[index];
    }

    /**
     * @brief Get the number of shared pointers pointing to the object
     *
     * @return long The number of shared pointers pointing to the object
     */
    long use_count() const noexcept
    {
        return this->_control_block->num_ptrs;
    }

    /**
     * @brief Check if the shared pointer is unique
     *
     * @return true If the shared pointer is unique
     * @return false If the shared pointer is not unique
     */
    bool unique() const noexcept
    {
        return this->_control_block->num_ptrs == 1;
    }

    /**
     * @brief Check if the shared pointer is empty
     *
     * @return true If the shared pointer is empty
     * @return false If the shared pointer is not empty
     */
    explicit operator bool() const noexcept
    {
        return this->_element != nullptr;
    }

    /**
     * @brief Stored value ordering
     *
     * @param other The shared pointer to compare with
     * @return true If the shared pointer is before the other shared pointer
     * @return false If the shared pointer is after the other shared pointer
     */
    bool owner_before(const shared_ptr& other) const noexcept
    {
        return *this->_element < *other._element;
    }

    /**
     * @brief Stored value equality
     *
     * @param other The shared pointer to compare with
     * @return true If the shared pointer is equal to the other shared pointer
     * @return false If the shared pointer is not equal to the other shared pointer
     */
    bool owner_equal(const shared_ptr& other) const noexcept
    {
        return *this->_element == *other._element;
    }

    // TODO:
    // - owner_hash (c++26)

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
