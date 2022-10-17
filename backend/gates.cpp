#include "types.hpp"

static void map_active_basis_pauli_x(QubitIdx gate_qubit_idx, BasisIdx active_basis_idx,
                                     ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    auto new_active_basis_idx = active_basis_idx ^ (1ull << gate_qubit_idx);
    auto path_weight = PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_pauli_y(QubitIdx gate_qubit_idx, BasisIdx active_basis_idx,
                                     ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    auto new_active_basis_idx = active_basis_idx ^ (1ull << gate_qubit_idx);
    auto path_weight = (active_basis_idx & (1ull << gate_qubit_idx))
                     ? PathWeight(0.0, -1.0)
                     : PathWeight(0.0, 1.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_pauli_z(QubitIdx gate_qubit_idx, BasisIdx active_basis_idx,
                                     ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    auto new_active_basis_idx = active_basis_idx;
    auto path_weight = (active_basis_idx & (1ull << gate_qubit_idx))
                     ? PathWeight(-1.0, 0.0)
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_half_pauli_z(QubitIdx gate_qubit_idx, BasisIdx active_basis_idx,
                                          ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    auto new_active_basis_idx = active_basis_idx;
    auto path_weight = (active_basis_idx & (1ull << gate_qubit_idx))
                     ? PathWeight(0.0, 1.0)
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_quarter_pauli_z(QubitIdx gate_qubit_idx, BasisIdx active_basis_idx,
                                             ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    auto new_active_basis_idx = active_basis_idx;
    auto path_weight = (active_basis_idx & (1ull << gate_qubit_idx))
                     ? PathWeight(RECP_SQRT_2, RECP_SQRT_2)
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_hadamard(QubitIdx gate_qubit_idx, BasisIdx active_basis_idx,
                                      ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    auto basis_mask = 1ull << gate_qubit_idx;
    auto new_active_basis_idx1 = active_basis_idx;
    auto new_active_basis_idx2 = active_basis_idx ^ basis_mask;
    auto path_weight1 = (active_basis_idx & basis_mask)
                      ? PathWeight(-RECP_SQRT_2, 0.0)
                      : PathWeight(RECP_SQRT_2, 0.0);
    auto path_weight2 = PathWeight(RECP_SQRT_2, 0.0);
    auto weighted_new_active_basis_idx1 = WeightedBasisIdx(new_active_basis_idx1, path_weight1);
    auto weighted_new_active_basis_idx2 = WeightedBasisIdx(new_active_basis_idx2, path_weight2);

    p_new_active_basis->insert(new_active_basis_idx1);
    p_new_active_basis->insert(new_active_basis_idx2);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx1);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx2);
}

static void map_active_basis_cx(QubitIdx control_qubit_idx, QubitIdx target_qubit_idx,
                                BasisIdx active_basis_idx, ComputationPaths *p_path,
                                ActiveBasisIdxes *p_new_active_basis) {
    auto control_basis_mask = 1ull << control_qubit_idx;
    auto target_basis_mask = 1ull << target_qubit_idx;
    auto new_active_basis_idx = (active_basis_idx & control_basis_mask)
                              ? (active_basis_idx ^ target_basis_mask)
                              : active_basis_idx;
    auto path_weight = PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_cy(QubitIdx control_qubit_idx, QubitIdx target_qubit_idx,
                                BasisIdx active_basis_idx, ComputationPaths *p_path,
                                ActiveBasisIdxes *p_new_active_basis) {
    auto control_basis_mask = 1ull << control_qubit_idx;
    auto target_basis_mask = 1ull << target_qubit_idx;
    auto new_active_basis_idx = (active_basis_idx & control_basis_mask)
                              ? (active_basis_idx ^ target_basis_mask)
                              : active_basis_idx;
    auto path_weight = (active_basis_idx & control_basis_mask)
                     ? ((active_basis_idx & target_basis_mask)
                        ? PathWeight(0.0, 1.0)
                        : PathWeight(0.0, -1.0))
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_cz(QubitIdx control_qubit_idx, QubitIdx target_qubit_idx,
                                BasisIdx active_basis_idx, ComputationPaths *p_path,
                                ActiveBasisIdxes *p_new_active_basis) {
    auto control_basis_mask = 1ull << control_qubit_idx;
    auto target_basis_mask = 1ull << target_qubit_idx;
    auto new_active_basis_idx = active_basis_idx;
    auto path_weight = (active_basis_idx & (control_basis_mask | target_basis_mask))
                     ? PathWeight(-1.0, 0.0)
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_cs(QubitIdx control_qubit_idx, QubitIdx target_qubit_idx,
                                BasisIdx active_basis_idx, ComputationPaths *p_path,
                                ActiveBasisIdxes *p_new_active_basis) {
    auto control_basis_mask = 1ull << control_qubit_idx;
    auto target_basis_mask = 1ull << target_qubit_idx;
    auto new_active_basis_idx = active_basis_idx;
    auto path_weight = (active_basis_idx & (control_basis_mask | target_basis_mask))
                     ? PathWeight(0.0, 1.0)
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_ct(QubitIdx control_qubit_idx, QubitIdx target_qubit_idx,
                                BasisIdx active_basis_idx, ComputationPaths *p_path,
                                ActiveBasisIdxes *p_new_active_basis) {
    auto control_basis_mask = 1ull << control_qubit_idx;
    auto target_basis_mask = 1ull << target_qubit_idx;
    auto new_active_basis_idx = active_basis_idx;
    auto path_weight = (active_basis_idx & (control_basis_mask | target_basis_mask))
                     ? PathWeight(RECP_SQRT_2, RECP_SQRT_2)
                     : PathWeight(1.0, 0.0);
    auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

    p_new_active_basis->insert(new_active_basis_idx);
    (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
}

static void map_active_basis_ch(QubitIdx control_qubit_idx, QubitIdx target_qubit_idx,
                                BasisIdx active_basis_idx, ComputationPaths *p_path,
                                ActiveBasisIdxes *p_new_active_basis) {
    auto control_basis_mask = 1ull << control_qubit_idx;
    auto target_basis_mask = 1ull << target_qubit_idx;

    if (active_basis_idx & control_basis_mask) {
        auto new_active_basis_idx1 = active_basis_idx;
        auto new_active_basis_idx2 = active_basis_idx ^ target_basis_mask;
        auto path_weight1 = (active_basis_idx & target_basis_mask)
                          ? PathWeight(-RECP_SQRT_2, 0.0)
                          : PathWeight(RECP_SQRT_2, 0.0);
        auto path_weight2 = PathWeight(RECP_SQRT_2, 0.0);
        auto weighted_new_active_basis_idx1 = WeightedBasisIdx(new_active_basis_idx1, path_weight1);
        auto weighted_new_active_basis_idx2 = WeightedBasisIdx(new_active_basis_idx2, path_weight2);

        p_new_active_basis->insert(new_active_basis_idx1);
        p_new_active_basis->insert(new_active_basis_idx2);
        (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx1);
        (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx2);
    } else {
        auto new_active_basis_idx = active_basis_idx;
        auto path_weight = PathWeight(1.0, 0.0);
        auto weighted_new_active_basis_idx = WeightedBasisIdx(new_active_basis_idx, path_weight);

        p_new_active_basis->insert(new_active_basis_idx);
        (*p_path)[active_basis_idx].push_back(weighted_new_active_basis_idx);
    }
}

void map_active_basis_single_qubit_gate(const GateSingleQubit &gate, const ActiveBasisIdxes &active_basis,
                                        ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    p_path->clear();
    p_new_active_basis->clear();

    BasisMapperSingleQubit mapper;
    switch (gate.gate_type) {
    case GateTypeSingleQubit::GATE_PAULI_X:
        mapper = map_active_basis_pauli_x; break;
    case GateTypeSingleQubit::GATE_PAULI_Y:
        mapper = map_active_basis_pauli_y; break;
    case GateTypeSingleQubit::GATE_PAULI_Z:
        mapper = map_active_basis_pauli_z; break;
    case GateTypeSingleQubit::GATE_HADAMARD:
        mapper = map_active_basis_hadamard; break;
    case GateTypeSingleQubit::GATE_HALF_Z:
        mapper = map_active_basis_half_pauli_z; break;
    case GateTypeSingleQubit::GATE_QUATER_Z:
        mapper = map_active_basis_quarter_pauli_z; break;
    default:
        throw;
    }

    for (auto basis : active_basis)
        mapper(gate.qubit_idx, basis, p_path, p_new_active_basis);
}

void map_active_basis_double_qubit_gate(const GateDoubleQubit &gate, const ActiveBasisIdxes &active_basis,
                                        ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis) {
    p_path->clear();
    p_new_active_basis->clear();

    BasisMapperDoubleQubit mapper;
    switch (gate.gate_type)
    {
    case GateTypeDoubleQubit::GATE_CX:
        mapper = map_active_basis_cx; break;
    case GateTypeDoubleQubit::GATE_CY:
        mapper = map_active_basis_cy; break;
    case GateTypeDoubleQubit::GATE_CZ:
        mapper = map_active_basis_cz; break;
    case GateTypeDoubleQubit::GATE_CS:
        mapper = map_active_basis_cs; break;
    case GateTypeDoubleQubit::GATE_CT:
        mapper = map_active_basis_ct; break;
    case GateTypeDoubleQubit::GATE_CH:
        mapper = map_active_basis_ch; break;
    default:
        throw;
    }

    for (auto basis : active_basis)
        mapper(gate.qubit_idx1, gate.qubit_idx2, basis, p_path, p_new_active_basis);
}
