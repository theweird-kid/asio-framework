#ifndef NET_MESSAGE_HPP
#define NET_MESSAGE_HPP

#include "net_common.hpp"
#include <ostream>

namespace wkd
{
    namespace net
    {
        // Message Header is sent at the start of all messages. The template Allows us
        // to use "enum class" to ensusre that the messages are valid at compile time
        template <typename T>
        struct message_header
        {
            T id{};
            uint32_t size{0};
        };

        template <typename T>
        struct message
        {
            message_header<T> header{};
            std::vector<uint8_t> body;

            // returns the size of entire message packet in bytes
            size_t size() const
            {
                return sizeof(message_header<T>) + body.size();
            }

            // Override for std::cout compatibility - produces friendly description of message
            friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
            {
                os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size();
            }

            // Pushes data into the message buffer
            template <typename DataType>
            friend message<T>& operator << (message<T>& msg, const DataType& data)
            {
                // Check that the type of data being pushed is trivially copyable
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

                // cache current size of vector, as this will be the point we insert the data
                size_t i = msg.body.size();

                // Resize the vector by the size of the data being pushed
                msg.body.resize(msg.body.size() + sizeof(DataType));

                // Physically copy the data into the newly allocated vector space
                std::memcpy(msg.body.data()+i, &data, sizeof(DataType));

                // Recalculate the message size
                msg.header.size = msg.size();

                // Return the target message so it can be "chained"
                return msg;
            }

            // Extract data from the vector
            template <typename DataType>
            friend message<T>& operator >> (message<T>& msg, DataType& data)
            {
                // Check that the type of data being extracted is trivially copyable
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be extracted from vector");

                // Cache the location towards the end of the vector where the pulled data starts
                size_t i = msg.body.size() - sizeof(DataType);

                // Physically copy the data from the vector into the user variable
                std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

                // Shrink the vector to remove read bytes, and reset end position
                msg.body.resize(i);

                // Recalculate the message size
                msg.header.size = msg.size();

                // Return the target message so it can be "chained"
                return msg;
            }
        };

        // Forward declare the connection type
        template <typename T>
        class connection;

        template <typename T>
        struct owned_message
        {
            std::shared_ptr<connection<T>> remote = nullptr;
            message<T> msg;

            friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg)
            {
                os << msg.msg;
                return os;
            }
        };
    }
}

#endif // NET_MESSAGE_HPP
