#include "serialization.hpp"

template<>
std::vector<unsigned char> serialize<GateOnQubit>(const GateOnQubit &gate) {
    std::vector<unsigned char> bytes;

    switch (gate.width) {
    case GateWidth::GATE_SINGLE_QUBIT:
    {
        auto qubit_idx = gate.gate.single_qubit.qubit_idx;
        auto gate_type = gate.gate.single_qubit.gate_type;
        auto qubit_total_num = gate.gate.single_qubit.qubit_total_num;

        char gate_type_char;
        switch (gate_type)
        {
        case GateTypeSingleQubit::GATE_PAULI_X:
            gate_type_char = 'X'; break;
        case GateTypeSingleQubit::GATE_PAULI_Y:
            gate_type_char = 'Y'; break;
        case GateTypeSingleQubit::GATE_PAULI_Z:
            gate_type_char = 'Z'; break;
        case GateTypeSingleQubit::GATE_HADAMARD:
            gate_type_char = 'H'; break;
        case GateTypeSingleQubit::GATE_HALF_Z:
            gate_type_char = 'S'; break;
        case GateTypeSingleQubit::GATE_QUATER_Z:
            gate_type_char = 'T'; break;
        default:
            throw;
        }

        bytes.assign(qubit_total_num, 'I');
        bytes[qubit_idx] = gate_type_char;
        break;
    }
    case GateWidth::GATE_DOUBLE_QUBIT:
    {
        auto control_qubit_idx = gate.gate.double_qubit.qubit_idx1;
        auto target_qubit_idx = gate.gate.double_qubit.qubit_idx2;
        auto gate_type = gate.gate.double_qubit.gate_type;
        auto qubit_total_num = gate.gate.double_qubit.qubit_total_num;

        char gate_type_char;
        switch (gate_type)
        {
        case GateTypeDoubleQubit::GATE_CX:
            gate_type_char = 'x'; break;
        case GateTypeDoubleQubit::GATE_CY:
            gate_type_char = 'y'; break;
        case GateTypeDoubleQubit::GATE_CZ:
            gate_type_char = 'z'; break;
        case GateTypeDoubleQubit::GATE_CS:
            gate_type_char = 's'; break;
        case GateTypeDoubleQubit::GATE_CT:
            gate_type_char = 't'; break;
        case GateTypeDoubleQubit::GATE_CH:
            gate_type_char = 'h'; break;
        default:
            throw;
        }

        bytes.assign(qubit_total_num, 'I');
        bytes[control_qubit_idx] = 'C';
        bytes[target_qubit_idx] = gate_type_char;
        break;
    }
    default:
        throw;
    }

    return bytes;
}

template<>
std::vector<unsigned char> serialize<ActiveBasisIdxes>(const ActiveBasisIdxes &active_basis) {
    auto &&basis_vec = std::vector<BasisIdx>(active_basis.begin(), active_basis.end());
    auto ptr_start = basis_vec.data();
    auto ptr_end = ptr_start + active_basis.size();
    auto ptr_byte_start = reinterpret_cast<unsigned char *>(ptr_start);
    auto ptr_byte_end = reinterpret_cast<unsigned char *>(ptr_end);
    return std::vector<unsigned char>(ptr_byte_start, ptr_byte_end);
}

template<>
std::vector<unsigned char> serialize<SparseState>(const SparseState &state) {
    auto size = state.size();
    std::vector<BasisIdx> basis;
    std::vector<BasisWeight> weights;
    basis.reserve(size);
    weights.reserve(size);

    for (auto it = state.begin(); it != state.end(); ++it) {
        basis.push_back(it->first);
        weights.push_back(it->second);
    }

    std::vector<unsigned char> bytes;
    bytes.reserve(sizeof(size) + sizeof(BasisIdx) * size + sizeof(BasisWeight) * size);

    auto ptr_byte_start = reinterpret_cast<unsigned char *>(&size);
    auto ptr_byte_end = reinterpret_cast<unsigned char *>(&size + 1);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<unsigned char *>(basis.data());
    ptr_byte_end = reinterpret_cast<unsigned char *>(basis.data() + size);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<unsigned char *>(weights.data());
    ptr_byte_end = reinterpret_cast<unsigned char *>(weights.data() + size);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    return bytes;
}

template<>
std::vector<unsigned char> serialize<CompletionStatus>(const CompletionStatus &status) {
    auto size = sizeof(status.id) + sizeof(status.code) + sizeof(status.child_num)
              + status.keys[0].size() + status.keys[1].size();
    std::vector<unsigned char> bytes;
    bytes.reserve(size);

    auto ptr_byte_start = reinterpret_cast<const unsigned char *>(&status.id);
    auto ptr_byte_end = ptr_byte_start + sizeof(status.id);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<const unsigned char *>(&status.code);
    ptr_byte_end = ptr_byte_start + sizeof(status.code);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<const unsigned char *>(&status.child_num);
    ptr_byte_end = ptr_byte_start + sizeof(status.child_num);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    auto key_size = status.keys[0].size();
    ptr_byte_start = reinterpret_cast<const unsigned char *>(&key_size);
    ptr_byte_end = ptr_byte_start + sizeof(key_size);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<const unsigned char *>(status.keys[0].data());
    ptr_byte_end = ptr_byte_start + status.keys[0].size();
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);
    
    key_size = status.keys[1].size();
    ptr_byte_start = reinterpret_cast<const unsigned char *>(&key_size);
    ptr_byte_end = ptr_byte_start + sizeof(key_size);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<const unsigned char *>(status.keys[1].data());
    ptr_byte_end = ptr_byte_start + status.keys[1].size();
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    return bytes;
}

template<>
std::vector<unsigned char> serialize<MergeStatus>(const MergeStatus &status) {
    auto size = sizeof(status.depth) + status.key.size();
    std::vector<unsigned char> bytes;
    bytes.reserve(size);

    auto ptr_byte_start = reinterpret_cast<const unsigned char *>(&status.depth);
    auto ptr_byte_end = ptr_byte_start + sizeof(status.depth);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    auto key_size = status.key.size();
    ptr_byte_start = reinterpret_cast<const unsigned char *>(&key_size);
    ptr_byte_end = ptr_byte_start + sizeof(key_size);
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    ptr_byte_start = reinterpret_cast<const unsigned char *>(status.key.data());
    ptr_byte_end = ptr_byte_start + status.key.size();
    bytes.insert(bytes.end(), ptr_byte_start, ptr_byte_end);

    return bytes;
}

template<>
GateOnQubit deserialize<GateOnQubit>(const unsigned char *begin, const unsigned char *end) {
    auto ptr_ctrl = std::find(begin, end, 'C');

    if (ptr_ctrl == end) {
        auto qubit_total_num = end - begin;
        auto ptr_gate = std::find_if(begin, end, [](auto c) { return c != 'I'; });
        assert(ptr_gate != end);
        auto qubit_idx = ptr_gate - begin;
        GateTypeSingleQubit gate_type;

        switch (*ptr_gate) {
        case 'X': gate_type = GateTypeSingleQubit::GATE_PAULI_X; break;
        case 'Y': gate_type = GateTypeSingleQubit::GATE_PAULI_Y; break;
        case 'Z': gate_type = GateTypeSingleQubit::GATE_PAULI_Z; break;
        case 'H': gate_type = GateTypeSingleQubit::GATE_HADAMARD; break;
        case 'S': gate_type = GateTypeSingleQubit::GATE_HALF_Z; break;
        case 'T': gate_type = GateTypeSingleQubit::GATE_QUATER_Z; break;
        default: throw;
        }

        return GateOnQubit {
            .width = GateWidth::GATE_SINGLE_QUBIT,
            .gate = GateOnQubit::GateUnion {
                .single_qubit = GateSingleQubit {
                    .gate_type = gate_type,
                    .qubit_idx = static_cast<unsigned long>(qubit_idx),
                    .qubit_total_num = static_cast<unsigned long>(qubit_total_num)
                }
            }
        };
    } else {
        auto qubit_total_num = end - begin;
        auto ptr_gate = std::find_if(begin, end, [](auto c) { return c != 'I' && c != 'C'; });
        assert(ptr_gate != end);
        auto target_qubit_idx = ptr_gate - begin;
        auto control_qubit_idx = ptr_ctrl - begin;
        GateTypeDoubleQubit gate_type;

        switch (*ptr_gate)
        {
        case 'x': gate_type = GateTypeDoubleQubit::GATE_CX; break;
        case 'y': gate_type = GateTypeDoubleQubit::GATE_CY; break;
        case 'z': gate_type = GateTypeDoubleQubit::GATE_CZ; break;
        case 's': gate_type = GateTypeDoubleQubit::GATE_CS; break;
        case 't': gate_type = GateTypeDoubleQubit::GATE_CT; break;
        case 'h': gate_type = GateTypeDoubleQubit::GATE_CH; break;
        default: throw;
        }

        return GateOnQubit {
            .width = GateWidth::GATE_DOUBLE_QUBIT,
            .gate = GateOnQubit::GateUnion {
                .double_qubit = GateDoubleQubit {
                    .gate_type = gate_type,
                    .qubit_idx1 = static_cast<unsigned long>(control_qubit_idx),
                    .qubit_idx2 = static_cast<unsigned long>(target_qubit_idx),
                    .qubit_total_num = static_cast<unsigned long>(qubit_total_num)
                }
            }
        };
    }
}

template <>
ActiveBasisIdxes deserialize<ActiveBasisIdxes>(const unsigned char *begin, const unsigned char *end) {
    assert((end - begin) % sizeof(BasisIdx) == 0);

    auto ptr_start = reinterpret_cast<const BasisIdx *>(begin);
    auto ptr_end = reinterpret_cast<const BasisIdx *>(end);
    return ActiveBasisIdxes(ptr_start, ptr_end);
}

template<>
SparseState deserialize<SparseState>(const unsigned char *begin, const unsigned char *end) {
    auto *p_size = reinterpret_cast<const std::size_t *>(begin);
    auto size = *p_size;
    begin += sizeof(size);

    auto ptr_basis_start = reinterpret_cast<const BasisIdx *>(begin);
    auto ptr_basis_end = ptr_basis_start + size;

    auto ptr_weight_start = reinterpret_cast<const BasisWeight *>(ptr_basis_end);
    auto ptr_weight_end = ptr_weight_start + size;

    assert(reinterpret_cast<const void *>(ptr_weight_end) == end);

    SparseState state;
    for (auto i = 0ul; i < size; ++i) {
        state.emplace(*ptr_basis_start, *ptr_weight_start);
        ++ptr_basis_start;
        ++ptr_weight_start;
    }

    return state;
}

template<>
CompletionStatus deserialize<CompletionStatus>(const unsigned char *begin, const unsigned char *end) {
    CompletionStatus status;

    status.id = *reinterpret_cast<const int *>(begin);
    begin += sizeof(status.id);

    status.code = *reinterpret_cast<const CompletionStatusCode *>(begin);
    begin += sizeof(status.code);

    status.child_num = *reinterpret_cast<const int *>(begin);
    begin += sizeof(status.child_num);

    auto key_size = *reinterpret_cast<const std::size_t *>(begin);
    begin += sizeof(key_size);
    status.keys[0].insert(status.keys[0].end(), begin, begin + key_size);
    begin += key_size;

    key_size = *reinterpret_cast<const std::size_t *>(begin);
    begin += sizeof(key_size);
    status.keys[1].insert(status.keys[1].end(), begin, begin + key_size);
    begin += key_size;

    assert(begin == end);

    return status;
}

template<>
MergeStatus deserialize<MergeStatus>(const unsigned char *begin, const unsigned char *end) {
    MergeStatus status;

    status.depth = *reinterpret_cast<const int *>(begin);
    begin += sizeof(status.depth);

    auto key_size = *reinterpret_cast<const std::size_t *>(begin);
    begin += sizeof(key_size);
    status.key.append(begin, begin + key_size);
    begin += key_size;

    assert(begin == end);

    return status;
}
