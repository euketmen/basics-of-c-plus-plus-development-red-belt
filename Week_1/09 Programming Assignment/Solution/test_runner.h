#pragma once

#ifndef _TEST_RUNNER_H_
#define _TEST_RUNNER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

//-------------------------------------------------------------------------------------------------
class TestRunner
{
public:
    template <typename test_func>
    void RunTest(test_func func, const std::string& test_name);

    TestRunner() = default;
    ~TestRunner()
    {
        if (fail_count > 0)
        {
            std::cerr << "========================================" << std::endl;
            std::cerr << "FAILED (failures = " << fail_count << ")" << std::endl;
            std::cerr << "========================================" << std::endl;
            exit(1);
        }
        else
        {
            std::cerr << "========================================" << std::endl;
            std::cerr << "OK" << std::endl;
            std::cerr << "========================================" << std::endl;
        }
    }
private:
    int fail_count = 0;
};
//-------------------------------------------------------------------------------------------------
template <typename test_func>
void TestRunner::RunTest(test_func func, const std::string& test_name)
{
    try
    {
        func();
        std::cerr << "PASSED: " << test_name << std::endl;
    }
    catch (const std::runtime_error & e)
    {
        ++fail_count;
        std::cerr << "FAIL: " << test_name << e.what() << std::endl;
    }
}
//-------------------------------------------------------------------------------------------------
template <typename T>
std::ostream& operator << (std::ostream& os, const std::set<T>& s)
{
    os << "{";
    bool first = true;
    for (const auto& x : s)
    {
        if (!first)
        {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}
//-------------------------------------------------------------------------------------------------
template <typename Key, typename Value>
std::ostream& operator << (std::ostream& os, const std::map<Key, Value>& map)
{
    os << "{";
    bool first = true;
    for (const auto& kv : map)
    {
        if (!first)
        {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}
//-------------------------------------------------------------------------------------------------
template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& s)
{
    os << "{";
    bool first = true;
    for (const auto& x : s)
    {
        if (!first)
        {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}
//-------------------------------------------------------------------------------------------------
template<typename T, typename U>
void AssertEqual(const T& t, const U& u, const std::string& hint)
{
    if (t != u)
    {
        std::ostringstream os;
        os << " Assertion failed: " << std::endl;
        os << t << " != " << u << " hint: " << hint;
        throw std::runtime_error(os.str());
    }
}
//-------------------------------------------------------------------------------------------------
void Assert(bool b, const std::string& hint)
{
    AssertEqual(b, true, hint);
}
//-------------------------------------------------------------------------------------------------
#define ASSERT_EQUAL(x, y)                       \
{                                                \
	auto calc_x = x;							 \
	auto calc_y = y;							 \
    {                                            \
        std::ostringstream os;                   \
        os << #x << " != " << #y << ", "         \
		<< __FILE__ << ":" << __LINE__;       	 \
        AssertEqual(calc_x, calc_y, os.str());   \
    }											 \
}
//-------------------------------------------------------------------------------------------------
#define ASSERT(x)                         \
{                                         \
    std::ostringstream os;                \
    os << #x << " is false, ";            \
    os << __FILE__ << ":" << __LINE__;    \
    Assert(x, os.str());                  \
}
//-------------------------------------------------------------------------------------------------
#define RUN_TEST(tr, func)    \
{                             \
    tr.RunTest(func, #func);  \
}
//-------------------------------------------------------------------------------------------------
#endif // _TEST_RUNNER_H_