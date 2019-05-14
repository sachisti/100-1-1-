module switch_kcd11()
{
  switch_main_box();

  translate([0.5, 0, 10.5])
    switch_button();
  
  switch_clicks();
  
  switch_contacts();  
}

module switch_main_box()
{
  color([0.25,0.25,0.25,1])
  {
      cube([12, 8.10, 9], false);
      translate([-1.5, -1.1, 9]) 
        cube([15, 10.3, 1], false);
      translate([-1.5, -1.1, 10]) 
        difference()
        {
            cube([15, 10.3, 0.505], false);
            {
              translate([-0.005, -0.005, 0])
                rotate([30,0,0])
                  cube([15.01, 2, 1], false);
              translate([-0.005, 10.295, 0])
                rotate([60,0,0])
                  cube([15.01, 1.01, 2], false);
              translate([-0.005, -0.005, 0])
                rotate([0,-30,0])
                  cube([2, 10.301, 2]);
              translate([15, -0.005, 0])
                rotate([0,-60,0])
                  cube([2, 10.301, 2]);
            }
        }
  }
}

module switch_contacts()
{
  color("silver")
  {
      translate([1, 2.16, -6.66])
        switch_contact();
      translate([6.2, 2.16, -6.66])
        switch_contact();
  }
}

module switch_contact()
{
  difference()
  {
      cube([0.4, 3.8, 6.66], false);
      translate([-0.005, 1.5, 1.15]) 
        cube([0.41, 0.8, 2.5], false);
  }
}


module switch_clicks()
{
    color("grey")
    {
        translate([-0.7, 2.3, 0])
          switch_click();
        
        translate([12.7,2.3,0])
          mirror([1,0,0])
           switch_click();
    }
}
module switch_click()
{
    cube([0.7, 5.8, 3], false);
    translate([0,0,2.9])
      rotate([0,-12,0])
        cube([0.7, 3.4, 3.2], false);
    translate([-0.6,0,6])
      rotate([0,12,0])
        cube([0.7, 3.4, 3.2], false);
}

module switch_button()
{
    color("grey")
    {
        cyld1 = 24;
        cyld2 = 12;
        intersection()
        {
            difference()
            {
                cube([13.33, 8.1, 3]);
                translate([-0.005, 8.105, cyld1])
                  rotate([90,0,0])
                    cylinder(8.11, cyld1, cyld1, [0, cyld1, 0], $fn=200);
            } 
            translate([-0.005, 8.095, -4])
              rotate([90,0,0])
                cylinder(8.11, cyld2, cyld2, [0, cyld1, 0], $fn=100);
        }
    }
}