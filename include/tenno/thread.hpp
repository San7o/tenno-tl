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

class jthread
{
  public:
    using native_handle_type = std::thread::native_handle_type;

    std::thread::id id;

    jthread() noexcept = default;
    jthread( const jthread& ) = delete;
    jthread([[maybe_unused]] jthread&& other ) noexcept
    {
        // TODO
    }

    template< class F, class... Args >
    explicit jthread( F&& f, Args&&... args )
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

    std::jthread::id get_id() const noexcept
    {
        return this->_inner_thread.get_id();
    }

    native_handle_type native_handle()
    {
        return this->_inner_thread.native_handle();
    }

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
    void swap([[maybe_unused]] tenno::jthread& other ) noexcept; // TODO

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

  private:
    std::thread _inner_thread;
    std::stop_source _stop_source;
};

} // namespace tenno
