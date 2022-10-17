#ifndef NAME_HPP_
#define NAME_HPP_

#include <string>

std::string get_storage_name(const std::string &job_id, int thread_id, int depth, int child_num);
std::string get_state_storage_name(const std::string &storage_name);
std::string get_basis_storage_name(const std::string &storage_name);
std::string get_gate_name(const std::string &job_id, int depth);

#endif
