chmod +x build_champsim.sh
./build_champsim.sh bimodal no no no no lru 4
chmod +x run_4core.sh
sed -i 's/\r//' run_4core.sh
./run_4core.sh bimodal-no-no-no-no-lru-4core 1 1 0 gcc_13B.trace.xz gcc_13B.trace.xz gcc_13B.trace.xz gcc_13B.trace.xz
