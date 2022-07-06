/**
 * Processes data returned from a GET request to the server.
 */

const server_url = "http://localhost:8000";
const test = 3;

let rev = false;
let display_states = false;
let display_gates = true;
let all_nodes = false;
let get_amplitudes = false;

let setGlobals = () => {
  rev = !document.getElementById("rev-states").checked;
  display_states = document.getElementById("disp-states").checked;
  display_gates = document.getElementById("disp-gates").checked;
  all_nodes = document.getElementById("disp-all-nodes").checked;
  get_amplitudes = document.getElementById("get-amps").checked;
};

let isZero = (entry) => entry[0] == 0 && entry[1] == 0;
let isOne = (entry) => entry[0] == 1 && entry[1] == 0;
let isIden = (gate) => {
  return (
    isOne(gate[0][0]) &&
    isZero(gate[0][1]) &&
    isZero(gate[1][0]) &&
    isOne(gate[1][1])
  );
};

async function makeRequest(url, circuit) {
  let response = await fetch(url, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(circuit),
  });
  let data = await response.json();
  return data;
}

function getQubitNum(circuit) {
  let qubits = 0;
  circuit.forEach((layer) => {
    if (layer.length > qubits) qubits = layer.length;
  });
  return qubits;
}

function handleNodes(amplitudes, states_per_layer, nodePosition) {
  let conjSqr = ([x, y]) => x * x + y * y;
  // Creates a list of coordinates where each node should go
  let layers = amplitudes.length;
  let nodes = [];
  for (let i = 0; i < layers; i++) {
    let amps = amplitudes[i];
    states_per_layer[i].forEach((state) => {
      if (
        all_nodes ||
        i == 0 ||
        i == layers - 1 ||
        amps[state][0] < 0 ||
        amps[state][1] < 0
      ) {
        //console.log(amps[state]);
        if (!isZero(amps[state])) {
          //console.log("here");
          let base = get_amplitudes ? Math.abs(conjSqr(amps[state])) : 0.75;
          nodes.push(...newNode(nodePosition(i, state), amps[state], base));
        }
      }
    });
  }

  // Adds nodes to canvas
  addElements(nodes);
}

function handleLines(circuit, data, states_per_layer, nodePosition) {
  let qubits = data.qubits;
  let layers = data.amplitudes.length;

  let getBit = (x, qubit) => ((x & (1 << (qubits - qubit - 1))) > 0 ? 1 : 0);
  let flipBit = (x, qubit) => x ^ (1 << (qubits - qubit - 1));

  let scalMulComplex = (a, [x, y]) => [a * x, a * y];
  let conjSqr = ([x, y]) => x * x + y * y;
  let mulComplex = ([x1, y1], [x2, y2]) => [
    x1 * x2 - y1 * y2,
    x1 * y2 + y1 * x2,
  ];

  let lines = [];
  // For each layer...
  for (let i = 0; i < layers - 1; i++) {
    //console.log("Layer: " + i);
    let states = states_per_layer[i];
    let gates = circuit[i];
    let amps = data.amplitudes[i];
    let next_amps = data.amplitudes[i + 1];

    // Checks if this layer is controlled and if so, by which qubits
    let controlled = 0;
    let anticontrolled = 0;
    gates.forEach((gate_str) => {
      controlled <<= 1;
      anticontrolled <<= 1;

      if (gate_str === "•" || gate_str === ".") {
        controlled |= 1;
      } else if (gate_str === "◦" || gate_str === "*") {
        controlled |= 1;
      }
    });

    let padding = qubits - gates.length;
    controlled <<= padding;
    anticontrolled <<= padding;
    //console.log("Controlled: " + controlled);

    // For each gate in the layer...
    gates.forEach((gate_str, index) => {
      //console.log("Gate: " + gate_str + "  " + index);
      let gate = getGate(gate_str);
      if (isIden(gate)) return;

      // Calculate the effect of the gate on each state
      states.forEach((state) => {
        // The current state of the individual qubit being acted on
        let input = getBit(state, index);

        //console.log(state + "  " + input);

        // Handles creating the line between input and output
        let handleOutput = (output) => {
          let weight = gate[output][input];
          if (!isZero(weight) && !isZero(amps[state])) {
            let next = output == input ? state : flipBit(state, index);
            //console.log(next);

            let calcWeight = (base) => {
              let denom = Math.abs(weight[0]) + Math.abs(weight[1]);
              let re_ratio = weight[0] / denom;
              let im_ratio = weight[1] / denom;
              return [base * re_ratio, base * im_ratio];
            };

            let calcAmpWeight = () => {
              /*
              console.log("----------------------");
              console.log(i);
              console.log(state);
              console.log(next);
              console.log(amps);
              console.log(next_amps);
              */
              return calcWeight(
                Math.abs(conjSqr(mulComplex(weight, amps[state])))
              );
            };

            let calcNoAmpWeight = () => {
              return calcWeight(0.5);
            };

            lines.push(
              ...newLine(
                nodePosition(i, state),
                nodePosition(i + 1, next),
                get_amplitudes ? calcAmpWeight() : calcNoAmpWeight()
              )
            );
          }
        };

        // If there's a control but not all controls are active, don't change state
        if (
          (controlled && (controlled & state) != controlled) ||
          (anticontrolled && (anticontrolled & ~state) != anticontrolled)
        ) {
          if (!isZero(amps[state])) {
            lines.push(
              ...newLine(
                nodePosition(i, state),
                nodePosition(i + 1, state),
                get_amplitudes
                  ? [Math.abs(conjSqr(amps[state])), 0.0]
                  : [0.5, 0.0]
              )
            );
          }
        } else {
          // Handles each possible output case
          handleOutput(0);
          handleOutput(1);
        }
      });
    });
  }

  addElements(lines);
}

function handleLabels(qubits, side_offset, reverse, labelY) {
  let to_string = (q) => {
    let k = "";
    for (let i = 0; i < qubits; i++) {
      k = (q & 1 ? "1" : "0") + k;
      q >>= 1;
    }
    return k;
  };

  let labels = [];
  let states = 1 << qubits;
  for (let i = 0; i < states; i++) {
    let text = "|" + to_string(reverse(i)) + String.fromCharCode(9002);

    let x1 = 12;
    let x2 = window.innerWidth - side_offset + 12;

    let y = labelY(i);

    labels.push(newLabel({ x: x1, y: y }, text));
    labels.push(newLabel({ x: x2, y: y }, text));
  }

  addElements(labels);
}

const subscripts = [
  String.fromCharCode(8320),
  String.fromCharCode(8321),
  String.fromCharCode(8322),
  String.fromCharCode(8323),
  String.fromCharCode(8324),
  String.fromCharCode(8325),
  String.fromCharCode(8326),
  String.fromCharCode(8327),
  String.fromCharCode(8328),
  String.fromCharCode(8329),
];

function handleGates(qubits, gates_arr, gateX) {
  let gate_strs = [];
  gates_arr.forEach((gates, index) => {
    let controlled = [];
    let anticontrolled = [];
    let unitaries = [];
    gates.forEach((gate_str, qubit) => {
      if (gate_str === "•" || gate_str === ".") {
        controlled.push(qubit);
      } else if (gate_str === "◦" || gate_str === "*") {
        anticontrolled.push(qubit);
      } else if (gate_str != 1 && gate_str !== "I") {
        unitaries.push({ qubit: qubit, gate: gate_str });
      }
    });

    let gate_prefix =
      "C".repeat(controlled.length) +
      String.fromCharCode(266).repeat(anticontrolled.length);

    let gate_subscript_prefix = "";
    let comma = qubits > 9;
    controlled.forEach((qubit) => {
      gate_subscript_prefix += subscripts[qubit] + (comma ? "," : "");
    });
    anticontrolled.forEach((qubit) => {
      gate_subscript_prefix += subscripts[qubit] + (comma ? "," : "");
    });

    let texts = [];
    unitaries.forEach((unitary) => {
      let text =
        gate_prefix +
        unitary.gate +
        gate_subscript_prefix +
        subscripts[unitary.qubit];
      texts.push(text);
    });

    gate_strs.push(texts.join());
  });

  let y = 24;
  let labels = [];
  for (let i = 0; i < gates_arr.length; i++) {
    let text = gate_strs[i];

    let label = newLabel({ x: 0, y: y }, text, 18);
    label.setX(gateX(i, label.width()));
    label.fontStyle("bold");

    labels.push(label);
  }

  addElements(labels);
}

function processCircuit(cir) {
  for (let i = 0; i < cir.cols.length; i++) {
    let gates = cir.cols[i];
    for (let j = 0; j < gates.length; j++) {
      let gate_str = gates[j];
      if (gate_str === "•") {
        cir.cols[i][j] = ".";
      } else if (gate_str === "◦") {
        cir.cols[i][j] = "*";
      } else if (gate_str == 1) {
        cir.cols[i][j] = "I";
      }
    }
  }
}

async function processData() {
  stage.destroyChildren();
  setGlobals();
  const circuit = await getCircuitFromFile();
  // TODO: handle error if circuit not available (null)
  processCircuit(circuit);

  circuit.qubits = getQubitNum(circuit.cols);
  circuit.with_amp = get_amplitudes;

  // Makes server request
  const data = await makeRequest(server_url, circuit);

  // Quick hacks to fix data format
  let init_state = {};
  init_state["0".repeat(data.qubits)] = [1, 0];

  data.amplitudes = [init_state].concat(data.amplitudes);

  //console.log(circuit);
  //console.log(data);

  let qubits = data.qubits;
  let layers = data.amplitudes.length;

  // Reverses the bits in a number
  let reverse = (q) => {
    let k = 0;
    for (let i = 0; i < qubits; i++) {
      k <<= 1;
      if (q & 1) k |= 1;
      q >>= 1;
    }
    return k;
  };

  // Calculates which states are present in each layer
  // Also changes the keys in data.amplitudes from strings to ints
  let states_per_layer = [];
  let amplitudes = [];
  data.amplitudes.forEach((layer) => {
    let states = [];
    let amps = {};
    Object.keys(layer).forEach((key) => {
      let int_key = reverse(parseInt(key, 2));
      states.push(int_key);
      amps[int_key] = layer[key];
    });
    states_per_layer.push(states);
    amplitudes.push(amps);
  });
  data.amplitudes = amplitudes;

  // Calculates offset of diagram sides based on the future label size
  let side_offset = 24;
  let top_offset = 48;
  let label_y_offset = 0;
  if (display_states || display_gates) {
    let temp_text = "|" + "0".repeat(qubits) + String.fromCharCode(9002);
    let temp_label = new Konva.Text({
      text: temp_text,
    });
    if (display_states) {
      temp_label.fontSize(16);
      side_offset += temp_label.width();
      label_y_offset = temp_label.height() / 2;
    }
    if (display_gates) {
      temp_label.fontSize(18);
      top_offset += temp_label.height();
    }
  }

  // Creates a function to calculate where nodes should go on the canvas
  let layer_spacing = (window.innerWidth - 2 * side_offset) / (layers - 1 + 0);
  let qubit_spacing =
    (window.innerHeight - top_offset - 48) / ((1 << qubits) + 1 - 2);

  let nodePosition = (layer, qubit) => {
    let q = rev ? reverse(qubit) : qubit;
    let x = layer_spacing * (layer + 0) + side_offset;
    let y = qubit_spacing * (q + 1 - 1) + top_offset;
    return { x: x, y: y };
  };

  // Adds shapes to canvas as necessary
  if (display_states) {
    let labelY = (qubit) => {
      let q = rev ? reverse(qubit) : qubit;
      let y = qubit_spacing * (q + 1 - 1) - label_y_offset + top_offset;
      return y;
    };

    handleLabels(qubits, side_offset, reverse, labelY);
  }

  if (display_gates) {
    let gateX = (layer, label_width) => {
      let x =
        layer_spacing * (layer + 0) +
        side_offset +
        layer_spacing / 2 -
        label_width / 2;
      return x;
    };

    handleGates(qubits, circuit.cols, gateX);
  }

  handleLines(circuit.cols, data, states_per_layer, nodePosition);
  handleNodes(data.amplitudes, states_per_layer, nodePosition);
}
