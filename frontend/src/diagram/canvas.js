/**
 * Main code for diagram generation.
 */

const stage = new Konva.Stage({
  container: "canvas",
  width: window.innerWidth,
  height: window.innerHeight,
  draggable: true,
});

const center = {
  x: window.innerWidth / 2,
  y: window.innerHeight / 2,
};

/********************************************************************************
 *                                                                              *
 *                                Canvas Setup                                  *
 *                                                                              *
 ********************************************************************************/

// Makes canvas fit to window
function updateStageSize() {
  stage.width(window.innerWidth);
  stage.height(window.innerHeight);

  center.x = window.innerWidth / 2;
  center.y = window.innerHeight / 2;
}
window.addEventListener("resize", updateStageSize);

//handles zooming via scroll on desktop
//  initial src: https://konvajs.org/docs/sandbox/Zooming_Relative_To_Pointer.html
const scaleBy = 0.94;
stage.on("wheel", (e) => {
  e.evt.preventDefault();

  let oldScale = stage.scaleX();
  let pointer = stage.getPointerPosition();

  let mousePointTo = {
    x: (pointer.x - stage.x()) / oldScale,
    y: (pointer.y - stage.y()) / oldScale,
  };

  let newScale = e.evt.deltaY > 0 ? oldScale * scaleBy : oldScale / scaleBy;

  stage.scale({ x: newScale, y: newScale });

  let newPos = {
    x: pointer.x - mousePointTo.x * newScale,
    y: pointer.y - mousePointTo.y * newScale,
  };
  stage.position(newPos);
  stage.batchDraw();
});

//handles zooming via pinch on touchscreens
//  initial src: https://konvajs.org/docs/sandbox/Multi-touch_Scale_Stage.html

let lastCenter = null;
let lastDist = 0;

stage.on("touchmove", function (e) {
  e.evt.preventDefault();

  let touch1 = e.evt.touches[0];
  let touch2 = e.evt.touches[1];

  if (touch1 && touch2) {
    // if the stage was under Konva's drag&drop
    // we need to stop it, and implement our own pan logic with two pointers
    if (stage.isDragging()) {
      stage.stopDrag();
    }

    let p1 = {
      x: touch1.clientX,
      y: touch1.clientY,
    };
    let p2 = {
      x: touch2.clientX,
      y: touch2.clientY,
    };

    if (!lastCenter) {
      lastCenter = midpointPoints(p1, p2);
      return;
    }
    let newCenter = midpointPoints(p1, p2);

    let dist = distPoints(p1, p2);

    if (!lastDist) {
      lastDist = dist;
    }

    // local coordinates of center point
    let pointTo = {
      x: (newCenter.x - stage.x()) / stage.scaleX(),
      y: (newCenter.y - stage.y()) / stage.scaleX(),
    };

    var scale = stage.scaleX() * (dist / lastDist);

    stage.scaleX(scale);
    stage.scaleY(scale);

    // calculate new position of the stage
    let dx = newCenter.x - lastCenter.x;
    let dy = newCenter.y - lastCenter.y;

    let newPos = {
      x: newCenter.x - pointTo.x * scale + dx,
      y: newCenter.y - pointTo.y * scale + dy,
    };

    stage.position(newPos);
    stage.batchDraw();

    lastDist = dist;
    lastCenter = newCenter;
  }
});

stage.on("touchend", function () {
  lastDist = 0;
  lastCenter = null;
});

/********************************************************************************
 *                                                                              *
 *                            Shape Generation Functions                        *
 *                                                                              *
 ********************************************************************************/

const color_re_p = "#007fff";
const color_re_n = "#ff8000";
const color_im_p = "#00ff80";
const color_im_n = "#ff007f";

// Creates a node at coordinate (x, y)
function newNode(coord, amp, base) {
  let denom = Math.abs(amp[0]) + Math.abs(amp[1]);
  let re_ratio = amp[0] / denom;
  let im_ratio = amp[1] / denom;
  let weight = [base * re_ratio, base * im_ratio];
  let circles = [];

  if (weight[1] != 0) {
    circles.push(
      new Konva.Circle({
        x: coord.x,
        y: coord.y,
        radius: (Math.abs(weight[0]) + Math.abs(weight[1])) * 8,
        stroke: amp[0] < 0 ? color_im_n : color_im_p,
        fill: amp[0] < 0 ? color_im_n : color_im_p,
        strokeWidth: (Math.abs(weight[0]) + Math.abs(weight[1])) * 2,
      })
    );
  }

  circles.push(
    new Konva.Circle({
      x: coord.x,
      y: coord.y,
      radius: Math.abs(weight[0] * 8),
      stroke: amp[0] < 0 ? color_re_n : color_re_p,
      fill: amp[0] < 0 ? color_re_n : color_re_p,
      strokeWidth: Math.abs(weight[0] * 2),
    })
  );

  return circles;
}

// Creates an curved line between (x1, y1) and (x2, y2)
//  src for line: https://konvajs.org/docs/sandbox/Modify_Curves_with_Anchor_Points.html
function newLine(coord1, coord2, weight) {
  let mid_x = (coord2.x - coord1.x) / 2;
  let anchor1 = { x: coord1.x + mid_x, y: coord1.y };
  let anchor2 = { x: coord2.x - mid_x, y: coord2.y };

  let lines = [];

  if (weight[1] != 0) {
    lines.push(
      new Konva.Shape({
        stroke: weight[1] < 0 ? color_im_n : color_im_p,
        strokeWidth: 8 * (Math.abs(weight[0]) + Math.abs(weight[1])),
        sceneFunc: (ctx, shape) => {
          ctx.beginPath();
          ctx.moveTo(coord1.x, coord1.y);
          ctx.bezierCurveTo(
            anchor1.x,
            anchor1.y,
            anchor2.x,
            anchor2.y,
            coord2.x,
            coord2.y
          );
          ctx.fillStrokeShape(shape);
        },
      })
    );
  }

  //console.log(weight);
  lines.push(
    new Konva.Shape({
      stroke: weight[0] < 0 ? color_re_n : color_re_p,
      strokeWidth: 8 * Math.abs(weight[0]),
      sceneFunc: (ctx, shape) => {
        ctx.beginPath();
        ctx.moveTo(coord1.x, coord1.y);
        ctx.bezierCurveTo(
          anchor1.x,
          anchor1.y,
          anchor2.x,
          anchor2.y,
          coord2.x,
          coord2.y
        );
        ctx.fillStrokeShape(shape);
      },
    })
  );

  return lines;
}

function newLabel(coord, text, font_size = 16) {
  let label = new Konva.Text({
    x: coord.x,
    y: coord.y,
    text: text,
    fontSize: font_size,
  });
  return label;
}

// Adds a series of elements to the stage
function addElements(elements) {
  let layer = new Konva.Layer();

  elements.forEach((element) => {
    layer.add(element);
  });

  layer.draw();
  stage.add(layer);
}

// Functions to download canvas as image:
// main src:           https://konvajs.org/docs/data_and_serialization/High-Quality-Export.html
// downloadURI() from: https://stackoverflow.com/a/15832662/512042
function downloadURI(uri, name) {
  let link = document.createElement("a");
  link.download = name;
  link.href = uri;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
}

function downloadCanvas(file_name = "diagram.png") {
  let dataURL = stage.toDataURL({ pixelRatio: 7 });
  downloadURI(dataURL, file_name);
}

async function emailImage() {
  let email_addr = document.getElementById("email-box").value;

  // checks if email_addr is a valid email address
  if (
    !email_addr.match(
      /^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
    )
  ) {
    alert("Please enter a valid email address");
    return;
  }

  // generate random string 6 character string and store in a variable named str
  var str = "feynmann-";
  var possible =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  for (var i = 0; i < 6; i++) {
    str += possible.charAt(Math.floor(Math.random() * possible.length));
  }
  str += ".png";

  downloadCanvas(str);

  // Makes server request
  const _ = await makeRequest(
    server_url,
    { email: email_addr, file: str },
    true
  );

  alert("Sent!");
}
