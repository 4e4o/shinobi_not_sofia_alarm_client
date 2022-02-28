git clone --recurse-submodules git@github.com:4e4o/shinobi_not_sofia_alarm_client.git  
mkdir build_12 && cd build_12  
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ../shinobi_not_sofia_alarm_client/ .  
cmake --build .  
sudo cmake --install .  

sudo systemctl daemon-reload  
sudo systemctl enable shinobi_not_sofia_alarm_client.service  
sudo systemctl start shinobi_not_sofia_alarm_client.service  
