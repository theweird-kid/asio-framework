#ifndef NET_CONNECTION_HPP
#define NET_CONNECTION_HPP

#include "net_common.hpp"
#include "net_thread_safe_queue.hpp"
#include "net_message.hpp"
#include <asio/impl/read.hpp>
#include <asio/impl/write.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <cstdint>
#include <memory>
#include <system_error>

namespace wkd
{
    namespace net
    {
        template <typename T>
        class connection : public std::enable_shared_from_this<connection<T>>
        {
        public:

            // Owner Type
            enum class owner
            {
                server,
                client
            };

            // Constructor
            connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket,
                thread_safe_queue<owned_message<T>>& qIn)
            : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessageIn(qIn)
            {
                m_nOwnerType = parent;
            }

            // Destructor
            virtual ~connection() {}

            // returns the ID of the connection
            uint32_t GetID() const
            {
                return m_id;
            }

        public:

            // Connect to client (Only Applicable for Servers)
            void ConnectToClient(uint32_t uid = 0)
            {
                if(m_nOwnerType == owner::server)
                {
                    if(m_socket.is_open())
                    {
                        m_id = uid;
                        ReadHeader();
                    }
                }
            }

            // Connect to server
            bool ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
            {
                // Only clients can connect to servers
                if(m_nOwnerType == owner::client)
                {
                    // Request asio attempts to connect to an endpoint
                    asio::async_connect(m_socket, endpoints,
                    [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                    {
                        if(!ec)
                        {
                            ReadHeader();
                        }
                    });
                }
                return true;
            }

            bool Disconnect()
            {
                if(IsConnected())
                {
                    asio::post(m_asioContext, [this]() { m_socket.close(); });
                    return true;
                }
                return false;
            }

            // retuns true if connected
            bool IsConnected() const
            {
                return m_socket.is_open();
            }

            // Sends msg back to remote
            void Send(const message<T>& msg)
            {
                asio::post(m_asioContext,
                [this, msg]()
                {
                    bool bWritingMessage = !m_qMessageOut.empty();
                    m_qMessageOut.push_back(msg);
                    if(!bWritingMessage)
                    {
                        WriteHeader();
                    }
                });
            }

        private:
            // ASYNC - Prime context ready to read a message header
            void ReadHeader()
            {
                asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        if(m_msgTemporaryIn.header.size > 0)
                        {
                            m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
                            ReadBody();
                        }
                        else
                        {
                            AddToIncomingMessageQueue();
                        }
                    }
                    else
                    {
                        std::cout << "[" << m_id << "] Read Header Failed\n";
                        m_socket.close();
                    }
                });
            }

            // ASYNC - Prime context ready to read a message body
            void ReadBody()
            {
                asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
                [this](std::error_code ec, std::size_t length)
                {
                    if(!ec)
                    {
                        AddToIncomingMessageQueue();
                    }
                    else
                    {
                        std::cout << "[" << m_id << "] Read Body Fail." << std::endl;
                        m_socket.close();
                    }
                });
            }

            // ASYNC - Prime context to write a message header
            void WriteHeader()
            {
                asio::async_write(m_socket, asio::buffer(&m_qMessageOut.front().header, sizeof(message_header<T>)),
                [this](std::error_code ec, std::size_t length)
                {
                    if(!ec)
                    {
                        if(m_qMessageOut.front().body.size() > 0)
                        {
                            WriteBody();
                        }
                        else
                        {
                            m_qMessageOut.pop_front();
                            if(!m_qMessageOut.empty())
                            {
                                WriteHeader();
                            }
                        }
                    }
                    else
                    {
                        std::cout << "[" << m_id << "] Write Header Fail." << std::endl;
                        m_socket.close();
                    }
                });
            }

            // ASYNC - Prime context to write a message body
            void WriteBody()
            {
                asio::async_write(m_socket, asio::buffer(m_qMessageOut.front().body.data(), m_qMessageOut.front().body.size()),
                [this](std::error_code ec, std::size_t length)
                {
                    if(!ec)
                    {
                        m_qMessageOut.pop_front();
                        if(!m_qMessageOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                    else
                    {
                        std::cout << "[" << m_id << "] Write Body Fail." << std::endl;
                        m_socket.close();
                    }
                });
            }

            // Add incoming message to the queue
            void AddToIncomingMessageQueue()
            {
                if(m_nOwnerType == owner::server)
                {
                    m_qMessageIn.push_back({this->shared_from_this(), m_msgTemporaryIn});
                }
                else
                {
                    m_qMessageIn.push_back({nullptr, m_msgTemporaryIn});
                }

                // Register Another task for asio to read another header
                ReadHeader();
            }

        protected:
            // each connection has a unique socket to remote
            asio::ip::tcp::socket m_socket;

            // this context is shared with the whole asio instance
            asio::io_context& m_asioContext;

            // holds all the messages to be sent to remote
            thread_safe_queue<message<T>> m_qMessageOut;

            // holds all the messages that have been recieved from the remote side of this connection
            // NOTE: It is a reference as the "owner" of this connection is expected to provide a queue
            thread_safe_queue<owned_message<T>>& m_qMessageIn;

            // temporary incoming message buffer
            message<T> m_msgTemporaryIn;

            // owner of the connection
            owner m_nOwnerType = owner::server;
            // Identifier
            uint32_t m_id = 0;
        };
    }
}


#endif // NET_CONNECTION_HPP
