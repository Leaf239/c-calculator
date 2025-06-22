build as
gcc main.c -o calc `pkg-config --cflags --libs gtk+-3.0`

install depending
sudo apt install libgtk-3-dev
