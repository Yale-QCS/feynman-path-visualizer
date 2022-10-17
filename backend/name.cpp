#include "name.hpp"

std::string get_storage_name(const std::string &job_id, int thread_id, int depth, int child_num) {
    return job_id + "t" + std::to_string(thread_id) + "d"
        + std::to_string(depth) + "c" + std::to_string(child_num);
}

std::string get_state_storage_name(const std::string &storage_name) {
    return storage_name + "state";
}

std::string get_basis_storage_name(const std::string &storage_name) {
    return storage_name + "basis";
}

std::string get_gate_name(const std::string &job_id, int depth) {
    return job_id + "gate" + std::to_string(depth);
}
