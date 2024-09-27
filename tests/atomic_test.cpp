/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

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

#include <tenno/atomic.hpp>
#include <valfuzz/valfuzz.hpp>

TEST(atomic_create, "creating tenno::atomic")
{
    [[maybe_unused]] auto a = tenno::atomic<int>();
}

TEST(atomic_create_pointer, "creating tenno::atomic with pointer")
{
    [[maybe_unused]] auto a = tenno::atomic<int *>();
}

/* generic */

TEST(atomic_generic_is_lock_free, "checking tenno::atomic::is_lock_free()")
{
    struct A
    {
        int a;
    };
    auto a = tenno::atomic<A>();
    ASSERT(!a.is_lock_free());
}

TEST(atomic_int_is_lock_free, "checking tenno::atomic<int>::is_lock_free()")
{
    auto a = tenno::atomic<int>();
    ASSERT(a.is_lock_free());
}

TEST(atomic_store_generic, "storing a value in tenno::atomic")
{
    struct A
    {
        int a;
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
}

TEST(atomic_load_generic, "loading a value from tenno::atomic")
{
    struct A
    {
        int a;
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
    auto b = a.load();
    ASSERT(b.a == 42);
}

TEST(atomic_exchange_generic, "exchanging a value in tenno::atomic")
{
    struct A
    {
        int a;
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
    auto b = a.exchange(A{43});
    ASSERT(b.a == 42);
    auto c = a.load();
    ASSERT(c.a == 43);
}

TEST(atomic_compare_exhange_weak_generic,
     "comparing and exchanging a value in tenno::atomic")
{
    struct A
    {
        int a;
        bool operator==(const A &rhs) const
        {
            return this->a == rhs.a;
        }
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
    A b{42};
    ASSERT(a.compare_exchange_weak(b, A{43}));
    ASSERT(b.a == 42);
    auto c = a.load();
    ASSERT(c.a == 43);
}

TEST(atomic_compare_exhange_weak_generic_fail,
     "comparing and exchanging a value in tenno::atomic")
{
    struct A
    {
        int a;
        bool operator==(const A &rhs) const
        {
            return this->a == rhs.a;
        }
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
    A c{43};
    ASSERT(!a.compare_exchange_weak(c, A{44}));
    ASSERT(c.a == 43);
    auto d = a.load();
    ASSERT(d.a == 42);
}

TEST(atomic_compare_exhange_strong_generic,
     "comparing and exchanging a value in tenno::atomic")
{
    struct A
    {
        int a;
        bool operator==(const A &rhs) const
        {
            return a == rhs.a;
        }
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
    A b{42};
    ASSERT(a.compare_exchange_strong(b, A{43}));
    ASSERT(b.a == 42);
    auto c = a.load();
    ASSERT(c.a == 43);
}

TEST(atomic_compare_exhange_strong_generic_fail,
     "comparing and exchanging a value in tenno::atomic")
{
    struct A
    {
        int a;
        bool operator==(const A &rhs) const
        {
            return a == rhs.a;
        }
    };
    auto a = tenno::atomic<A>();
    a.store(A{42});
    A c{43};
    ASSERT(!a.compare_exchange_strong(c, A{44}));
    ASSERT(c.a == 43);
    auto d = a.load();
    ASSERT(d.a == 42);
}

/* pointer */

TEST(atomic_pointer_is_lock_free,
     "checking tenno::atomic<int *>::is_lock_free()")
{
    auto a = tenno::atomic<int *>();
    ASSERT(!a.is_lock_free());
}

TEST(atomic_pointer_store, "storing a value in tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
}

TEST(atomic_pointer_load, "loading a value from tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
    int c = a.load();
    ASSERT(c == 42);
}

TEST(atomic_pointer_exchange, "exchanging a value in tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
    int c = 43;
    auto d = a.exchange(&c);
    ASSERT(d == 42);
    auto e = a.load();
    ASSERT(e == 43);
}

TEST(atomic_pointer_compare_exhange_weak,
     "comparing and exchanging a value in tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
    int c = 43;
    ASSERT(a.compare_exchange_weak(&b, &c));
    auto d = a.load();
    ASSERT(d == 43);
}

TEST(atomic_pointer_compare_exhange_weak_fail,
     "comparing and exchanging a value in tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
    int c = 43;
    int d = 44;
    ASSERT(!a.compare_exchange_weak(&c, &d));
    auto e = a.load();
    ASSERT(e == 42);
}

TEST(atomic_pointer_compare_exhange_strong,
     "comparing and exchanging a value in tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
    int c = 43;
    ASSERT(a.compare_exchange_strong(&b, &c));
    auto d = a.load();
    ASSERT(d == 43);
}

TEST(atomic_pointer_compare_exhange_strong_fail,
     "comparing and exchanging a value in tenno::atomic<int *>")
{
    auto a = tenno::atomic<int *>();
    int b = 42;
    a.store(&b);
    int c = 43;
    int d = 44;
    ASSERT(!a.compare_exchange_strong(&c, &d));
    auto e = a.load();
    ASSERT(e == 42);
}

/* int specialization */

TEST(atomic_int_store, "storing a value in tenno::atomic<int>")
{
    auto a = tenno::atomic<int>();
    a.store(42);
}

TEST(atomic_int_load, "loading a value from tenno::atomic<int>")
{
    auto a = tenno::atomic<int>();
    a.store(42);
    auto b = a.load();
    ASSERT(b == 42);
}

TEST(atomic_int_exchange_success, "successful tenno::atomic<int>.exchange()")
{
    auto a = tenno::atomic<int>();
    a.store(42);
    auto e = a.exchange(43);
    ASSERT(e == 42);
    ASSERT(a.load() == 43);
}

TEST(atomic_int_compare_exhange_weak_success,
     "succesful tenno::atomic<int>.compare_exchange_weak()")
{
    auto a = tenno::atomic<int>();
    a.store(42);
    auto expected = 42;
    auto b = a.compare_exchange_weak(expected, 43);
    ASSERT(b);
    auto value = a.load();
    ASSERT(value == 43);
}

TEST(atomic_int_compare_exhange_weak_fail,
     "unsuccesfull tenno::atomic<int>.compare_exchange_weak()")
{
    auto a = tenno::atomic<int>();
    a.store(42);
    auto expected = 43;
    auto b = a.compare_exchange_weak(expected, 43);
    ASSERT(!b);
    auto value = a.load();
    ASSERT(value == 42);
}

TEST(atomic_int_compare_exhange_strong_success,
     "succesful tenno::atomic<int>.compare_exchange_strong())")
{
    auto a = tenno::atomic<int>();
    a.store(42);
    auto expected = 42;
    auto b = a.compare_exchange_strong(expected, 43);
    ASSERT(b);
    auto value = a.load();
    ASSERT(value == 43);
}

/* char specialization */

TEST(atomic_char_store, "storing a value in tenno::atomic<char>")
{
    auto a = tenno::atomic<char>();
    a.store('A');
}

TEST(atomic_char_load, "loading a value from tenno::atomic<char>")
{
    auto a = tenno::atomic<char>();
    a.store('A');
    auto b = a.load();
    ASSERT(b == 'A');
}

TEST(atomic_char_exchange_success, "successful tenno::atomic<char>.exchange()")
{
    auto a = tenno::atomic<char>();
    a.store('A');
    auto e = a.exchange('B');
    ASSERT(e == 'A');
    ASSERT(a.load() == 'B');
}

TEST(atomic_char_compare_exhange_weak_success,
     "succesful tenno::atomic<char>.compare_exchange_weak()")
{
    auto a = tenno::atomic<char>();
    a.store('A');
    auto expected = 'A';
    auto b = a.compare_exchange_weak(expected, 'B');
    ASSERT(b);
    auto value = a.load();
    ASSERT(value == 'B');
}

TEST(atomic_char_compare_exhange_weak_fail,
     "unsuccesfull tenno::atomic<char>.compare_exchange_weak()")
{
    auto a = tenno::atomic<char>();
    a.store('A');
    auto expected = 'B';
    auto b = a.compare_exchange_weak(expected, 'B');
    ASSERT(!b);
    auto value = a.load();
    ASSERT(value == 'A');
}

TEST(atomic_char_compare_exhange_strong_success,
     "succesful tenno::atomic<char>.compare_exchange_strong())")
{
    auto a = tenno::atomic<char>();
    a.store('A');
    auto expected = 'A';
    auto b = a.compare_exchange_strong(expected, 'B');
    ASSERT(b);
    auto value = a.load();
    ASSERT(value == 'B');
}

/* long specialization */

TEST(atomic_long_store, "storing a value in tenno::atomic<long>")
{
    auto a = tenno::atomic<long>();
    a.store(42);
}

TEST(atomic_long_load, "loading a value from tenno::atomic<long>")
{
    auto a = tenno::atomic<long>();
    a.store(42);
    auto b = a.load();
    ASSERT(b == 42);
}

TEST(atomic_long_exchange_success, "successful tenno::atomic<long>.exchange()")
{
    auto a = tenno::atomic<long>();
    a.store(42);
    auto e = a.exchange(43);
    ASSERT(e == 42);
    ASSERT(a.load() == 43);
}

TEST(atomic_long_compare_exhange_weak_success,
     "succesful tenno::atomic<long>.compare_exchange_weak()")
{
    auto a = tenno::atomic<long>();
    a.store(42);
    long expected = 42;
    auto b = a.compare_exchange_weak(expected, 43);
    ASSERT(b);
    auto value = a.load();
    ASSERT(value == 43);
}

TEST(atomic_long_compare_exhange_weak_fail,
     "unsuccesfull tenno::atomic<long>.compare_exchange_weak()")
{
    auto a = tenno::atomic<long>();
    a.store(42);
    long expected = 43;
    auto b = a.compare_exchange_weak(expected, 43);
    ASSERT(!b);
    auto value = a.load();
    ASSERT(value == 42);
}

TEST(atomic_long_compare_exhange_strong_success,
     "succesful tenno::atomic<long>.compare_exchange_strong())")
{
    auto a = tenno::atomic<long>();
    a.store(42);
    long expected = 42;
    auto b = a.compare_exchange_strong(expected, 43);
    ASSERT(b);
    auto value = a.load();
    ASSERT(value == 43);
}
