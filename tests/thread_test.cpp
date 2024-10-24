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

#include <tenno/thread.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(jthread_create, "tenno::jthread creation")
{
    int i = 0;
    tenno::jthread t1(
        [&i]()
        {
            i = 10;
        });
    ASSERT(t1.joinable());
}

TEST(jthread_swap, "tenno::jthread swap")
{
    int i = 0;
    tenno::jthread t1(
        [&i]()
        {
            i = 10;
        });
    int j = 0;
    tenno::jthread t2(
        [&j]()
        {
            j = 10;
        });
    std::thread::id id1 = t1.id;
    std::thread::id id2 = t2.id;
    t1.swap(t2);
    ASSERT(t1.joinable());
    ASSERT(t2.joinable());
    ASSERT(t1.id == id2);
    ASSERT(t2.id == id1);
}
