#!/bin/bash

## You should change #YOUR_HOME_PATH#, #THIS_SHELL_PATH# and #PASSWORD# parameters.
## I also set the following config use crontab to auto run this shell:
## 0 10 * * * . /etc/profile;. #YOUR_HOME_PATH#/.bash_profile;cd #THIS_SHELL_PATH# && bash hosts.autoupdate.osx_10.10.3.sh >hosts.log 2>&1

wget -q https://raw.githubusercontent.com/racaljk/hosts/master/hosts -O ./hosts
cat /private/etc/hosts > hosts.tmp
grep -v '^#' hosts | grep -v '^$' >> hosts.tmp
uniq hosts.tmp > ./hosts
rm -rf hosts.tmp
echo '#PASSWORD#' | sudo -S cp ./hosts /private/etc/hosts

## For Mac OSX Yosemite
sudo discoveryutil mdnsflushcache
sudo discoveryutil udnsflushcaches

## For Mac OSX Lion and Mountain Lion
# sudo killall -HUP mDNSResponder

