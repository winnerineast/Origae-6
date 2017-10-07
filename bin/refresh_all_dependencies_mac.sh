#!/usr/bin/env bash
#
# Before run it, please check each github url and branches or tags one by one
# because the script will OVERRIDE all of your existing source codes without any further warning!!!!!!!!!!!
#

#
# the source code be used in Origae are coming from various submodules, so you need to refresh submodules time to time
# in order to gain new features or update invoking ways.
# submodule source codes =====> Origae source codes =====> automatic test to identify any change =====> manual change Origae source codes
# =====> automatic test again to make sure changes works =====> build Origae again =====> enjoy!
#

#
# Starting directory you need to be ------ Origae-6 root directory which is sth. like '~/Origae-6'
#

#
# Define some functions here
#
fresh_submodule(){
    git ls-remote $2 'refs/heads/*'
    echo "Please enter the version of Nvidia $1 to be cloned in Origae-6"
    read submodule_version
    read -r -p "The next step will DELETE your Origae's submodule [$1], Are you sure? [y/N] " response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
    then
        git submodule deinit -f $1
        git rm -f $1
        if [ -d $1 ]; then
            rm -rf DIGITS
        fi
        if [ "$#" -eq 3 ]; then
            git submodule add --force -b $3 $2 $1
            git submodule checkout
        else
            git submodule add --force -b $submodule_version $2 $1
        fi
    else
        exit
    fi
}
#
# Refresh all submodules
fresh_submodule DIGITS https://github.com/NVIDIA/DIGITS.git
fresh_submodule jetson-inference https://github.com/dusty-nv/jetson-inference.git
fresh_submodule nvcaffe https://github.com/NVIDIA/caffe.git
fresh_submodule caffe https://github.com/BVLC/caffe.git