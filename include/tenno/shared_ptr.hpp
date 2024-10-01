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
#include <memory> // for std::allocator
#include <tenno/mutex.hpp>
#include <tenno/types.hpp>

namespace tenno
{

/*
struct allocator
{
    virtual allocator();
    virtual ~allocator();
    virtual void *allocate(tenno::size size);
    virtual void deallocate(void *ptr, tenno::size size);
};

struct deleter
{

};
*/

template <class T> class weak_ptr;

/**
 * @brief A shared pointer
 *
 * @tparam T The type of the object to point to
 */
template <class T> class shared_ptr
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
        T *object;
        std::allocator<control_block> allocator;
        std::default_delete<T> deleter;
        long num_ptrs;
        long num_weak_ptrs;
        tenno::mutex cb_mutex;
        control_block() = default;
        void deallocate()
        {
            this->deleter(this->object);
            if (this->num_weak_ptrs == 0)
                this->allocator.deallocate(this, 1);
        }
    };

    friend control_block;
    friend class tenno::weak_ptr<T>;

    /**
     * @brief Default constructor
     */
    shared_ptr() noexcept : _element(nullptr), _control_block(nullptr)
    {
    }

    /**
     * @brief Construct a new shared pointer object
     *
     * @param ptr The object to point to
     */
    shared_ptr(T *ptr)
    {
        auto *cb = new control_block();
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = std::allocator<control_block>();
        cb->deleter = std::default_delete<T>();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Construct a new shared pointer object
     *
     * @param ptr The object to point to
     * @param deleter The deleter to use to delete the object
     */
    shared_ptr(T *ptr, std::default_delete<T> deleter)
    {
        auto *cb = new control_block();
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = std::allocator<control_block>();
        cb->deleter = deleter;
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Construct a new shared pointer object
     *
     * @param ptr The object to point to
     * @param deleter The deleter to use to delete the object
     * @param allocator The allocator to use to allocate the control block
     */
    shared_ptr(T *ptr, std::default_delete<T> deleter,
               std::allocator<control_block> alloc)
    {
        auto *cb = alloc.allocate(1);
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = alloc;
        cb->deleter = deleter;
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Copy constructor
     *
     * @param other The shared pointer to copy
     */
    shared_ptr(const shared_ptr &other) noexcept
        : _element(other._element), _control_block(other._control_block)
    {
        tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
        this->_control_block->num_ptrs++;
    }

    /**
     * @brief Move constructor
     *
     * @param other The shared pointer to move
     */
    shared_ptr(shared_ptr &&other) noexcept
        : _element(other._element), _control_block(other._control_block)
    {
        other._element = nullptr;
        other._control_block = nullptr;
    }

    /**
     * @brief Destructor
     */
    ~shared_ptr()
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }
    }

    /**
     * @brief Move assignment operator
     *
     * @param other The shared pointer to move
     * @return shared_ptr& The moved shared pointer
     */
    shared_ptr &operator=(shared_ptr &&other) noexcept
    {
        if (this == &other)
            return *this;

        this->_element = other._element;
        this->_control_block = other._control_block;
        return *this;
    }

    /**
     * @brief Copy assignment operator
     *
     * @param other The shared pointer to copy
     * @return shared_ptr& The copied shared pointer
     */
    shared_ptr &operator=(const shared_ptr &other) noexcept
    {
        if (this == &other)
            return *this;

        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(
                    this->_control_block->cb_mutex);
                this->_control_block->num_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0)
            {
                this->_control_block->deallocate();
            }
        }

        this->_element = other._element;
        this->_control_block = other._control_block;
        other._control_block->num_ptrs++;
        return *this;
    }

    /**
     * @brief Reset the shared pointer
     */
    void reset() noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        this->_element = nullptr;
        this->_control_block = nullptr;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     */
    void reset(T *ptr) noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        auto *cb = new control_block();
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = std::allocator<control_block>();
        cb->deleter = std::default_delete<T>();
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     * @param deleter The deleter to use to delete the object
     */
    void reset(T *ptr, std::default_delete<T> deleter) noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        auto *cb = new control_block();
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = std::allocator<control_block>();
        cb->deleter = deleter;
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Reset the shared pointer to point to the object managed by the
     * control block cb
     *
     * @param cb The control block to manage the object
     * @param deleter The deleter to use to delete the object
     * @param allocator The allocator to use to allocate the object
     */
    void reset(T *ptr, std::default_delete<T> deleter,
               std::allocator<control_block> alloc) noexcept
    {
        if (!this->_control_block)
            return;

        {
            tenno::lock_guard<tenno::mutex> lock(
                this->_control_block->cb_mutex);
            this->_control_block->num_ptrs--;
        }
        if (this->_control_block->num_ptrs == 0)
        {
            this->_control_block->deallocate();
        }

        auto *cb = alloc.allocate(1);
        cb->object = ptr;
        cb->num_ptrs = 1;
        cb->num_weak_ptrs = 0;
        cb->allocator = alloc;
        cb->deleter = deleter;
        cb->cb_mutex = tenno::mutex();
        this->_element = cb->object;
        this->_control_block = cb;
    }

    /**
     * @brief Swap the shared pointer with another shared pointer
     *
     * @param other The shared pointer to swap with
     */
    void swap(shared_ptr &other) noexcept
    {
        T *tmp_element = this->_element;
        control_block *tmp_control_block = this->_control_block;

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
    T *get() const noexcept
    {
        return this->_element;
    }

    /**
     * @brief Dereference operator
     *
     * @return T& The object pointed to by the shared pointer
     */
    T &operator*() const noexcept
    {
        return *this->_element;
    }

    /**
     * @brief Member access operator
     *
     * @return T* The object pointed to by the shared pointer
     */
    T *operator->() const noexcept
    {
        return this->_element;
    }

    /**
     * @brief Array access operator
     *
     * @param index The index of the element to access
     * @return T& The element at the given index
     */
    auto &operator[](tenno::size index) const noexcept
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
        if (!this->_control_block)
            return 0;
        return this->_control_block->num_ptrs;
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
    bool owner_before(const shared_ptr &other) const noexcept
    {
        return *this->_element < *other._element;
    }

    /**
     * @brief Stored value equality
     *
     * @param other The shared pointer to compare with
     * @return true If the shared pointer is equal to the other shared pointer
     * @return false If the shared pointer is not equal to the other shared
     * pointer
     */
    bool owner_equal(const shared_ptr &other) const noexcept
    {
        if (!this->_element && !other._element) return true;
        if (!this->_element || !other._element) return false;
        return *this->_element == *other._element;
    }

    // TODO:
    // - owner_hash (c++26)

#ifndef TENNO_DEBUG
  private:
#endif
    T *_element;
    control_block *_control_block;
};

template <typename T>
class weak_ptr
{
  public:
    using element_type = T;

    constexpr weak_ptr() noexcept
    {
        this->_control_block = nullptr;
        this->_ptr = nullptr;
    }

    weak_ptr( const weak_ptr& r ) noexcept
    {
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return;
        }
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        this->_control_block->num_weak_ptrs++;
    }
    
    weak_ptr( weak_ptr&& r ) noexcept
    {
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return;
        }
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        r._control_block = nullptr;
        r._ptr = nullptr;
    }

    weak_ptr( const tenno::shared_ptr<T>& r ) noexcept
    {
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return;
        }
        tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
        this->_ptr = r._element;
        this->_control_block = r._control_block;
        this->_control_block->num_weak_ptrs++;
    }

    ~weak_ptr()
    {
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0 && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
    }

    weak_ptr& operator=( const weak_ptr& r ) noexcept
    {
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0 && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return *this;
        }
        tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        this->_control_block->num_weak_ptrs++;
        return *this;
    }

    weak_ptr& operator=( const shared_ptr<T>& r ) noexcept
    {
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0 && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return *this;
        }
        tenno::lock_guard<tenno::mutex> lock(r._control_block->cb_mutex);
        this->_ptr = r._element;
        this->_control_block = r._control_block;
        this->_control_block->num_weak_ptrs++;
        return *this;
    }

    weak_ptr& operator=( weak_ptr&& r ) noexcept
    {
        if (this->_control_block)
        {
            {
                tenno::lock_guard<tenno::mutex> lock(this->_control_block->cb_mutex);
                this->_control_block->num_weak_ptrs--;
            }
            if (this->_control_block->num_ptrs == 0 && this->_control_block->num_weak_ptrs == 0)
                this->_control_block->deallocate();
        }
        if (!r._control_block)
        {
            this->_control_block = nullptr;
            this->_ptr = nullptr;
            return *this;
        }
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        r._control_block = nullptr;
        r._ptr = nullptr;
        return *this;
    }

    void reset() noexcept
    {
        if (this->_control_block)
            this->_control_block->num_weak_ptrs--;
        this->_control_block = nullptr;
        this->_ptr = nullptr;
    }

    void swap( weak_ptr& r ) noexcept
    {
        auto cb_tmp = this->_control_block;
        auto ptr_tmp = this->_ptr;
        this->_control_block = r._control_block;
        this->_ptr = r._ptr;
        r._control_block = cb_tmp;
        r._ptr = ptr_tmp;
    }

    long use_count() const noexcept
    {
        if (this->_control_block)
            return this->_control_block->num_ptrs;
        return 0;
    }

    bool expired() const noexcept
    {
        if (!this->_control_block) return true;
        return this->_control_block->num_ptrs == 0;
    }

    tenno::shared_ptr<T> lock() const noexcept
    {
        if (this->_control_block->num_ptrs == 0)
            return tenno::shared_ptr<T>();
        auto sp = tenno::shared_ptr<T>(this->_ptr);
        sp._control_block = this->_control_block;
        sp._control_block->num_ptrs++;
        return sp;
    }

    template< class Y >
    bool owner_before( const weak_ptr<Y>& other ) const noexcept
    {
        if (!this->_ptr || !other._ptr) return false;
        if (this->_ptr && !other._ptr) return true;
        if (!this->_ptr && other._ptr) return false;
        return *this->_ptr < *other._ptr;
    }
		
    template< class Y >
    bool owner_before( const tenno::shared_ptr<Y>& other ) const noexcept
    {
        return *this->_ptr < *other._element;
    }

    template< class Y >
    bool owner_equal( const tenno::shared_ptr<Y>& other ) const noexcept
    {
        if (!this->_ptr && !other._element) return true;
        if (!this->_ptr || !other._element) return false;
        return *this->_ptr == *other._element;
    }

    // TODO
    /*
    tenno::size_t owner_hash() const noexcept
    {
        return std::hash<element_type>()(this->_ptr);
    }
    */

  private:
    element_type *_ptr;
    tenno::shared_ptr<T>::control_block *_control_block;
};

} // namespace tenno
