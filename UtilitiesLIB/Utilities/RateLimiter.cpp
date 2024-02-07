#include "RateLimiter.hpp"

#include <chrono>

#include <spdlog/spdlog.h>

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
      m_lastUsed(0),
      m_protectConsumption()
  {
  }

  void RateLimiter::consume(uint32_t amount)
  {
    {
      std::lock_guard<std::mutex> guard(m_protectConsumption);

      spdlog::trace("consuming {}. m_lastUsed: {}, m_rate: {}", amount, m_lastUsed.load(), m_rate);
      auto now = steady_clock::now();
      auto diff = now - m_lastAccessed.load();
      uint64_t change = diff / m_timePerToken;
      spdlog::trace("change {}", change);
      m_lastUsed -= std::min(change, m_lastUsed.load());
      m_lastAccessed.store(now);
      auto available = m_rate - m_lastUsed;
      if (available >= amount)
      {
        m_lastUsed.fetch_add(amount);
        return;
      }
      std::this_thread::sleep_for(m_timePerToken * (amount - available));
    }
    consume(amount);
  }
} // namespace utils