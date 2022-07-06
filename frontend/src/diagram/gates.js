/**
 * Contains matrices for supported gates.
 */

const r2i = 0.70710678118654752440084436210485;
const I = [
  [
    [1, 0],
    [0, 0],
  ],
  [
    [0, 0],
    [1, 0],
  ],
];
const X = [
  [
    [0, 0],
    [1, 0],
  ],
  [
    [1, 0],
    [0, 0],
  ],
];
const Y = [
  [
    [0, 0],
    [0, -1],
  ],
  [
    [0, 1],
    [0, 0],
  ],
];
const Z = [
  [
    [1, 0],
    [0, 0],
  ],
  [
    [0, 0],
    [-1, 0],
  ],
];
const H = [
  [
    [r2i, 0],
    [r2i, 0],
  ],
  [
    [r2i, 0],
    [-r2i, 0],
  ],
];
const S = [
  [
    [1, 0],
    [0, 0],
  ],
  [
    [0, 0],
    [0, 1],
  ],
];
const T = [
  [
    [1, 0],
    [0, 0],
  ],
  [
    [0, 0],
    [r2i, r2i],
  ],
];

function getGate(gate_str) {
  switch (gate_str) {
    case "X":
      return X;
    case "Y":
      return Y;
    case "Z":
      return Z;
    case "H":
      return H;
    case "S":
      return S;
    case "T":
      return T;
    default:
      return I;
  }
}
