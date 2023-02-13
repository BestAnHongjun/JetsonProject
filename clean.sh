shopt -s extglob

# clean bin/
cd bin
rm -rf !(.gitignore)
cd ..

# clean build/
cd build
rm -rf !(.gitignore)
cd ..

# clean lib/
cd lib 
rm -rf !(.gitignore)
cd ..
