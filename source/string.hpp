#pragma once


#include "memory/buffer.hpp"
#include "number/numeric.hpp"



inline u32 str_len(const char* str)
{
    const char* s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}


inline void str_reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;

    while (start < end) {
        std::swap(*(str + start), *(str + end));
        start++;
        end--;
    }
}


inline int str_cmp(const char* p1, const char* p2)
{
    const unsigned char* s1 = (const unsigned char*)p1;
    const unsigned char* s2 = (const unsigned char*)p2;

    unsigned char c1, c2;

    do {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;

        if (c1 == '\0') {
            return c1 - c2;
        }

    } while (c1 == c2);

    return c1 - c2;
}


// NOTE: Capacity is a holdover from before StringAdapter was a template,
// originally, the class was backed by a Buffer<>. Eventually maybe I'll
// refactor out the parameter.
//
// NOTE: Capacity ideally wouldn't be a template parameter, but I haven't
// removed it yet, as we do want to be able to copy a string with a smaller
// capacity into a larger one, without compiler errors due to mismatched
// types. Of course, there are solutions, but I have to make some changes.
template <u32 Capacity, typename Memory> class StringAdapter {
public:
    using Buffer = Memory;

    template <typename... MemArgs>
    StringAdapter(const char* init, MemArgs&&... mem_args)
        : mem_(std::forward<MemArgs>(mem_args)...)
    {
        mem_.push_back('\0');
        (*this) += init;
    }

    template <typename... MemArgs>
    StringAdapter(char c, u32 count, MemArgs&&... mem_args)
        : mem_(std::forward<MemArgs>(mem_args)...)
    {
        while (count--) {
            mem_.push_back(c);
        }
    }

    StringAdapter()
    {
        mem_.push_back('\0');
    }

    StringAdapter(const StringAdapter& other)
    {
        clear();

        for (auto it = other.begin(); it not_eq other.end(); ++it) {
            push_back(*it);
        }
    }

    const StringAdapter& operator=(const StringAdapter& other)
    {
        clear();

        for (auto it = other.begin(); it not_eq other.end(); ++it) {
            push_back(*it);
        }
        return *this;
    }


    const StringAdapter& operator=(StringAdapter&& other)
    {
        clear();

        for (auto it = other.begin(); it not_eq other.end(); ++it) {
            push_back(*it);
        }
        return *this;
    }


    template <u32 OtherCapacity, typename OtherMem>
    StringAdapter(const StringAdapter<OtherCapacity, OtherMem>& other)
    {
        static_assert(OtherCapacity <= Capacity);

        clear();

        for (auto it = other.begin(); it not_eq other.end(); ++it) {
            push_back(*it);
        }
    }

    template <u32 OtherCapacity, typename OtherMem>
    const StringAdapter&
    operator=(const StringAdapter<OtherCapacity, OtherMem>& other)
    {
        clear();

        for (auto it = other.begin(); it not_eq other.end(); ++it) {
            push_back(*it);
        }
        return *this;
    }

    char& operator[](int pos)
    {
        return mem_[pos];
    }

    void push_back(char c)
    {
        if (not mem_.full()) {
            mem_[mem_.size() - 1] = c;
            mem_.push_back('\0');
        }
    }

    void pop_back()
    {
        mem_.pop_back();
        mem_.pop_back();
        mem_.push_back('\0');
    }

    typename Buffer::Iterator begin() const
    {
        return mem_.begin();
    }

    typename Buffer::Iterator end() const
    {
        return mem_.end() - 1;
    }

    typename Buffer::Iterator insert(typename Buffer::Iterator pos, char val)
    {
        return mem_.insert(pos, val);
    }

    StringAdapter& operator+=(const char* str)
    {
        while (*str not_eq '\0') {
            push_back(*(str++));
        }
        return *this;
    }

    template <u32 OtherCapacity, typename OtherMem>
    StringAdapter&
    operator+=(const StringAdapter<OtherCapacity, OtherMem>& other)
    {
        (*this) += other.c_str();
        return *this;
    }

    StringAdapter& operator=(const char* str)
    {
        this->clear();

        *this += str;

        return *this;
    }

    bool operator==(const char* str)
    {
        return str_cmp(str, this->c_str()) == 0;
    }

    bool full() const
    {
        return mem_.full();
    }

    u32 length() const
    {
        return mem_.size() - 1;
    }

    bool empty() const
    {
        return mem_.size() == 1;
    }

    void clear()
    {
        mem_.clear();
        mem_.push_back('\0');
    }

    const char* c_str() const
    {
        return mem_.data();
    }

    u32 remaining() const
    {
        return (mem_.capacity() - 1) - mem_.size();
    }

private:
    Buffer mem_;
};


template <u32 Capacity>
using StringBuffer = StringAdapter<Capacity, Buffer<char, Capacity + 1>>;



template <u32 Capacity, typename Mem>
bool is_numeric(const StringAdapter<Capacity, Mem>& buf)
{
    for (auto c : buf) {
        if (c < '0' or c > '9') {
            return false;
        }
    }

    return true;
}


template <u32 Capacity, typename Mem>
bool operator==(StringAdapter<Capacity, Mem> buf, const char* str)
{
    return str_cmp(str, buf.c_str()) == 0;
}


// I was trying to track down certain bugs, where invalid strings were being
// passed to str_len.
inline bool validate_str(const char* str)
{
    for (int i = 0; i < 100000; ++i) {
        if (str[i] == '\0') {
            return true;
        }
    }
    return false;
}
