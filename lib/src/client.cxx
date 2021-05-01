#include <client.hxx>

#include <asio/buffer.hpp>
#include <asio/this_coro.hpp>
#include <asio/use_awaitable.hpp>

#include <array>

namespace pc::ntp
{
   namespace
   {
      using net::use_awaitable;
   }

   net::awaitable<Connection>
       Connection::make_connection(string host, string service, udp protocol)
   {
      auto executor = co_await asio::this_coro::executor;

      udp::resolver resolver{executor};
      endpoint_t    receiver_endpoint =
          *(co_await resolver.async_resolve(protocol, host, service, use_awaitable));
      ntp::Connection connection{executor,
                                 receiver_endpoint.protocol(),
                                 std::forward<endpoint_t>(receiver_endpoint)};
      co_return connection;
   }

   net::awaitable<std::size_t> Connection::Packet(ntp::Packet const& sendPacket)
   {
      co_return co_await socket().async_send_to(
          net::buffer(&sendPacket, sizeof(sendPacket)), receiver, use_awaitable);
   }

   net::awaitable<ntp::Packet> Connection::Packet()
   {
      ntp::Packet packet;

      co_await socket().async_receive_from(
          net::buffer(&packet, sizeof(packet)), sender, use_awaitable);
      co_return packet;
   }
   net::awaitable<std::uint32_t> Connection::RawTime()
   {
      {
         ntp::Packet sendPacket;
         // Set li = 0, vn = 3, and mode = 3
         sendPacket.li_vn_mode = 0b00100011;

         co_await Packet(sendPacket);
      }

      auto const packet = co_await Packet();
      co_return packet.refTm_s;
   }
   net::awaitable<std::uint32_t> Connection::TimeFromUnixEpoch()
   {
      auto const time      = co_await RawTime();
      auto const unix_time = ntohl(time) - NTP_EPOCH_UNIX_EPOCH_TIMESTAMP_DELTA;
      co_return unix_time;
   }
} // namespace pc::ntp