#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <vector>
#include <string>
#include <memory>
#include <boost/fiber/buffered_channel.hpp>

#include "serialization.hpp"

template <typename T>
class Channel {
    using SPByteChan = std::shared_ptr<boost::fibers::buffered_channel<std::vector<unsigned char>>>;

 public:
    class ChannelPublisher;
    class ChannelSubscriber;

    Channel(const std::string &name) {
        sp_byte_chan = std::make_shared<boost::fibers::buffered_channel<std::vector<unsigned char>>>(128);
    }

    ChannelPublisher get_publisher() {
        return ChannelPublisher(sp_byte_chan);
    }

    ChannelSubscriber get_subscriber() {
        return ChannelSubscriber(sp_byte_chan);
    }

    class ChannelPublisher {
     public:
        ChannelPublisher(SPByteChan sp_byte_chan) : sp_byte_chan(sp_byte_chan) {}
        void produce(const T &val) {
            auto bytes = serialize(val);
            sp_byte_chan->push(std::move(bytes));
        }
     private:
        SPByteChan sp_byte_chan;
    };

    class ChannelSubscriber {
     public:
        ChannelSubscriber(SPByteChan sp_byte_chan) : sp_byte_chan(sp_byte_chan) {}
        T consume() {
            auto bytes = sp_byte_chan->value_pop();
            auto object = deserialize<T>(bytes.data(), bytes.data() + bytes.size());
            return object;
        }
     private:
        SPByteChan sp_byte_chan;
    };

 private:
    SPByteChan sp_byte_chan;
};

using CompletionChannel = Channel<CompletionStatus>;
using CompletionChannelVec = std::vector<CompletionChannel>;
using MergeChannel = Channel<MergeStatus>;
using MergeChannelVec = std::vector<MergeChannel>;

#endif
