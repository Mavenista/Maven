echo "beginning compilation process"
export MAVEN_VERSION="2025b"
cd mavenc/ && bash build.sh
cd ../maven/ && bash build.sh
cd ..
cd ./std && maven build
cd ..
rm -rf ./Release
mkdir ./Release
mkdir ./Release/$MAVEN_VERSION/
mv ./maven/maven ./Release/$MAVEN_VERSION/
mv ./mavenc/mavenc ./Release/$MAVEN_VERSION/
cp -r ./std/build/std0.1.0/native ./Release/$MAVEN_VERSION/std
cd ./Release/$MAVEN_VERSION/
tar cvzf Maven-$MAVEN_VERSION-$1.tar.gz .
echo "ended compilation process"