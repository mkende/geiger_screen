include <box.scad>

linear_extrude(3) bottom();

translate([thickness, thickness, thickness])
  rotate([90, 0, 0])
    linear_extrude(thickness) front();

translate([0, 0, thickness]) 
  rotate([0, 90, 0])
    rotate([0, 0, 90])
      linear_extrude(thickness) left();

translate([length + 2 * margin + 2 * kerf + thickness, 0, thickness]) 
  rotate([0, 90, 0])
    rotate([0, 0, 90])
      linear_extrude(thickness) right();

translate([thickness, 2 * thickness + width + 2 * margin + 2 * kerf, thickness])
  rotate([90, 0, 0])
    linear_extrude(thickness) back();

translate([0, 0, height + 2 * kerf + thickness])
  linear_extrude(3) top();
