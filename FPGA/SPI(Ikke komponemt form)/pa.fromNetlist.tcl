
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name SPI -dir "/home/anders/Dropbox/Robtek/4_semester/DIG/SPI/planAhead_run_2" -part xc3s500efg320-4
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "/home/anders/Dropbox/Robtek/4_semester/DIG/SPI/SPI.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {/home/anders/Dropbox/Robtek/4_semester/DIG/SPI} }
set_property target_constrs_file "SPI.ucf" [current_fileset -constrset]
add_files [list {SPI.ucf}] -fileset [get_property constrset [current_run]]
link_design
