#ifndef SERIALIZATION_HPP_
#define SERIALIZATION_HPP_

#include <stdexcept>
#include <algorithm>

#include "types.hpp"

template<typename T>
std::vector<unsigned char> serialize(const T &val) {
    // Unimplemented for the general case.
    throw std::runtime_error("unimplemented");
}

template<>
std::vector<unsigned char> serialize<GateOnQubit>(const GateOnQubit &gate);

template<>
std::vector<unsigned char> serialize<ActiveBasisIdxes>(const ActiveBasisIdxes &active_basis);

template<>
std::vector<unsigned char> serialize<SparseState>(const SparseState &state);

template<>
std::vector<unsigned char> serialize<CompletionStatus>(const CompletionStatus &status);

template<>
std::vector<unsigned char> serialize<MergeStatus>(const MergeStatus &status);

template <typename T>
T deserialize(const unsigned char *begin, const unsigned char *end) {
    // Unimplemented for the general case.
    throw std::runtime_error("unimplemented");
}

template<>
GateOnQubit deserialize<GateOnQubit>(const unsigned char *begin, const unsigned char *end);

template <>
ActiveBasisIdxes deserialize<ActiveBasisIdxes>(const unsigned char *begin, const unsigned char *end);

template<>
SparseState deserialize<SparseState>(const unsigned char *begin, const unsigned char *end);

template<>
CompletionStatus deserialize<CompletionStatus>(const unsigned char *begin, const unsigned char *end);

template<>
MergeStatus deserialize<MergeStatus>(const unsigned char *begin, const unsigned char *end);

#endif
