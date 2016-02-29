#!/bin/bash

wget -q https://raw.githubusercontent.com/racaljk/hosts/master/hosts -O ./hosts
cat /private/etc/hosts > hosts.tmp
grep -v '^#' hosts | grep -v '^$' >> hosts.tmp
uniq hosts.tmp > ./hosts
rm -rf hosts.tmp
echo 'password' | sudo -S cp ./hosts /private/etc/hosts

sudo discoveryutil mdnsflushcache
sudo discoveryutil udnsflushcaches
# sudo killall -HUP mDNSResponder

