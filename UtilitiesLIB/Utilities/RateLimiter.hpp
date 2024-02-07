#ifndef UTILITIES_RATELIMITER_HPP
#define UTILITIES_RATELIMITER_HPP

#include <atomic>
#include <chrono>
#include <mutex>

namespace utils
{
  class RateLimiter
  {
  public:
    RateLimiter(uint32_t limitPerSecond);

    /* Blocking call. Will consume as many tokens as asked for. Don't do something dumb like request
     * to use more that is available, since this doesn't have error handling*/
    void consume(uint32_t amount);

  private:
    const std::chrono::nanoseconds m_timePerToken;
    const uint32_t m_rate;
    std::atomic<std::chrono::steady_clock::time_point> m_lastAccessed;
    std::atomic_uint64_t m_lastUsed;
    std::mutex m_protectConsumption;
  };
} // namespace utils

#endif