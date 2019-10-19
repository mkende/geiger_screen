// The laser cutter kerf.
kerf = 0.2 / 2;

// The thickness of the wood plank.
thickness = 3;

length = 120;
width = 70;
height = 48;

margin = 0.2;

// Size of the openning in the box.
openning = 2;

$fn = 100;

// Smallest width of wood that we want anywhere (used between openning and also for the closing mechanism).
safety = 3;

hook_inside = 4.5;  // > sqrt(2*safety^2)

epsilon = 0.01;

module bottom() {
  square([length + 2 * margin + 2 * kerf + 2 * thickness, width + 2 * margin + 2 * kerf + 2 * thickness]);
}

module front() {
  difference() {
    // The -4 on the height is to have some margin for the movement of the top piece.
    square([length + 2 * margin + 2 * kerf, height + 2 * kerf - 4]);
    
    for(s = [2 : safety + openning : height - safety - openning - 2])
      translate([safety, s])
        square([length + 2 * margin + 2 * kerf - 2 * safety, openning]);
  }
}

module side_hook() {
  translate([0, 2])
  difference() {
    circle(d = 2 * safety + hook_inside + 2 * kerf);
    circle(d = hook_inside + 2 * kerf);
    translate([0, -50])
      square([2 * safety + hook_inside + 2 * kerf + 2 * epsilon, 100], center = true);
  }
  translate([- safety - hook_inside / 2 - kerf, 0])
    square([safety, 2]);
  translate([hook_inside / 2 + kerf, 0])
    square([safety, 2]);
}

module left() {
  square([width + 2 * margin + 2 * kerf + 2 * thickness, height + 2 * kerf]);
  translate([safety + hook_inside / 2 + kerf, height])
    side_hook();
}

module right() {
  difference() {
    square([width + 2 * margin + 2 * kerf + 2 * thickness, height + 2 * kerf]);
    translate([8.5 + thickness, 5 + epsilon])
      square([60, height + 2 * kerf + epsilon - 5]);
  }
  translate([safety + hook_inside / 2 + kerf, height])  
    side_hook();
}

module back() {
  difference() {
    square([length + 2 * margin + 2 * kerf, height + 2 * kerf]);
    translate([10 + margin, 2])
      square([15, 8]);
  }
}

module top_hook() {
  translate([-epsilon, -epsilon])
  difference() {
    square([thickness + margin + kerf + 2 * epsilon, 2 * safety + 2 * margin + hook_inside]);
    translate([- 2 * epsilon, safety + margin + (hook_inside - safety) / 2])
      square([thickness + margin + kerf + 4 * epsilon, safety]);
  }
}

module top() {
  difference() {
    square([length + 2 * margin + 2 * kerf + 2 * thickness, width + 2 * margin + 2 * kerf + 2 * thickness]);
    top_hook();
    translate([length + margin + kerf +  thickness, 0])
      top_hook();
    translate([length + 2 * margin + 2 * kerf + thickness - 3, width + margin + kerf + thickness - 18 - 5])
      square([thickness + 3 + epsilon, 18]);
  }
}