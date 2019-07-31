#!/usr/bin/env bash
mkdir $(pwd)/.sonar
plugins_g1_root=https://github.com/SonarOpenCommunity/sonar-cxx/releases/download/cxx-1.2.2
plugin_c=${plugins_g1_root}/sonar-c-plugin-1.2.2.1653.jar
plugin_cxx=${plugins_g1_root}/sonar-cxx-plugin-1.2.2.1653.jar
plugins_root=$(pwd)/.sonar/plugins
data_root=$(pwd)/.sonar/data
logs_root=$(pwd)/.sonar/logs
mkdir $plugins_root
mkdir $data_root
mkdir $logs_root
if [[ ! -f ${plugins_root}/plugin-c.jar ]]; then
    echo "Plugin for C not found, downloading"
    #wget -O $(pwd)/.sonar/plugins/plugin-c.jar $plugin_c
fi
if [[ ! -f ${plugins_root}/plugin-cxx.jar ]]; then
    echo "Plugin for C++ not found, downloading"
    wget -O $(pwd)/.sonar/plugins/plugin-cxx.jar $plugin_cxx
fi
docker run -d --name sonarqube\
    -v $(pwd)/.sonar/plugins:/opt/sonarqube/extensions/plugins\
    -p 9000:9000 -p 9092:9092\
    sonarqube:7.7-community
echo -n "Write your login key: "
read login_key
docker run -t -i\
    -v $(pwd)/src:/root/src \
    --link sonarqube\
    newtmitch/sonar-scanner sonar-scanner\
    -Dsonar.projectKey=psh -Dsonar.sources=. \
    -Dsonar.host.url=http://sonarqube:9000 -Dsonar.login=$login_key