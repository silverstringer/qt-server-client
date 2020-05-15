## Qt Server Client 
## Requirements
  
- C++17 compiler support
- Cmake 
- QT

## Build
  git clone https://github.com/silverstringer/qt-server-client.git


  mkdir build &&  cd build 

  cmake .. && make -j4
  
## Install
  make install

## Usage

Client send package by timer(timeout).
Server recieve and parse package. 
Server has container, after parse package 
make task thread concurrency read and write to container and send result to client 
 
## License
