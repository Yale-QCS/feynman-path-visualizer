#include <thread>

#include "spawn.hpp"
#include "faymann.hpp"

Future spawn_faymann_async(
    CompletionChannel::ChannelPublisher compl_pub,
    const std::string &gate_key,
    const std::string &storage_name,
    bool with_amp,
    int cur_id, int cur_level) {

    return std::async(std::launch::async, faymann_path_propagate, compl_pub,
                      gate_key, storage_name, with_amp, cur_id, cur_level);
}

Future spawn_faymann_merge_state_and_store(
    MergeChannel::ChannelPublisher merge_pub,
    const StorageKeys &keys, int cur_depth) {
    return std::async(std::launch::async, faymann_path_merge_state_and_store,
                      merge_pub, keys, cur_depth);
}
