chmod +x build_champsim.sh
./build_champsim.sh bimodal no no no no lru 2
chmod +x run_2core.sh
sed -i 's/\r//' run_2core.sh
./run_2core.sh bimodal-no-no-no-no-lru-2core 1 10 0 bzip2_183B.trace.xz bzip2_183B.trace.xz
