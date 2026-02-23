cmake .

cmake --build .

chmod +x myApp

./myApp -a 255.0.0.1 -p 9999 -r Admin -i 0 -L mylib