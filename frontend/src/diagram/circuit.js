function openCircuitPane() {
  document.getElementById("circuit").style.width = "100%";
}

function closeCircuitPane() {
  document.getElementById("circuit").style.width = "0%";
}

function getCircuitFromPane() {
  return JSON.parse(document.getElementById("circuit-text").value);
}

function getCircuitFromFile() {
  const circuit_file = document.getElementById("circuit-file-btn").files[0];
  if (circuit_file) {
    const reader = new FileReader();

    return new Promise((resolve, reject) => {
      reader.onload = function (evt) {
        let contents = JSON.parse(evt.target.result);
        resolve(contents);
      };

      reader.onerror = function (evt) {
        reject(null);
      };

      reader.readAsText(circuit_file, "UTF-8");
    });
  }

  return null;
}

function getTestCircuit(i) {
  // TODO
  switch (i) {
    case 1:
      return {
        cols: [["H"], [1, "H"], ["•", "X"], ["H"], [1, "H"]],
      };

    case 2:
      return {
        cols: [["H"], ["•", "X"], ["H"], [1, "H"]],
      };

    case 3:
      return {
        cols: [
          ["H"],
          ["•", "X"],
          [1, "Z"],
          ["H"],
          [1, "H"],
          ["X", "•"],
          [1, "H"],
        ],
      };

    case 4:
      return {
        cols: [
          ["H"],
          ["•", "X"],
          [1, "Z"],
          [1, "•", "X"],
          ["H"],
          [1, "H"],
          ["X", "•"],
          [1, "H"],
        ],
      };

    case 5:
      return {
        cols: [["H"], [1, "H"], [1, 1, "H"], [1, 1, 1, "H"]],
      };

    case 6:
      return {
        cols: [
          ["H"],
          [1, "H"],
          [1, 1, "H"],
          [1, 1, 1, "H"],
          [1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, "H"],
        ],
      };

    case 7:
      return {
        cols: [
          ["H"],
          [1, "H"],
          [1, 1, "H"],
          [1, 1, 1, "H"],
          [1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, 1, 1, "H"],
        ],
      };

    case 8:
      return {
        cols: [
          ["H"],
          [1, "H"],
          [1, 1, "H"],
          [1, 1, 1, "H"],
          [1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, 1, 1, "H"],
          [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, "H"],
        ],
      };

    case 9:
      return {
        cols: [
          ["H"],
          [".", "X"],
          ["I", "T"],
          ["I", "T"],
          ["I", "S"],
          ["I", ".", "X"],
          ["H"],
          ["I", "H"],
          ["X", "."],
          ["Y", "."],
          ["Y", "."],
          ["I", "T", "."],
          ["I", "T", "."],
          ["I", "S", "."],
          ["I", "Z", "."],
          [".", "I", "H"],
          [".", "I", "H"],
          ["I", "H"],
        ],
      };

    default: {
    }
  }
}
