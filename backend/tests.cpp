#include "types.hpp"

GatesOnQubit TEST_GATES_1 {
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 0,
                .qubit_idx2 = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    }
};

GatesOnQubit TEST_GATES_2 {
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 0,
                .qubit_idx2 = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    }
};

GatesOnQubit TEST_GATES_3 {
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 0,
                .qubit_idx2 = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_PAULI_Z,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 1,
                .qubit_idx2 = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    }
};

GatesOnQubit TEST_GATES_4 {
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 0,
                .qubit_idx2 = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_PAULI_Z,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 1,
                .qubit_idx2 = 2,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_DOUBLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .double_qubit = GateDoubleQubit {
                .gate_type = GateTypeDoubleQubit::GATE_CX,
                .qubit_idx1 = 1,
                .qubit_idx2 = 0,
                .qubit_total_num = 3
            }
        }
    },
    GateOnQubit {
        .width = GateWidth::GATE_SINGLE_QUBIT,
        .gate = GateOnQubit::GateUnion {
            .single_qubit = GateSingleQubit {
                .gate_type = GateTypeSingleQubit::GATE_HADAMARD,
                .qubit_idx = 1,
                .qubit_total_num = 3
            }
        }
    }
};


