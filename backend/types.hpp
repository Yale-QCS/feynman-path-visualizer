#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <complex>
#include <unordered_set>
#include <unordered_map>

#include <bitset>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/fiber/buffered_channel.hpp>

using QubitIdx = unsigned long;
using BasisIdx = unsigned long long;
using SinglePath = std::pair<BasisIdx, BasisIdx>;
using PathWeight = std::complex<double>;
using BasisWeight = std::complex<double>;
using WeightedBasisIdx = std::pair<BasisIdx, BasisWeight>;
using ActiveBasisIdxes = std::unordered_set<BasisIdx>;
using ComputationPaths = std::unordered_map<BasisIdx, std::vector<WeightedBasisIdx>>;
using SparseState = std::unordered_map<BasisIdx, BasisWeight>;
using BasisMapperSingleQubit = void (*)(QubitIdx, BasisIdx, ComputationPaths *, ActiveBasisIdxes *);
using BasisMapperDoubleQubit = void (*)(QubitIdx, QubitIdx, BasisIdx, ComputationPaths *, ActiveBasisIdxes *);
using StorageKey = std::string;
using StorageKeys = std::vector<StorageKey>;

enum class GateWidth {
    GATE_SINGLE_QUBIT,
    GATE_DOUBLE_QUBIT
};

enum class GateTypeSingleQubit {
    GATE_PAULI_X,
    GATE_PAULI_Y,
    GATE_PAULI_Z,
    GATE_HADAMARD,
    GATE_HALF_Z,
    GATE_QUATER_Z
};

enum class GateTypeDoubleQubit {
    GATE_CX,
    GATE_CY,
    GATE_CZ,
    GATE_CS,
    GATE_CT,
    GATE_CH
};

struct GateSingleQubit {
    GateTypeSingleQubit gate_type;
    QubitIdx qubit_idx;
    QubitIdx qubit_total_num;
};

struct GateDoubleQubit {
    GateTypeDoubleQubit gate_type;
    QubitIdx qubit_idx1, qubit_idx2;
    QubitIdx qubit_total_num;
};

struct GateOnQubit {
    GateWidth width;
    union GateUnion {
        GateSingleQubit single_qubit;
        GateDoubleQubit double_qubit;
    } gate;
};

enum class CompletionStatusCode {
    E_SUCCESS,
    E_FAILED
};

struct CompletionStatus {
    CompletionStatusCode code;
    int id;
    int child_num;
    StorageKey keys[2];
};

struct MergeStatus {
    int depth;
    StorageKey key;
};

using GatesOnQubit = std::vector<GateOnQubit>;

constexpr auto RECP_SQRT_2 = 0.70710678118654752440;

constexpr auto SPLIT_THRESHOLD = 256ul;

struct SingleStepResult {
    // std::shared_ptr<SparseState> up_new_state[2];
    StorageKey keys[2];
    int child_num;
};

constexpr auto TRUNCATE_THRESHOLD = 1e-10;

#endif
