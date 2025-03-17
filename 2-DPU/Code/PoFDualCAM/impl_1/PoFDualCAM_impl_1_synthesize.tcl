if {[catch {

# define run engine funtion
source [file join {F:/MySoftware/Lattice/radiant} scripts tcl flow run_engine.tcl]
# define global variables
global para
set para(gui_mode) 1
set para(prj_dir) "F:/3-MyLaptopDIR/PoFDualCAM/2-DPU/Code/PoFDualCAM"
# synthesize IPs
# synthesize VMs
# propgate constraints
file delete -force -- PoFDualCAM_impl_1_cpe.ldc
run_engine_newmsg cpe -f "PoFDualCAM_impl_1.cprj" "MyOSC.cprj" "MyADC.cprj" -a "LIFCL"  -o PoFDualCAM_impl_1_cpe.ldc
# synthesize top design
file delete -force -- PoFDualCAM_impl_1.vm PoFDualCAM_impl_1.ldc
run_engine_newmsg synthesis -f "PoFDualCAM_impl_1_lattice.synproj"
run_postsyn [list -a LIFCL -p LIFCL-40 -t CABGA256 -sp 9_High-Performance_1.0V -oc Industrial -top -w -o PoFDualCAM_impl_1_syn.udb PoFDualCAM_impl_1.vm] "F:/3-MyLaptopDIR/PoFDualCAM/2-DPU/Code/PoFDualCAM/impl_1/PoFDualCAM_impl_1.ldc"

} out]} {
   runtime_log $out
   exit 1
}
