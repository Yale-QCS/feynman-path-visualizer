#ifndef SPAWN_HPP_
#define SPAWN_HPP_

#include <future>
#include <memory>

#include "types.hpp"
#include "channel.hpp"

using Future = std::future<void>;
using SPFuture = std::shared_ptr<Future>;

Future spawn_faymann_async(
    CompletionChannel::ChannelPublisher compl_pub,
    const std::string &gate_key,
    const std::string &storage_name,
    bool with_amp,
    int cur_id, int cur_level);

Future spawn_faymann_merge_state_and_store(
    MergeChannel::ChannelPublisher merge_pub,
    const StorageKeys &keys, int cur_depth);


#endif
