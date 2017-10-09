#!/usr/bin/env bash
# Switch batch to homebrew master branches
cd /usr/local/Homebrew/Library/Taps/homebrew/homebrew-core
git checkout master
cd /usr/local/Homebrew/Library/Taps/homebrew/homebrew-science
git checkout master

# Update homebrew; hopefully this works without errors!
brew update

# Switch back to the caffe branches with the formulae that you modified earlier
cd /usr/local/Homebrew/Library/Taps/homebrew/homebrew-core
git rebase master caffe
# Fix any merge conflicts and commit to caffe branch
cd /usr/local/Homebrew/Library/Taps/homebrew/homebrew-science
git rebase master caffe
# Fix any merge conflicts and commit to caffe branch

# Done!