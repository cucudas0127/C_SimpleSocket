
#-----------------------------------------------------------------------------
# PROGRAMMER   : cucudas0127
# REVISION     : 2020.11.012
# DESCRIPTS    : Simple Socket Server/Client Code.
# Environment Setting
# OS : ubuntu 18.04
#-----------------------------------------------------------------------------

# if you want using make command
# you have to install cmake
# sudo apt install make 


CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
PROJECT("Simple Socket Server/Client Code ")

ADD_EXECUTABLE( SimpleClient.out SimpleClient.c )
ADD_EXECUTABLE( SimpleServer.out SimpleServer.c )