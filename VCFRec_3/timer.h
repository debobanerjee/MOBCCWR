//
// Created by Shubham on 3/24/2020.
//

#ifndef CODE_TIMER_H
#define CODE_TIMER_H

#include <bits/stdc++.h>
using namespace std;

/** To check Time Limit **/
class Timer {
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};
/** END **/

#endif //CODE_TIMER_H
