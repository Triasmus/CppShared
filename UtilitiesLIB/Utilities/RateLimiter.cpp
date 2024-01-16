#include "RateLimiter.hpp"

#include <chrono>

namespace
{
  using namespace std::chrono;
}

namespace utils
{
  RateLimiter::RateLimiter(uint32_t rate)
    : m_timePerToken(nanoseconds(seconds(1)) / rate + nanoseconds(1)),
      m_rate(rate),
      m_lastAccessed(steady_clock::time_point::min()),
      m_lastUsed(0)
  {
  }

  void RateLimiter::consume(uint32_t amount)
  {
    auto now = steady_clock::now();
    auto diff = now - m_lastAccessed.load();
    uint64_t change = diff / m_timePerToken;
    m_lastUsed -= std::max(change, m_lastUsed.load());
    m_lastAccessed.store(now);
    auto available = m_rate - m_lastUsed;
    if (available >= amount)
      m_lastUsed.fetch_add(amount);
    else
    {
      std::this_thread::sleep_for(m_timePerToken * (amount - available));
      consume(amount);
    }
  }
} // namespace utils