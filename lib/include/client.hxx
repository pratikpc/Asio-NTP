#pragma once

#include <Packet.hxx>

#include <asio/awaitable.hpp>
#include <asio/ip/udp.hpp>

namespace pc::ntp
{
   namespace
   {
      namespace net    = asio;
      using udp        = net::ip::udp;
      using socket_t   = udp::socket;
      using endpoint_t = udp::endpoint;
      using std::string;
   } // namespace

   struct Connection
   {
      // The difference in time between the Unix Epoch and the NTP Delta Timestamp
      static constexpr time_t NTP_EPOCH_UNIX_EPOCH_TIMESTAMP_DELTA = 2'20'89'88'800;

      static net::awaitable<Connection>

          make_connection(string host, string service = "ntp", udp protocol = udp::v4());
      template <typename Executor>
      Connection(Executor&                               executor,
                 typename socket_t::protocol_type const& protocol,
                 endpoint_t&&                            receiver) :
          socket_{executor, protocol},
          receiver{std::forward<endpoint_t>(receiver)}
      {
      }

      socket_t& socket() noexcept
      {
         return socket_;
      }

      net::awaitable<std::size_t> Packet(ntp::Packet const& sendPacket);

      net::awaitable<ntp::Packet>   Packet();
      net::awaitable<std::uint32_t> RawTime();
      net::awaitable<std::uint32_t> TimeFromUnixEpoch();

    private:
      socket_t   socket_;
      endpoint_t receiver;
      endpoint_t sender;
   };
} // namespace pc::ntp
