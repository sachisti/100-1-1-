velkost_diery = 3.3;

module diera_na_lego_konektor()
{
    cylinder( h = 6, d = 5, $fn = 100, center=true);
    cylinder( h = 0.5, d = 6, $fn = 100, center=true);
}

module diera_na_skrutku()
{
    cylinder( h = 6, d = velkost_diery, $fn = 100, center=true);
}

module diera_na_lego_osku()
{
    cube([4.9, 1.9, 7], center=true);
}

module drziak_lego_kolesa()
{
    difference()
    {
        cylinder( h = 2.5, d = 24, $fn = 100, center=true);
        translate( [0, 0, 1] )
          cylinder( h = 0.8, d = 20, $fn = 100, center = true);
        translate( [0, 0, -1])
        {
           diera_na_lego_osku();
           rotate([0, 0, 90])
             diera_na_lego_osku();
        }
        for (i = [1:2])
          rotate([0, 0, i * 180 + 60])
            translate([8, 0, 0.8])
              diera_na_lego_konektor();
        
        for (i = [1:4])
          rotate([0, 0, 90 * i + 15])
            translate([8, 0, 0.8])
              diera_na_skrutku();
    }
}

//drziak_lego_kolesa();