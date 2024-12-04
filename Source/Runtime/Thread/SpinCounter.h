#pragma once

namespace StepEngine
{
    class SpinCounter
    {
    public:
        SpinCounter(unsigned int maxSpinCount) : m_maxSpinCount(maxSpinCount), m_curSpinCount(0) {}
        void Spin();
    private:
        unsigned int m_maxSpinCount;
        unsigned int m_curSpinCount;
    };
}
