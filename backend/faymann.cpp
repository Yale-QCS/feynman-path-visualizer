#include <rapidjson/document.h>

#include "name.hpp"
#include "types.hpp"
#include "gates.hpp"
#include "spawn.hpp"
#include "kvstore.hpp"
#include "channel.hpp"
#include "faymann.hpp"
#include "serialization.hpp"

#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>

KVStore kv_store;

static std::string int_to_bin_str(unsigned long val, std::size_t width) {
    std::string str;
    str.reserve(width);

    while (val != 0) {
        if (val & 1) str += '1';
        else str += '0';
        val >>= 1;
    }
    while (str.size() < width) str += '0';
    return std::string(str.rbegin(), str.rend());
}

static void faymann_path_advance_state(const SparseState &current_state, const ComputationPaths &paths,
                                       SparseState *new_state, bool with_amp) {
    new_state->clear();

    if (with_amp) {
        for (const auto &basis_and_weight : current_state) {
            const auto &basis = basis_and_weight.first;
            const auto &amplitude = basis_and_weight.second;
            const auto &paths_from_a_basis = paths.at(basis);

            for (const auto &path : paths_from_a_basis) {
                const auto &target_basis = path.first;
                const auto &path_weight = path.second;

                auto it_target_basis = new_state->find(target_basis);
                if (it_target_basis == new_state->end())
                    it_target_basis = new_state->insert(std::make_pair(target_basis, BasisWeight(0.0, 0.0))).first;
                it_target_basis->second += amplitude * path_weight;
            }
        }
    } else {
        for (const auto &basis_and_weight : current_state) {
            const auto &basis = basis_and_weight.first;
            const auto &paths_from_a_basis = paths.at(basis);

            for (const auto &path : paths_from_a_basis) {
                const auto &target_basis = path.first;

                auto it_target_basis = new_state->find(target_basis);
                if (it_target_basis == new_state->end())
                    it_target_basis = new_state->insert(std::make_pair(target_basis, BasisWeight(1.0, 0.0))).first;
            }
        }
    }
}

static SingleStepResult faymann_path_distributed_single_step(const GateOnQubit &gate,
                                                             const SparseState &state,
                                                             bool with_amp, int id, int depth) {
    ActiveBasisIdxes active_basis, new_active_basis;
    ComputationPaths paths;

    active_basis.reserve(state.size());
    new_active_basis.reserve(state.size() * 2);

    for (const auto &basis_with_weight : state)
        active_basis.insert(basis_with_weight.first);

    switch (gate.width)
    {
    case GateWidth::GATE_SINGLE_QUBIT:
        map_active_basis_single_qubit_gate(gate.gate.single_qubit, active_basis, &paths, &new_active_basis);
        break;
    case GateWidth::GATE_DOUBLE_QUBIT:
        map_active_basis_double_qubit_gate(gate.gate.double_qubit, active_basis, &paths, &new_active_basis);
        break;
    default:
        throw;
    }

    SingleStepResult result;
    SparseState new_state;

    faymann_path_advance_state(state, paths, &new_state, with_amp);

    if (new_active_basis.size() >= SPLIT_THRESHOLD) {
        auto parity = 0;
        SparseState split_new_states[2];
        for (const auto &basis_with_weight : new_state) {
            split_new_states[parity].insert(basis_with_weight);
            parity = 1 - parity;
        }
        auto name0 = get_storage_name("", id, depth, 0);
        kv_store.put(name0, serialize(split_new_states[0]));
        auto name1 = get_storage_name("", id, depth, 1);
        kv_store.put(name1, serialize(split_new_states[1]));

        result.child_num = 2;
        result.keys[0] = std::move(name0);
        result.keys[1] = std::move(name1);
    } else {
        auto name = get_storage_name("", id, depth, 0);
        kv_store.put(name, serialize(new_state));

        result.child_num = 1;
        result.keys[0] = std::move(name);
    }

    return result;
}

static std::shared_ptr<ActiveBasisIdxes> faymann_path_merge_basis(const std::vector<std::shared_ptr<ActiveBasisIdxes>> &vec_active_basis) {
    auto up_merged_active_basis = std::make_shared<ActiveBasisIdxes>();

    for (const auto &up_active_basis : vec_active_basis)
        up_merged_active_basis->insert(up_active_basis->begin(), up_active_basis->end());

    return up_merged_active_basis;
}

static std::shared_ptr<SparseState> faymann_path_merge_state(const std::vector<std::shared_ptr<SparseState>> &vec_states) {
    auto up_merged_state = std::make_shared<SparseState>();

    for (const auto &up_state : vec_states) {
        for (const auto &basis_with_weight : *up_state) {
            const auto &basis = basis_with_weight.first;
            const auto &weight = basis_with_weight.second;
            (*up_merged_state)[basis] += weight;
        }
    }

    return up_merged_state;
}

void faymann_path_propagate(CompletionChannel::ChannelPublisher compl_pub,
                            const std::string &gate_key,
                            const std::string &storage_name,
                            bool with_amp,
                            int cur_id, int cur_level) {

    auto state_bytes = kv_store.get(storage_name);
    auto state = deserialize<SparseState>(state_bytes.data(), state_bytes.data() + state_bytes.size());

    auto gate_bytes = kv_store.get(gate_key);
    auto gate = deserialize<GateOnQubit>(gate_bytes.data(), gate_bytes.data() + gate_bytes.size());

    auto result = faymann_path_distributed_single_step(gate, state, with_amp, cur_id, cur_level);
    CompletionStatus up_exec_status;
    up_exec_status.child_num = result.child_num;
    up_exec_status.id = cur_id;
    up_exec_status.code = CompletionStatusCode::E_SUCCESS;
    up_exec_status.keys[0] = std::move(result.keys[0]);
    up_exec_status.keys[1] = std::move(result.keys[1]);

    compl_pub.produce(up_exec_status);
}

static void faymann_path_distributed_print_state(const StorageKeys &keys) {
    std::vector<std::shared_ptr<SparseState>> vec_state;

    for (const auto key : keys) {
        auto bytes_state = kv_store.get(key);
        auto state = deserialize<SparseState>(bytes_state.data(), bytes_state.data() + bytes_state.size());
        vec_state.push_back(std::make_shared<SparseState>(std::move(state)));
    }

    auto merged_state = faymann_path_merge_state(vec_state);
    for (const auto &basis_amp : *merged_state) {
        std::cout << std::bitset<3>(basis_amp.first) << '\t' << basis_amp.second << std::endl;
    }
    std::cout << std::endl;
}

void faymann_path_merge_state_and_store(
    MergeChannel::ChannelPublisher merge_pub,
    const StorageKeys &keys, int depth) {
    std::vector<std::shared_ptr<SparseState>> vec_state;

    for (const auto key : keys) {
        auto bytes_state = kv_store.get(key);
        auto state = deserialize<SparseState>(bytes_state.data(), bytes_state.data() + bytes_state.size());
        vec_state.push_back(std::make_shared<SparseState>(std::move(state)));
    }

    auto sp_merged_state = faymann_path_merge_state(vec_state);
    auto bytes_merged_state = serialize(*sp_merged_state);
    StorageKey key = "merged" + std::to_string(depth);

    kv_store.put(key, std::move(bytes_merged_state));

    MergeStatus status;
    status.depth = depth;
    status.key = std::move(key);

    merge_pub.produce(status);
}

void faymann_path_distributed_with_propagation(const GatesOnQubit &gates, std::string *p_out_buf,
                                               int qubit_num, bool with_amp) {

    rapidjson::Document json_dom;
    json_dom.SetObject();
    auto &json_dom_alloc = json_dom.GetAllocator();
    rapidjson::Value amp_array(rapidjson::kArrayType);

    auto num_gates = gates.size();

    for (auto i = 0ul; i < num_gates; ++i) {
        auto name = get_gate_name("", i);
        kv_store.put(name, serialize(gates[i]));
    }

    auto initial_gate_key = get_gate_name("", 0);

    auto initial_state = SparseState{std::make_pair(0ull, BasisWeight(1.0, 0.0))};

    auto state_key = get_storage_name("", -1, -1, -1);
    kv_store.put(state_key, serialize(initial_state));

    CompletionChannelVec vec_comp_chan;
    MergeChannelVec vec_merge_chan;
    std::vector<CompletionChannel::ChannelSubscriber> vec_comp_subscribers;
    std::vector<MergeChannel::ChannelSubscriber> vec_merge_subscribers;
    for (auto i = 0ul; i < num_gates; ++i) {
        vec_comp_chan.emplace_back("worker_channel" + std::to_string(i));
        vec_comp_subscribers.push_back(vec_comp_chan.back().get_subscriber());
        vec_merge_chan.emplace_back("merge_channel" + std::to_string(i));
        vec_merge_subscribers.push_back(vec_merge_chan.back().get_subscriber());
    }

    auto initial_publisher = vec_comp_chan.begin()->get_publisher();

    auto sp_future = spawn_faymann_async(initial_publisher, initial_gate_key, state_key, with_amp, 0, 0);

    std::vector<Future> futures;
    futures.push_back(std::move(sp_future));

    auto child_num = 1;
    for (auto depth = 0ul; depth < num_gates; ++depth) {
        auto new_child_num = 0;
        auto &comp_subscriber = vec_comp_subscribers[depth];
        std::vector<StorageKey> keys;

        for (auto &fut : futures) {
            fut.wait();
        }
        futures.clear();

        while (child_num-- > 0) {
            auto status = comp_subscriber.consume();
            new_child_num += status.child_num;
            for (auto i = 0; i < status.child_num; ++i)
                keys.emplace_back(std::move(status.keys[i]));
        }
        child_num = new_child_num;

        if (depth < num_gates - 1) {
            auto gate_key = get_gate_name("", depth + 1);
            auto publisher = vec_comp_chan[depth + 1].get_publisher();
            for (auto id = 0; id < child_num; ++id) {
                auto key = keys[id];
                auto fut = spawn_faymann_async(publisher, gate_key, key, with_amp, id, depth + 1);
                futures.push_back(std::move(fut));
            }
        }

        spawn_faymann_merge_state_and_store(vec_merge_chan[depth].get_publisher(), keys, depth).wait();
        auto &merge_subscriber = vec_merge_subscribers[depth];
        auto merge_status = merge_subscriber.consume();
        auto merged_state_bytes = kv_store.get(merge_status.key);
        auto merged_state = deserialize<SparseState>(merged_state_bytes.data(), merged_state_bytes.data() + merged_state_bytes.size());
        for (const auto &basis_amp : merged_state) {
            std::cout << std::bitset<3>(basis_amp.first) << '\t' << basis_amp.second << std::endl;
        }
        std::cout << std::endl;

        rapidjson::Value obj;
        obj.SetObject();
        for (const auto &basis_amp : merged_state) {
            const auto &basis = basis_amp.first;
            const auto &amplitude = basis_amp.second;

            if (amplitude.imag() * amplitude.imag() + amplitude.real() * amplitude.real()
                < TRUNCATE_THRESHOLD) continue;

            rapidjson::Value amp_pair(rapidjson::kArrayType);
            amp_pair.PushBack(amplitude.real(), json_dom_alloc);
            amp_pair.PushBack(amplitude.imag(), json_dom_alloc);

            auto amp_str = int_to_bin_str(basis, qubit_num);
            rapidjson::Value amp_str_val;
            amp_str_val.SetString(amp_str.c_str(), amp_str.size(), json_dom_alloc);

            obj.AddMember(amp_str_val, amp_pair, json_dom_alloc);
        }
        amp_array.PushBack(obj, json_dom_alloc);
    }

    json_dom.AddMember("success", true, json_dom_alloc);
    json_dom.AddMember("qubits", qubit_num, json_dom_alloc);
    json_dom.AddMember("amplitudes", amp_array, json_dom_alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_dom.Accept(writer);
    
    const char* output = buffer.GetString();
    std::cout << output << std::endl;

    p_out_buf->assign(output);
}
