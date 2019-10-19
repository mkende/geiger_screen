include <box.scad>

bottom();

translate([0, 80])
  top();

translate([130, 0])
  front();

translate([130, 50])
  back();

translate([185, 100])
  rotate([0, 0, 90])
    left();

translate([250, 100])
  rotate([0, 0, 90])
    right();


