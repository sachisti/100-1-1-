module og_spoj()
{
          rotate([0,0,-36])
            translate([-17,-2,0])
              cube([17, 7, 1], center=false);
    
}

module og_drziak()
{
    translate([19.5, 0, 6.5])
      difference()
      {
        union()
        {
          cylinder(h = 1, d = 10, $fn = 100);
          og_spoj();
          mirror([0,1,0])
            og_spoj();
        }
        translate([0,0,-0.1])
          cylinder(h = 1.2, d = 3.3, $fn = 100);
      }
}    

module og_drziaky()
{
    og_drziak();
    rotate([0,0,180])
      og_drziak();
}

module oporna_gulicka()
{
    //translate([-10,-10,-11.5])
    //  cube([20,20,19]);
    cylinder( h = 13, d1 = 22, d2 = 29, center = true, $fn = 100);
    translate( [0, 0, 6.5] )
      cylinder ( h = 1, d = 33 , $fn = 100);
    translate( [0, 0, -3.5] )
      sphere( d= 16, $fn = 100);
    og_drziaky();
}

//oporna_gulicka();