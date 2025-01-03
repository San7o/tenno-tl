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

#include <thread> /* based on thread */

namespace tenno
{

/**
 * @brief The jthread class is a wrapper around std::thread that automatically
 *        rejoins the thread on destruction
 */
class jthread
{
  public:
    /**
     * @brief The type of the implementation defined thread handle
     */
    using native_handle_type = std::thread::native_handle_type;

    std::thread::id id;

    jthread() noexcept = default;
    jthread(const jthread &) = delete;

    /**
     * @brief Construct a new jthread object by moving the other object
     *
     * @param other The other jthread object to move
     */
    jthread(jthread &&other) noexcept
    {
        this->_inner_thread = std::move(other._inner_thread);
        this->id = this->_inner_thread.get_id();
    }

    /**
     * @brief Construct a new jthread object with the given function and
     * arguments
     * @tparam F The type of the function to execute
     * @tparam Args The type of the arguments to pass to the function
     * @param f The function to execute
     * @param args The arguments to pass to the function
     */
    template <class F, class... Args> explicit jthread(F &&f, Args &&...args)
    {
        this->_inner_thread = std::thread(f, args...);
        this->id = this->_inner_thread.get_id();
    }

    ~jthread()
    {
        if (this->joinable())
        {
            this->_inner_thread.join();
        }
    }

    bool joinable() const noexcept
    {
        return this->_inner_thread.joinable();
    }

    std::thread::id get_id() const noexcept
    {
        return this->_inner_thread.get_id();
    }

    /**
     * @brief Get the native handle of the thread
     *
     * @return native_handle_type The native handle of the thread
     */
    native_handle_type native_handle()
    {
        return this->_inner_thread.native_handle();
    }

    /**
     * @brief Get the number of concurrent threads supported by the
     * implementation
     *
     * @return unsigned int The number of concurrent threads supported by the
     * implementation
     */
    static unsigned int hardware_concurrency() noexcept
    {
        return std::thread::hardware_concurrency();
    }

    void join()
    {
        this->_inner_thread.join();
    }

    void detach()
    {
        this->_inner_thread.detach();
    }

    void swap(tenno::jthread &other) noexcept
    {
        this->_inner_thread.swap(other._inner_thread);
        this->id = this->_inner_thread.get_id();
        other.id = other._inner_thread.get_id();
    }

#if __cplusplus >= 202002L // C++20
    std::stop_source get_stop_source() noexcept
    {
        return this->_stop_source;
    }

    std::stop_token get_stop_token() const noexcept
    {
        return this->_stop_source.get_token();
    }

    bool request_stop() noexcept
    {
        return this->_stop_source.request_stop();
    }
#endif

  private:
    std::thread _inner_thread;
#if __cplusplus >= 202002L // C++20
    std::stop_source _stop_source;
#endif
};

} // namespace tenno
