#pragma once

// Source https://lettier.github.io/posts/2016-04-26-lets-make-a-ntp-client-in-c.html
#include <cstdint>
namespace pc::ntp
{
   namespace
   {
      using std::uint32_t;
      using std::uint8_t;
   } // namespace
   struct Packet
   {
      uint8_t li_vn_mode = 0; // Eight bits. li, vn, and mode.
                              // li.   Two bits.   Leap indicator.
                              // vn.   Three bits. Version number of the protocol.
                              // mode. Three bits. Client will pick mode 3 for client.

      uint8_t stratum   = 0; // Eight bits. Stratum level of the local clock.
      uint8_t poll      = 0; // Eight bits. Maximum interval between successive messages.
      uint8_t precision = 0; // Eight bits. Precision of the local clock.

      uint32_t rootDelay      = 0; // 32 bits. Total round trip delay time.
      uint32_t rootDispersion = 0; // 32 bits. Max error aloud from primary clock source.
      uint32_t refId          = 0; // 32 bits. Reference clock identifier.

      uint32_t refTm_s = 0; // 32 bits. Reference time-stamp seconds.
      uint32_t refTm_f = 0; // 32 bits. Reference time-stamp fraction of a second.

      uint32_t origTm_s = 0; // 32 bits. Originate time-stamp seconds.
      uint32_t origTm_f = 0; // 32 bits. Originate time-stamp fraction of a second.

      uint32_t rxTm_s = 0; // 32 bits. Received time-stamp seconds.
      uint32_t rxTm_f = 0; // 32 bits. Received time-stamp fraction of a second.

      uint32_t txTm_s = 0; // 32 bits. Transmit time-stamp seconds.
      uint32_t txTm_f = 0; // 32 bits. Transmit time-stamp fraction of a second.   };
   };
} // namespace pc::ntp