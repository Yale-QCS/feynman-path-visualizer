#include <algorithm>
#include <App.h>
#include <rapidjson/document.h>

#include "faymann.hpp"

extern GatesOnQubit TEST_GATES_1, TEST_GATES_2, TEST_GATES_3, TEST_GATES_4;

static void string_to_gate(GatesOnQubit *p_gates, const std::string &str, int qubit_num) {
    auto has_control = str.find('.') == std::string::npos;
    GateOnQubit gate;

    if (!has_control) {
        gate.width = GateWidth::GATE_DOUBLE_QUBIT;

        auto iter = std::find_if(str.begin(), str.end(), [](auto c) { return c == '.'; });
        if (iter == str.end()) throw std::runtime_error("bad input");
        gate.gate.double_qubit.qubit_idx1 = iter - str.begin();

        iter = std::find_if(str.begin(), str.end(), [](auto c) { return c != '.' && c != 'I'; });
        if(iter == str.end()) throw std::runtime_error("bad input");

        switch (*iter) {
        case 'X': gate.gate.double_qubit.gate_type = GateTypeDoubleQubit::GATE_CX; break;
        case 'Y': gate.gate.double_qubit.gate_type = GateTypeDoubleQubit::GATE_CY; break;
        case 'Z': gate.gate.double_qubit.gate_type = GateTypeDoubleQubit::GATE_CZ; break;
        case 'S': gate.gate.double_qubit.gate_type = GateTypeDoubleQubit::GATE_CS; break;
        case 'T': gate.gate.double_qubit.gate_type = GateTypeDoubleQubit::GATE_CT; break;
        case 'H': gate.gate.double_qubit.gate_type = GateTypeDoubleQubit::GATE_CH; break;
        default: throw std::runtime_error("bad input");
        }
        gate.gate.double_qubit.qubit_idx2 = iter - str.begin();
        gate.gate.double_qubit.qubit_total_num = qubit_num;
    } else {
        gate.width = GateWidth::GATE_SINGLE_QUBIT;

        auto iter = std::find_if(str.begin(), str.end(), [](auto c) { return c != 'I'; });
        if (iter == str.end()) throw std::runtime_error("bad input");

        switch (*iter) {
        case 'X': gate.gate.single_qubit.gate_type = GateTypeSingleQubit::GATE_PAULI_X; break;
        case 'Y': gate.gate.single_qubit.gate_type = GateTypeSingleQubit::GATE_PAULI_Y; break;
        case 'Z': gate.gate.single_qubit.gate_type = GateTypeSingleQubit::GATE_PAULI_Z; break;
        case 'H': gate.gate.single_qubit.gate_type = GateTypeSingleQubit::GATE_HADAMARD; break;
        case 'S': gate.gate.single_qubit.gate_type = GateTypeSingleQubit::GATE_HALF_Z; break;
        case 'T': gate.gate.single_qubit.gate_type = GateTypeSingleQubit::GATE_QUATER_Z; break;
        default: throw std::runtime_error("bad input");
        }
        gate.gate.single_qubit.qubit_idx = iter - str.begin();
        gate.gate.single_qubit.qubit_total_num = qubit_num;
    }

    p_gates->push_back(gate);
}

static void parse_json(const std::string &str, GatesOnQubit *p_gates,
                       int *p_qubit_num, bool *p_with_amp) {
    rapidjson::Document document;
    document.Parse(str.c_str());

    GatesOnQubit gates;

    auto qubit_num = document["qubits"].GetInt();
    auto with_amp = document["with_amp"].GetBool();

    for (auto &sub_arr : document["cols"].GetArray()) {
        std::string gate_str;
        for (auto &gate : sub_arr.GetArray())
            gate_str += gate.GetString();
        string_to_gate(&gates, gate_str, qubit_num);
    }

    *p_gates = std::move(gates);
    *p_qubit_num = qubit_num;
    *p_with_amp = with_amp;
}

int main() {
    uWS::App().get("/", [](auto *res, auto *req) {
        res->end("Hello World!");
    }).post("/", [](auto* res, auto* req) {
        res->onAborted([]() {
            std::cout <<  "Aborted!" << '\n';
        });

        /* Allocate automatic, stack, variable as usual */
        std::string buffer;
        /* Move it to storage of lambda */
        res->onData([res, buffer = std::move(buffer)](std::string_view data, bool last) mutable {
            /* Mutate the captured data */
            buffer.append(data.data(), data.length());

            if (last) {
                std::cout << "Start new faymann path simulation." << std::endl;

                std::string out_buf;
                try {
                    GatesOnQubit gates;
                    int qubit_num;
                    bool with_amp;
                    parse_json(buffer, &gates, &qubit_num, &with_amp);
                    faymann_path_distributed_with_propagation(gates, &out_buf, qubit_num, with_amp);
                } catch (const std::exception &e) {
                    std::cerr << "Exception occured: " << e.what() << std::endl;
                    res->end("{ \"success\": false }");
                    return;
                } catch (...) {
                    std::cerr << "Exception occured: unknown" << std::endl;
                    res->end("{ \"success\": false }");
                    return;
                }

                res->end(out_buf);
            }
        });
    }).listen(23333, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening for connections on port 23333..." << std::endl;
        }
    }).run();

    std::cout << "Shoot! We failed to listen and the App fell through, exiting now!" << std::endl;

    return 0;
}
