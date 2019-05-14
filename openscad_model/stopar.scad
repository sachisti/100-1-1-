include <drziak_lego_kolesa.scad>;
include <oporna_gulicka.scad>;
include <vypinace.scad>;

velkost_diery = 3.3;
cele = 1;

module dierak()
{
    translate([-30,-105,26])
    {
        cube([60,40,7], center=false);
    }
}  

module bok_podvozku()
{
  translate([20, 44, 27])
    rotate([0, 0, -27.5])
      cube([140, 50, 5]);    
}

module baterka_3s_900mah()
{
    cube([65, 25, 20]);
}

module boky_podvozku()
{
  bok_podvozku();
  mirror([0,1,0])
    bok_podvozku();
}

module odrez_predok()
{
    translate([-125, -100, 25])
      cube([100, 200, 8]);
}

module odrez_bok()
{
    translate([20, 78, 25])
      cube([50, 30, 8]);    
}

module odrez_boky()
{
    odrez_bok();
    mirror([0, 1, 0])
      odrez_bok();
}

module zarazky_baterky()
{
    translate([173.5, -14, 15])
      cube([2.5, 28, 10]);
    translate([155, -15.5, 15])
      cube([20, 2, 10]);
    translate([155, 13.5, 15])
      cube([20, 2, 10]);

    translate([115, -16.5, 15])
      cube([12, 3, 10]);
    translate([115, 13.5, 15])
      cube([12, 3, 10]);

    translate([103.5, -8, 16])
      cube([3.5, 16, 9]);
}

module chytak()
{
      rotate([90, 0, 0])
      {
        difference()
        {
          union()
          {          
            translate([-4.1, 0, 0])
              cube([20, 5, 12]);
            cylinder(h = 12, d = 10, $fn = 100);    
          }
          translate([0,0,-1])
            cylinder(h = 14, d = 5, $fn = 100);
          translate([-1.25, -5.5, -1])
            cube([2.5, 4, 14]);          
        }
      }
}

module dva_chytaky()
{
  translate([-33, -53, 27])
      chytak();
  translate([-33, -18, 27])
      chytak();    
}
module chytaky()
{
  dva_chytaky();
  mirror([0,1,0])
    dva_chytaky();
}

module podvozok()
{   
    difference ()
    {   
        union()
        {
          translate([0,0,27])        
            cylinder(h=5, r=100, center=false, $fn=100);
        
          translate([150,0,23.5])        
            cylinder(h=8.5, r=30, center=false, $fn=100);

          translate([120,0,23.5])        
            cylinder(h=8.5, r=20, center=false, $fn=100);

          boky_podvozku();
            
          zarazky_baterky();
        }
       
        odrez_predok();
        odrez_boky();
        
        dierak();
        rotate([0,0,180]) dierak(); 
    } 
    chytaky();
}

module podstava()
{
    cylinder(h=2, d=22, $fn=100);
}

module stit()
{
    cylinder(h=12, d=10, $fn=100);
}

module base()
{
    podstava();
    stit();
}

module vrchDiera()
{
    translate([0, 0, -0.1])
      cylinder(h=12.2, d=6, $fn=100);
    
}

module jamy()
{
    translate([8, 0, -0.1])
       cylinder(h=2.2, d=3, $fn=100);
    translate([-8, 0, -0.1])
       cylinder(h=2.2, d=3, $fn=100);
    translate([0, 8, -0.1])
       cylinder(h=2.2, d=3, $fn=100);
    translate([0, -8, -0.1])
       cylinder(h=2.2, d=3, $fn=100);
}

module cervik()
{
    rotate([0, 0, 45])
    translate([-5.5, 0, 8])
        rotate([0, 90, 0])
            cylinder(h=11, d=3, $fn=100);
}
module koza()
{
    difference()
    {
        base();
        vrchDiera();
        jamy();
        cervik();
    }
}

module koliesko(){
    
 cylinder(h=28.5, d=55, $fn=100);    
}



module Lko()
{
   cube([40, 43, 3.3]);
   cube([40, 3.3, 27]);

    translate([20,3.3,27]) 
    {
      rotate([90,0,0])
      {
        cylinder(h=3.3, d=40, $fn=100);
      }
    }
}

module hornyVrt()
{
    rotate([-90,0,0])
        cylinder(h=3.5, d=velkost_diery, $fn=100);
}

module vrt()
{
    rotate([0,0,0])
        cylinder(h=25, d=velkost_diery, $fn=100);
}

module velkyOtvor()
{
    translate([20,-0.1,34])
      rotate([-90,0,0])
          cylinder(h=3.5, d=13, $fn=100);
    translate([20,-0.1, 20]) 
    {
      rotate([-90,0,0])
      {
          cylinder(h=3.5, d=13, $fn=100);
      }
    }
    translate([13.5,-0.1, 20]) 
    {
            cube([13, 3.5, 14]);
    }
}

module horneDiery()
{
//1    
    translate([4.5, -0.1, 27])
        hornyVrt();
    
//2
    translate([35.5, -0.1, 27])
        hornyVrt();
    
//3
    translate([20, 0, 27])
        rotate([0,-60,0])
            translate([15.5, -0.1, 0])
                hornyVrt();
    
//4
    translate([20, 0, 27])
        rotate([0,60,0])
            translate([15.5, -0.1, 0])
                hornyVrt();
    
//5
    translate([20, 0, 27])
        rotate([0,60,0])
            translate([-15.5, -0.1, 0])
                hornyVrt();
                
//6
    translate([20, 0, 27])
        rotate([0,-60,0])
            translate([-15.5, -0.1, 0])
              hornyVrt();          
}

module dolneDiery()
{
    translate([5, 9.5, -5.1])
        vrt();
    translate([5, 33.5, -5.1])
        vrt();
    translate([35, 9.5, -5.1])
        vrt();
    translate([35, 33.5, -5.1])
        vrt();
}

module diery()
{
    velkyOtvor();
    horneDiery();
    dolneDiery();

}

module lko()
{
    difference() 
    {
        Lko();
        diery();
    }
}

module vrch()
{
    translate(v = [0, 0, 44.7]) 
    { 
        cylinder(h=6, r=5, center=false, $fn=100); 
    }
    translate(v = [0, 0, 50.7]) 
    { 
        cylinder(h=15.5, r=3, center=false, $fn=100);
    }
}

module motor()
{
    translate([0,0.2,0])
    {
        rotate([-90,0,0])
        {        
            cylinder(h=22.7, r=16.5, center=false, $fn=100);
            translate(v = [0, 0,22.7]) 
            { 
                cylinder(h=22, r=18.5, center=false, $fn=100); 
            }

            translate(v = [0, -7, 0])
            {
                vrch();   
            }
        }
    }
}


module motorsl()
{
    rotate([0,180,0])
    motor();
    translate([20,-48.3,27])
        rotate([0,180,0])
    {
            lko(); 
        }   
}

module motor_so_vsetkym()
{
    motorsl();
    translate([0,-66.5,-7])
    {
        rotate([-90,0,0])
        {
          koza();
          translate([0,0,-1.25])
            rotate([0,0,-15])
              drziak_lego_kolesa();
        }
    }    
    translate([0,-97.6,-7])
    {
        rotate([-90,0,0])
        koliesko();    
    }
}

module dieryVPodvozku_pre_drziaky_motorov()
{
    translate([20,-48.3,27])
        rotate([0,180,0])
            dolneDiery();
}

module stlpik()
{
    cylinder(d=5, h=25 + 10, $fn=6);
}

module diery_pre_stlpiky_k_opornej_gulicke()
{
    translate([150, 0, 21.5])
      rotate([0, 0, 90])
      {
        translate([-19.5,0,0])
          vrt();
        translate([19.5,0,0])
          vrt();        
      }
}

module diery_pre_suchy_zips_k_baterke()
{
    translate([130, 13, 21.5])
      cube([22, 2, 12]);
    translate([130, -15, 21.5])
      cube([22, 2, 12]);
}

module diery_pre_vypinace()
{
   translate([104, -38, 23])    
      cube([14,9,10]);    
   mirror([0,1,0])
     translate([104, -38, 23])    
       cube([14,9,10]);  
}

module arduino(){
 translate([143, -26.5, 67])
 rotate([180,0,180])
 {    
       cube([57,53,25]);
  
       translate([4,3,25])
          cylinder(h=10,d=7, center = false, $fn=6);
    
       translate([4,50.5,25])
           cylinder(h=10,d=7, center = false, $fn=6);
 
       translate([55,17,25])
           cylinder(h=10,d=7, center = false, $fn=6); 

       translate([55,45,25])
           cylinder(h=10,d=7, center = false, $fn=6);
 }
}
    
module diery_pre_arduino()
{
 translate([143, -26.5, 95])
 rotate([180,0,180])
 {
         translate([4,3,25])
         cylinder(h=60,d=velkost_diery, center = false, $fn=100);
            
         translate([4,50.5,25])
         cylinder(h=60,d=velkost_diery, center = false, $fn=100);
         
         translate([55,17,25])
         cylinder(h=60,d=velkost_diery, center = false, $fn=100); 

         translate([55,45,25])
         cylinder(h=60,d=velkost_diery, center = false, $fn=100);
 }
}

module polo_vela_dier()
{
    translate([105,-48, 23])
      cylinder(h=15, d=6, $fn=100);

    translate([128,-34, 23])
      cylinder(h=15, d=6, $fn=100);

    translate([30, -12, 23])
      cylinder(h=15, d=6, $fn=100);

    translate([90,-34, 23])
      cylinder(h=15, d=6, $fn=100);

    translate([82,-60, 23])
      cylinder(h=15, d=6, $fn=100);

    translate([60,-68, 23])
      vrt();

    translate([48,-68, 23])
      vrt();

    translate([8, 0, 23])
      vrt();

    translate([-8,0, 23])
      vrt();
      
    translate([70,0, 23])
      cylinder(h=15, d=6, $fn=100);
      
    translate([70,12, 23])
      vrt();

    translate([70,-12, 23])
      vrt();

    translate([25,-59.5, 23])
      vrt();

    translate([25,-51.5, 23])
      vrt();

    translate([-5,-59.5, 23])
      vrt();

    translate([-5,-51.5, 23])
      vrt();

}

module vela_dier()
{
    polo_vela_dier();
    mirror([0,1,0])
      polo_vela_dier();
}

module zdroj()
{
    translate([64,5,32])
    rotate([0,0,90])
    {
        cube([43,21,13]);
    }
}

module radicova_doska()
{
    translate([44,-55,32])
      cube([28,35,25]);
}

module diery_pre_zdroj()
{
    translate([46,11,22])
      vrt();
    translate([61.5,41,22])
      vrt();
}

module diery_pre_radic()
{
    translate([49,-50,22])
      vrt();
    translate([64,-50,22])
      vrt();
}

module oporna_gulicka_so_stlpikmi()
{
    oporna_gulicka();
    translate([-19.5,0,7.5])
      stlpik();
    translate([19.5,0,7.5])
      stlpik();    
}

module robot()
{
    if (cele == 1)
    {
        motor_so_vsetkym();
        rotate([0,0,180]) motor_so_vsetkym(); 
        translate([150, 0, -19])
          rotate([0, 0, 90])
            oporna_gulicka_so_stlpikmi();
          
        translate([108, -12.5, 3])
          baterka_3s_900mah();
        
        translate([105, -38, 23])
          switch_kcd11();
        mirror([0,1,0])
          translate([105, -38, 23])
            switch_kcd11();
          
        arduino();
        zdroj();
        radicova_doska();
    }

    difference()
    {
        podvozok();
        
        dieryVPodvozku_pre_drziaky_motorov();
        rotate([0,0,180])
          dieryVPodvozku_pre_drziaky_motorov(); 
        
        diery_pre_stlpiky_k_opornej_gulicke();
        diery_pre_suchy_zips_k_baterke();
        diery_pre_vypinace();
        diery_pre_arduino();
        diery_pre_zdroj();
        diery_pre_radic();
        vela_dier();
    }
}

module opacny_chytak()
{
    translate([-33, -47.5, 27])
      rotate([0,0,180])
        chytak();
}

module opacne_chytaky()
{
    opacny_chytak();
    translate([0,36,0])
      opacny_chytak();
    
    mirror([0,1,0])
    {
      opacny_chytak();
      translate([0,36,0])
        opacny_chytak();
    }
}

module diery_pre_kabliky_k_senzorom()
{
   translate([-80, -6, 14])
     cube([15, 12, 20]);

   translate([-70, -52, 14])
     cube([5, 40, 20]);

   mirror([0,1,0])
     translate([-70, -52, 14])
       cube([5, 40, 20]);
}

module strecha()
{
    difference()
    {
      union()
      {
        translate([-101, -68, 27])    
          cube([55,136,5]);
        translate([-99.5, -68, 19.5])    
          cube([8,136,11.5]);
        translate([-60.5, -68, 19.5])    
          cube([8,136,11.5]);  
        translate([-99.5, -22.5, 7])
          cube([27, 46, 21]);
        translate([-92.5, -68, -6])    
          cube([32,3,37]);
        mirror([0,1,0])
          translate([-92.5, -68, -6])    
            cube([32,3,37]);
          
      }
      translate([-100.5,-15,-0.5])
        cube([35, 31, 24]);
      translate([-78,-18.25,16.5])
        rotate([0,90,0])
          cylinder(h = 10, d = velkost_diery, $fn = 100);
      translate([-78,18.75,16.5])
        rotate([0,90,0])
          cylinder(h = 10, d = velkost_diery, $fn = 100);
      translate([-79,-25,6])        
          cube([3, 52, 15]);
      
      diery_pre_stlpiky_k_opornym_gulickam();
      mirror([0, 1, 0])
        diery_pre_stlpiky_k_opornym_gulickam();
      
      diery_pre_kabliky_k_senzorom();
      
            //toto
      translate([-82,-25,-2.5])
        cube([15, 51, 13]);
    }    
}

module diery_svetelnych_senzorov()
{
  for (dier = [0:3])
    translate([-90, -40.4 + 11.4 * dier, -14])
      rotate([0, 90, 0])
        cylinder(d = velkost_diery, h=20, $fn = 100);   
}

module senzory()
{
    difference()
    {
      translate([-79, -46.6, -29])
        cube([7.5,46.5, 23]);
      diery_svetelnych_senzorov();
    }
}

module sharp_usko()
{
    difference()
    {
      union()
      {
        cylinder(h = 2, r = 3.75, $fn = 100);
        translate([0, -3.75, 0])
          cube([3.75, 3.75 * 2, 2]);
      }
      translate([0, 0, -0.2])
        cylinder(h = 2.4, d = 3.2, $fn = 100);
    }    
}

module sharp_sensor()
{
    cube([29.5, 13, 13.5]);
    
    translate([-3.75, 13 / 2, 0])
      sharp_usko();
    translate([29.5 + 3.75, 13 / 2, 0])
      mirror([1,0,0])
        sharp_usko();
}

module diery_pre_stlpiky_k_opornym_gulickam()
{
    translate([-76, -63, 12])
      {
        translate([-19.5,0,0])
          vrt();
        translate([19.5,0,0])
          vrt();        
      }
}

module sikmy_stlp()
{
    translate([-85,-7.8,-22.5])
      rotate([30, 0, 0])
        cube([3, 10, 35]);    
}

module krytka_pre_senzory()
{
   difference()
   {
      union()
      {
        difference()
        {
          translate([-85, -46.6, -28])
              cube([3, 93, 60]);
          translate([-100.5,-15,-8.5])
            cube([35, 31, 32]);
          translate([-86, -5, -17.5])                
            cube([4.1, 10, 10]);
          
        }
    
        sikmy_stlp();
          mirror([0,1,0])
            sikmy_stlp();
    
        translate([-87.5, -46.6, -28])
          rotate([0,10,0])
            cube([3, 93, 15]);
      }
      
      diery_svetelnych_senzorov();
      
      mirror([0,1,0])
        diery_svetelnych_senzorov();

   }
}

module senzoricky_modul()
{
    opacne_chytaky();
    strecha();
    krytka_pre_senzory();
    if (cele == 1)
    {
        translate([-76, -63, -23])        
            oporna_gulicka_so_stlpikmi();
            
        mirror([0, 1, 0])
            translate([-76, -63, -23])        
                oporna_gulicka_so_stlpikmi();
        
        senzory();
        mirror([0, 1, 0]) senzory();
        
        translate([-70, 15, 10])
          rotate([90, 0, -90])
          {
            sharp_sensor();
          }
    }
}

robot();
senzoricky_modul();