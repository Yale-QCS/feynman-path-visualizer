#ifndef FAYMANN_HPP_
#define FAYMANN_HPP_

#include "types.hpp"
#include "channel.hpp"

void faymann_path_propagate(CompletionChannel::ChannelPublisher compl_pub,
                            const std::string &gate_key,
                            const std::string &storage_name,
                            bool with_amp,
                            int cur_id, int cur_level);

void faymann_path_distributed_with_propagation(const GatesOnQubit &gates, std::string *p_out_buf,
                                               int qubit_num, bool with_amp);

void faymann_path_merge_state_and_store(
    MergeChannel::ChannelPublisher merge_pub,
    const StorageKeys &keys, int depth);

#endif
