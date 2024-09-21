docker run -d -v "${PWD}:/root" --privileged -ti --name TPECOMPILER agodio/itba-so-multi-platform:3.0
docker start TPECOMPILER
docker exec -it TPECOMPILER make clean -C/root/Toolchain
docker exec -it TPECOMPILER make clean -C/root/
docker exec -it TPECOMPILER make -C/root/Toolchain
docker exec -it TPECOMPILER make -C/root/
docker stop TPECOMPILER 
docker rm TPECOMPILER 
