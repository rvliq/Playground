
#include "stdafx.h"

#include <iostream>
#include <stdexcept>

using namespace std;


namespace playground
{

    // Very simplified version of std::vector container class.
    template<typename T>
    class vector
    {
    public:

        // Constructor.
        explicit vector(size_t defaultItemCount) :
            m_items(new T[defaultItemCount]),
            m_itemCount(defaultItemCount)
        {
            cout << "Create vector" << endl;
        }

        // Copy constructor.
        vector(const vector<T>& other) :
            m_items(new T[other.m_itemCount]),
            m_itemCount(other.m_itemCount)
        {
            for (size_t i = 0; i < m_itemCount; ++i)
            {
                m_items[i] = other.m_items[i];
            }

            cout << "Create vector with copy constructor" << endl;
        }

        // Move constructor.
        vector(vector<T>&& other) :
            m_items(nullptr),
            m_itemCount(0)
        {
            swap(m_items, other.m_items);
            swap(m_itemCount, other.m_itemCount);

            cout << "Create vector with move constructor" << endl;
        }

        // Destructor.
        ~vector()
        {
            delete[] m_items;

            cout << "Delete vector" << endl;
        }

        // Assignment operator.
        const vector& operator=(const vector<T>& other)
        {
            delete[] m_items;

            m_items = new T[other.m_itemCount];
            m_itemCount = other.m_itemCount;

            for (size_t i = 0; i < m_itemCount; ++i)
            {
                m_items[i] = other.m_items[i];
            }

            cout << "Copy vector with assignment operator" << endl;

            return *this;
        }

        // Move assignment operator.
        const vector& operator=(vector<T>&& other)
        {
            swap(m_items, other.m_items);
            swap(m_itemCount, other.m_itemCount);

            cout << "Move vector with move assignment operator" << endl;

            return *this;
        }

        // Subscript operator.
        T& operator[](size_t i)
        {
            if (i > m_itemCount + 1)
            {
                throw out_of_range("Item index our of range");
            }

            // Can also implement as: return *(m_items + i);
            return m_items[i];
        }

    private:

        // Buffer containing item values.
        T* m_items;

        // Amount of items in buffer.
        size_t m_itemCount;
    };

    // Complier applies copy elision for this function. Only the single instance of vector class
    // is created if function is called as: auto r = getValues<T>(count);
    template<typename T>
    vector<T> getValues(size_t count)
    {
        return vector<T>(count);
    }

    // Compiler could not apply return value optimization to this function.
    template<typename T>
    vector<T> getValues(const T& value, size_t count)
    {
        vector<T> values(count);

        for (size_t i = 0; i < count; ++i)
        {
            values[i] = value;
        }

        return values;
    }

    void test()
    {
        // Test: initialize variable with a value returned from function.
        // With RVO the variable value is constructed in-place.
        {
            cout << "Calling function with RVO:" << endl;
            auto v = getValues<int>(1000);
        }

        cout << endl;

        // Test: initialize variable with a value returned from function.
        // Without RVO the intermediate value is created and returned
        // from function (rvalue), and variable is constructed from this
        // value with move constructor.
        {
            cout << "Calling function without RVO (initializing with rvalue):" << endl;
            // No need to specify template parameter when calling this function,
            // it will be deduced by compiler.
            auto v = getValues(1, 1000);
        }

        cout << endl;

        // Test: assign variable with rvalue.
        {
            cout << "Assigning with rvalue:" << endl;
            vector<int> v(10);
            v = getValues<int>(1000);
        }

        cout << endl;

        // Test: initialize variable with lvalue.
        {
            cout << "Initializing with lvalue:" << endl;
            vector<int> v(10);
            vector<int> v2 = v;
        }

        cout << endl;

        // Test: assign variable with lvalue.
        {
            cout << "Assigning with lvalue:" << endl;
            vector<int> v(10);
            vector<int> v2(20);
            v2 = v;
        }
    }
}


int main()
{
    playground::test();
    return 0;
}
