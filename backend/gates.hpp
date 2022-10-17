#ifndef GATES_HPP_
#define GATES_HPP_

#include "types.hpp"

void map_active_basis_single_qubit_gate(const GateSingleQubit &gate, const ActiveBasisIdxes &active_basis,
                                        ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis);

void map_active_basis_double_qubit_gate(const GateDoubleQubit &gate, const ActiveBasisIdxes &active_basis,
                                        ComputationPaths *p_path, ActiveBasisIdxes *p_new_active_basis);

#endif
