#!/usr/bin/env sh
# Create test repositories
export ZPM_TEST_REPOS=/tmp/zpm-test-suite/repositories
mkdir -p $ZPM_TEST_REPOS
rm -rf $ZPM_TEST_REPOS/*

cd $ZPM_TEST_REPOS

# Create example plugin in $ZPM_TEST_REPOS/example-plugin
git init $ZPM_TEST_REPOS/example-plugin
cd $ZPM_TEST_REPOS/example-plugin
echo "example () { echo 'example' }" > example.plugin.zsh
git add .
git commit -m 'initial commit'

# Create example theme in $ZPM_TEST_REPOS/example-theme
git init $ZPM_TEST_REPOS/example-theme
cd $ZPM_TEST_REPOS/example-theme
echo "export PROMPT='> '" > example.zsh-theme
git add .
git commit -m 'initial commit'

